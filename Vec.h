#ifndef VEC_H
#define VEC_H

#include <cassert>
#include <cmath>

//#include <initializer_list>

#define fori(N) for(unsigned int i=0;i<(N);++i)

template<unsigned int N,class T>
struct Vec  
{
  T v[N];
  
  Vec<N,T>()
  {
  }

/*
  Vec<N,T>(std::initializer_list<T> list)
  {
    assert(list.size()==N);    
    const T* it=list.begin();
    fori(N) { v[i]=*it; ++it; }//list[i];
  }
*/

  explicit Vec<N,T>(T v0)
  {
    assert(N==1);
    v[0]=v0;
  }

  Vec<N,T>(T v0,T v1)
  {
    assert(N==2);
    v[0]=v0; v[1]=v1;
  }
  
  Vec<N,T>(T v0,T v1,T v2)
  {
    assert(N==3);
    v[0]=v0; v[1]=v1; v[2]=v2;
  }
  
  Vec<N,T>(T v0,T v1,T v2,T v3)
  {
    assert(N==4);
    v[0]=v0; v[1]=v1; v[2]=v2; v[3]=v3;
  }
  
  Vec<N,T>(T v0,T v1,T v2,T v3,T v4)
  {
    assert(N==5);
    v[0]=v0; v[1]=v1; v[2]=v2; v[3]=v3; v[4]=v4;
  }
  
  Vec<N,T>(T v0,T v1,T v2,T v3,T v4,T v5)
  {
    assert(N==6);
    v[0]=v0; v[1]=v1; v[2]=v2; v[3]=v3; v[4]=v4; v[5]=v5;
  }

  T &operator()(int index)
  {
    assert(0<=index && (unsigned int)index<N);
    return v[index];
  }
  
  const T &operator()(int index) const
  {
    assert(0<=index && (unsigned int)index<N);
    return v[index];
  }
  
  T &operator[](int index)
  {
    assert(0<=index && (unsigned int)index<N);
    return v[index];
  }
  
  const T &operator[](int index) const
  {
    assert(0<=index && (unsigned int)index<N);
    return v[index];
  }
    Vec<N,T> operator*=(const Vec<N,T>& u)
  {
    fori(N) v[i]*=u(i);
    return *this;
  }
  
  Vec<N,T> operator+=(const Vec<N,T>& u)
  {
    fori(N) v[i]+=u(i);
    return *this;
  }
    
  Vec<N,T> operator*=(T s)
  {
    fori(N) v[i]*=s;
    return *this;
  }

  Vec<N,T> operator+=(T s)
  {
    fori(N) v[i]+=s;
    return *this;
  }
};

template<unsigned int N,class T>
Vec<N,T> operator-(const Vec<N,T>& u)
{
  Vec<N,T> r;
  fori(N) r(i)=-u(i);
  return r;
}

template<unsigned int N,class T>
Vec<N,T> operator+(const Vec<N,T>& u,const Vec<N,T>& v)
{
  Vec<N,T> r;
  fori(N) r(i)=u(i)+v(i);
  return r;
}

template<unsigned int N,class T>
Vec<N,T> operator-(const Vec<N,T>& u,const Vec<N,T>& v)
{
  Vec<N,T> r;
  fori(N) r(i)=u(i)-v(i);
  return r;
}

template<unsigned int N,class T>
Vec<N,T> operator-(const Vec<N,T>& u,const float v)
{
  Vec<N,T> r;
  fori(N) r(i)=u(i)-v;
  return r;
}

template<unsigned int N,class T>
Vec<N,T> operator*(const Vec<N,T>& u,const Vec<N,T>& v)
{
  Vec<N,T> r;
  fori(N) r(i)=u(i)*v(i);
  return r;
}

template<unsigned int N,class T>
Vec<N,T> operator/(const Vec<N,T>& u,const Vec<N,T>& v)
{
  Vec<N,T> r;
  fori(N) r(i)=u(i)/v(i);
  return r;
}

template<unsigned int N,class T>
Vec<N,T> operator*(const T s,const Vec<N,T>& u)
{
  Vec<N,T> r;
  fori(N) r(i)=s*u(i);
  return r;
}

template<unsigned int N,class T>
Vec<N,T> operator*(const Vec<N,T>& u,const T s)
{
  Vec<N,T> r;
  fori(N) r(i)=u(i)*s;
  return r;
}

template<unsigned int N,class T>
Vec<N,T> operator/(const Vec<N,T>& u,const T s)
{
  Vec<N,T> r;
  fori(N) r(i)=u(i)/s;
  return r;
}

#undef fori

typedef Vec<2,double>         Vec2d;
typedef Vec<2,float>          Vec2f;
typedef Vec<2,int>            Vec2i;
typedef Vec<2,unsigned int>   Vec2ui;
typedef Vec<2,short>          Vec2s;
typedef Vec<2,unsigned short> Vec2us;
typedef Vec<2,char>           Vec2c;
typedef Vec<2,unsigned char>  Vec2uc;

