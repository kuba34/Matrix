#ifndef MATRIX_HH
#define MATRIX_HH

#include <vector>
#include <cstdlib>

template <typename T>
class Matrix {

    T **matrix;
    size_t rows;
    size_t cols;

public:
    Matrix() = delete;
    explicit Matrix(size_t rows, size_t cols);
    explicit Matrix(size_t n);
    Matrix(const Matrix&);
    Matrix(Matrix&&);

    void ones();
    void zeros();
    void fill(const T& value);

    size_t getRows() const;
    size_t getCols() const;

    Matrix& operator+=(const Matrix& M);
    Matrix& operator=(const Matrix& M);
    Matrix operator+(const Matrix& M);
    Matrix& operator*=(const Matrix& M);
    Matrix& operator*=(const T& s);
    Matrix operator*(const Matrix& M);
    Matrix operator*(const T& s);
    
    const T* operator[](size_t row) const;
    T* operator[](size_t row);
    
    std::vector<T> get_row(size_t row) const;
    std::vector<T> get_column(size_t col) const;

    Matrix<T> orthogonal();

    virtual ~Matrix();

    //TODO
    T det();
    T trace();
    void insert_row(size_t n, T* row);
    void insert_row(size_t n, std::vector<T> row);
    void insert_column(size_t n, T* row);
    void insert_column(size_t n, std::vector<T> row);
    void swap_rows(size_t n, size_t m);
    void swap_columns(size_t n, size_t m);
    void resize(size_t n, size_t m);
};

#include <stdexcept>
#include <iostream>
#include <iomanip>

template <typename T>
Matrix<T> Matrix<T>::orthogonal()
{
    Matrix<T> M(cols,rows);
    for(size_t i=0;i<rows;i++) {
        for(size_t j=0;j<cols;j++)
            M[j][i]=matrix[i][j];
    }
    return M;
}

template <typename T>
Matrix<T>::Matrix(size_t rows, size_t cols): rows(rows), cols(cols)
{
    matrix=new T*[rows];
    for(size_t i=0;i<rows;i++) {
        matrix[i]=new T[cols];
    }
    for(size_t i=0;i<rows;i++) {
        for(size_t j=0;j<cols;j++)
            matrix[i][j]=static_cast<T>(0);
    }
}

template <typename T>
Matrix<T>::Matrix(size_t n): Matrix(n,n) {}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& M): rows(M.rows), cols(M.cols)
{
    matrix=new T*[rows];
    for(size_t i=0;i<rows;i++) {
        matrix[i]=new T[cols];
    }
    for(size_t i=0;i<rows;i++) {
        for(size_t j=0;j<cols;j++)
            matrix[i][j]=M[i][j];
    }
}

template <typename T>
Matrix<T>::Matrix(Matrix<T>&& M): matrix(M.matrix), rows(M.rows), cols(M.cols)
{
    M.rows=0;
    M.cols=0;
    M.matrix=nullptr;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& M)
{
    if(this!=&M) {
        rows=M.getRows();
        cols=M.getCols();

        matrix=new T*[rows];
        for(size_t i=0;i<rows;i++) {
            matrix[i]=new T[cols];
        }
        for(size_t i=0;i<rows;i++) {
            for(size_t j=0;j<cols;j++)
                matrix[i][j]=M[i][j];
        }
    }
    return *this;
}

template <typename T>
T* Matrix<T>::operator[](size_t r)
{
    return matrix[r];
}

