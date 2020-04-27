#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"
#include "stack.h"
#include "gmath.h"

int main(int argc, char **argv) {

  // lighting values here for easy access
  // ambient color
  color ambient;
  // point light: location / color
  double light[2][3];
  // view point
  double view[3];
  // reflective constants: ambient, diffuse, specular
  // these determine how much of each type and color value of light objects will reflect
  // ambient is just light present everywhere (like moonlight at 12am)
  // diffusion is a matte, soft reflection (like shining a light on a rock)
  // specular is the equivelent of a "shine" on objects (like a polish on a car)
  // ***color values should not be brighter than 255***
  double areflect[3];
  double dreflect[3];
  double sreflect[3];

  // ambient light
  ambient.red = 50;
  ambient.green = 50;
  ambient.blue = 50;

  // one point light source
  light[LOCATION][0] = 0.5;
  light[LOCATION][1] = 0.75;
  light[LOCATION][2] = 1;

  light[COLOR][RED] = 0;
  light[COLOR][GREEN] = 255;
  light[COLOR][BLUE] = 255;

  // view vector
  view[0] = 0;
  view[1] = 0;
  view[2] = 1;

  // reflective constants
  areflect[RED] = 0.1;
  areflect[GREEN] = 0.1;
  areflect[BLUE] = 0.1;

  dreflect[RED] = 0.5;
  dreflect[GREEN] = 0.5;
  dreflect[BLUE] = 0.5;

  sreflect[RED] = 0.5;
  sreflect[GREEN] = 0.5;
  sreflect[BLUE] = 0.5;

  screen s;
  zbuffer zb;
  struct matrix * edges;
  struct matrix * polygons;
  struct stack * csystems;

  edges = new_matrix(4, 4);
  polygons = new_matrix(4, 4);
  csystems = new_stack();

  if ( argc == 2 )
    parse_file( argv[1], csystems, edges, polygons, s, zb,
                view, ambient, light, areflect, sreflect, dreflect);
  else
    parse_file( "stdin", csystems, edges, polygons, s, zb,
                view, ambient, light, areflect, sreflect, dreflect);

  free_matrix( edges );
  free_matrix( polygons );
  free_stack(csystems);
}
