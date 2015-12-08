#ifndef __NODEMODEL_H__	//guard against cyclic dependancy
#define __NODEMODEL_H__

#include "node.h"
#include <GL/glut.h>

enum ModelType{
	Sphere,
	Cube,
	Teapot,
	Custom
};

class NodeModel:public Node{
public:
	NodeModel(ModelType whatType);	//constructor
	ModelType modelType;

	virtual void nodeSpecificCodeDown();
};

#endif