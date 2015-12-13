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
	std::vector<Node*> getTransformations();
	void printTree();
	Node* findNodeById(int id);

private:
	void printTree(Node *node);
	Node *currentNode;
	Node *rootNode;
	std::vector<Node*> translationVector;
	std::vector<Node*> getTransformations(Node *node);
	Node* findNodeById(int id, Node *node);
};

#endif