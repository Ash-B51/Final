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

#define X 0
#define Y 1
#define Z 2
#define W 3

//Globals
float camPos[] = {0, 0, 15.42f};	//where the camera is
float camTarget[] = { 0, 0, -2};



float angle = 0.0f;
float SceneScale = 0.5f;

float boxPos[] = {0, 0, 0 };
float boxGamma = 0;
float boxSize = 1;
bool boxPick = false;
bool boxDel = false;

double* m_start = new double[3];
double* m_end = new double[3];

float boxScale = 1;
float flyOuter = 0;
float flyMiddle = 0;
float flyInner = 0;
float p_pos[][2] = {{1, 1}, {-1, -1}, {-1, 1}, {1, -1}};


//OpenGL functions
//keyboard stuff
void keyboard(unsigned char key, int xIn, int yIn)
{
	int mod = glutGetModifiers();
	switch (key)
	{
		case 'q':
		case 27:	//27 is the esc key
			exit(0);
			break;

		case 'x':
			if ( mod == GLUT_ACTIVE_ALT)
				printf("x ALT\n");
			else
				printf("x\n");

		case 'a':
			camTarget[X] -= 0.1f;
			break;

		case 's':
			camTarget[Y] -= 0.1f;
			break;

		case 'd':
			camTarget[X] += 0.1f;
			break;
		case 'w':
			camTarget[Y] += 0.1f;
			break;

		case '1':
			boxPick = !boxPick;
			break;

		case 'j':
			camPos[X] -= 0.1f;
			break;

		case 'k':
			camPos[Y] -= 0.1f;
			break;

		case 'l':
			camPos[X] += 0.1f;
			break;
		case 'i':
			camPos[Y] += 0.1f;
			break;
	}
}

void special(int key, int xIn, int yIn){
	switch (key){
		case GLUT_KEY_DOWN:
			camPos[Y] -= 0.2f;
			camTarget[Y] -+ 0.2f;
			break;
		case GLUT_KEY_UP:
			camPos[Y] += 0.2f;
			camTarget[Y]+= 0.2f;
			break;
		case GLUT_KEY_LEFT:
			camPos[X] -= 0.2f;
			camTarget[X] -= 0.2f;
			break;
		case GLUT_KEY_RIGHT:
			camPos[X] += 0.2f;
			camTarget[X] += 0.2f;
			break;
	}
}
//mouse
void mouse(int btn, int state, int x, int y){
	if (btn == GLUT_LEFT_BUTTON){   // ray cast
		if (state == GLUT_UP){
		}

		if (state == GLUT_DOWN){
			//printf("time for un projection!!!!\n");



			//printf("(%f,%f,%f)----(%f,%f,%f)\n", m_start[0], m_start[1], m_start[2], m_end[0], m_end[1], m_end[2]);


			double matModelView[16], matProjection[16]; 
			int viewport[4]; 

			// get matrix and viewport:
			glGetDoublev( GL_MODELVIEW_MATRIX, matModelView ); 
			glGetDoublev( GL_PROJECTION_MATRIX, matProjection ); 
			glGetIntegerv( GL_VIEWPORT, viewport ); 

			// window pos of mouse, Y is inverted on Windows
			double winX = (double)x; 
			double winY = viewport[3] - (double)y; 

			// get point on the 'near' plane (third param is set to 0.0)
			gluUnProject(winX, winY, 0.0, matModelView, matProjection, 
					viewport, &m_start[0], &m_start[1], &m_start[2]); 

			// get point on the 'far' plane (third param is set to 1.0)
			gluUnProject(winX, winY, 1.0, matModelView, matProjection, 
					viewport, &m_end[0], &m_end[1], &m_end[2]); 

			// now you can create a ray from m_start to m_end
			//printf("(%f,%f,%f)----(%f,%f,%f)\n\n", m_start[0], m_start[1], m_start[2], m_end[0], m_end[1], m_end[2]);
		
		
		
			//----------------------------------------
			// test box - Ray intersection
			//----------------------------------------
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
			

			double A = Rd[0] * Rd[0] + Rd[1] * Rd[1] + Rd[2] * Rd[2];
			double* R0Pc = new double[3];
			R0Pc[0] = R0[0] - boxPos[0];
			R0Pc[1] = R0[1] - boxPos[1];
			R0Pc[2] = R0[2] - boxPos[2];

			double B = 2 * ( R0Pc[0] * Rd[0] + R0Pc[1] * Rd[1] + R0Pc[2] * Rd[2]);
			double C = (R0Pc[0]*R0Pc[0] + R0Pc[1] * R0Pc[1] + R0Pc[2] * R0Pc[2]) - (boxSize * boxSize);

			double discriminent = B*B - 4* A *C;

			if( discriminent < 0)
				printf("no intersection!\n");
			else{
				double t1 = (-B + sqrt(discriminent)) / (2*A);
				double t2 = (-B - sqrt(discriminent)) / (2*A);

				printf("Intersection at t= %f, %f\n", t1, t2);

				boxPick = true;
			}
		}
	}
	
	if (btn == GLUT_RIGHT_BUTTON){  // right-click resets box
		if (state == GLUT_UP){
		}
		
		if (state == GLUT_DOWN){
			boxPick = false;
			boxDel = false;
			boxScale = 1;
			boxScale = 1;
			boxScale = 1;
			flyOuter = 0;
			flyMiddle = 0;
			flyInner = 0;
		}
	}
}
void mouseMotion(int x, int y){
}
void mousePassiveMotion(int x, int y){
}


