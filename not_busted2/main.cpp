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


#include "Mesh.h"
Mesh *myMesh;


#define X 0
#define Y 1
#define Z 2
#define W 3

//Globals

float box[5][3]={-20,20,10,70,5,10,-20,5,0,70,25,15,-20,10,20}; // box1-5 positions

int score=0;
int timeLimit=900; //60fpsx30sec=900 counter

char* text,*text1;
char* c;
char buf[50];

bool pick[5] = {false, false,false,false,false};

float posg[] = {20,0,86}; // gun position
float rotg[] = {0,0};

double min[5][3] = {-23,17,7,67,2,7,-23,2,-3,67,22,12,-23,7,17};
double max[5][3] = {-17,23,13,73,8,13,-17,8,3,73,28,18,-17,13,23};

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

bool drawLaser=false;
int delayLaser=5;
int lbox;

//particle stuff
float boxScale = 1;
float fly = 0;
float p_pos[][2] = {{1, 1}, {-1, -1}, {-1, 1}, {1, -1}};
float boxAlpha = 1.1;
float boxSize = 1;
bool boxPick[5] = {false, false, false, false, false};
bool boxDel = false;
float* hitBox;

//texture(s)
GLubyte* cannon_tex;
GLuint textures[1];
int width, height, texmax;
float verts[8][3] = { {-1,-1,1}, {-1,1,1}, {1,1,1}, {1,-1,1}, {-1,-1,-1}, {-1,1,-1}, {1,1,-1}, {1,-1,-1} };
float cols[6][3] = { {1,0,0}, {0,1,1}, {1,1,0}, {0,1,0}, {0,0,1}, {1,0,1} };

//states
bool gameStart=false;
bool gameOver=false;



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

	}
}

//use  arrows to rotate camera in X-Y axis
void special(int key, int xIn, int yIn){
}

/* drawPolygon - (taken from 3GC3 LectureCode6) takes 4 indices and an array of vertices
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

/* cube - (taken from 3GC3 LectureCode6) takes an array of 8 vertices, and draws 6 faces
 *  with drawPolygon, making up a box
 */
void cube(float v[8][3])
{
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glColor3fv(cols[1]);
	drawPolygon(0, 3, 2, 1, v);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glColor3fv(cols[2]);
	drawPolygon(1, 0, 4, 5, v);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glColor3fv(cols[3]);
	drawPolygon(5, 1, 2, 6, v);
	
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glColor3fv(cols[4]);
	drawPolygon(2, 3, 7, 6, v);
	
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glColor3fv(cols[5]);
	drawPolygon(6, 5, 4, 7, v);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glColor3fv(cols[0]);
	drawPolygon(4, 0, 3, 7, v);
	
	
}

/* drawBox - (taken from 3GC3 LectureCode6) takes centre point, width, height and depth of a box and rotation,
 *  calculates its corner vertices, and draws it with the cube function
 */
