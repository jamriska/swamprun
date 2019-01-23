#ifndef ARRAY2_H
#define ARRAY2_H

#include <cstdio>
#include <cassert>
#include "Vec.h"

/*
int mod(const int a,const int n)
{
  const int r = a%n;
  return r<0 ? r+n : r;
}

struct WrapIndex
{
  const int& index;
  WrapIndex(const int& index_) : index(index_) {}
};

struct ClampIndex
{
  const int& index;
  ClampIndex(const int& index_) : index(index_) {}
};

struct MirrorIndex
{
  const int& index;
  MirrorIndex(const int& index_) : index(index_) {}
};

WrapIndex wrap(const int& index) 
{
  return WrapIndex(index);
}

ClampIndex clamp(const int& index) 
{
  return ClampIndex(index);
}

MirrorIndex mirror(const int& index) 
{
  return MirrorIndex(index);
}
*/

template<typename T> class Array2
{
public:  
  Array2() : s(0,0),d(0) {}

  Array2(int M,int N)
  {
    assert(M>0 && N>0);
    s = Vec2i(M,N);
    d = new T[s(0)*s(1)];
  }

  explicit Array2(const Vec2i& s_)
  {
    // XXX: predelat na neco jako assert(all(s>0));
    assert(s_(0)>0 && s_(1)>0);
    s = s_;
    d = new T[s(0)*s(1)];
  }
   
  Array2(const Array2<T>& a)
  {    
    //  printf("COPY CONSTRUCTOR\n");
    s = a.s;
    
    if (s(0)>0 && s(1)>0)
    {
      d = new T[s(0)*s(1)];
    
      // XXX: optimize this:
      for(int i=0;i<s(0)*s(1);i++) d[i] = a.d[i];
    }
    else
    {
      d = 0;
    }
  }

  Array2& operator=(const Array2<T>& a)
  {
    // printf("ASSIGNMENT\n");
    // printf("slow copy\n");
    if (this!=&a)
    {
      if (s(0)==a.s(0) && s(1)==a.s(1))
      {
        // XXX: optimize this:
        for(int i=0;i<s(0)*s(1);i++) d[i] = a.d[i];
      }
      else
      {
        delete[] d;
        s = a.s;
    
        if (a.s(0)>0 && a.s(1)>0)
        {
          d = new T[s(0)*s(1)];  
          // XXX: optimize this:
          for(int i=0;i<s(0)*s(1);i++) d[i] = a.d[i];
        }
        else
        {
          d = 0;
        }
      }
    }
    else
    {
    //  printf("SELF ASSIGNMENT\n");
    }    
    
    return *this;
  }
        
  ~Array2()
  {
    delete[] d;
  }
  
  T& operator()(int i,int j)
  {
    assert(d!=0);
    assert(i>=0 && i<s(0) &&
           j>=0 && j<s(1));
    
    return d[i+j*s(0)];
  }
  
  const T& operator()(int i,int j) const
  {
    assert(d!=0);
    assert(i>=0 && i<s(0) &&
           j>=0 && j<s(1));
    
    return d[i+j*s(0)];
  }
  
  T& operator()(const Vec<2,int>& ij)
  {
    assert(d!=0);
    assert(ij(0)>=0 && ij(0)<s(0) &&
           ij(1)>=0 && ij(1)<s(1));
    
    return d[ij(0)+ij(1)*s(0)];
  }
  
  const T& operator()(const Vec<2,int>& ij) const
  {
    assert(d!=0);
    assert(ij(0)>=0 && ij(0)<s(0) &&
           ij(1)>=0 && ij(1)<s(1));
    
    return d[ij(0)+ij(1)*s(0)];
  }
   
  Vec2i size() const
  {
    return s;
  }
  
  int size(int dim) const
  {
    assert(dim==0 || dim==1);
    return size()(dim);
  }
  
  int width() const
  {
    return size(0);
  }
  
  int height() const
  {
    return size(1);
  }

  int numel() const
  {
    return size(0)*size(1);  
  }
  
  T* data()
  {
    return d;
  }
  
  const T* data() const
  {
    return d;
  }

  void clear()
  {
    delete[] d;
    s = Vec2i(0,0);
    d = 0;    
  }
  
  void swap(Array2<T>& b)
  {
    Vec2i tmp_s = s;
    s = b.s;
    b.s = tmp_s;        
    
    T* tmp_d = d;
    d = b.d;
    b.d = tmp_d;
  }
      
private:  
  Vec2i  s;
  T*     d;
};
  
