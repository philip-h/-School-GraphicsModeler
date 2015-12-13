#include "sceneGraph.h"
#include "node.h"
#include <stdio.h>

SceneGraph::SceneGraph(){
	rootNode = new Node();
	currentNode = rootNode;
	printf("scene graph init done\n");
}

//Scene Graph Navigation
//resets the current node to the root of the graph
void SceneGraph::goToRoot(){
	currentNode = rootNode;
}

//moves to a child node i
void SceneGraph::goToChild(int i){
	if (i < currentNode->children->size() && i >= 0)
	{
		currentNode = currentNode->children->at(i);
	} else
	{
		printf("child out of range");
	}
}

Node* SceneGraph::getParentOfID(int id){
	getParentOfID(id, rootNode);
	return nodeToReturn2;
}

void SceneGraph::getParentOfID(int id, Node* node)
{
	if (node->children->size() > 0)
	{
		for (int i = 0; i < node->children->size(); i++)
		{
			if (node->children->at(i)->ID == id)
				nodeToReturn2 = node;
			else
				getParentOfID(id, node->children->at(i));
		}
	} 
}

//inserts a child node into the current node
void SceneGraph::insertChildNodeHere(Node *node){
	currentNode->children->push_back(node);
}

//deletes the current node, relinking the children as necessary
/* DOES NOT WORK */
void SceneGraph::deleteNode(Node* node){
	node = getParentOfID(node->ID);//scale
	node = getParentOfID(node->ID);//roatte
	node = getParentOfID(node->ID);//group

	node->children->clear();
}

//draw the scenegraph
void SceneGraph::draw(){
	rootNode->draw();
}

//Clear the scengraph
void SceneGraph::clearScene()
{
	goToRoot();
	currentNode->children->clear();
}

std::vector<Node*> SceneGraph::getTransformations()
{
	goToRoot();
	return getTransformations(currentNode);
}

std::vector<Node*> SceneGraph::getTransformations(Node *node)
{
	if (node->isTranslation)
		translationVector.push_back(node);

	if (node->children->size() > 0)
	{
		for (int i = 0; i < node->children->size(); i++)
		{
			getTransformations(node->children->at(i));
		}
	}

	return translationVector;

}

Node* SceneGraph::highlightSelectedShape(int id)
{
	if (id == -1)
	{
		unhighlightAllShapes(rootNode);
		return 0;
	} else
	{
		goToRoot();
		highlightSelectedShapes(id, currentNode);
		return nodeToReturn;
	}
}

void SceneGraph::highlightSelectedShapes(int id, Node* node)
{
	
	if (id == node->ID)
	{
		

		if (node->children->at(1)->selectedID == node->children->at(1)->ID)
		{
			node->children->at(1)->selectedID = 0;
			nodeToReturn = 0;
		}else
		{
			unhighlightAllShapes(rootNode);
			node->children->at(1)->selectedID = node->children->at(1)->ID;
			nodeToReturn = node;
		}
	}
	else
	{
		if (node->children->size() > 0)
		{
			for (int i = 0; i < node->children->size(); i++)
			{
				highlightSelectedShapes(id, node->children->at(i));
			}
		}
	}
}

int fact (int n)
{
	if (n == 1)
		return 1;
	else
		return fact (n-1) * n;
}

void SceneGraph::unhighlightAllShapes(Node *node)
{
	node->selectedID = 0;

	if (node->children->size() > 0)
	{
		for (int i = 0; i < node->children->size(); i++)
		{
			unhighlightAllShapes(node->children->at(i));
		}
	}
}

void SceneGraph::printTree()
{	
	goToRoot();
	printTree(currentNode);
}

void SceneGraph::printTree(Node *node)
{
	node->describeNode();

	if (node->children->size() > 0)
	{
		for (int i = 0; i < node->children->size(); i++)
		{
			printTree(node->children->at(i));
		}
	}
}

int SceneGraph::getChildrenThisNode()
{
	return currentNode->children->size();
}

Node* SceneGraph::getCurrentNode()
{
	return currentNode;
}