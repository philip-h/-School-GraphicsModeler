/* CS 3GC3 - Texturing sample
 * by R. Teather
 */

#include "Header.h"
#include "ray.h"
#include <stdio.h>
 #include <stdlib.h>
 #include <math.h>

float verts[8][3] = { {-1,-1,1}, {-1,1,1}, {1,1,1}, {1,-1,1}, {-1,-1,-1}, {-1,1,-1}, {1,1,-1}, {1,-1,-1} };
float cols[6][3] = { {1,0,0}, {0,1,1}, {1,1,0}, {0,1,0}, {0,0,1}, {1,0,1} };
float light_pos[] = {5,10,5,1};

float pos[] = {0,1,0};

float camPos[] = {2.5, 2.5, 5};
float angle = 0.0f;
int CURRENT = 0;

double start[] ={0,0,0}, end[]={1,1,1};

//-1 for no shape currently selected.
int selectedShapeID = -1;

/* drawPolygon - takes 4 indices and an array of vertices
 *   and draws a polygon using the vertices indexed by the indices
 */
void drawPolygon(int a, int b, int c, int d, float v[8][3]){
	glBegin(GL_POLYGON);

		glTexCoord2f(0, 0);
		glVertex3fv(v[a]);

		glTexCoord2f(0, 1);
		glVertex3fv(v[b]);

		glTexCoord2f(1, 1);
		glVertex3fv(v[c]);

		glTexCoord2f(1, 0);
		glVertex3fv(v[d]);
	glEnd();
}

/* cube - takes an array of 8 vertices, and draws 6 faces
 *  with drawPolygon, making up a box
 */
void cube(float vertices[8][3])
{
  glColor3f(0, 1, 0);
  drawPolygon(0, 3, 2, 1, vertices);

  glColor3f(0, 1, 0);
  drawPolygon(1, 0, 4, 5, vertices);

  glColor3f(0, 1, 0);
  drawPolygon(5, 1, 2, 6, vertices);

  glColor3f(0, 1, 0);
  drawPolygon(2, 3, 7, 6, vertices);

  glColor3f(0, 1, 0);
  drawPolygon(6, 5, 4, 7, vertices);

  glColor3f(0, 1, 0);
  drawPolygon(4, 0, 3, 7, vertices);
}

/* drawBox - takes centre point, width, height and depth of a box,
 *  calculates its corner vertices, and draws it with the cube function
 */
void drawBox(float* c, float w, float h, float d)
{
	float vertices[8][3] = { {c[0]-w/2, c[1]-h/2, c[2]+d/2},
							 {c[0]-w/2, c[1]+h/2, c[2]+d/2},
							 {c[0]+w/2, c[1]+h/2, c[2]+d/2},
							 {c[0]+w/2, c[1]-h/2, c[2]+d/2},
							 {c[0]-w/2, c[1]-h/2, c[2]-d/2},
							 {c[0]-w/2, c[1]+h/2, c[2]-d/2},
							 {c[0]+w/2, c[1]+h/2, c[2]-d/2},
							 {c[0]+w/2, c[1]-h/2, c[2]-d/2} };

	cube(vertices);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'q':
		case 27:
			exit (0);
			break;
	}
	glutPostRedisplay();
}


/**
 * This method returns an array of intersection point. If the ray intersects the bounding box then
 * return the closest intersection point. If no intersection return empty array.
 */
std::vector<vertex3D> checkCollision(BoundedBox box, Ray ray){
	// printf("orig x: %f, y: %f, z: %f\n", ray.orig.x, ray.orig.y, ray.orig.z);
	// printf("direction x: %f, y: %f, z: %f\n", ray.dir.x, ray.dir.y, ray.dir.z);

	//contains array with our closest intersection point of the ray to our bounded box. If empty no intersection.
	std::vector<vertex3D> intersectPoint;

	//Get the bounding points of our box
	vertex3D min = box.min; //p1
	vertex3D max = box.max; //p2

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
		return intersectPoint; //false no intersection return empty intersect point array
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
		return intersectPoint; //false no intersection return empty intersect point array
	}

	if (tzmin > tmin){
		tmin = tzmin;
	}

	if (tzmax < tmax){
		tmax = tzmax;
	}

	vertex3D point;
	point.x = ray.orig.x + ray.dir.x*tmin;
	point.y = ray.orig.y + ray.dir.y*tmin;
	point.z = ray.orig.z + ray.dir.z*tmin;

	intersectPoint.push_back(point);

	printf("closest point: (%f,%f,%f) \n", point.x, point.y, point.z);

	//true there is an intersection. return the intersect point in array.
	return intersectPoint;
}

