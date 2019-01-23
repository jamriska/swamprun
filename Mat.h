#ifndef MAT_H
#define MAT_H

#include "Vec.h"

#include <cassert>
#include <cmath>

#define fori(I) for (unsigned int i=0;i<(I);i++)
#define forj(J) for (unsigned int j=0;j<(J);j++)
#define fork(K) for (unsigned int k=0;k<(K);k++)
#define forij(I,J) for (unsigned int i=0;i<(I);i++) for (unsigned int j=0;j<(J);j++)

template<unsigned int M, unsigned int N, class T>
struct Mat
{
    T m[M][N];

    Mat<M,N,T>() {}

    Mat<M,N,T>(T a00, T a01,
               T a10, T a11)
    {
        assert(M==2 && N==2);

        m[0][0] = a00; m[0][1] = a01;
        m[1][0] = a10; m[1][1] = a11;
    }

    Mat<M,N,T>(T a00, T a01, T a02,
               T a10, T a11, T a12,
               T a20, T a21, T a22)
    {
        assert(M==3 && N==3);

        m[0][0] = a00; m[0][1] = a01; m[0][2] = a02;
        m[1][0] = a10; m[1][1] = a11; m[1][2] = a12;
        m[2][0] = a20; m[2][1] = a21; m[2][2] = a22;
    }

    Mat<M,N,T>(T a00, T a01, T a02, T a03,
               T a10, T a11, T a12, T a13,
               T a20, T a21, T a22, T a23,
               T a30, T a31, T a32, T a33)
    {
        assert(M==4 && N==4);

        m[0][0] = a00; m[0][1] = a01; m[0][2] = a02; m[0][3] = a03;
        m[1][0] = a10; m[1][1] = a11; m[1][2] = a12; m[1][3] = a13;
        m[2][0] = a20; m[2][1] = a21; m[2][2] = a22; m[2][3] = a23;
        m[3][0] = a30; m[3][1] = a31; m[3][2] = a32; m[3][3] = a33;
    }

    T &operator()(const int i,const int j)
    {
        assert(0<=i && i<(int)M);
        assert(0<=j && j<(int)N);
        return m[i][j];
    }

    const T &operator()(const int i,const int j) const
    {
        assert(0<=i && i<(int)M);
        assert(0<=j && j<(int)N);
        return m[i][j];
    }
    
    const T* constRowMajorData() const
    {
        return (T*)(&m[0][0]);
    }
   /* T &operator()(const Vec2i &idx)
    {
        assert(0<=idx(0) && idx(0)<M);
        assert(0<=idx(1) && idx(1)<N);
        return m[idx(0)][idx(1)];
    }

    const T &operator()(const Vec2i &idx) const
    {
        assert(0<=idx(0) && idx(0)<M);
        assert(0<=idx(1) && idx(1)<N);
        return m[idx(0)][idx(1)];
    }*/
};

template<unsigned int M1, unsigned int N1,
         unsigned int M2, unsigned int N2,
         class T>
Mat<M1,N2,T> operator*(const Mat<M1,N1,T> &mat1, const Mat<M2,N2,T> &mat2)
{
    assert(N1==M2);
    Mat<M1,N2,T> C;

    fori(M1)
    forj(N2)
    {
        T dot = 0;
        fork(N1) dot += mat1(i,k) * mat2(k,j);
        C(i,j) = dot;
    }

    return C;
}

template<unsigned int M, unsigned int N, class T>
Vec<M,T> operator*(const Mat<M,N,T> &A, const Vec<N,T> &u)
{
    Vec<M,T> v;

    fori(M)
    {
        T dot = 0;
        forj(N) dot += A(i,j) * u(j);
        v(i) = dot;
    }

    return v;
}

template<unsigned int M, unsigned int N, class T>
Vec<N,T> operator*(const Vec<M,T> &u,const Mat<M,N,T> &A)
{
    Vec<N,T> v;

    forj(N)
    {
        T dot = 0;
        fori(M) dot += A(i,j) * u(i);
        v(j) = dot;
    }

    return v;
}

