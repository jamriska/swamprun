#ifndef ARRAY_H
#define ARRAY_H

#include <cstdio>
#include <cassert>

template<typename T> class Array
{
public:  
  Array() : s(0),d(0) {}

  Array(int N)
  {
    assert(N>0);
    s = N;
    d = new T[s];
  }
   
  Array(const Array<T>& a)
  {    
    s = a.s;
    
    if (s>0)
    {
      d = new T[s];
    
      // XXX: optimize this:
      for(int i=0;i<s;i++) d[i] = a.d[i];
    }
    else
    {
      d = 0;
    }
  }

  Array& operator=(const Array<T>& a)
  {
    // printf("ASSIGNMENT\n");
    // printf("slow copy\n");
    if (this!=&a)
    {
      if (s==a.s)
      {
        // XXX: optimize this:
        for(int i=0;i<s;i++) d[i] = a.d[i];
      }
      else
      {
        delete[] d;
        s = a.s;
    
        if (a.s>0)
        {
          d = new T[s];  
          // XXX: optimize this:
          for(int i=0;i<s;i++) d[i] = a.d[i];
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
        
  ~Array()
  {
    delete[] d;
  }
  
  T& operator()(int i)
  {
    assert(d!=0);
    assert(i>=0 && i<s);
    
    return d[i];
  }
  
  const T& operator()(int i) const
  {
    assert(d!=0);
    assert(i>=0 && i<s);
    
    return d[i];
  }
     
  int size() const
  {
    return s;
  }
  
  int size(int dim) const
  {
    assert(dim==0);
    return s;
  }
  
  int length() const
  {
    return s;
  }
  
  int numel() const
  {
    return s;
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
    s = 0;
    d = 0;    
  }
  
  void swap(Array<T>& b)
  {
    int tmp_s = s;
    s = b.s;
    b.s = tmp_s;        
    
    T* tmp_d = d;
    d = b.d;
    b.d = tmp_d;
  }
      
private:  
  int s;
  T*  d;
};
  
template<typename T> int size(const Array<T>& a)
{
  return a.size();
}

template<typename T> int size(const Array<T>& a,int dim)
{
  return a.size(dim);
}

template<typename T> int numel(const Array<T>& a)
{
  return a.numel();  
}

template<typename T> void clear(Array<T>* a)
{
  a->clear();
}

template<typename T> void swap(Array<T>& a,Array<T>& b)
{
  a.swap(b);    
}

template<typename T> T min(const Array<T>& a)
{
  assert(numel(a)>0);
  
  const int n = numel(a);

  const T* d = a.data();  

  T minval = d[0];
   
  for(int i=1;i<n;i++) minval = (d[i]<minval) ? d[i] : minval;

  return minval;
}

template<typename T> T max(const Array<T>& a)
{
  assert(numel(a)>0);
  
  const int n = numel(a);

  const T* d = a.data();  

  T maxval = d[0];
   
  for(int i=1;i<n;i++) maxval = (maxval<d[i]) ? d[i] : maxval;

  return maxval;
}

template<typename T> Vec<2,T> minmax(const Array<T>& a)
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

template<typename T> int indexMin(const Array<T>& a)
{
  assert(numel(a)>0);
  
  T minValue = a(0);
  int minIndex = 0;
  
  for(int i=0;i<a.size();i++)
  {
    if (a(i)<minValue)
    {
      minValue = a(i);
      minIndex = i;
    }
  }
  
  return minIndex;
}

template<typename T> int indexMax(const Array<T>& a)
{
  assert(numel(a)>0);
  
  T maxValue = a(0);
  int maxIndex = 0;
  
  for(int i=0;i<a.size();i++)
  {
    if (maxValue<a(i))
    {
      maxValue = a(i);
      maxIndex = i;
    }
  }
  
  return maxIndex;
}

template<typename T> T sum(const Array<T>& a)
{
  assert(numel(a)>0);
  
  const int n = numel(a);

  const T* d = a.data();  

  T sumval = d[0];
   
  for(int i=1;i<n;i++) sumval += d[i];

  return sumval;
}

template<typename T> void fill(Array<T>* a,const T& value)
{
  assert(a!=0);
  assert(a->numel()>0);
  
  const int n = a->numel();
  T* d = a->data();  
   
  for(int i=0;i<n;i++) d[i] = value;  
}

template<typename T,typename F> Array<T> apply(const Array<T>& a,F fun)
{
  assert(numel(a) > 0);
  
  Array<T> fun_a(size(a));
  
  const int n = numel(a);
   
  for(int i=0;i<n;i++) fun_a.data()[i] = fun(a.data()[i]);
  
  return fun_a;
}

typedef Array<double>                  Array1d;
typedef Array<float>                   Array1f;
typedef Array<int>                     Array1i;
typedef Array<unsigned int>            Array1ui;
typedef Array<short>                   Array1s;
typedef Array<unsigned short>          Array1us;
typedef Array<char>                    Array1c;
typedef Array<unsigned char>           Array1uc;

typedef Array< Vec<2,double> >         Array1V2d;
typedef Array< Vec<2,float> >          Array1V2f;
typedef Array< Vec<2,int> >            Array1V2i;
typedef Array< Vec<2,unsigned int> >   Array1V2ui;
typedef Array< Vec<2,short> >          Array1V2s;
typedef Array< Vec<2,unsigned short> > Array1V2us;
typedef Array< Vec<2,char> >           Array1V2c;
typedef Array< Vec<2,unsigned char> >  Array1V2uc;

typedef Array< Vec<3,double> >         Array1V3d;
typedef Array< Vec<3,float> >          Array1V3f;
typedef Array< Vec<3,int> >            Array1V3i;
typedef Array< Vec<3,unsigned int> >   Array1V3ui;
typedef Array< Vec<3,short> >          Array1V3s;
typedef Array< Vec<3,unsigned short> > Array1V3us;
typedef Array< Vec<3,char> >           Array1V3c;
typedef Array< Vec<3,unsigned char> >  Array1V3uc;

typedef Array< Vec<4,double> >         Array1V4d;
typedef Array< Vec<4,float> >          Array1V4f;
typedef Array< Vec<4,int> >            Array1V4i;
typedef Array< Vec<4,unsigned int> >   Array1V4ui;
typedef Array< Vec<4,short> >          Array1V4s;
typedef Array< Vec<4,unsigned short> > Array1V4us;
typedef Array< Vec<4,char> >           Array1V4c;
typedef Array< Vec<4,unsigned char> >  Array1V4uc;


typedef Array<double>                  A1d;
typedef Array<float>                   A1f;
typedef Array<int>                     A1i;
typedef Array<unsigned int>            A1ui;
typedef Array<short>                   A1s;
typedef Array<unsigned short>          A1us;
typedef Array<char>                    A1c;
typedef Array<unsigned char>           A1uc;

typedef Array< Vec<2,double> >         A1V2d;
typedef Array< Vec<2,float> >          A1V2f;
typedef Array< Vec<2,int> >            A1V2i;
typedef Array< Vec<2,unsigned int> >   A1V2ui;
typedef Array< Vec<2,short> >          A1V2s;
typedef Array< Vec<2,unsigned short> > A1V2us;
typedef Array< Vec<2,char> >           A1V2c;
typedef Array< Vec<2,unsigned char> >  A1V2uc;

typedef Array< Vec<3,double> >         A1V3d;
typedef Array< Vec<3,float> >          A1V3f;
typedef Array< Vec<3,int> >            A1V3i;
typedef Array< Vec<3,unsigned int> >   A1V3ui;
typedef Array< Vec<3,short> >          A1V3s;
typedef Array< Vec<3,unsigned short> > A1V3us;
typedef Array< Vec<3,char> >           A1V3c;
typedef Array< Vec<3,unsigned char> >  A1V3uc;

typedef Array< Vec<4,double> >         A1V4d;
typedef Array< Vec<4,float> >          A1V4f;
typedef Array< Vec<4,int> >            A1V4i;
typedef Array< Vec<4,unsigned int> >   A1V4ui;
typedef Array< Vec<4,short> >          A1V4s;
typedef Array< Vec<4,unsigned short> > A1V4us;
typedef Array< Vec<4,char> >           A1V4c;
typedef Array< Vec<4,unsigned char> >  A1V4uc;

#endif