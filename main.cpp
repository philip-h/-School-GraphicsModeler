/* CS 3GC3 - Texturing sample
 * by R. Teather
 */
#include "Header.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "structs.h"
#include "PMath/PVector.h"
#include "BoundedBox.h"
#include "ray.h"

/* Rotations on the 3 axes */
float xyzRotation[] = {-11, 40, 0};

/* The 6 direction vectors */
PVector3f forward(0,0,-1);
PVector3f back = -forward;
PVector3f up(0,1,0);
PVector3f down = -up;
PVector3f left = up * forward;
PVector3f right = -left;

/* Camera Vector for translations */
PVector3f cam(0.0f, 0.0f, 15.0f);

const float cameraSpeed = 0.75f;

enum LightControl {LIGHT0, LIGHT1};
LightControl lightControl = LIGHT0;
enum ShapeControl {SHAPES, SCENE};
ShapeControl shapeControl = SCENE;

/* LIGHTING */
float light_pos0 [3] = {0, 3, 3};
float amb0[4]  = {1, 1, 1, 1}; 
float diff0[4] = {0, 0, 0, 1}; 
float spec0[4] = {1, 1, 1, 1}; 

float light_pos1 [3] =  {3 , 3, 0};
float amb1[4]  = {1, 1, 1, 1}; 
float diff1[4] = {0, 0, 0, 1}; 
float spec1[4] = {1, 1, 1, 1}; 

/* MATERIALS */
Obsidian lol;

//node ids
int masterID = 0;
int getID(){
	return masterID++;
}

int selectedShapeID;

//sceneGraph
#include "sceneGraph.h"
#include "nodeGroup.h"
#include "nodeModel.h"
#include "nodeTransform.h"
SceneGraph *SG;

/* Shape trasnformation vectors */
Vector3D shapeTransformations;
Node *selectedShapeNode;

//function which will populate a sample graph 
void initGraph(){

	/* Node group for transformations */
	NodeGroup *group;
	/* Initial transformation objects*/
	NodeTransform *translation, *rotation, *scale;
	/* Node model to draw the shape */
	NodeModel *model;
	/* Vector to hold the initial shapes of the scene*/
	std::vector<ModelType> models;
	

	/* Vector to hold the initial positions of each shape in the scene */
	Vector3D initialPosition;
	std::vector<Vector3D> shapeLocations;

	initialPosition.x = 1;
	initialPosition.y = 0;
	initialPosition.z = 2;
	shapeLocations.push_back(initialPosition);
	models.push_back(Cube);

	initialPosition.x = 1;
	initialPosition.y = 1;
	initialPosition.z = 2;
	shapeLocations.push_back(initialPosition);
	models.push_back(Sphere);

	initialPosition.x = 5;
	initialPosition.y = 0;
	initialPosition.z = 2;
	shapeLocations.push_back(initialPosition);
	models.push_back(Cube);

	initialPosition.x = 5;
	initialPosition.y = 1;
	initialPosition.z = 2;
	shapeLocations.push_back(initialPosition);
	models.push_back(Sphere);

	initialPosition.x = 1;
	initialPosition.y = 0;
	initialPosition.z = 5;
	shapeLocations.push_back(initialPosition);
	models.push_back(Cube);

	initialPosition.x = 1;
	initialPosition.y = 1;
	initialPosition.z = 5;
	shapeLocations.push_back(initialPosition);
	models.push_back(Sphere);

	initialPosition.x = 5;
	initialPosition.y = 0;
	initialPosition.z = 6;
	shapeLocations.push_back(initialPosition);
	models.push_back(Cube);

	initialPosition.x = 5;
	initialPosition.y = 1;
	initialPosition.z = 6;
	shapeLocations.push_back(initialPosition);
	models.push_back(Teapot);

	/* All shapes start out with no rotation */
	/*All shapes start with a scale factor of 1*/
	
	/* IMPLEMENT THE STAND LOCATION AND HARD CODE CUBE AND STUFF */
	for (int i = 0; i < models.size(); i++)
	{

		group = new NodeGroup();
		SG->insertChildNodeHere(group);
		SG->goToChild(i);

		/*Apply rotation to each stand*/
		rotation = new NodeTransform(Rotate);
		SG->insertChildNodeHere(rotation);
		SG->goToChild(0);
		/* Apply scaling to each stand*/
		scale = new NodeTransform(Scale);
		SG->insertChildNodeHere(scale);
		SG->goToChild(0);
		/* Apply translation to each stand*/
		translation = new NodeTransform(Translate, shapeLocations[i]);
		SG->insertChildNodeHere(translation);
		SG->goToChild(0);
		/* Draw each stand */
		model = new NodeModel(models[i]);
		SG->insertChildNodeHere(model);

		model = new NodeModel(WireCube);
		SG->insertChildNodeHere(model);

		SG->goToRoot();
	}
}

