#ifndef APR_MATRIX
#define APR_MATRIX

#include <iostream>
#include <string>

class Matrix {
public:
	int m_iRow;
	int m_iCol;
	double *m_pfData;
public:
	// Create empty matrix with specified sizes
	Matrix(int iNewRow = 1, int iNewCol = 1);
	// Create matrix from file
	Matrix(std::string sFileName);
	// Copy constructor
	Matrix(const Matrix &mOther);
	// Destructor
	~Matrix();
	// Resize matrix to new size and clear all data
	void Resize(int iNewRow, int iNewCol);
	// Assigment operator
	Matrix& operator=(const Matrix &mOther);

	// Matrix access operator
	double& operator()(int iRow, int iCol);
	double& operator()(int iN);
	// Matrix access const operator
	double operator()(int iRow, int iCol) const;
	double operator()(int iN) const;

	// Operations defined between two matrices
	Matrix operator+(const Matrix &mOther);
	Matrix operator-(const Matrix &mOther);
	Matrix operator+=(const Matrix &mOther);
	Matrix operator-=(const Matrix &mOther);
	Matrix operator*(const Matrix &mOther);

	// Operations defined between matrix and scalar
	friend Matrix operator*(double fScalar, const Matrix &mOther);
	Matrix operator*(double fScalar);
	Matrix operator/(double fScalar);
	Matrix operator*=(double fScalar);
	Matrix operator/=(double fScalar);

	// Matrix equality checks
	bool operator==(const Matrix &mOther);
	bool operator!=(const Matrix &mOther);

	// Output matrix to desired stream
	friend std::ostream& operator<<(std::ostream &osStream, const Matrix &mOther);

	double Norm(void);
	Matrix Transpose(void);
	Matrix SubstituteBackward(const Matrix& mOther);
	Matrix SubstituteForward(const Matrix& mOther);
	Matrix LU(Matrix *mPermutations=nullptr);
	Matrix Inverse(void);
public:
	static Matrix Identity(int iSize);
};

#endif