template<typename T> Vec2i size(const Array2<T>& a)
{
  return a.size();
}

template<typename T> int size(const Array2<T>& a,int dim)
{
  return a.size(dim);
}

template<typename T> int numel(const Array2<T>& a)
{
  return a.numel();  
}

template<typename T> void clear(Array2<T>* a)
{
  a->clear();
}

template<typename T> void swap(Array2<T>& a,Array2<T>& b)
{
  a.swap(b);    
}

template<typename T> T min(const Array2<T>& a)
{
  assert(numel(a)>0);
  
  const int n = numel(a);

  const T* d = a.data();  

  T minval = d[0];
   
  for(int i=1;i<n;i++) minval = (d[i]<minval) ? d[i] : minval;

  return minval;
}

template<typename T> T max(const Array2<T>& a)
{
  assert(numel(a)>0);
  
  const int n = numel(a);

  const T* d = a.data();  

  T maxval = d[0];
   
  for(int i=1;i<n;i++) maxval = (maxval<d[i]) ? d[i] : maxval;

  return maxval;
}

template<typename T> Vec<2,T> minmax(const Array2<T>& a)
{
  assert(numel(a)>0);
  
  const int n = numel(a);

  const T* d = a.data();  

  T minval = d[0];
  T maxval = d[0];
   
  for(int i=1;i<n;i++)
  {
    minval = (d[i]<minval) ? d[i] : minval;
    maxval = (maxval<d[i]) ? d[i] : maxval;
  }

  return Vec<2,T>(minval,maxval);  
}

template<typename T> Vec2i indexMin(const Array2<T>& a)
{
  assert(numel(a)>0);
  
  T minValue = a(0,0);
  Vec2i minIndex = Vec2i(0,0);
  
  for(int j=0;j<a.height();j++)
  {
    for(int i=0;i<a.width();i++)
    {
      if (a(i,j)<minValue)
      {
        minValue = a(i,j);
        minIndex = Vec2i(i,j);
      }    
    }
  }
  
  return minIndex;
}

template<typename T> Vec2i indexMax(const Array2<T>& a)
{
  assert(numel(a)>0);
  
  T maxValue = a(0,0);
  Vec2i maxIndex = Vec2i(0,0);
  
  for(int j=0;j<a.height();j++)
  {
    for(int i=0;i<a.width();i++)
    {
      if (maxValue<a(i,j))
      {
        maxValue = a(i,j);
        maxIndex = Vec2i(i,j);
      }    
    }
  }
  
  return maxIndex;
}

template<typename T> T sum(const Array2<T>& a)
{
  assert(numel(a)>0);
  
  const int n = numel(a);

  const T* d = a.data();  

  T sumval = d[0];
   
  for(int i=1;i<n;i++) sumval += d[i];

  return sumval;
}

template<typename T> void fill(Array2<T>* a,const T& value)
{
  assert(a!=0);
  assert(a->numel()>0);
  
  const int n = a->numel();
  T* d = a->data();  
   
  for(int i=0;i<n;i++) d[i] = value;  
}

template<typename T,typename F> Array2<T> apply(const Array2<T>& a,F fun)
{
  assert(numel(a) > 0);
  
  Array2<T> fun_a(size(a));
  
  const int n = numel(a);
   
  for(int i=0;i<n;i++) fun_a.data()[i] = fun(a.data()[i]);
  
  return fun_a;
}

