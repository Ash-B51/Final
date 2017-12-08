#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif


#include "Mesh.h"
Mesh *myMesh;


#define X 0
#define Y 1
#define Z 2
#define W 3

//Globals
float camPos[] = {0.5, 0.6, 7.12f};	//where the camera is
float camTarget[] = { 1.8, -0.6, -2};
float box1[]={-28,20,10};

float angle = 0.0f;

float SceneScale = 0.75f;

char* text;
char* c;



//OpenGL functions
//keyboard stuff
//use wasd to move camera position in x-z axis
void keyboard(unsigned char key, int xIn, int yIn)
{
	int mod = glutGetModifiers();
	switch (key)
	{
		case 'q':
		case 27:	//27 is the esc key
			exit(0);
			break;

		//camera conttrol
		case 'd':
			camPos[X] += 0.1f;
			break;

		case 's':
			camPos[Z] += 0.1f;
			break;

		case 'a':
			camPos[X] -= 0.1f;
			break;
		case 'w':
			camPos[Z]-= 0.1f;
			break;


		//object creation 
		case 'z':
			break;
		case 'x':
			break;
		case 'c':
			break;
		case 'v':
			break;
		

	}
}

//use  arrows to rotate camera in X-Y axis
void special(int key, int xIn, int yIn){
	switch (key){
		case GLUT_KEY_DOWN:
			camPos[Y] -= 0.1f;
			camTarget[Y] += 0.2f;
			break;
		case GLUT_KEY_UP:
			camPos[Y] += 0.1f;
			camTarget[Y]-= 0.2f;
			break;
		case GLUT_KEY_LEFT:
			camTarget[X] -= 0.2f;
			break;
		case GLUT_KEY_RIGHT:
			camTarget[X] += 0.2f;
			break;
	}
}


//initial call when screes is made
void init(void)
{
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 100);
	
		
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);

	float ambient[4] = {0.6f, 0.3, 0.1, 1};
	float diffuse[4] = {0.5f,0.5,0.5, 1};
	float spec[4] = {0.1f, 0.1f, 0.1f, 1};
	float pos0[4]={-50,15,25,0};
	float pos1[4]={-100,100,20,0};

	glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,spec);
	glLightfv(GL_LIGHT0, GL_POSITION, pos0);
	
	glLightfv(GL_LIGHT1,GL_AMBIENT,ambient);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,diffuse);
	glLightfv(GL_LIGHT1,GL_SPECULAR,spec);
	glLightfv(GL_LIGHT1, GL_POSITION, pos1);

	
	float shiny = 10; //10, 100

	
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

	

	myMesh = new Mesh();
	myMesh->Init(); 
}


//text bitmap for the heads up display
void HUD(){
	glLoadIdentity();

	text="Score:%d\0",0;
	for(c=text;c!="/0/";c++){
		glutBitmapCharacter(GLUT_STROKE_ROMAN,*c);
	}
	
	

}

/* display function - GLUT display callback function
 *		clears the screen, sets the camera position, draws the mesh
 */
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	HUD();
	box1[X]+=0.5;
	if(box1[X]>80){box1[X]=-28;}

	gluLookAt(camPos[0], camPos[1], camPos[2], camTarget[0], camTarget[1], camTarget[2], 0,1,0);
	
	//scale output to make it better fit
	glScalef(SceneScale, SceneScale, SceneScale);

	glPushMatrix();
		glScalef(0.1,0.1,0.1);
		glTranslatef(box1[X],box1[Y],box1[Z]);
		float shiny1 = 50; //10, 100
		float m_dif1[] = {1, 0, 0, 1.0};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif1);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny1);
		glutSolidCube(9);
		
		glPushMatrix();
			glutSolidCube(6);
			glPushMatrix();
				glutSolidCube(3);
			glPopMatrix();
		glPopMatrix();		
	glPopMatrix();


	glPushMatrix();
		glScalef(0.1,0.1,0.1);
		float shiny = 50; //10, 100
		float m_dif[] = {1, 0, 0, 1.0};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
		myMesh->DrawFloor();
		myMesh->DrawLeft();	
		myMesh->DrawRight();
	glPopMatrix();
	
	glutSwapBuffers();
}




void reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)((w+0.0f)/h), 1, 100);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
}


//allows camera movement
void FPSTimer(int value){ //60fps
	glutTimerFunc(17, FPSTimer, 0);
	glutPostRedisplay();
}


//main function call
int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("3GC3 Shooting Gallery");	//creates the window
	
	//display callback
	glutDisplayFunc(display);

	//keyboard callback
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);


	//resize callback
	glutReshapeFunc(reshape);

	//fps timer callback
	glutTimerFunc(17, FPSTimer, 0);

	init();

	glutMainLoop();				//starts the event glutMainLoop
	return(0);					//return may not be necessary on all compilers

}
