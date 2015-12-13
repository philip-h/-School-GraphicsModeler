#ifndef __NODE_H__	//guard against cyclic dependancy
#define __NODE_H__

//Node Class.
//should act as a template class for other sub-types of
//Nodes

#include <vector>
#include "structs.h"
// using namespace std;

extern int getID();

//if you add more derived classes
//add the types here
enum NodeType{
	root,
	group,
	transformation,
	model
};

class Node{
public:
	Node();	//constructor

	NodeType nodeType;
	bool isDrawable;
	bool isTranslation;
	bool isRotation;
	bool isScale;
	int ID;
	int selectedID;
	std::vector<Node*> *children;
	Node* parent;
	int currentChild;

	Vector3D amount3;
	Vector4D amount4;

	void draw();
	virtual void nodeSpecificCodeDown();
	virtual void nodeSpecificCodeUp();
	virtual Vector3D getShapePosition();
	virtual void describeNode();
};

#endif