typedef Array2<double>                  Array2d;
typedef Array2<float>                   Array2f;
typedef Array2<int>                     Array2i;
typedef Array2<unsigned int>            Array2ui;
typedef Array2<short>                   Array2s;
typedef Array2<unsigned short>          Array2us;
typedef Array2<char>                    Array2c;
typedef Array2<unsigned char>           Array2uc;

typedef Array2< Vec<2,double> >         Array2V2d;
typedef Array2< Vec<2,float> >          Array2V2f;
typedef Array2< Vec<2,int> >            Array2V2i;
typedef Array2< Vec<2,unsigned int> >   Array2V2ui;
typedef Array2< Vec<2,short> >          Array2V2s;
typedef Array2< Vec<2,unsigned short> > Array2V2us;
typedef Array2< Vec<2,char> >           Array2V2c;
typedef Array2< Vec<2,unsigned char> >  Array2V2uc;

typedef Array2< Vec<3,double> >         Array2V3d;
typedef Array2< Vec<3,float> >          Array2V3f;
typedef Array2< Vec<3,int> >            Array2V3i;
typedef Array2< Vec<3,unsigned int> >   Array2V3ui;
typedef Array2< Vec<3,short> >          Array2V3s;
typedef Array2< Vec<3,unsigned short> > Array2V3us;
typedef Array2< Vec<3,char> >           Array2V3c;
typedef Array2< Vec<3,unsigned char> >  Array2V3uc;

typedef Array2< Vec<4,double> >         Array2V4d;
typedef Array2< Vec<4,float> >          Array2V4f;
typedef Array2< Vec<4,int> >            Array2V4i;
typedef Array2< Vec<4,unsigned int> >   Array2V4ui;
typedef Array2< Vec<4,short> >          Array2V4s;
typedef Array2< Vec<4,unsigned short> > Array2V4us;
typedef Array2< Vec<4,char> >           Array2V4c;
typedef Array2< Vec<4,unsigned char> >  Array2V4uc;


typedef Array2<double>                  A2d;
typedef Array2<float>                   A2f;
typedef Array2<int>                     A2i;
typedef Array2<unsigned int>            A2ui;
typedef Array2<short>                   A2s;
typedef Array2<unsigned short>          A2us;
typedef Array2<char>                    A2c;
typedef Array2<unsigned char>           A2uc;

typedef Array2< Vec<2,double> >         A2V2d;
typedef Array2< Vec<2,float> >          A2V2f;
typedef Array2< Vec<2,int> >            A2V2i;
typedef Array2< Vec<2,unsigned int> >   A2V2ui;
typedef Array2< Vec<2,short> >          A2V2s;
typedef Array2< Vec<2,unsigned short> > A2V2us;
typedef Array2< Vec<2,char> >           A2V2c;
typedef Array2< Vec<2,unsigned char> >  A2V2uc;

typedef Array2< Vec<3,double> >         A2V3d;
typedef Array2< Vec<3,float> >          A2V3f;
typedef Array2< Vec<3,int> >            A2V3i;
typedef Array2< Vec<3,unsigned int> >   A2V3ui;
typedef Array2< Vec<3,short> >          A2V3s;
typedef Array2< Vec<3,unsigned short> > A2V3us;
typedef Array2< Vec<3,char> >           A2V3c;
typedef Array2< Vec<3,unsigned char> >  A2V3uc;

typedef Array2< Vec<4,double> >         A2V4d;
typedef Array2< Vec<4,float> >          A2V4f;
typedef Array2< Vec<4,int> >            A2V4i;
typedef Array2< Vec<4,unsigned int> >   A2V4ui;
typedef Array2< Vec<4,short> >          A2V4s;
typedef Array2< Vec<4,unsigned short> > A2V4us;
typedef Array2< Vec<4,char> >           A2V4c;
typedef Array2< Vec<4,unsigned char> >  A2V4uc;

#endif