typedef Vec<3,double>         Vec3d;
typedef Vec<3,float>          Vec3f;
typedef Vec<3,int>            Vec3i;
typedef Vec<3,unsigned int>   Vec3ui;
typedef Vec<3,short>          Vec3s;
typedef Vec<3,unsigned short> Vec3us;
typedef Vec<3,char>           Vec3c;
typedef Vec<3,unsigned char>  Vec3uc;

typedef Vec<4,double>         Vec4d;
typedef Vec<4,float>          Vec4f;
typedef Vec<4,int>            Vec4i;
typedef Vec<4,unsigned int>   Vec4ui;
typedef Vec<4,short>          Vec4s;
typedef Vec<4,unsigned short> Vec4us;
typedef Vec<4,char>           Vec4c;
typedef Vec<4,unsigned char>  Vec4uc;

typedef Vec<5,double>         Vec5d;
typedef Vec<5,float>          Vec5f;
typedef Vec<5,int>            Vec5i;
typedef Vec<5,unsigned int>   Vec5ui;
typedef Vec<5,short>          Vec5s;
typedef Vec<5,unsigned short> Vec5us;
typedef Vec<5,char>           Vec5c;
typedef Vec<5,unsigned char>  Vec5uc;

typedef Vec<6,double>         Vec6d;
typedef Vec<6,float>          Vec6f;
typedef Vec<6,int>            Vec6i;
typedef Vec<6,unsigned int>   Vec6ui;
typedef Vec<6,short>          Vec6s;
typedef Vec<6,unsigned short> Vec6us;
typedef Vec<6,char>           Vec6c;
typedef Vec<6,unsigned char>  Vec6uc;


typedef Vec<2,double>         V2d;
typedef Vec<2,float>          V2f;
typedef Vec<2,int>            V2i;
typedef Vec<2,unsigned int>   V2ui;
typedef Vec<2,short>          V2s;
typedef Vec<2,unsigned short> V2us;
typedef Vec<2,char>           V2c;
typedef Vec<2,unsigned char>  V2uc;

typedef Vec<3,double>         V3d;
typedef Vec<3,float>          V3f;
typedef Vec<3,int>            V3i;
typedef Vec<3,unsigned int>   V3ui;
typedef Vec<3,short>          V3s;
typedef Vec<3,unsigned short> V3us;
typedef Vec<3,char>           V3c;
typedef Vec<3,unsigned char>  V3uc;

typedef Vec<4,double>         V4d;
typedef Vec<4,float>          V4f;
typedef Vec<4,int>            V4i;
typedef Vec<4,unsigned int>   V4ui;
typedef Vec<4,short>          V4s;
typedef Vec<4,unsigned short> V4us;
typedef Vec<4,char>           V4c;
typedef Vec<4,unsigned char>  V4uc;

typedef Vec<5,double>         V5d;
typedef Vec<5,float>          V5f;
typedef Vec<5,int>            V5i;
typedef Vec<5,unsigned int>   V5ui;
typedef Vec<5,short>          V5s;
typedef Vec<5,unsigned short> V5us;
typedef Vec<5,char>           V5c;
typedef Vec<5,unsigned char>  V5uc;

typedef Vec<6,double>         V6d;
typedef Vec<6,float>          V6f;
typedef Vec<6,int>            V6i;
typedef Vec<6,unsigned int>   V6ui;
typedef Vec<6,short>          V6s;
typedef Vec<6,unsigned short> V6us;
typedef Vec<6,char>           V6c;
typedef Vec<6,unsigned char>  V6uc;


template<unsigned int N,class T> T dot(const Vec<N,T>& u,const Vec<N,T>& v)
{
  assert(N>0);  
  T sumprod = u(0)*v(0);
  for(unsigned int i=1;i<N;i++) sumprod += u(i)*v(i);
  return sumprod;
}

template<class T>
inline T cross(const Vec<2,T> &a, const Vec<2,T> &b)
{ return a[0]*b[1]-a[1]*b[0]; }

template<class T>
inline Vec<3,T> cross(const Vec<3,T> &a, const Vec<3,T> &b)
{ return Vec<3,T>(a[1]*b[2]-a[2]*b[1], a[2]*b[0]-a[0]*b[2], a[0]*b[1]-a[1]*b[0]); }

template<unsigned int N,class T> T norm(const Vec<N,T>& u) { return sqrtf(dot(u,u)); }
template<unsigned int N,class T> Vec<N,T> normalize(const Vec<N,T>& u) { return u/norm(u); }
/*
template<unsigned int N, class T>
inline T max(const Vec<N,T> &a)
{
   T m=a.v[0];
   for(unsigned int i=1; i<N; ++i) if(a.v[i]>m) m=a.v[i];
   return m;
}*/

#endif