std::vector<BoundedBox> calcShapeBoundedBoxes()
{
	std::vector<BoundedBox> boundedBoxes;
	std::vector<Node*> translations = SG->getTransformations();
	
	Vector3D p1, p2;
	for (int i = 0; i < translations.size(); i++)
	{
		p1 = translations[i]->getShapePosition();
		p2 = translations[i]->getShapePosition();
		
		p1.x -= 0.5f;
		p1.y += 0.5f;
		p1.z -= 0.5f;
 
		p2.x += 0.5f;
		p2.y -= 0.5f;
		p2.z += 0.5f;

		int shapeId = translations[i]->ID;

		BoundedBox boundedBox(p1,p2,shapeId);
		boundedBoxes.push_back(boundedBox);

	}

	return boundedBoxes;
}

/**
 * This method returns an array of intersection point. If the ray intersects the bounding box then
 * return the closest intersection point. If no intersection return empty array.
 */
int checkCollision(std::vector<BoundedBox> shapes, Ray ray)
{
	double smallestTmin = 100000;
	int selectedShape = -1;

	for (int i=0; i<shapes.size(); i++){
		BoundedBox box = shapes[i];

		//Get the bounding points of our box
		Vector3D min = box.min; //p1
		Vector3D max = box.max; //p2

	 	double tmin = (min.x - ray.orig.x) / ray.dir.x;
		double tmax = (max.x - ray.orig.x) / ray.dir.x;

		if (tmin > tmax){
			std::swap(tmin, tmax);
		}

		double tymin = (min.y - ray.orig.y) / ray.dir.y;
		double tymax = (max.y - ray.orig.y) / ray.dir.y;

		if (tymin > tymax){
			std::swap(tymin, tymax);
		}

		if ((tmin > tymax) || (tymin > tmax)) {
			continue; // no intersection point continue to next bounded box
		}

		if (tymin > tmin) {
			tmin = tymin;
		}

		if (tymax < tmax){
			tmax = tymax;
		}

		double tzmin = (min.z - ray.orig.z) / ray.dir.z;
		double tzmax = (max.z - ray.orig.z) / ray.dir.z;

		if (tzmin > tzmax){
			std::swap(tzmin, tzmax);
		}

		if ((tmin > tzmax) || (tzmin > tmax)){
			continue; // no intersection point continue to next bounded box
		}

		if (tzmin > tmin){
			tmin = tzmin;
		}

		if (tzmax < tmax){
			tmax = tzmax;
		}

		//There is an intersection point. Now we want to check if it is the closest intersection point to our ray.
		//If so then remove the previous intersection point and set it to this new intersection point.
		if (tmin < smallestTmin){
			smallestTmin = tmin;
			selectedShape = box.shapeId;
		}
	}
	//true there is an intersection. return the intersect point in array.
	return selectedShape;
}

