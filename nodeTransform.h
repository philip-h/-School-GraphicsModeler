#ifndef __NODETRANSFORM_H__	//guard against cyclic dependancy
#define __NODETRANSFORM_H__

#include "node.h"
#include "structs.h"
enum TransformType{
	Translate,
	Rotate,
	Scale
};

class NodeTransform:public Node{
public:
	NodeTransform(TransformType whatType, Vector3D vec3);	//constructor
	NodeTransform(TransformType whatType, Vector4D vec4);
	NodeTransform(TransformType whatType);

	TransformType transformationType;
	Vector3D amount3;
	Vector4D amount4;

	virtual void nodeSpecificCodeDown();
	virtual Vector3D getShapePosition();
	virtual void describeNode();
};

#endif