template <typename T>
const T* Matrix<T>::operator[](size_t r) const
{
    return matrix[r];
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix& M)
{
    if(rows!=M.getRows() || cols!=M.getCols())
        throw std::invalid_argument("Matrixes have different dimentions.");

    Matrix<T> N(rows,cols);

    for(size_t i=0;i<rows;i++) {
        for(size_t j=0;j<cols;j++)
            N[i][j]=matrix[i][j]+M[i][j];
    }

    return N;
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix& M)
{
    if(rows!=M.getRows() || cols!=M.getCols())
        throw std::invalid_argument("Matrixes have different dimentions.");

    for(size_t i=0;i<rows;i++) {
        for(size_t j=0;j<cols;j++)
            matrix[i][j]+=M[i][j];
    }

    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const T& s)
{
    Matrix<T> m(rows,cols);

    for(size_t i=0;i<rows;i++) {
        for(size_t j=0;j<cols;j++)
            m[i][j]=matrix[i][j]*s;
    }

    return m;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix& M)
{
    if(M.getRows()!=cols)
        throw std::domain_error("Matrixes cannot be multiply!");

    Matrix<T> C(rows,M.getCols());
    T s;

    for(size_t i=0;i<rows;i++) {
        for(size_t j=0;j<M.getCols();j++) {
            s = 0;
            for(size_t k=0;k<M.getRows();k++) s += matrix[i][k] * M[k][j];
            C[i][j] = s;
        }
    }

    return C;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const T& s)
{
    for(size_t i=0;i<rows;i++) {
        for(size_t j=0;j<cols;j++)
            matrix[i][j]*=s;
    }

    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix& M)
{
    if(M.getRows()!=cols)
        throw std::domain_error("Matrixes cannot be multiply!");

    T s;
    for(size_t i=0;i<rows;i++) {
        for(size_t j=0;j<M.getCols();j++) {
            s = 0;
            for(size_t k=0;k<M.getRows();k++) s += matrix[i][k] * M[k][j];
            matrix[i][j] = s;
        }
    }

    return *this;
}

template <typename T>
std::vector<T> Matrix<T>::get_row(size_t r) const
{
    if(r>=rows)
        throw std::out_of_range("Index out of bound");
    
    std::vector<T> Row(matrix[r],matrix[r]+cols);

    return Row;
}

template <typename T>
std::vector<T> Matrix<T>::get_column(size_t col) const
{
    if(col>=cols)
        throw std::out_of_range("Index out of bound");

    std::vector<T> c;
    c.reserve(cols);

    for(size_t i=0;i<rows;i++) {
        c.push_back(matrix[i][col]);
    }

    return c;
}

template <typename T>
void Matrix<T>::ones()
{
    if(rows!=cols)
        throw std::domain_error("Only for square matrix.");

    for(size_t i=0;i<rows;i++) {
        for(size_t j=0;j<cols;j++)
            matrix[i][j]=static_cast<T>(0);
    }
    for(size_t i=0;i<rows;i++) 
        matrix[i][i]=static_cast<T>(1);
}

template <typename T>
void Matrix<T>::zeros()
{
    for(size_t i=0;i<rows;i++) {
        for(size_t j=0;j<cols;j++)
            matrix[i][j]=static_cast<T>(0);
    }
}

template <typename T>
void Matrix<T>::fill(const T& value)
{
    for(size_t i=0;i<rows;i++) {
        for(size_t j=0;j<cols;j++)
            matrix[i][j]=value;
    }
}

template <typename T>
size_t Matrix<T>::getRows() const
{
    return rows;
}

template <typename T>
size_t Matrix<T>::getCols() const
{
    return cols;
}

template <typename T>
void Matrix<T>::swap_rows(size_t n, size_t m)
{
    if(n>rows||n<0||m>rows||m<0)
        throw std::invalid_argument("Argument out of bound");

    auto tmp1=matrix[n];
    auto tmp2=matrix[m];

    matrix[n]=tmp2;
    matrix[m]=tmp2;
}

template <typename T>
Matrix<T>::~Matrix()
{
    for(size_t i=0;i<rows;i++)
        delete [] matrix[i];
    delete [] matrix;
}

template <typename T>
std::ostream& operator<<(std::ostream& Out, Matrix<T>& M)
{
    size_t r=M.getRows();
    size_t c=M.getCols();
    Out << "\n";
    for(size_t i=0;i<r;i++) {
        for(size_t j=0;j<c;j++)
            Out << std::setw(5) << M[i][j] << " ";
        Out << "\n";
    }

    return Out;
}

#endif // MATRIX_HH