void Intersect(int x, int y)
{

	//allocate matricies memory
	double matModelView[16], matProjection[16];
	int viewport[4];

	//grab the matricies
	glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
	glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	//unproject the values
	double winX = (double)x;
	double winY = viewport[3] - (double)y;

	double start[] = {0,0,0};
	double end[] = {1,1,1};

	// get point on the 'near' plane (third param is set to 0.0)
	gluUnProject(winX, winY, 0.0, matModelView, matProjection,
         viewport, &start[0], &start[1], &start[2]);

	// get point on the 'far' plane (third param is set to 1.0)
	gluUnProject(winX, winY, 1.0, matModelView, matProjection,
         viewport, &end[0], &end[1], &end[2]);

	//create new ray with the origin as the mouse click start position
	Ray ray = Ray(start, end);

	std::vector<BoundedBox> shapes = calcShapeBoundedBoxes();

	selectedShapeID = checkCollision(shapes, ray);
	selectedShapeNode = SG->highlightSelectedShape(selectedShapeID);
	
	
	if (selectedShapeNode != 0)
	{	
		printf("shapes\n");
		shapeControl = SHAPES;
	} else 
	{
		printf("scene\n");
		//selectedShapeNode->describeNode();
		shapeControl = SCENE;
	}

	glutPostRedisplay();
}

void drawAxis()
{
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex3f(0,0,0);
	glVertex3f(500,0,0);
	glVertex3f(0,0,0);
	glVertex3f(0,500,0);
	glVertex3f(0,0,0);
	glVertex3f(0,0,500);
	glEnd();
	glEnable(GL_LIGHTING);
}

void drawLight()
{
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 0.0f, 1.0f);
	
	glPushMatrix();
	glTranslatef(light_pos0[0], light_pos0[1], light_pos0[2]);
	glutSolidSphere(0.1, 12, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(light_pos1[0], light_pos1[1], light_pos1[2]);
	glutSolidSphere(0.1, 12, 10);
	glPopMatrix();

	glEnable(GL_LIGHTING);
}

void lockCamera()
{
	/* Lock Rotation */
	if (xyzRotation[0] > -5.0f)
		xyzRotation[0] = -5.0f;
	if (xyzRotation[0] < -45.0f)
		xyzRotation[0] = -45.0f;

	if (xyzRotation[1] < 10.0f)
		xyzRotation[1] = 10.0f;
	if (xyzRotation[1] > 80.0f)
		xyzRotation[1] = 80.0f;

	if (xyzRotation[2] > 30.0f)
		xyzRotation[2] = 30.0f;
	if (xyzRotation[2] < -45.0f)
		xyzRotation[2] = -45.0f;

	/* Lock Translation */

	if (cam.x < -5)
		cam.x = -5;
	if (cam.x >  5)
		cam.x =  5;

	if (cam.y < -1)
		cam.y = -1;
	if (cam.y >  3)
		cam.y =  3;

	if (cam.z > 20)
		cam.z = 20;
	if (cam.z < 12)
		cam.z = 12;

}

/* Moves camera positions along a vector*/
void moveCamera(PVector3f dirVector, float amt)
{
	cam = cam + (dirVector*amt);
	//lockCamera();
}

