#include "Matrix.h"
#include <fstream>

// Create empty matrix with specified sizes
Matrix::Matrix(int iNewRow, int iNewCol) : m_iRow(iNewRow), m_iCol(iNewCol)
{
	m_pfData = new double[m_iRow * m_iCol];
	for (int iRow = 1; iRow <= m_iRow; ++iRow) {
		for (int iCol = 1; iCol <= m_iCol; ++iCol) {
			operator()(iRow, iCol) = 0.0;
		}
	}
}

// Create matrix from file
Matrix::Matrix(std::string sFileName)
{
	// try opening file with specified name
	std::ifstream ifs(sFileName);
	// if file wasn't open
	if (!ifs) {
		// create a substitute matrix (1x1)
		std::cerr << "Error opening file " << sFileName << std::endl;
		*this = Matrix();
		return;
	}
	// count the number of rows and collumns the matrix will have
	m_iRow = m_iCol = 0;
	int iRow = 0, iCol = 1;
	while (ifs) {
		std::string strInput;
		getline(ifs, strInput);

		// only count non empty rows
		if (strInput.compare("")) {
			++iRow;
		}

		// determine the number of collumns (count spaces and tabs) based on the first row
		if (m_iCol == 0) {
			for (unsigned int i = 0; i < strInput.size(); ++i) {
				if (strInput[i] == ' ' || strInput[i] == '	') {
					++iCol;
				}
			}
			m_iCol = iCol;
		}
	}

	// initialize new matrix
	m_iRow = iRow;
	m_pfData = new double[m_iRow * m_iCol];

	// start from the beginning of the file
	iRow = 1; iCol = 1;
	ifs.clear();
	ifs.seekg(0);

	// read all numbers from file and insert them into matrix
	while (ifs) {
		double fNumber;
		ifs >> fNumber;
		operator()(iRow, iCol) = fNumber;

		// move to next uninitialized cell in matrix
		++iCol;
		if (iCol > m_iCol) {
			iCol = 1;
			++iRow;
			if (iRow > m_iRow) {
				return;
			}
		}
	}
}

// Copy constructor
Matrix::Matrix(const Matrix &mOther)
{
	m_iRow = mOther.m_iRow;
	m_iCol = mOther.m_iCol;
	m_pfData = new double[m_iRow * m_iCol];
	for (int iRow = 1; iRow <= m_iRow; ++iRow) {
		for (int iCol = 1; iCol <= m_iCol; ++iCol) {
			operator()(iRow, iCol) = mOther(iRow, iCol);
		}
	}
}

// Destructor
Matrix::~Matrix()
{
	delete[] m_pfData;
}

// Resize matrix to new size and clear all data
void Matrix::Resize(int iNewRow, int iNewCol)
{
	if (iNewRow != m_iRow || iNewCol != m_iCol) {
		delete[] m_pfData;
		m_iRow = iNewRow;
		m_iCol = iNewCol;
		m_pfData = new double[m_iRow * m_iCol];
	}
	for (int iRow = 1; iRow <= m_iRow; ++iRow) {
		for (int iCol = 1; iCol <= m_iCol; ++iCol) {
			operator()(iRow, iCol) = 0.0;
		}
	}
}

// Assigment operator
Matrix& Matrix::operator=(const Matrix &mOther)
{
	if (m_iRow != mOther.m_iRow || m_iCol != mOther.m_iCol) {
		Resize(mOther.m_iRow, mOther.m_iCol);
	}
	for (int iRow = 1; iRow <= m_iRow; ++iRow) {
		for (int iCol = 1; iCol <= m_iCol; ++iCol) {
			operator()(iRow, iCol) = mOther(iRow, iCol);
		}
	}
	return *this;
}

// Matrix access operator
double& Matrix::operator()(int iRow, int iCol)
{
	_ASSERT(iRow > 0 && iRow <= m_iRow);
	_ASSERT(iCol > 0 && iCol <= m_iCol);
	return m_pfData[(iRow-1) * m_iCol + (iCol-1)];
}

double& Matrix::operator()(int iN)
{
	if (m_iRow == 1) {
		return this->operator()(1, iN);
	}
	else if (m_iCol == 1) {
		return this->operator()(iN, 1);
	}
	return this->operator()(iN, iN);
}

// Matrix access const operator
double Matrix::operator()(int iRow, int iCol) const
{
	_ASSERT(iRow > 0 && iRow <= m_iRow);
	_ASSERT(iCol > 0 && iCol <= m_iCol);
	return m_pfData[(iRow - 1) * m_iCol + (iCol - 1)];
}

double Matrix::operator()(int iN) const
{
	if (m_iRow == 1) {
		return this->operator()(1, iN);
	}
	else if (m_iCol == 1) {
		return this->operator()(iN, 1);
	}
	return this->operator()(iN, iN);
}

