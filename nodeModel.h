#ifndef __NODEMODEL_H__	//guard against cyclic dependancy
#define __NODEMODEL_H__

#include "node.h"
#include "Header.h"

enum ModelType{
	Sphere,
	Cube,
	Teapot,
	WireCube,
	Custom
};

class NodeModel:public Node{
public:
	NodeModel(ModelType whatType);	//constructor
	ModelType modelType;

	virtual void nodeSpecificCodeDown();
	virtual void describeNode();
};

#endif