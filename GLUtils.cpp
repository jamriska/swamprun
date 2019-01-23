#include "GLUtils.h"

bool glCheckErrorFileLine(const char* file,int line)
{
  bool error = false;
  
  GLenum glErr = glGetError();
  while (glErr != GL_NO_ERROR)
  {
    printf("glError in file %s @ line %d: %s\n",file,line,gluErrorString(glErr));
    error = true;    
    glErr = glGetError();
  }
  
  return error;
}

void glColor(const Vec3f& c)
{
  glColor3f(c(0),c(1),c(2));
}

void glColor(const Vec4f& c)
{
  glColor4f(c(0),c(1),c(2),c(3));
}

void glNormal(const Vec3f& n)
{
  glNormal3f(n(0),n(1),n(2));  
}

void glVertex(const Vec2f& v)
{
  glVertex2f(v(0),v(1));  
}

void glVertex(const Vec3f& v)
{
  glVertex3f(v(0),v(1),v(2));
}
