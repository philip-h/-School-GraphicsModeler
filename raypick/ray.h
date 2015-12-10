#include "Header.h"

struct vertex3D{
  double x,y,z;
};

class Ray{
public:
  vertex3D orig; //start point (x,y,z) for origin of the ray
  vertex3D dir; //direction vector (x,y,z) for the ray

  Ray(double start[], double end[]);

  void normalize();
};

class BoundedBox{
public:
  vertex3D min; //p1 point of our bounded box
  vertex3D max; //p2 point of our bounded box
  int shapeId; //id for the shape that the bounding box is for in the node graph

  BoundedBox(double p1[], double p2[], int id);
};