void drawBox(float* c, float w, float h, float d, float* rotg)
{
	
	float vertices[8][3] = { {c[0]-w/2 - rotg[0]/50, c[1]-h/2 - rotg[1]/50, c[2]+d/2},
							 {c[0]-w/2 - rotg[0]/50, c[1]+h/2 - rotg[1]/50, c[2]+d/2},
							 {c[0]+w/2 - rotg[0]/50, c[1]+h/2 - rotg[1]/50, c[2]+d/2},
							 {c[0]+w/2 - rotg[0]/50, c[1]-h/2 - rotg[1]/50, c[2]+d/2}, 
							 {c[0]-w/2 + rotg[0]/50, c[1]-h/2 + rotg[1]/50, c[2]-d/2}, 
							 {c[0]-w/2 + rotg[0]/50, c[1]+h/2 + rotg[1]/50, c[2]-d/2}, 
							 {c[0]+w/2 + rotg[0]/50, c[1]+h/2 + rotg[1]/50, c[2]-d/2},
							 {c[0]+w/2 + rotg[0]/50, c[1]-h/2 + rotg[1]/50, c[2]-d/2}};

	cube(vertices);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/* LoadPPM -- (Taken from 3GC3 LectureCode6) loads the specified ppm file, and returns the image data as a GLubyte 
 *  (unsigned byte) array. Also returns the width and height of the image, and the
 *  maximum colour value by way of arguments
 */
GLubyte* LoadPPM(char* file, int* width, int* height, int* max)
{
	GLubyte* img;
	FILE *fd;
	int n, m;
	int  k, nm;
	char c;
	int i;
	char b[100];
	float s;
	int red, green, blue;
	
	fd = fopen(file, "r");
	fscanf(fd,"%[^\n] ",b);
	if(b[0]!='P'|| b[1] != '3')
	{
		printf("%s is not a PPM file!\n",file); 
		exit(0);
	}
	fscanf(fd, "%c",&c);
	while(c == '#') 
	{
		fscanf(fd, "%[^\n] ", b);
		printf("%s\n",b);
		fscanf(fd, "%c",&c);
	}
	ungetc(c,fd); 
	fscanf(fd, "%d %d %d", &n, &m, &k);


	nm = n*m;

	img = (GLubyte*)malloc(3*sizeof(GLuint)*nm);


	s=255.0/k;


	for(i=0;i<nm;i++) 
	{
		fscanf(fd,"%d %d %d",&red, &green, &blue );
		img[3*nm-3*i-3]=red*s;
		img[3*nm-3*i-2]=green*s;
		img[3*nm-3*i-1]=blue*s;
	}

	*width = n;
	*height = m;
	*max = k;

	return img;
}

void mouse(int btn, int state, int x, int y){
	if (btn == GLUT_LEFT_BUTTON){
		if (state == GLUT_UP){
			gameStart=true;
		}

		if (state == GLUT_DOWN){
			if (boxScale == 1){
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
				
				for (b=0;b<5;b++){
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
							lbox = b;
							score = score +1;
							boxPick[b] = true;
							pick[b] = false;

							rotg[0] = box[b][0] - 10;
							rotg[1] = box[b][1] - 10;
							drawLaser=true;
							break;
						}
				}
			}
		}
	}
}


//initial call when screen is made
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
	

	//texture stuff
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, textures);
	
	//load the texture (cannon)
	cannon_tex = LoadPPM("cannon.ppm", &width, &height, &texmax);

	//setup second texture (using cannon image)
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	//set texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//create a texture using the "tex" array
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, cannon_tex);
	glBindTexture(GL_TEXTURE_2D, 0);
	myMesh = new Mesh();
	myMesh->Init(); 
}

//handles displaying text
void textToScreen(float x,float y,void*font,char *string){
	char *c;
	c=string;
	glLoadIdentity();
	glRasterPos2f(x,y);
	for(c=string;*c!='\0';c++){
		glutBitmapCharacter(font, *c);
	}
}

//handles displaying text
void scoreToScreen(float x,float y,void*font){
	char *c;
	sprintf(buf,"Score: %u",score);
	glLoadIdentity();
	glRasterPos2f(x,y);
	for(c=buf;*c!='\0';c++){
		glutBitmapCharacter(font, *c);
	}
	
	

}

//when called in display will show instrunctions to user on how to play
void gameBootUp(void){
	
	//positioning the camera
    	gluLookAt(	camPos[0], camPos[1], camPos[2], camTarget[0], camTarget[1], camTarget[2], 0.0f, 1.0f,  0.0f);

	
	
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

	//instructions for the game
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0,1000,1000,0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();		
			//write to screen
			glPushMatrix();
				text="Try to hit as many targets as you can within the time limit \0";
				text1="click anywhere to begin\0";
				textToScreen(305,250,GLUT_BITMAP_TIMES_ROMAN_24,text);
				textToScreen(400,298,GLUT_BITMAP_TIMES_ROMAN_24,text1);
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

	


}

