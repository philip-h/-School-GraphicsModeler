#include "BoundedBox.h"

BoundedBox::BoundedBox(Vector3D p1, Vector3D p2, int id)
{
 	min = p1;
	max = p2;
  	shapeId = id;
}