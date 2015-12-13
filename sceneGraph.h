#ifndef __SCENEGRAPH_H__	//guard against cyclic dependancy
#define __SCENEGRAPH_H__

#include <vector>
#include "node.h"
#include "structs.h"

class SceneGraph{
public:
	SceneGraph();	//constructor

	//Scene Graph Navigation
	void goToRoot();
	void goToChild(int i);
	void goToParent();
	void insertChildNodeHere(Node *node);
	void deleteThisNode();					
	//Scene Graph Draw
	void draw();
	void clearScene();
	std::vector<Vector3D> getTransformations();
	void printTree();

private:
	void printTree(Node *node);
	Node *currentNode;
	Node *rootNode;
	std::vector<Vector3D> translationVector;
	std::vector<Vector3D> getTransformations(Node *node);
};

#endif