//when called in display will start moving the cubes and enable partivles
void gamePlay(void){
	//box 1 movement 
	box[0][0]+=0.7;
	min[0][0]+=0.7;
	max[0][0]+=0.7;
	if(box[0][0]>85){box[0][0]=-20;min[0][0]=-23;max[0][0]=-17;}

	//box 2 movement
	box[1][0]-=0.3;
	min[1][0]-=0.3;
	max[1][0]-=0.3;
	if(box[1][0]<-30){box[1][0]=70;min[1][0]=67;max[1][0]=73;}

	//box 3 movement
	box[2][0]+=0.5;
	min[2][0]+=0.5;
	max[2][0]+=0.5;
	if(box[2][0]>85){box[2][0]=-20;min[2][0]=-23;max[2][0]=-17;}

	//box 4 movement
	box[3][0]-=0.6;
	min[3][0]-=0.6;
	max[3][0]-=0.6;
	if(box[3][0]<-30){box[3][0]=70;min[3][0]=67;max[3][0]=73;}

	//box 5 movement
	box[4][0]+=0.4;
	min[4][0]+=0.4;
	max[4][0]+=0.4;
	if(box[4][0]>85){box[4][0]=-20;min[4][0]=-23;max[4][0]=-17;}
	
	//positioning the camera
    gluLookAt(	camPos[0], camPos[1], camPos[2], camTarget[0], camTarget[1], camTarget[2], 0.0f, 1.0f,  0.0f);
	
	//gl material box variable

	float m_dif[] = {1, 0, 0, 1.0};
	float m_amb[] = {0.5f, 0.1f, 0, 1.0};
	float m_spec[] = {0.1f, 0.1f, 0.1f, 1.0};
	float shiny = 5; 
	float m_difb1[]={1,1,1,1};
	
	//this is the cannon

	glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_amb);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

		glColor3f(0,0,1);

		drawBox(posg, 0.5, 0.5, 3, rotg);
		

	glPopMatrix();

	//box 1 
	if (!boxPick[0]){
		glPushMatrix();
			glTranslatef(box[0][0],box[0][1],box[0][2]);

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
	}
	
	//box 2
	if (!boxPick[1]){
		glPushMatrix();
			glTranslatef(box[1][0],box[1][1],box[1][2]);

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
	}

	//box 3
	if (!boxPick[2]){
		glPushMatrix();
			glTranslatef(box[2][0],box[2][1],box[2][2]);

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
	}
	
	//box 4
	if (!boxPick[3]){
		glPushMatrix();
			glTranslatef(box[3][0],box[3][1],box[3][2]);

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
	}

	//box 5
	if (!boxPick[4]){
		glPushMatrix();
			glTranslatef(box[4][0],box[4][1],box[4][2]);

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
	}


	//floor and walls 
	glPushMatrix();
		float m_dif1[] = {0, 0.5, 0, 1.0};
		float m_dif2[] = {0, 0, 0.5, 1.0};
		float m_dif3[] = {0.5, 0, 0, 1.0};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_amb);
		myMesh->DrawFloor();
		
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif3);	
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_amb);	
		myMesh->DrawLeft();
		
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_amb);
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
				scoreToScreen(417,20,GLUT_BITMAP_TIMES_ROMAN_24);
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

	glMatrixMode(GL_MODELVIEW);


	//particle stuff
	if (boxPick[0] || boxPick[1] || boxPick[2] || boxPick[3] || boxPick[4]){ // ray pick check
		if (boxPick[0]){
			hitBox = box[0];
		} 
		else if(boxPick[1]) {
			hitBox = box[1];
		}
		else if(boxPick[2]) {
			hitBox = box[2];
		}
		else if(boxPick[3]) {
			hitBox = box[3];
		}
		else if(boxPick[4]) {
			hitBox = box[4];
		}
		if (boxScale > 0){ // stops drawing once box size is > 0
			boxScale -= 0.05;
			boxAlpha -= 0.05;
			float box_dif[] = {1, 0, 0, boxAlpha};
			// material lighting effects
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, box_dif);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_amb);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
			
			// draws boxes in 4 quadrants at fly; 
			// outer box
			for (int i = 0; i < 4; i++){
				glPushMatrix();
					glTranslatef(hitBox[X] + (p_pos[i][0] * fly), hitBox[Y] + (p_pos[i][1] * fly), hitBox[Z]);
					glScalef(boxScale, boxScale, boxScale);
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, box_dif);
					glutSolidCube(6);
				glPopMatrix();
			}
			
			// middle box
			for (int i = 0; i < 4; i++){
				glPushMatrix();
					glTranslatef(hitBox[X] + (p_pos[i][0] * (fly - 0.1)), hitBox[Y] + (p_pos[i][1] * (fly - 0.1)), hitBox[Z] + (p_pos[i][1] * (fly - 0.05)));
					glScalef(boxScale, boxScale, boxScale);
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, box_dif);
					glutSolidCube(4);
				glPopMatrix();
			}
			
			// inner box
			for (int i = 0; i < 4; i++){
				glPushMatrix();
					glTranslatef(hitBox[X] + (p_pos[i][0] * (fly + 0.15)), hitBox[Y] + (p_pos[i][1] * (fly + 0.15)), hitBox[Z] + (p_pos[i][1] * (fly + 0.55)));
					glScalef(boxScale, boxScale, boxScale);
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, box_dif);
					glutSolidCube(2);
				glPopMatrix();
			}
			
			fly += 0.5;
		} else {
			if(boxPick[0]){
			//reset effects
			boxPick[0] = false;
			//reset box position
			box[0][0]=-20;min[0][0]=-23;max[0][0]=-17;
			}
			
			if(boxPick[1]){
			//reset effects
			boxPick[1] = false;
			//reset box position
			box[1][0]=70;min[1][0]=67;max[1][0]=73;
			}
			
			if(boxPick[2]){
			//reset effects
			boxPick[2] = false;
			//reset box position
			box[2][0]=-20;min[2][0]=-23;max[2][0]=17;
			}

			if(boxPick[3]){
			//reset effects
			boxPick[3] = false;
			//reset box position
			box[3][0]=70;min[3][0]=67;max[3][0]=73;
			}

			if(boxPick[4]){
			//reset effects
			boxPick[4] = false;
			//reset box position
			box[4][0]=-20;min[4][0]=-23;max[4][0]=17;
			}
			boxDel = false;
			boxScale = 1.0;
			fly = 0.0;
			boxAlpha = 1.1;
		}
	} 
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	timeLimit--;
	if(timeLimit<0){gameOver=true;gameStart=false;}
	

}


