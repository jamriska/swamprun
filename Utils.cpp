#include "Utils.h"
#include <cstring>

#define M_PI 3.14159265358979323846

float lerp(const float a,const float b,const float t)
{
  return (1.0f-t)*a+t*b;
}

Vec2f cart2pol(const Vec2f& u)
{
  float theta = atan2f(u(1),u(0));
  if (theta<0.0f) theta += 2.0f*M_PI;
  
  float r = sqrtf(u(0)*u(0)+u(1)*u(1));
  
  return Vec2f(theta,r);  
}

Vec2f pol2cart(const Vec2f& u)
{
  return Vec2f(u(1)*cosf(u(0)),u(1)*sinf(u(0)));  
}

Vec2f perp(const Vec2f& u)
{
  return Vec2f(-u(1),u(0));
}

#include "stb_image.h"

template<int N,typename T> bool imageRead_uc(const char* fileName,Array2<T>* image,float scale,float gamma)
{
  if (image==0) return false;
  
  stbi_hdr_to_ldr_gamma(gamma);
  stbi_hdr_to_ldr_scale(scale);
  
  int x,y,n;
  unsigned char* data = stbi_load(fileName,&x,&y,&n,N);
  
  image->clear();
  
  if (data==0) return false;
  
  *image = Array2<T>(x,y);
  
  memcpy(image->data(),data,x*y*sizeof(T));
  
  stbi_image_free(data);
  
  return true; 
}

template<int N,typename T> bool imageRead_f(const char* fileName,Array2<T>* image,float scale,float gamma)
{
  if (image==0) return false;

  stbi_ldr_to_hdr_scale(scale);
  stbi_ldr_to_hdr_gamma(gamma);
  
  int x,y,n;
  float* data = stbi_loadf(fileName,&x,&y,&n,N);
  
  image->clear();
  
  if (data==0) return false;
  
  *image = Array2<T>(x,y);
  
  memcpy(image->data(),data,x*y*sizeof(T));
  
  stbi_image_free(data);
  
  return true; 
}

bool imageRead(const char* fileName,Array2<unsigned char>* image,float scale,float gamma)
{
  return imageRead_uc<1,unsigned char>(fileName,image,scale,gamma);
}

bool imageRead(const char* fileName,Array2<Vec2uc>* image,float scale,float gamma)
{
  return imageRead_uc<2,Vec2uc>(fileName,image,scale,gamma);  
}

bool imageRead(const char* fileName,Array2<Vec3uc>* image,float scale,float gamma)
{
  return imageRead_uc<3,Vec3uc>(fileName,image,scale,gamma);    
}

bool imageRead(const char* fileName,Array2<Vec4uc>* image,float scale,float gamma)
{
  return imageRead_uc<4,Vec4uc>(fileName,image,scale,gamma);      
}

bool imageRead(const char* fileName,Array2<float>* image,float scale,float gamma)
{
  return imageRead_f<1,float>(fileName,image,scale,gamma);
}

bool imageRead(const char* fileName,Array2<Vec2f>* image,float scale,float gamma)
{
  return imageRead_f<2,Vec2f>(fileName,image,scale,gamma);  
}

bool imageRead(const char* fileName,Array2<Vec3f>* image,float scale,float gamma)
{
  return imageRead_f<3,Vec3f>(fileName,image,scale,gamma);    
}

bool imageRead(const char* fileName,Array2<Vec4f>* image,float scale,float gamma)
{
  return imageRead_f<4,Vec4f>(fileName,image,scale,gamma);      
}
