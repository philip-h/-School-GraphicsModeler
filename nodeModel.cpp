#include "nodeModel.h"
#include <stdio.h> //needed for printf command
#include "Header.h"

NodeModel::NodeModel(ModelType whatType){	//constructor
	nodeType = model;
	modelType = whatType;
	isDrawable = true;
}

//as the node moves down through it, we want to perform down action
//which in this case means drawing the model
void NodeModel::nodeSpecificCodeDown(){
	
	switch (modelType){
	case Sphere:
		glColor3f(1.0f, 0.0f, 0.0f);
		glutSolidSphere(0.5, 12, 10);
		break;
	case Cube:
		glColor3f(0.84, 0.65, 0.23);
		glutSolidCube(1);
		break;
	case Teapot:
		glColor3f(0.0f, 0.0f, 0.0f);
		glutSolidTeapot(.67);
		break;
	case Custom:
		//own mesh thing
		break;
	}
	
}