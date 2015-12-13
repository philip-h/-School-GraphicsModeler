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

//sceneGraph
#include "sceneGraph.h"
#include "nodeGroup.h"
#include "nodeModel.h"
#include "nodeTransform.h"
SceneGraph *SG;

//Vector of bounded boxes
std::vector<BoundedBox> boundedBoxes;

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

void calcShapeBoundedBoxes()
{
	boundedBoxes.clear();
	std::vector<Vector3D> trans = SG->getTransformations();

	Vector3D p1, p2;
	for (int i = 0; i < trans.size(); i++)
	{
		p1.x = trans[i].x - 0.5f;
		p1.y = trans[i].y + 0.5f;
		p1.z = trans[i].z - 0.5f;

		p2.x = trans[i].x + 0.5f;
		p2.y = trans[i].y - 0.5f;
		p2.z = trans[i].z + 0.5f;

		BoundedBox boundedBox(p1,p2,0);
		boundedBoxes.push_back(boundedBox);

	}
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

	/*WASD+Space+z to control the camera*/
	else if (key == 'w')
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
	calcShapeBoundedBoxes();

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

	init();

	glutMainLoop();				//starts the event loop
	return(0);					//return may not be necessary on all compilers
}