// Operations defined between two matrices
Matrix Matrix::operator+(const Matrix &mOther)
{
	_ASSERT(m_iRow == mOther.m_iRow && m_iCol == mOther.m_iCol);
	Matrix mNew(*this);
	for (int iRow = 1; iRow <= m_iRow; ++iRow) {
		for (int iCol = 1; iCol <= m_iCol; ++iCol) {
			mNew(iRow, iCol) += mOther(iRow, iCol);
		}
	}
	return mNew;
}

// Operations defined between two matrices
Matrix Matrix::operator-(const Matrix &mOther)
{
	_ASSERT(m_iRow == mOther.m_iRow && m_iCol == mOther.m_iCol);
	Matrix mNew(*this);
	for (int iRow = 1; iRow <= m_iRow; ++iRow) {
		for (int iCol = 1; iCol <= m_iCol; ++iCol) {
			mNew(iRow, iCol) -= mOther(iRow, iCol);
		}
	}
	return mNew;
}

// Operations defined between two matrices
Matrix Matrix::operator+=(const Matrix &mOther)
{
	for (int iRow = 1; iRow <= m_iRow; ++iRow) {
		for (int iCol = 1; iCol <= m_iCol; ++iCol) {
			operator()(iRow, iCol) += mOther(iRow, iCol);
		}
	}
	return *this;
}

// Operations defined between two matrices
Matrix Matrix::operator-=(const Matrix &mOther)
{
	for (int iRow = 1; iRow <= m_iRow; ++iRow) {
		for (int iCol = 1; iCol <= m_iCol; ++iCol) {
			operator()(iRow, iCol) -= mOther(iRow, iCol);
		}
	}
	return *this;
}

// Operations defined between two matrices
Matrix Matrix::operator*(const Matrix &mOther)
{
	_ASSERT(m_iCol == mOther.m_iRow);
	Matrix mNew;
	mNew.Resize(m_iRow, mOther.m_iCol);
	for (int iRow = 1; iRow <= mNew.m_iRow; ++iRow) {
		for (int iCol = 1; iCol <= mNew.m_iCol; ++iCol) {
			for (int i = 1; i <= m_iCol; ++i) {
				mNew(iRow, iCol) += operator()(iRow, i) * mOther(i, iCol);
			}
		}
	}
	return mNew;
}

// Operations defined between matrix and scalar
Matrix operator*(double fScalar, const Matrix &mOther)
{
	Matrix mNew(mOther);
	return mNew * fScalar;
}

// Operations defined between matrix and scalar
Matrix Matrix::operator*(double fScalar)
{
	Matrix mNew(*this);
	for (int iRow = 1; iRow <= m_iRow; ++iRow) {
		for (int iCol = 1; iCol <= m_iCol; ++iCol) {
			mNew(iRow, iCol) *= fScalar;
		}
	}
	return mNew;
}

// Operations defined between matrix and scalar
Matrix Matrix::operator/(double fScalar)
{
	Matrix mNew(*this);
	for (int iRow = 1; iRow <= m_iRow; ++iRow) {
		for (int iCol = 1; iCol <= m_iCol; ++iCol) {
			mNew(iRow, iCol) /= fScalar;
		}
	}
	return mNew;
}

// Operations defined between matrix and scalar
Matrix Matrix::operator*=(double fScalar)
{
	for (int iRow = 1; iRow <= m_iRow; ++iRow) {
		for (int iCol = 1; iCol <= m_iCol; ++iCol) {
			operator()(iRow, iCol) *= fScalar;
		}
	}
	return *this;
}

// Operations defined between matrix and scalar
Matrix Matrix::operator/=(double fScalar)
{
	for (int iRow = 1; iRow <= m_iRow; ++iRow) {
		for (int iCol = 1; iCol <= m_iCol; ++iCol) {
			operator()(iRow, iCol) /= fScalar;
		}
	}
	return *this;
}

// Matrix equality checks
bool Matrix::operator==(const Matrix &mOther)
{
	if (!(m_iRow == mOther.m_iRow && m_iCol == mOther.m_iCol)) {
		return false;
	}
	for (int iRow = 1; iRow <= m_iRow; ++iRow) {
		for (int iCol = 1; iCol <= m_iCol; ++iCol) {
			if (fabs(operator()(iRow, iCol) - mOther(iRow, iCol)) > 1e-7) {
				return false;
			}
		}
	}
	return true;
}

// Matrix equality checks
bool Matrix::operator!=(const Matrix &mOther)
{
	return !operator==(mOther);
}

// Output matrix to desired stream
std::ostream& operator<<(std::ostream &osStream, const Matrix &mOther)
{
	for (int iRow = 1; iRow <= mOther.m_iRow; ++iRow) {
		for (int iCol = 1; iCol <= mOther.m_iCol; ++iCol) {
			osStream << std::to_string(mOther(iRow, iCol)) << " ";
		}
		// don't add new line character after last row
		if (iRow == mOther.m_iRow) {
			break;
		}
		osStream << std::string("\n");
	}
	return osStream;
}

