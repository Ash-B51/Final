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
float camPos[] = {20.0, 0.6, 87.5};	//where the camera is
float camTarget[] = { 19.8, 0, 0};
float box1[]={-20,20,10};
float box2[]={70,5,10};

float angle = 0.0f;

int score=0;

char* text;
char* c;
char buf[50];


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
	glEnable(GL_DEPTH_TEST);
		

	


	//lighting stuff
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
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

	
	float shiny = 10;
	
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
	

	

	myMesh = new Mesh();
	myMesh->Init(); 
}

//handels displaying text
void textToScreen(float x,float y,void*font,char *string){
	char *c;
	char scorei=(char) score;
	string=string+(char) score+'\0';
	glLoadIdentity();
	glRasterPos2f(x,y);
	for(c=string;*c!='\0';c++){
		glutBitmapCharacter(font, *c);
	}
	
	

}


/* display function - GLUT display callback function
 *		clears the screen, sets the camera position, draws the mesh
 */
void display(void)
{
	
	//erase old frame before drawyning new positions 
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	
	//box 1 movement 
	box1[X]+=0.4;
	if(box1[X]>85){box1[X]=-30;}

	//box 2 movement
	box2[X]-=0.4;
	if(box2[X]<-30){box2[X]=80;}

	
	//positioning the camera
	gluLookAt(camPos[0], camPos[1], camPos[2], camTarget[0], camTarget[1], camTarget[2], 0,1,0);
	
	
	//gl material box variable

	float m_dif[] = {1, 0, 0, 1.0};
	float m_amb[] = {0.5f, 0.1f, 0, 1.0};
	float m_spec[] = {0.1f, 0.1f, 0.1f, 1.0};
	float shiny = 5; 
	float m_difb1[]={1,1,1};

	//box 1 
	glPushMatrix();
		glTranslatef(box1[X],box1[Y],box1[Z]);

		//material lighting effects
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);	
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_amb);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

		//drawing cube in realtion to each other
		glutSolidCube(6);
		glPushMatrix();
			glTranslatef(0,0,1.2);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_difb1);
			glutSolidCube(4);
			glPushMatrix();
				glTranslatef(0,0,1.2);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);			
				glutSolidCube(2);
			glPopMatrix();
		glPopMatrix();		
	glPopMatrix();

	//box 2
	glPushMatrix();
		glTranslatef(box2[X],box2[Y],box2[Z]);

		//material lighting effects
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);	
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_amb);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

		//drawing cube in realtion to each other
		glutSolidCube(6);
		glPushMatrix();
			glTranslatef(0,0,1.2);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_difb1);
			glutSolidCube(4);
			glPushMatrix();
				glTranslatef(0,0,1.2);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);			
				glutSolidCube(2);
			glPopMatrix();
		glPopMatrix();		
	glPopMatrix();



	//floor and walls 
	glPushMatrix();
		float m_dif1[] = {0, 0.5, 0, 1.0};
		float m_dif2[] = {0, 0, 0.5, 1.0};
		float m_dif3[] = {0.5, 0, 0, 1.0};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif1);
		myMesh->DrawFloor();
		
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif3);		
		myMesh->DrawLeft();
		
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif2);	
		myMesh->DrawRight();
	glPopMatrix();

	//HUD Stuff

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0,1000,1000,0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();		
			//write to screen
			glPushMatrix();
				//come back to this in a bit
				//int n;
				//n=sprintf(buf,"Score: %3i",score);
				text="SCORE: \0";
				textToScreen(5,20,GLUT_BITMAP_TIMES_ROMAN_24,text);
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	
	
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