//when called in display will reset the game
void gameFinish(void){

	//positioning the camera
    	gluLookAt(	camPos[0], camPos[1], camPos[2], camTarget[0], camTarget[1], camTarget[2], 0.0f, 1.0f,  0.0f);

	
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

	//instructions for the game
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0,1000,1000,0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();		
			//write to screen
			glPushMatrix();
				text="Game Over\0";
				textToScreen(410,250,GLUT_BITMAP_TIMES_ROMAN_24,text);
				scoreToScreen(417,298,GLUT_BITMAP_TIMES_ROMAN_24);
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}




/* display function - GLUT display callback function
 *		clears the screen, sets the camera position, draws the mesh
 */
void display(void){
	//erases old frame before drawing new one
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(!gameStart && !gameOver){gameBootUp();}//will display if game is booted up for the first time
	if(gameStart && !gameOver){gamePlay();}
	if(gameOver){gameFinish();}

	if(drawLaser){
		glBegin(GL_LINES);
		glColor3f(1,0,0);
		glVertex3f(box[lbox][0],box[lbox][1],box[lbox][2]);
		glVertex3f(posg[0], posg[1], posg[2]);
		glEnd();
		delayLaser--;
		if(delayLaser<0){drawLaser=false;delayLaser=5;}			
	}
	//flushes back buffer
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

//emulates first person camera movement
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


	
	glutInitWindowSize(2000, 2000);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("3GC3 Shooting Gallery");	//creates the window
	

	glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	//display callback states
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
