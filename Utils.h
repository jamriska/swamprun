#ifndef UTILS_H
#define UTILS_H

#ifdef __INTEL_COMPILER
// Currently there is no way to detect C++0x mode on Windows.
// You can force specific features by manually defining COMPILER_HAS_XXX.
#endif

#if (defined(_MSC_VER) && !defined(__INTEL_COMPILER))  
  #if _MSC_VER >= 1600
    #define COMPILER_HAS_LAMBDAS
    #define COMPILER_HAS_RVALUE_REFERENCES
  #endif    
#endif

#if (defined(__GNUC__) && !defined(__INTEL_COMPILER))
  #if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 4)) && defined(__GXX_EXPERIMENTAL_CXX0X__)
    #define COMPILER_HAS_LAMBDAS
    #define COMPILER_HAS_RVALUE_REFERENCES
    #define COMPILER_HAS_INITIALIZER_LISTS
  #endif      
#endif


#include "Vec.h"
#include "Mat.h"
#include "Array.h"
#include "Array2.h"

#include <cstdio>

float lerp(const float a,const float b,const float t);

template<int N> Vec<N,float> lerp(const Vec<N,float>& a,const Vec<N,float>& b,const float t)
{
  return (1.0f-t)*a+t*b;
}

template<int N> Vec<N,float> lerp(const Vec<N,float>& a,const Vec<N,float>& b,const Vec<N,float>& t)
{
  return (1.0f-t)*a+t*b;
}

template<unsigned int N> Vec<N,float> abs(const Vec<N,float>& x)
{
  Vec<N,float> out;
  for(unsigned int i=0;i<N;i++) out(i) = fabs(x(i));
  return out;
}

template<unsigned int N> Vec<N,float> floor(const Vec<N,float>& x)
{
  Vec<N,float> out;
  for(unsigned int i=0;i<N;i++) out(i) = floorf(x(i));
  return out;
}

template<unsigned int N> Vec<N,float> ceil(const Vec<N,float>& x)
{
  Vec<N,float> out;
  for(unsigned int i=0;i<N;i++) out(i) = ceilf(x(i));
  return out;
}

template<unsigned int N> Vec<N,float> exp(const Vec<N,float>& x)
{
  Vec<N,float> out;
  for(unsigned int i=0;i<N;i++) out(i) = expf(x(i));
  return out;
}

Vec2f cart2pol(const Vec2f& u);

Vec2f pol2cart(const Vec2f& u);

Vec2f perp(const Vec2f& u);

/*
TODO
----

float gausspdf(float mu,float sigma,const float x)

template<int N> Vec<N,float> gausspdf(const Vec<N,float>& mean,const Mat<N,N,float>& cov,const Vec<N,float>& x)
{
  
}

template<int N> Vec<N,float> reflect(const Vec<N,float>& I,const Vec<N,float>& N)
{
  
}

// ma smysl i v jiny dimenzi nez 3 ?
template<int N> Vec<N,float> refract(const Vec<N,float>& I,const Vec<N,float>& N,const float eta)
{
  
}

template<int N> Vec<N,float> refract(const Vec<N,float>& I,const Vec<N,float>& N,const float eta,bool* tir)
{

}
*/

template<typename T> bool arrayWrite(const char* fileName,const Array<T>& array)
{
  const int n = array.length();
  const T*  d = array.data();  
  const int s = sizeof(T);
  
  if (d==0) return false;
  
  FILE* f = fopen(fileName,"wb");
  
  if (!f) return false;
  
  fwrite(&n,sizeof(n),1,f);
  fwrite(&s,sizeof(s),1,f);
  fwrite(d,sizeof(T)*n,1,f);
  
  fclose(f);
  
  return true;
}

template<typename T> bool arrayRead(const char* fileName,Array<T>* array)
{
  if (array==0) return false;
  
  FILE* f = fopen(fileName,"rb");
  
  if (!f) return false;

  int n,s;  
  
  fread(&n,sizeof(n),1,f);
  fread(&s,sizeof(s),1,f);
  
  if (n<=0 || s!=sizeof(T)) { fclose(f); return false; }
  
  (*array) = Array<T>(n);
  fread(array->data(),sizeof(T)*n,1,f);
    
  fclose(f);
  
  return true;  
}

template<typename T> Array<T> arrayRead(const char* fileName)
{
  Array<T> array;
  arrayRead(fileName,&array);
  return array;
}

template<typename T> bool arrayWrite(const char* fileName,const Array2<T>& array)
{
  const int w = array.width();
  const int h = array.height();
  const T*  d = array.data();  
  const int s = sizeof(T);
  
  if (d==0) return false;
  
  FILE* f = fopen(fileName,"wb");
  
  if (!f) return false;
  
  fwrite(&w,sizeof(w),1,f);
  fwrite(&h,sizeof(h),1,f);
  fwrite(&s,sizeof(s),1,f);
  fwrite(d,sizeof(T)*w*h,1,f);
  
  fclose(f);
  
  return true;
}

template<typename T> bool arrayRead(const char* fileName,Array2<T>* array)
{
  if (array==0) return false;
  
  FILE* f = fopen(fileName,"rb");
  
  if (!f) return false;

  int w,h,s;  
  
  fread(&w,sizeof(w),1,f);
  fread(&h,sizeof(h),1,f);
  fread(&s,sizeof(s),1,f);
  
  if (w<=0 || h<=0 || s!=sizeof(T)) { fclose(f); return false; }
  
  (*array) = Array2<T>(w,h);
  fread(array->data(),sizeof(T)*w*h,1,f);
    
  fclose(f);
  
  return true;  
}

template<typename T> Array2<T> arrayRead(const char* fileName)
{
  Array2<T> array;
  arrayRead(fileName,&array);
  return array;
}

bool imageRead(const char* fileName,Array2<unsigned char>* image,float scale=1.0f,float gamma=1.0f);

bool imageRead(const char* fileName,Array2<Vec2uc>* image,float scale=1.0f,float gamma=1.0f);

bool imageRead(const char* fileName,Array2<Vec3uc>* image,float scale=1.0f,float gamma=1.0f);

bool imageRead(const char* fileName,Array2<Vec4uc>* image,float scale=1.0f,float gamma=1.0f);

bool imageRead(const char* fileName,Array2<float>* image,float scale=1.0f,float gamma=1.0f);

bool imageRead(const char* fileName,Array2<Vec2f>* image,float scale=1.0f,float gamma=1.0f);

bool imageRead(const char* fileName,Array2<Vec3f>* image,float scale=1.0f,float gamma=1.0f);

bool imageRead(const char* fileName,Array2<Vec4f>* image,float scale=1.0f,float gamma=1.0f);

template<typename T> Array2<T> imageRead(const char* fileName,float scale=1.0,float gamma=1.0f)
{
  Array2<T> image;
  imageRead(fileName,&image,scale,gamma);
  return image;
}

#endif