//initialization
void init(void)
{
	glClearColor(5, 5, 5, 5);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 100);

				//initialize the values
			m_start[0] = 0;
			m_start[1] = 0;
			m_start[2] = 0;

			m_end[0] = 0;
			m_end[1] = 0;
			m_end[2] = 0;
}


/* display function - GLUT display callback function
 *		clears the screen, sets the camera position, draws the ground plane and movable box
 */
void display(void)
{

	
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camPos[0], camPos[1], camPos[2], camTarget[0], camTarget[1], camTarget[2], 0,1,0);
	

	glScalef(SceneScale, SceneScale, SceneScale);

	glBegin(GL_LINES);
		glColor3f(1,1,1);
		glVertex3f(m_start[0], m_start[1], m_start[2]);
		glVertex3f(m_end[0], m_end[1], m_end[2]);
	glEnd();


	if (boxPick){ // ray pick check
		if (boxScale > 0){ // stops drawing once box size is > 0
			boxScale -= 0.06;

			// draws boxes in 4 quadrants at flyOuter; outer box
			for (int i = 0; i < 4; i++){
				glPushMatrix();
				glTranslatef(p_pos[i][0] * flyOuter, p_pos[i][1] * flyOuter, 0);
				glScalef(boxScale, boxScale, boxScale);
				glColor3f(0.5, 0.2, 0.2);
				glRotatef(boxGamma, 0, 0, 1);
				glutSolidCube(6);
				glPopMatrix();
			}
			
			// middle box
			for (int i = 0; i < 4; i++){
				glPushMatrix();
				glTranslatef(p_pos[i][0] * flyMiddle, p_pos[i][1] * flyOuter, 0);
				glScalef(boxScale, boxScale, boxScale);
				glRotatef(boxGamma, 0, 0, 1);
				glColor3f(0.6, 0.6, 0.6);
				glutSolidCube(4);
				glPopMatrix();
			}
			
			// inner box
			for (int i = 0; i < 4; i++){
				glPushMatrix();
				glTranslatef(p_pos[i][0] * flyInner, p_pos[i][1] * flyOuter, 0);
				glScalef(boxScale, boxScale, boxScale);
				glRotatef(boxGamma, 0, 0, 1);
				glColor3f(0.31, 0.06, 0.05);
				glutSolidCube(2);
				glPopMatrix();
			}
			
			flyOuter += 0.7;
			flyMiddle += 0.6;
			flyInner += 0.52;
		}
	} else {	//base box
	
		glScalef(boxScale, boxScale, 1);
		glColor3f(0.5, 0.2, 0.2);
		glutSolidCube(6);
		glPushMatrix();
			glTranslatef(0,0,1.2);
			glColor3f(0.6, 0.6, 0.6);
			glutSolidCube(4);
			glPushMatrix();
				glTranslatef(0,0,1.2);
				glColor3f(0.31, 0.06, 0.05);
				glutSolidCube(2);
			glPopMatrix();
		glPopMatrix();
		
	}
	
	//flush out to single buffer
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

void FPSTimer(int value){ //60fps
	glutTimerFunc(17, FPSTimer, 0);
	glutPostRedisplay();
}

/* main function - program entry point */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(1000, 800);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Box Particle Test");	//creates the window

	//display callback
	glutDisplayFunc(display);

	//keyboard callback
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

	//mouse callbacks
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutPassiveMotionFunc(mousePassiveMotion);

	//resize callback
	glutReshapeFunc(reshape);

	//fps timer callback
	glutTimerFunc(17, FPSTimer, 0);

	init();

	//createOurMenu();


	glutMainLoop();				//starts the event glutMainLoop
	return(0);					//return may not be necessary on all compilers
}
