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

void SceneGraph::goToParent(){
	if (currentNode->parent != 0)
		currentNode = currentNode->parent;
}

//inserts a child node into the current node
void SceneGraph::insertChildNodeHere(Node *node){
	currentNode->children->push_back(node);
}

//deletes the current node, relinking the children as necessary
/* DOES NOT WORK */
void SceneGraph::deleteThisNode(){
	if (currentNode->children->size() != 0)
		return;
	else
	{

		/*Delete this node from the parent's vector*/
		printf("ID of child: %d\n", currentNode->ID);
		//printf("ID of parent before change: %d\n", currentNode->parent->ID);
		goToParent();
		printf("ID of parent: %d\n", currentNode->ID);
		currentNode->children->pop_back();
		deleteThisNode();
	}
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
		return highlightSelectedShapes(id, currentNode);
	}
}

Node* SceneGraph::highlightSelectedShapes(int id, Node* node)
{
	Node *selectedNode;
	if (id == node->ID)
	{
		if (node->children->at(1)->selectedID == node->children->at(1)->ID)
		{
			node->children->at(1)->selectedID = 0;
			return 0;
		}else
		{
			unhighlightAllShapes(rootNode);
			node->children->at(1)->selectedID = node->children->at(1)->ID;
			return node;
		}

	}
	else
	{
		if (node->children->size() > 0)
		{
			for (int i = 0; i < node->children->size(); i++)
			{
				selectedNode = highlightSelectedShapes(id, node->children->at(i));
				if (selectedNode == 0)
				{
					printf("pointer is 0\n");
					return 0;
				}
			}
		}
		return selectedNode;
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