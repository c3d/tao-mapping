/*****************************************************************

  Prototypes for the fractional Brownian motion algorithm. These
  functions were originally the work of F. Kenton Musgrave.  For
  documentation of the different functions please refer to the book:
  "Texturing and modeling: a procedural approach"
  by David S. Ebert et. al.
  // From the QT example named Boxes
  
******************************************************************/

#ifndef _fbm_h
#define _fbm_h

#include  <math.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    double x;
    double y;
    double z;
} Vector3D;

float noise3(float vec[]);
#endif

#ifdef __cplusplus
}
#endif