/*
template<unsigned int M, unsigned int N, class T>
std::ostream &operator<<(std::ostream &out, const Mat<M,N,T> &A)
{
    fori(M)
    {
        forj(N) out << A(i,j) << " "; //predelat
        if (i<M-1) out << std::endl;
    }
    return out;
}

template<unsigned int M, unsigned int N, class T>
std::istream &operator>>(std::istream &in, Mat<N,N,T> &A)
{
    fori(M) forj(N) in >> A(i,j);
    return in;
}
*/

template<unsigned int N, class T>
Mat<N,N,T> identity()
{
    Mat<N,N,T> A;
    forij(N,N) A(i,j) = ((i==j) ? 1 : 0);
    return A;
}

template<unsigned int M, unsigned int N, class T>
Mat<N,M,T> transpose(const Mat<M,N,T> &A)
{
    Mat<N,M,T> At;

    forij(N,M) At(i,j) = A(j,i);

    return At;
}

template<unsigned int N, class T>
T trace(const Mat<N,N,T> &A)
{
    T sum = 0;

    fori(N) sum += A(i,i);

    return sum;
}

template<class T>
T determinant(const Mat<2,2,T> &A)
{
    return (A(0,0)*A(1,1) - A(0,1)*A(1,0));
}

template<class T>
T determinant(const Mat<3,3,T> &A)
{
    return ( (A(0,0)*A(1,1)*A(2,2) + A(0,1)*A(1,2)*A(2,0) + A(0,2)*A(1,0)*A(2,1)) -
             (A(2,0)*A(1,1)*A(0,2) + A(2,1)*A(1,2)*A(0,0) + A(2,2)*A(1,0)*A(0,1)) );
}

template<class T>
T determinant(const Mat<4,4,T> &A)
{
   return (A(0,3)*A(1,2)*A(2,1)*A(3,0) - A(0,2)*A(1,3)*A(2,1)*A(3,0) - A(0,3)*A(1,1)*A(2,2)*A(3,0) + A(0,1)*A(1,3)*A(2,2)*A(3,0) +
           A(0,2)*A(1,1)*A(2,3)*A(3,0) - A(0,1)*A(1,2)*A(2,3)*A(3,0) - A(0,3)*A(1,2)*A(2,0)*A(3,1) + A(0,2)*A(1,3)*A(2,0)*A(3,1) +
           A(0,3)*A(1,0)*A(2,2)*A(3,1) - A(0,0)*A(1,3)*A(2,2)*A(3,1) - A(0,2)*A(1,0)*A(2,3)*A(3,1) + A(0,0)*A(1,2)*A(2,3)*A(3,1) +
           A(0,3)*A(1,1)*A(2,0)*A(3,2) - A(0,1)*A(1,3)*A(2,0)*A(3,2) - A(0,3)*A(1,0)*A(2,1)*A(3,2) + A(0,0)*A(1,3)*A(2,1)*A(3,2) +
           A(0,1)*A(1,0)*A(2,3)*A(3,2) - A(0,0)*A(1,1)*A(2,3)*A(3,2) - A(0,2)*A(1,1)*A(2,0)*A(3,3) + A(0,1)*A(1,2)*A(2,0)*A(3,3) +
           A(0,2)*A(1,0)*A(2,1)*A(3,3) - A(0,0)*A(1,2)*A(2,1)*A(3,3) - A(0,1)*A(1,0)*A(2,2)*A(3,3) + A(0,0)*A(1,1)*A(2,2)*A(3,3));
}

template<class T>
Mat<2,2,T> inverse(const Mat<2,2,T> &A)
{
    Mat<2,2,T> Ainv;

    T d = determinant(A);

//    assert(d > eps);

    Ainv(0,0) =  A(1,1)/d; Ainv(0,1) = -A(0,1)/d;
	Ainv(1,0) = -A(1,0)/d; Ainv(1,1) =  A(0,0)/d;

    return Ainv;
}

