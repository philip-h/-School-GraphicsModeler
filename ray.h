#include "Header.h"
#include "structs.h"

class Ray{
public:
  Vector3D orig; //start point (x,y,z) for origin of the ray
  Vector3D dir; //direction vector (x,y,z) for the ray

  Ray(double start[], double end[]);

  void normalize();
};