//callbacks
void keyboard(unsigned char key, int x, int y)
{
	/*Esc to exit the program*/
	if(key == 27)
	{
		exit(0);
	}

	/*CAMERA CONTROL AND SHAPE CONTROL*/
	else if (shapeControl == SCENE)
	{
		/*WASD+Space+z to control the camera*/
		if (key == 'w')
		{
			moveCamera(forward, cameraSpeed);
		} else if (key == 'a')
		{
			moveCamera(left, cameraSpeed);
		} else if (key == 's')
		{
			moveCamera(back, cameraSpeed);
		} else if (key == 'd')
		{
			moveCamera(right, cameraSpeed);
			/* Space bar */
		}  else if (key == 32)
		{
			moveCamera(up, cameraSpeed);
		} else if (key == 'c')
		{
			moveCamera(down, cameraSpeed);
		}
	} else if (shapeControl == SHAPES)
	{
		if (key == 'w')
		{
			selectedShapeNode->describeNode();
		} else if (key == 'a')
		{
			//moveCamera(left, cameraSpeed);
		} else if (key == 's')
		{
			//moveCamera(back, cameraSpeed);
		} else if (key == 'd')
		{
			//moveCamera(right, cameraSpeed);
			/* Space bar */
		}  else if (key == 32)
		{
			//moveCamera(up, cameraSpeed);
		} else if (key == 'c')
		{
			//moveCamera(down, cameraSpeed);
		}
	}

	/*Z to toggle between moving light sources */
	else if (key == 'z')
	{
		switch(lightControl)
		{
			case LIGHT0:
				lightControl = LIGHT1;
				break;
			case LIGHT1:
				lightControl = LIGHT0;
				break;
			default:
				break;
		}
	}

	/* Move Lights */
	else if (key == 'f')
	{
		if (lightControl == LIGHT0)
		{
			light_pos0[0]--;
		} else if (lightControl == LIGHT1)
		{
			light_pos1[0]--;
		}
	} else if (key == 'h')
	{
		if (lightControl == LIGHT0)
		{ 
			light_pos0[0]++;
		} else if (lightControl == LIGHT1)
		{
			light_pos1[0]++;
		}
	} else if (key == 't')
	{
		if (lightControl == LIGHT0)
		{
			light_pos0[2]++;
			light_pos0[0]++;
		} else if (lightControl == LIGHT1)
		{
			light_pos1[2]++;
			light_pos1[0]++;
		}
	} else if (key == 'g')
	{
		if (lightControl == LIGHT0)
		{
			light_pos0[2]--;
			light_pos0[0]--;
		} else if (lightControl == LIGHT1)
		{

			light_pos1[2]--;
			light_pos1[0]--;
		}
	} 

	/* Clear the scene */
	else if (key == 'r')
	{
		SG->clearScene();

	}

	/* Restore original Scene */
	else if (key == 'x')
	{
		initGraph();
	}

	glutPostRedisplay();
}

void special(int key, int x, int y)
{

	/* arrow key presses move the camera */
	switch(key){
		/* Rotate Camera*/
     	case GLUT_KEY_LEFT:
	        xyzRotation[1]--;
	        break;
      	case GLUT_KEY_RIGHT:
       		xyzRotation[1]++;
        	break;
      	case GLUT_KEY_UP:
       		xyzRotation[0]--;
        	break;
      	case GLUT_KEY_DOWN:
	        xyzRotation[0]++;
	        break;
 	}
  	// lockCamera();

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if(button ==  GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		Intersect(x,y);
	}
}

void init(void)
{	
	 /* LIGHTING */
	glEnable(GL_LIGHTING); 
	glEnable(GL_LIGHT0); 
	glEnable(GL_LIGHT1);

	GLuint id = 1;

	glEnable(GL_DEPTH_TEST);


	glClearColor(0.934, 0.898, 0.211, 1);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 100);

	//init our scenegraph
	SG = new SceneGraph();

	//initializing our world
	initGraph();



}


/* display function - GLUT display callback function
 *		clears the screen, sets the camera position, draws the ground plane and movable box
 */
void display(void)
{
	/* Clear the Screen */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/*6 DOF camera controls*/
	glTranslatef(-cam.x,-cam.y,-cam.z);
	
	glRotatef(-xyzRotation[0],1,0,0);
	glRotatef(-xyzRotation[1],0,1,0);
	glRotatef(-xyzRotation[2],0,0,1);

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb0); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0); 
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);

	glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, amb1); 
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diff1); 
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec1);

	 /* MATERIALS */
  	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  lol.m_amb); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  lol.m_diff); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  lol.m_spec); 
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  lol.shiny); 

	drawAxis();
	drawLight();
	//draw the sceneGraph
	SG->draw();

	glutSwapBuffers();
}

/* main function - program entry point */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("SimpleSceneGraph");	//creates the window

	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);

	init();

	glutMainLoop();				//starts the event loop
	return(0);					//return may not be necessary on all compilers
}