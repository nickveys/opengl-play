#include <stdio.h>
#include <gl.h>

#include "Matrix.h"

Matrix::Matrix(float a0,  float a1,  float a2,  float a3, 
               float a4,  float a5,  float a6,  float a7,
               float a8,  float a9,  float a10, float a11,
               float a12, float a13, float a14, float a15){
	m[0] = a0; m[4] = a4; m[8]  = a8;  m[12] = a12;
	m[1] = a1; m[5] = a5; m[9]  = a9;  m[13] = a13;
	m[2] = a2; m[6] = a6; m[10] = a10; m[14] = a14;
	m[3] = a3; m[7] = a7; m[11] = a11; m[15] = a15;
}

Matrix::~Matrix() {
}

void Matrix::rotateBy(float dx, float dy, float dz) {
	/* Compose rotation (from Sample 13) */
	glPushMatrix();
	   glLoadIdentity();
	   
	   // put new x,y,and z components of "rotation" together on the stack
	   glMultMatrixf(m);
	   glRotated(dx, 1, 0, 0);
	   glRotated(dy, 0, 1, 0);
	   glRotated(dz, 0, 0, 1);
	   
	   glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glPopMatrix();
}

void Matrix::identity() {
	for(int i = 0; i <= 15; i++) m[i] = 0.0f;
	m[0] = m[5] = m[10] = m[15] = 1.0f;
}

Matrix& Matrix::operator = (Matrix& om) {
	for(int i = 0; i <= 15; i++) this->m[i] = om.m[i];
	return *this;
}

void Matrix::print() {
	printf("[%5.2f %5.2f %5.2f %5.2f]\n", m[0], m[4], m[8],  m[12]);
	printf("[%5.2f %5.2f %5.2f %5.2f]\n", m[1], m[5], m[9],  m[13]); 
	printf("[%5.2f %5.2f %5.2f %5.2f]\n", m[2], m[6], m[10], m[14]); 
	printf("[%5.2f %5.2f %5.2f %5.2f]\n", m[3], m[7], m[11], m[15]); 
}