template<class T>
Mat<3,3,T> inverse(const Mat<3,3,T> &A)
{
    Mat<3,3,T> Ainv;

    T c00 = A(1,1)*A(2,2) - A(1,2)*A(2,1);
	T c01 = A(1,2)*A(2,0) - A(1,0)*A(2,2);
	T c02 = A(1,0)*A(2,1) - A(1,1)*A(2,0);

    T d = A(0,0)*c00 + A(0,1)*c01 + A(0,2)*c02;

//  assert(d > eps);

    Ainv(0,0) = c00/d;
	Ainv(1,0) = c01/d;
	Ainv(2,0) = c02/d;
	Ainv(0,1) = (A(0,2)*A(2,1) - A(0,1)*A(2,2))/d;
	Ainv(1,1) = (A(0,0)*A(2,2) - A(0,2)*A(2,0))/d;
	Ainv(2,1) = (A(0,1)*A(2,0) - A(0,0)*A(2,1))/d;
	Ainv(0,2) = (A(0,1)*A(1,2) - A(0,2)*A(1,1))/d;
	Ainv(1,2) = (A(0,2)*A(1,0) - A(0,0)*A(1,2))/d;
	Ainv(2,2) = (A(0,0)*A(1,1) - A(0,1)*A(1,0))/d;

    return Ainv;
}