double Matrix::Norm(void)
{
	double sum = 0.0;
	for (int i = 1; i <= m_iRow; ++i) {
		for (int j = 1; j <= m_iCol; ++j) {
			sum += pow(this->operator()(i, j), 2);
		}
	}
	return sqrt(sum);
}

// Switch matrix rows and collumns
Matrix Matrix::Transpose(void)
{
	Matrix mNew;
	mNew.Resize(m_iCol, m_iRow);
	for (int iRow = 1; iRow <= m_iRow; ++iRow) {
		for (int iCol = 1; iCol <= m_iCol; ++iCol) {
			mNew(iCol, iRow) = operator()(iRow, iCol);
		}
	}
	return mNew;
}

Matrix Matrix::SubstituteBackward(const Matrix& mOther)
{
	_ASSERT(mOther.m_iCol == 1);
	_ASSERT(mOther.m_iRow == m_iRow);
	Matrix mNew(mOther);
	for (int i = m_iRow; i > 0; --i) {
		mNew(i, 1) /= operator()(i, i);
		for (int j = 1; j < i; ++j) {
			mNew(j, 1) -= operator()(j, i) * mNew(i, 1);
		}
	}
	return mNew;
}

Matrix Matrix::SubstituteForward(const Matrix& mOther)
{
	_ASSERT(mOther.m_iCol == 1);
	_ASSERT(mOther.m_iRow == m_iRow);
	Matrix mNew(mOther);
	for (int i = 1; i < m_iRow; ++i) {
		for (int j = i + 1; j <= m_iRow; ++j) {
			mNew(j, 1) -= operator()(j, i) * mNew(i, 1);
		}
	}
	return mNew;
}

Matrix Matrix::LU(Matrix *mPermutations)
{
	_ASSERT(m_iRow == m_iCol);
	bool bUsingPivoting = mPermutations != nullptr;
	if (bUsingPivoting) {
		// create an identity matrix
		mPermutations->Resize(m_iRow, m_iCol);
		for (int i = 1; i <= m_iRow; ++i) {
			mPermutations->operator()(i, i) = 1.0;
		}
	}
	Matrix mNew(*this);
	for (int i = 1; i < m_iRow; ++i) {
		double fPivot = mNew(i, i);
		int iSwitchedRow = i;
		if (bUsingPivoting) {
			// find the largest element in collumn
			for (int j = i + 1; j <= m_iRow; ++j) {
				if (mNew(j, i) > fPivot) {
					fPivot = mNew(j, i);
					iSwitchedRow = j;
				}
			}
		}
		// throw exception if pivot is 0
		if (fabs(mNew(iSwitchedRow, i)) < 1e-7) {
			throw 0;
		}
		// switch matrix rows
		if (bUsingPivoting && i != iSwitchedRow) {
			for (int j = 1; j <= m_iCol; ++j) {
				double fTmp = mNew(i, j);
				mNew(i, j) = mNew(iSwitchedRow, j);
				mNew(iSwitchedRow, j) = fTmp;

				fTmp = mPermutations->operator()(i, j);
				mPermutations->operator()(i, j) = mPermutations->operator()(iSwitchedRow, j);
				mPermutations->operator()(iSwitchedRow, j) = fTmp;
			}
		}
		for (int j = i + 1; j <= m_iRow; ++j) {
			mNew(j, i) /= mNew(i, i);
			for (int k = i + 1; k <= m_iRow; ++k) {
				mNew(j, k) -= mNew(j, i) * mNew(i, k);
			}
		}
	}
	// check if last pivot isn't 0
	if (fabs(mNew(m_iRow, m_iCol)) < 1e-7) {
		throw 0;
	}
	return mNew;
}

Matrix Matrix::Inverse(void)
{
	_ASSERT(m_iRow == m_iCol);
	Matrix mP;
	Matrix mLU = LU(&mP);
	Matrix mInverse(m_iRow, m_iCol);

	for (int i = 1; i <= m_iCol; ++i) {
		Matrix mB(m_iRow, 1);
		for (int j = 1; j <= m_iRow; ++j) {
			mB(j, 1) = mP(j, i);
		}
		Matrix mX = mLU.SubstituteBackward(mLU.SubstituteForward(mB));
		for (int j = 1; j <= m_iRow; ++j) {
			mInverse(j, i) = mX(j, 1);
		}
	}

	return mInverse;
}

Matrix Matrix::Identity(int iSize)
{
	Matrix mI(iSize, iSize);
	for (int i = 1; i <= iSize; ++i) {
		mI(i, i) = 1.0;
	}
	return mI;
}
