#include "nodeTransform.h"
#include <stdio.h> //needed for printf command
#include "Header.h"

//for gltranslate and gl scale
NodeTransform::NodeTransform(TransformType whatType, Vector3D vec3){
	nodeType = transformation;
	transformationType = whatType;
	amount3 = vec3;
	if (transformationType == Rotate)
	{
		isRotation = true;
	} else if (transformationType == Translate)
	{
		isTranslation = true;
	} else if (transformationType == Scale)
	{
		isScale = true;
	}
}

//for gl rotate
NodeTransform::NodeTransform(TransformType whatType, Vector4D vec4){
	nodeType = transformation;
	transformationType = whatType;
	amount4 = vec4;
	if (transformationType == Rotate)
	{
		isRotation = true;
	} else if (transformationType == Translate)
	{
		isTranslation = true;
	} else if (transformationType == Scale)
	{
		isScale = true;
	}
}

NodeTransform::NodeTransform(TransformType whatType)
{
	transformationType = whatType;

	if (transformationType == Rotate)
	{
		amount4.w = 0;
		amount4.x = 0;
		amount4.y = 0;
		amount4.z = 0;
		isRotation = true;
	} else if (transformationType == Translate)
	{
		amount3.x = 0;
		amount3.y = 0; 
		amount3.z = 0;
		isTranslation = true;
	} else if (transformationType == Scale)
	{
		amount3.x = 1;
		amount3.y = 1;
		amount3.z = 1;
		isRotation = true;
	}
}

void NodeTransform::nodeSpecificCodeDown(){
	switch (transformationType){
	case Translate:
		glTranslatef(amount3.x, amount3.y, amount3.z);
		break;
	case Rotate:
		glRotatef(amount4.w, amount4.x, amount4.y, amount4.z);
		break;
	case Scale:
		glScalef(amount3.x, amount3.y, amount3.z);
		break;
	}
}

Vector3D NodeTransform::getShapePosition() {
	if (transformationType == Translate)
	{
		return amount3;
	} else
	{
		Vector3D empty;
		empty.x = -1; 
		empty.y = -1;
		empty.z = -1;
		return empty;
	}
}

void NodeTransform::describeNode()
{
	printf("This is a transform node(ID %d): transformation: ", ID);
	switch (transformationType){
	case Translate:
		printf("Translate by (%.2f,%.2f,%.2f)\n", amount3.x, amount3.y, amount3.z);
		break;
	case Rotate:
		printf("Rotate by (%.2f,%.2f,%.2f,%.2f)\n", amount4.w, amount4.x, amount4.y, amount4.z);
		break;
	case Scale:
		printf("Scale by (%.2f,%.2f,%.2f)\n", amount3.x, amount3.y, amount3.z);
		break;
	}
}
