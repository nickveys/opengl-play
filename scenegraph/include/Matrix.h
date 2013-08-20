#ifndef MATRIX_H_
#define MATRIX_H_

/**
 * 4x4 matrix class.
 */
class Matrix {
	public:
		Matrix() { for(int i = 0; i <= 15; i++) m[i] = 0.0f; }
		Matrix(float a0,  float a1,  float a2,  float a3, 
               float a4,  float a5,  float a6,  float a7,
               float a8,  float a9,  float a10, float a11,
               float a12, float a13, float a14, float a15);
		Matrix(Matrix& p) { for(int i = 0; i <= 15; i++) m[i] = p.m[i]; }
		~Matrix();

		/* Rotate the current matrix by the given xyz deltas */
		void rotateBy(float dx, float dy, float dz);

		void identity();

		Matrix& operator = (Matrix& om);

		void print();

		float m[16];
};

#endif /*MATRIX_H_*/
