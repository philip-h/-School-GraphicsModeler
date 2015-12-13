#include "ray.h"

Ray::Ray(double start[], double end[])
{
  //the origin as the mouse click start position
  orig.x = start[0];
  orig.y = start[1];
  orig.z = start[2];

  //ray direction is the vector (end - start)
  dir.x = end[0] - start[0];
  dir.y = end[1] - start[1];
  dir.z = end[2] - start[2];

  //normalize the ray direction vector
  normalize();
}

//sets the direction to a normalized amount sqrt(x^2+y^2+z^2)
void Ray::normalize()
{
    //magnitude
    double mag = sqrt(dir.x*dir.x + dir.y*dir.y + dir.z*dir.z);

    //unit direction vector
    dir.x = dir.x/mag;
    dir.y = dir.y/mag;
    dir.z = dir.z/mag;
}