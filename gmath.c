
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"


/*============================================
  IMPORANT NOTE

  Ambient light is represeneted by a color value

  Point light sources are 2D arrays of doubles.
       - The fist index (LOCATION) represents the vector to the light.
       - The second index (COLOR) represents the color.

  Reflection constants (ka, kd, ks) are represened as arrays of
  doubles (red, green, blue)
  ============================================*/


//lighting functions

// total lighting is determined by the sum of the ambient, diffused, and specualar lighting values
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
  normalize(normal);
  normalize(view);
  normalize(light[LOCATION]);
  color a = calculate_ambient(alight, areflect);
  color d = calculate_diffuse(light, dreflect, normal);
  color s = calculate_specular(light, sreflect, view, normal);
  i.red = a.red + d.red + s.red;
  i.blue = a.blue + d.blue + s.blue;
  i.green = a.green + d.green + s.green;
  limit_color(&i);
  return i;
}

// calculated by: Ambient color * ambient relective constant
color calculate_ambient(color alight, double *areflect ) {
  color a;
  a.red = alight.red * areflect[RED];
  a.blue = alight.blue * areflect[BLUE];
  a.green = alight.green * areflect[GREEN];
  return a;
}

// calculated by: Point color * Diffused reflective constant * (View vector dot Surface normal)
color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  double dot = dot_product(normal, light[LOCATION]);
  if (dot < 0) dot = 0;
  d.red = light[COLOR][RED] * dreflect[RED] * dot;
  d.blue = light[COLOR][BLUE] * dreflect[BLUE] * dot;
  d.green = light[COLOR][GREEN] * dreflect[GREEN] * dot;
  return d;
}

// This is the most complicated as specular is very strong in a certain direction but dies off quickly
// whilehen the light vector is closer to the normal, the specular reflective value of the surface is much greater
// We can do this by using cosine, aka. when angle big value small and vice versa
// =============================================================================================== //
// N -> Surface Normal
// L -> Light vector
// R -> Reflective vector
// V -> View vector
// T -> Projection of L onto N
// S -> Translation vector mapping T onto R
// Ks -> Specular reflective constant
// Kl -> Point light color
// theta -> L/N angle
// alpha -> R/V angle
// we need to find: Ks * Kl * cos(alpha)
// cos(alpha) = R dot V
// R = T + S
// T = N (N dot L)
// S = T - L = N (N dot L) - L
// R = 2 N (N dot L) - L
// So: cos(alpha) = (2 N (N dot L) - L) dot V
// We should raise cos(alpha) to some power n such that the "shine" dies off quickly
// =============================================================================================== //
// FINAL EQUATION: Ks * Kl * ((2 N (N dot L) - L) dot V)^n
color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s;
  double R[3], dot;
  dot = dot_product(normal, light[LOCATION]);
  if (dot < 0) dot = 0;
  R[0] = 2 * normal[0] * dot - light[LOCATION][0];
  R[1] = 2 * normal[1] * dot - light[LOCATION][1];
  R[2] = 2 * normal[2] * dot - light[LOCATION][2];
  dot = dot_product(R,view);
  if (dot < 0) dot = 0;
  dot = pow(dot, SPECULAR_EXP);
  s.red = sreflect[RED] * light[COLOR][RED] * dot;
  s.blue = sreflect[BLUE] * light[COLOR][BLUE] * dot;
  s.green = sreflect[GREEN] * light[COLOR][GREEN] * dot;
  return s;
}

//limit each component of c to a max of 255
void limit_color( color * c ) {
  if (c->red > 255) c->red = 255;
  if (c->blue > 255) c->blue = 255;
  if (c->green > 255) c->green = 255;
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double magnitude;
  magnitude = sqrt( vector[0] * vector[0] +
                    vector[1] * vector[1] +
                    vector[2] * vector[2] );
  int i;
  for (i=0; i<3; i++) {
    vector[i] = vector[i] / magnitude;
  }
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}


//Calculate the surface normal for the triangle whose first
//point is located at index i in polygons
double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