template<class T>
Mat<4,4,T> inverse(const Mat<4,4,T> &A)
{
   Mat<4,4,T> Ainv;

   T d = determinant(A);

//  assert(d > eps);

   Ainv(0,0) = (A(1,2)*A(2,3)*A(3,1) - A(1,3)*A(2,2)*A(3,1) + A(1,3)*A(2,1)*A(3,2) - A(1,1)*A(2,3)*A(3,2) - A(1,2)*A(2,1)*A(3,3) + A(1,1)*A(2,2)*A(3,3))/d;
   Ainv(0,1) = (A(0,3)*A(2,2)*A(3,1) - A(0,2)*A(2,3)*A(3,1) - A(0,3)*A(2,1)*A(3,2) + A(0,1)*A(2,3)*A(3,2) + A(0,2)*A(2,1)*A(3,3) - A(0,1)*A(2,2)*A(3,3))/d;
   Ainv(0,2) = (A(0,2)*A(1,3)*A(3,1) - A(0,3)*A(1,2)*A(3,1) + A(0,3)*A(1,1)*A(3,2) - A(0,1)*A(1,3)*A(3,2) - A(0,2)*A(1,1)*A(3,3) + A(0,1)*A(1,2)*A(3,3))/d;
   Ainv(0,3) = (A(0,3)*A(1,2)*A(2,1) - A(0,2)*A(1,3)*A(2,1) - A(0,3)*A(1,1)*A(2,2) + A(0,1)*A(1,3)*A(2,2) + A(0,2)*A(1,1)*A(2,3) - A(0,1)*A(1,2)*A(2,3))/d;
   Ainv(1,0) = (A(1,3)*A(2,2)*A(3,0) - A(1,2)*A(2,3)*A(3,0) - A(1,3)*A(2,0)*A(3,2) + A(1,0)*A(2,3)*A(3,2) + A(1,2)*A(2,0)*A(3,3) - A(1,0)*A(2,2)*A(3,3))/d;
   Ainv(1,1) = (A(0,2)*A(2,3)*A(3,0) - A(0,3)*A(2,2)*A(3,0) + A(0,3)*A(2,0)*A(3,2) - A(0,0)*A(2,3)*A(3,2) - A(0,2)*A(2,0)*A(3,3) + A(0,0)*A(2,2)*A(3,3))/d;
   Ainv(1,2) = (A(0,3)*A(1,2)*A(3,0) - A(0,2)*A(1,3)*A(3,0) - A(0,3)*A(1,0)*A(3,2) + A(0,0)*A(1,3)*A(3,2) + A(0,2)*A(1,0)*A(3,3) - A(0,0)*A(1,2)*A(3,3))/d;
   Ainv(1,3) = (A(0,2)*A(1,3)*A(2,0) - A(0,3)*A(1,2)*A(2,0) + A(0,3)*A(1,0)*A(2,2) - A(0,0)*A(1,3)*A(2,2) - A(0,2)*A(1,0)*A(2,3) + A(0,0)*A(1,2)*A(2,3))/d;
   Ainv(2,0) = (A(1,1)*A(2,3)*A(3,0) - A(1,3)*A(2,1)*A(3,0) + A(1,3)*A(2,0)*A(3,1) - A(1,0)*A(2,3)*A(3,1) - A(1,1)*A(2,0)*A(3,3) + A(1,0)*A(2,1)*A(3,3))/d;
   Ainv(2,1) = (A(0,3)*A(2,1)*A(3,0) - A(0,1)*A(2,3)*A(3,0) - A(0,3)*A(2,0)*A(3,1) + A(0,0)*A(2,3)*A(3,1) + A(0,1)*A(2,0)*A(3,3) - A(0,0)*A(2,1)*A(3,3))/d;
   Ainv(2,2) = (A(0,1)*A(1,3)*A(3,0) - A(0,3)*A(1,1)*A(3,0) + A(0,3)*A(1,0)*A(3,1) - A(0,0)*A(1,3)*A(3,1) - A(0,1)*A(1,0)*A(3,3) + A(0,0)*A(1,1)*A(3,3))/d;
   Ainv(2,3) = (A(0,3)*A(1,1)*A(2,0) - A(0,1)*A(1,3)*A(2,0) - A(0,3)*A(1,0)*A(2,1) + A(0,0)*A(1,3)*A(2,1) + A(0,1)*A(1,0)*A(2,3) - A(0,0)*A(1,1)*A(2,3))/d;
   Ainv(3,0) = (A(1,2)*A(2,1)*A(3,0) - A(1,1)*A(2,2)*A(3,0) - A(1,2)*A(2,0)*A(3,1) + A(1,0)*A(2,2)*A(3,1) + A(1,1)*A(2,0)*A(3,2) - A(1,0)*A(2,1)*A(3,2))/d;
   Ainv(3,1) = (A(0,1)*A(2,2)*A(3,0) - A(0,2)*A(2,1)*A(3,0) + A(0,2)*A(2,0)*A(3,1) - A(0,0)*A(2,2)*A(3,1) - A(0,1)*A(2,0)*A(3,2) + A(0,0)*A(2,1)*A(3,2))/d;
   Ainv(3,2) = (A(0,2)*A(1,1)*A(3,0) - A(0,1)*A(1,2)*A(3,0) - A(0,2)*A(1,0)*A(3,1) + A(0,0)*A(1,2)*A(3,1) + A(0,1)*A(1,0)*A(3,2) - A(0,0)*A(1,1)*A(3,2))/d;
   Ainv(3,3) = (A(0,1)*A(1,2)*A(2,0) - A(0,2)*A(1,1)*A(2,0) + A(0,2)*A(1,0)*A(2,1) - A(0,0)*A(1,2)*A(2,1) - A(0,1)*A(1,0)*A(2,2) + A(0,0)*A(1,1)*A(2,2))/d;

   return Ainv;
}

