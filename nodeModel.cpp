#include "nodeModel.h"
#include <stdio.h> 
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
		glutSolidSphere(0.6, 12, 10);
		break;
	case Cube:
		glutSolidCube(1);
		break;
	case WireCube:
		if (selectedID == ID)
		{
			glDisable(GL_LIGHTING);
			glColor3f(0.0f, 1.0f, 0.0f);
			glutWireCube(1);
			glEnable(GL_LIGHTING);
		} 
		break;
	case Teapot:
		glutSolidTeapot(.67);
		break;
	case Custom:
		//own mesh thing
		break;
	}
	
}

void NodeModel::describeNode()
{
	printf("This is a model node: drawing a ");
	switch (modelType){
	case Sphere:
		printf("Sphere\n");
		break;
	case Cube:
		printf("Cube\n");
		break;
	case WireCube:
		printf("WireCube\n");
		break;
	case Teapot:
		printf("Teapot\n");
		break;
	case Custom:
		//own mesh thing
		printf("Custom Shape\n");
		break;
	}

}