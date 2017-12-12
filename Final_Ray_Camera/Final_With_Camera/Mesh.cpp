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
#include <math.h>
#include "Mesh.h"

Mesh::Mesh(){}

void Mesh::Init(){
	

	numOfVert = 0;
	numOfFace = 0;
	holder=-1;

	

}




void Mesh::DrawFloor(){

	glBegin(GL_QUADS);
			//v1
			glNormal3f(0,1.0,0);
			glVertex3f(-25.0,-3.0,-20.0);
			//v2
			glNormal3f(0,1.0,0);
			glVertex3f(75.0,-3.0,-20.0);
			//v3	
			glNormal3f(0,1.0,0);
			glVertex3f(75.0,-3.0,80.0);
			//v4
			glNormal3f(0,1.0,0);
			glVertex3f(-25.0,-3.0,80.0);
			
		glEnd();

	
	
}


void Mesh::DrawLeft(){

	glBegin(GL_QUADS);
			//v1
			glNormal3f(-1.0,0,0);
			glVertex3f(-25.0,-3.0,-20.0);
			//v2
			glNormal3f(-1.0,0,0);
			glVertex3f(-25.0,-3.0,80.0);
			//v3	
			glNormal3f(-1.0,0,0);
			glVertex3f(-25.0,80.0,80.0);
			//v4
			glNormal3f(-1.0,0,0);
			glVertex3f(-25.0,80.0,-20.0);
			
		glEnd();

	
	
}


void Mesh::DrawRight(){


	glBegin(GL_QUADS);
			//v1
			glNormal3f(-1.0,0,0);
			glVertex3f(75.0,-3.0,-20.0);
			//v2
			glNormal3f(-1.0,0,0);
			glVertex3f(75.0,-3.0,80.0);
			//v3	
			glNormal3f(-1.0,0,0);
			glVertex3f(75.0,80.0,80.0);
			//v4
			glNormal3f(-1.0,0,0);
			glVertex3f(75.0,80.0,-20.0);
			
		glEnd();

	
	
}






	