template<unsigned int N, class T>
Mat<N,N,T> inverse(const Mat<N,N,T> &A)
{
    Mat<N,N,T> invA;

    invA = A;

    Vec<N,int> colIndex(N);
    Vec<N,int> rowIndex(N);
    Vec<N,bool> pivoted(N);

    fori(N) pivoted(i) = false;

    int i1, i2, row = 0, col = 0;
    T save;

    for (int i0 = 0; i0 < N; i0++)
    {
        T fMax = 0.0f;
        for (i1 = 0; i1 < N; i1++)
        {
            if (!pivoted(i1))
            {
                for (i2 = 0; i2 < N; i2++)
                {
                    if (!pivoted(i2))
                    {
                        T fs = abs(invA(i1,i2));
                        if (fs > fMax)
                        {
                            fMax = fs;
                            row = i1;
                            col = i2;
                        }
                    }
                }
            }
        }

//        assert(fmax > eps)

        pivoted(col) = true;

        if (row != col)
        {
            forj(N) { T tmp = invA(row,j); invA(row,j) = invA(col,j); invA(col,j) = tmp; }
        }

        rowIndex(i0) = row;
        colIndex(i0) = col;

        T inv = ((T)1.0)/invA(col,col);
        invA(col,col) = (T)1.0;
        for (i2 = 0; i2 < N; i2++)
        {
            invA(col,i2) *= inv;
        }

        for (i1 = 0; i1 < N; i1++)
        {
            if (i1 != col)
            {
                save = invA(i1,col);
                invA(i1,col) = (T)0.0;
                for (i2 = 0; i2 < N; i2++)
                {
                    invA(i1,i2) -= invA(col,i2)*save;
                }
            }
        }
    }

    for (i1 = N-1; i1 >= 0; i1--)
    {
        if (rowIndex(i1) != colIndex(i1))
        {
            for (i2 = 0; i2 < N; i2++)
            {
                save = invA(i2,rowIndex(i1));
                invA(i2,rowIndex(i1)) = invA(i2,colIndex(i1));
                invA(i2,colIndex(i1)) = save;
            }
        }
    }

    return invA;
}

