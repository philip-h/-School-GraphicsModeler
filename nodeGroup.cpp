#include "nodeGroup.h"
#include <stdio.h> //needed for printf command
#include "Header.h"

NodeGroup::NodeGroup(){
	nodeType = group;
}

void NodeGroup::nodeSpecificCodeDown(){
	glPushMatrix();
}

void NodeGroup::nodeSpecificCodeUp(){
	glPopMatrix();
}

void NodeGroup::describeNode()
{
	printf("This is a group node!\n");
}