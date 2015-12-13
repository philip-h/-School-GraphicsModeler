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
	Node* highlightSelectedShape(int id);

private:
	void printTree(Node *node);
	Node *currentNode;
	Node *rootNode;
	Node *nodeToReturnFromHighlight;
	std::vector<Node*> translationVector;
	std::vector<Node*> getTransformations(Node *node);
	void highlightSelectedShapes(int id, Node *node);
	void unhighlightAllShapes(Node *node);
};

#endif