typedef Mat<2,2,float> Mat2x2f;
typedef Mat<2,3,float> Mat2x3f;
typedef Mat<2,4,float> Mat2x4f;
typedef Mat<2,5,float> Mat2x5f;
typedef Mat<2,6,float> Mat2x6f;
typedef Mat<2,7,float> Mat2x7f;
typedef Mat<2,8,float> Mat2x8f;
typedef Mat<3,2,float> Mat3x2f;
typedef Mat<3,3,float> Mat3x3f;
typedef Mat<3,4,float> Mat3x4f;
typedef Mat<3,5,float> Mat3x5f;
typedef Mat<3,6,float> Mat3x6f;
typedef Mat<3,7,float> Mat3x7f;
typedef Mat<3,8,float> Mat3x8f;
typedef Mat<4,2,float> Mat4x2f;
typedef Mat<4,3,float> Mat4x3f;
typedef Mat<4,4,float> Mat4x4f;
typedef Mat<4,5,float> Mat4x5f;
typedef Mat<4,6,float> Mat4x6f;
typedef Mat<4,7,float> Mat4x7f;
typedef Mat<4,8,float> Mat4x8f;
typedef Mat<5,2,float> Mat5x2f;
typedef Mat<5,3,float> Mat5x3f;
typedef Mat<5,4,float> Mat5x4f;
typedef Mat<5,5,float> Mat5x5f;
typedef Mat<5,6,float> Mat5x6f;
typedef Mat<5,7,float> Mat5x7f;
typedef Mat<5,8,float> Mat5x8f;
typedef Mat<6,2,float> Mat6x2f;
typedef Mat<6,3,float> Mat6x3f;
typedef Mat<6,4,float> Mat6x4f;
typedef Mat<6,5,float> Mat6x5f;
typedef Mat<6,6,float> Mat6x6f;
typedef Mat<6,7,float> Mat6x7f;
typedef Mat<6,8,float> Mat6x8f;
typedef Mat<7,2,float> Mat7x2f;
typedef Mat<7,3,float> Mat7x3f;
typedef Mat<7,4,float> Mat7x4f;
typedef Mat<7,5,float> Mat7x5f;
typedef Mat<7,6,float> Mat7x6f;
typedef Mat<7,7,float> Mat7x7f;
typedef Mat<7,8,float> Mat7x8f;
typedef Mat<8,2,float> Mat8x2f;
typedef Mat<8,3,float> Mat8x3f;
typedef Mat<8,4,float> Mat8x4f;
typedef Mat<8,5,float> Mat8x5f;
typedef Mat<8,6,float> Mat8x6f;
typedef Mat<8,7,float> Mat8x7f;
typedef Mat<8,8,float> Mat8x8f;
typedef Mat<2,2,double> Mat2x2d;
typedef Mat<2,3,double> Mat2x3d;
typedef Mat<2,4,double> Mat2x4d;
typedef Mat<2,5,double> Mat2x5d;
typedef Mat<2,6,double> Mat2x6d;
typedef Mat<2,7,double> Mat2x7d;
typedef Mat<2,8,double> Mat2x8d;
typedef Mat<3,2,double> Mat3x2d;
typedef Mat<3,3,double> Mat3x3d;
typedef Mat<3,4,double> Mat3x4d;
typedef Mat<3,5,double> Mat3x5d;
typedef Mat<3,6,double> Mat3x6d;
typedef Mat<3,7,double> Mat3x7d;
typedef Mat<3,8,double> Mat3x8d;
typedef Mat<4,2,double> Mat4x2d;
typedef Mat<4,3,double> Mat4x3d;
typedef Mat<4,4,double> Mat4x4d;
typedef Mat<4,5,double> Mat4x5d;
typedef Mat<4,6,double> Mat4x6d;
typedef Mat<4,7,double> Mat4x7d;
typedef Mat<4,8,double> Mat4x8d;
typedef Mat<5,2,double> Mat5x2d;
typedef Mat<5,3,double> Mat5x3d;
typedef Mat<5,4,double> Mat5x4d;
typedef Mat<5,5,double> Mat5x5d;
typedef Mat<5,6,double> Mat5x6d;
typedef Mat<5,7,double> Mat5x7d;
typedef Mat<5,8,double> Mat5x8d;
typedef Mat<6,2,double> Mat6x2d;
typedef Mat<6,3,double> Mat6x3d;
typedef Mat<6,4,double> Mat6x4d;
typedef Mat<6,5,double> Mat6x5d;
typedef Mat<6,6,double> Mat6x6d;
typedef Mat<6,7,double> Mat6x7d;
typedef Mat<6,8,double> Mat6x8d;
typedef Mat<7,2,double> Mat7x2d;
typedef Mat<7,3,double> Mat7x3d;
typedef Mat<7,4,double> Mat7x4d;
typedef Mat<7,5,double> Mat7x5d;
typedef Mat<7,6,double> Mat7x6d;
typedef Mat<7,7,double> Mat7x7d;
typedef Mat<7,8,double> Mat7x8d;
typedef Mat<8,2,double> Mat8x2d;
typedef Mat<8,3,double> Mat8x3d;
typedef Mat<8,4,double> Mat8x4d;
typedef Mat<8,5,double> Mat8x5d;
typedef Mat<8,6,double> Mat8x6d;
typedef Mat<8,7,double> Mat8x7d;
typedef Mat<8,8,double> Mat8x8d;

/*
Mat<2,2,float> identity2x2f();
Mat<3,3,float> identity3x3f();
Mat<4,4,float> identity4x4f();
Mat<5,5,float> identity5x5f();
Mat<6,6,float> identity6x6f();
Mat<7,7,float> identity7x7f();
Mat<8,8,float> identity8x8f();
Mat<2,2,double> identity2x2d();
Mat<3,3,double> identity3x3d();
Mat<4,4,double> identity4x4d();
Mat<5,5,double> identity5x5d();
Mat<6,6,double> identity6x6d();
Mat<7,7,double> identity7x7d();
Mat<8,8,double> identity8x8d();
*/

#undef fori
#undef forj
#undef fork
#undef forij

#endif
