#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif
#include <stdio.h>
#include <stdlib.h>


#include "Mesh.h"
Mesh *myMesh;


#define X 0
#define Y 1
#define Z 2
#define W 3

//Globals

float box1[]={-20,20,10};
float box2[]={70,5,10};

int score=0;

char* text;
char* c;
char buf[50];

bool pick[2] = {false, false};
float posg[] = {0.75,-0.25,6};
float rotg[] = {0, 0, 0};

double min[2][3] = {-23,17,7,67,2,7};
double max[2][3] = {-17,23,13,73,8,13};

double tnear;
double tfar;
double t1;
double t2;
double d1;
double d2;
int b;
int i;
double* m_start = new double[3];
double* m_end = new double[3];

float oh;
float ang;
float hyp;
float guna;
float mov=0;

float camPos[] = {20.0, 0.6, 87.5};	//where the camera is
float camTarget[] = { 20.0, 20.0, 0};

// angle of rotation for the camera direction
float angle = 0.0f;

// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;

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

void mouse(int btn, int state, int x, int y){
	if (btn == GLUT_LEFT_BUTTON){
		if (state == GLUT_UP){
		}

		if (state == GLUT_DOWN){

			double matModelView[16], matProjection[16]; 
			int viewport[4]; 

			glGetDoublev( GL_MODELVIEW_MATRIX, matModelView ); 
			glGetDoublev( GL_PROJECTION_MATRIX, matProjection ); 
			glGetIntegerv( GL_VIEWPORT, viewport ); 

			double winX = (double)x; 
			double winY = viewport[3] - (double)y; 

			gluUnProject(winX, winY, 0.0, matModelView, matProjection, 
					viewport, &m_start[0], &m_start[1], &m_start[2]); 

			gluUnProject(winX, winY, 1.0, matModelView, matProjection, 
					viewport, &m_end[0], &m_end[1], &m_end[2]); 

			double* R0 = new double[3];
			double* Rd = new double[3];

			double xDiff = m_end[0] - m_start[0];
			double yDiff = m_end[1] - m_start[1];
			double zDiff = m_end[2] - m_start[2];

			double mag = sqrt(xDiff*xDiff + yDiff*yDiff + zDiff*zDiff);
			R0[0] = m_start[0];
			R0[1] = m_start[1];
			R0[2] = m_start[2];

			Rd[0] = xDiff / mag;
			Rd[1] = yDiff / mag;
			Rd[2] = zDiff / mag;
                        

                        // slab algorith used to select objects. gn is a number that is used to keep track of which
			// object has been selected. pick is a bool used to choose an object.
			
			for (b=0;b<2;b++){
				tnear = -INFINITY;
				tfar = INFINITY;
				for (i=0; i<3; i++){
					if (Rd[i] == 0.0) {
						if (R0[i] < min[b][i] or R0[i]> max[b][i]){
							pick[b] = true;
							break;
						}
					}
					else{

						t1 = (min[b][i] - R0[i]) / (Rd[i]);
						t2 = (max[b][i]  - R0[i]) / (Rd[i]);
						if (t1>t2){
							d1 = t1;
							d2 = t2;
							t1 = d2;
							t2 = d1;
						}
						if (t1>tnear){
							tnear = t1; 
						}
						if (t2<tfar){
							tfar = t2;
						}
						if (tnear > tfar){
							pick[b] = true;
							
							break;
						}
						if (tfar < 0){
							pick[b] = true;
							
							break; 
						}
					}
					}
					pick[b] = !pick[b];
					if (pick[b] == true){
						
						
						score = score +1;
						pick[b] = false;
						printf("(%i)\n\n", score);
						break;
					}
				
			
			}
		}
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
void display(void){
    
    
	//erase old frame before drawyning new positions 
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	
	//box 1 movement 
	box1[X]+=0.4;
	min[0][0]+=0.4;
	max[0][0]+=0.4;
	if(box1[X]>85){box1[X]=-20;min[0][0]=-23;max[0][0]=-17;}

	//box 2 movement
	box2[X]-=0.4;
	min[1][0]-=0.4;
	max[1][0]-=0.4;
	if(box2[X]<-30){box2[X]=70;min[1][0]=67;max[1][0]=73;}

	
	//positioning the camera
    gluLookAt(	camPos[0], camPos[1], camPos[2], camTarget[0], camTarget[1], camTarget[2], 0.0f, 1.0f,  0.0f);
	
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

void mouseMove(int x, int y) {
    float xOrigin = 20;
    float yOrigin = 0.6;
    
    float deltaAngleY;
    
    // update deltaAngle
    deltaAngle = (x - xOrigin) * 0.001f;
    deltaAngleY = (y - yOrigin) * 0.001f;
    
    // update camera's direction
    camTarget[0] = 20 * (1 + sin(angle + deltaAngle));
    camTarget[1] = 20* (1 + -sin (angle + deltaAngleY));
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
    
    //mouse callback
   
	glutMouseFunc(mouse);
     glutPassiveMotionFunc(mouseMove);
    
	//resize callback
	glutReshapeFunc(reshape);

	//fps timer callback
	glutTimerFunc(17, FPSTimer, 0);

	init();

	glutMainLoop();				//starts the event glutMainLoop
	return(0);					//return may not be necessary on all compilers

}