//function which preforms intersection test
bool Intersect(int x, int y){
	// printf("%i, %i\n", x, y);

	//allocate matricies memory
	double matModelView[16], matProjection[16];
	int viewport[4];

	//vectors


	//grab the matricies
	glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
	glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	//unproject the values
	double winX = (double)x;
	double winY = viewport[3] - (double)y;

	// get point on the 'near' plane (third param is set to 0.0)
	gluUnProject(winX, winY, 0.0, matModelView, matProjection,
         viewport, &start[0], &start[1], &start[2]);

	// get point on the 'far' plane (third param is set to 1.0)
	gluUnProject(winX, winY, 1.0, matModelView, matProjection,
         viewport, &end[0], &end[1], &end[2]);


	// printf("near point: %f,%f,%f\n", start[0], start[1], start[2]);
	// printf("far point: %f,%f,%f\n", end[0], end[1], end[2]);

	//create new ray with the origin as the mouse click start position
	Ray ray = Ray(start, end);

	//boxes are defined by 2 points p1, p2
	double box1Min[] = {-5, -1, -1};
	double box1Max[] = {-4, 1, 1};
	BoundedBox box1 = BoundedBox(box1Min, box1Max, 2);

	double box2Min[] = {-1,-1,-1};
	double box2Max[] = {1,1,1};
	BoundedBox box2 = BoundedBox(box2Min, box2Max, 1);

	std::vector<BoundedBox> shapes;
	shapes.push_back(box1);
	shapes.push_back(box2);

	double closest_distance = -1;
	for (int i=0; i<shapes.size(); i++){
		std::vector<vertex3D> intersectPoint = checkCollision(shapes[i], ray);
		printf("Intersection: %i ShapeID: %i with p1: (%f,%f,%f) p2: (%f,%f,%f) \n",
					 intersectPoint.empty(),
					 shapes[i].shapeId,
					 shapes[i].min.x, shapes[i].min.y, shapes[i].min.z,
					 shapes[i].max.x, shapes[i].max.y, shapes[i].max.z);

		if (!intersectPoint.empty()){
			selectedShapeID = shapes[i].shapeId;
		}
	}


	return false; //else returns false
}


void special(int key, int x, int y)
{
	/* arrow key presses move the camera */
	switch(key)
	{
		case GLUT_KEY_LEFT:
			camPos[0]-=0.1;
			break;

		case GLUT_KEY_RIGHT:
			camPos[0]+=0.1;
			break;

		case GLUT_KEY_UP:
			camPos[2] -= 0.1;
			break;

		case GLUT_KEY_DOWN:
			camPos[2] += 0.1;
			break;

		case GLUT_KEY_HOME:
			camPos[1] += 0.1;
			break;

		case GLUT_KEY_END:
			camPos[1] -= 0.1;
			break;

	}
	glutPostRedisplay();
}

void init(void)
{	GLuint id = 1;

	glEnable(GLUT_DEPTH);

	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 100);
}


void mouse(int button, int state, int x, int y){
	if(button ==  GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		Intersect(x,y);
	}

}

/* drawAxis() -- draws an axis at the origin of the coordinate system
 *   red = +X axis, green = +Y axis, blue = +Z axis
 */
void drawAxis()
{
	glBegin(GL_POLYGON);
  glColor3f(1, 0, 0);
  glVertex3f(0,0,0);
  glVertex3f(50,0,0);
  glColor3f(0,1,0);
  glVertex3f(0,0,0);
  glVertex3f(0,50,0);
  glColor3f(0,0,1);
  glVertex3f(0,0,0);
  glVertex3f(0,0,50);
  glEnd();

  glBegin(GL_LINES);
  glColor3f(1, 0, 0);
  glVertex3f(0,0,0);
  glVertex3f(50,0,0);
  glColor3f(0,1,0);
  glVertex3f(0,0,0);
  glVertex3f(0,50,0);
  glColor3f(0,0,1);
  glVertex3f(0,0,0);
  glVertex3f(0,0,50);
  glEnd();
}

/* display function - GLUT display callback function
 *		clears the screen, sets the camera position, draws the ground plane and movable box
 */
void display(void)
{
	float origin[3] = {0,0,0};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0, 0,1,0);
	glColor3f(1,1,1);

	drawAxis();

	glBegin(GL_LINES);
		glColor3f(1,0,0);
		glVertex3f(start[0], start[1], start[2]);
		glVertex3f(end[0], end[1], end[2]);
	glEnd();


	//center at origin (0,0,0) with radius 1
	//so p1: (-1,-1,-1) p2: (1,1,1)
	glColor3f(1,1,1);
	glutSolidSphere(1,10,10);

	//You need to glPushMatrix and glPopMatrix for each transformation you draw
	//original p1: (-1,-1,-1) p2: (1,1,1)
	//translation causes p1: (-1-5,-1,-1) p2: (1-5,1,1)
	//p1: (-4,-1,-1) p2: (4,1,1)
	glPushMatrix(); //sets orgin as draw point
	glColor3f(0.5,0,0.55);
	glTranslatef(-5, 0, 0);
	glutSolidCube(1);
	glPopMatrix(); //resets orgin for next object

	if (selectedShapeID == 1){
		float boundedBoxCenter1[] = {0,0,0};
		drawBox(boundedBoxCenter1, 1.0, 1.0 , 1.0);
	}
	if (selectedShapeID == 2){
		float boundedBoxCenter2[] = {-5,0,0};
		drawBox(boundedBoxCenter2, 1.0, 1.0 , 1.0);
	}

	glutSwapBuffers();
}

/* main function - program entry point */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);


	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Ray GL");	//creates the window

	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);

	init();

	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}