#ifndef GLUTILS_H
#define GLUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glew.h"
#include <GL/gl.h>

#include "Vec.h"
#include "Mat.h"
#include "Array2.h"

#define glCheckError() glCheckErrorFileLine(__FILE__, __LINE__)

bool glCheckErrorFileLine(const char* file,int line);

void glColor(const Vec3f& c);
void glColor(const Vec4f& c);

void glNormal(const Vec3f& n);

void glVertex(const Vec2f& v);
void glVertex(const Vec3f& v);

class GLTexture
{
public:
  GLTexture(GLenum target) : target(target),initialized(false) {}

  ~GLTexture()
  {
    if (initialized)
    {
      glDeleteTextures(1,&texture);
    }   
  }

  GLTexture& setParameter(GLenum name,GLint param)
  {
    bind();
    glTexParameteri(target,name,param);
    return *this;
  }

  GLTexture& setParameter(GLenum name,GLfloat param)
  {
    bind();
    glTexParameterf(target,name,param);
    return *this;
  }

  GLTexture& setWrapS(GLint param)
  {
    return setParameter(GL_TEXTURE_WRAP_S,param);
  }

  GLTexture& setWrapT(GLint param)
  {
    return setParameter(GL_TEXTURE_WRAP_T,param);
  }

  GLTexture& setWrapR(GLint param)
  {
    return setParameter(GL_TEXTURE_WRAP_R,param);
  }

  GLTexture& setMinFilter(GLint param)
  {
    return setParameter(GL_TEXTURE_MIN_FILTER,param);
  }

  GLTexture& setMagFilter(GLint param)
  {
    return setParameter(GL_TEXTURE_MAG_FILTER,param);
  }

  GLTexture& setMinLod(GLfloat param)
  {
    return setParameter(GL_TEXTURE_MIN_LOD,param);
  }

  GLTexture& setMaxLod(GLfloat param)
  {
    return setParameter(GL_TEXTURE_MAX_LOD,param);
  }

  GLTexture& setBaseLevel(GLint param)
  {
    return setParameter(GL_TEXTURE_BASE_LEVEL,param);
  }

  GLTexture& setMaxLevel(GLint param)
  {
    return setParameter(GL_TEXTURE_MAX_LEVEL,param);
  }

  GLTexture& setLodBias(GLfloat param)
  {
    return setParameter(GL_TEXTURE_LOD_BIAS,param);
  }

  GLTexture& setGenerateMipmap(GLboolean param)
  {
    return setParameter(GL_GENERATE_MIPMAP,param);
  }

  GLTexture& bind()
  {
    if (!initialized) init();
    glBindTexture(target,texture);
    return *this;
  }

private:
  const GLenum target;
  GLuint texture;
  bool initialized;

  void init()
  {
    glGenTextures(1,&texture);
    
    initialized = true;
  }
};

class GLTexture1D : public GLTexture
{
public:
  GLTexture1D() : GLTexture(GL_TEXTURE_1D) {};
  
  ~GLTexture1D() {}

  GLTexture1D& setParameter(GLenum name,GLint param) { GLTexture::setParameter(name,param); return *this; }
  GLTexture1D& setParameter(GLenum name,GLfloat param) { GLTexture::setParameter(name,param); return *this; }
  GLTexture1D& setWrapS(GLint param) { GLTexture::setWrapS(param); return *this; }
  GLTexture1D& setMinFilter(GLint param) { GLTexture::setMinFilter(param); return *this; }
  GLTexture1D& setMagFilter(GLint param) { GLTexture::setMagFilter(param); return *this; }
  GLTexture1D& setMinLod(GLfloat param) { GLTexture::setMinLod(param); return *this; }
  GLTexture1D& setMaxLod(GLfloat param) { GLTexture::setMaxLod(param); return *this; }
  GLTexture1D& setBaseLevel(GLint param) { GLTexture::setBaseLevel(param); return *this; }
  GLTexture1D& setMaxLevel(GLint param) { GLTexture::setMaxLevel(param); return *this; }
  GLTexture1D& setLodBias(GLfloat param) { GLTexture::setLodBias(param); return *this; }
  GLTexture1D& setGenerateMipmap(GLboolean param) { GLTexture::setGenerateMipmap(param); return *this; }
  GLTexture1D& bind() { GLTexture::bind(); return *this; }

};

class GLTexture2D : public GLTexture
{
public:
  GLTexture2D() : GLTexture(GL_TEXTURE_2D) {};

  ~GLTexture2D() {}

  GLTexture2D& setParameter(GLenum name,GLint param) { GLTexture::setParameter(name,param); return *this; }
  GLTexture2D& setParameter(GLenum name,GLfloat param) { GLTexture::setParameter(name,param); return *this; }
  GLTexture2D& setWrapS(GLint param) { GLTexture::setWrapS(param); return *this; }
  GLTexture2D& setWrapT(GLint param) { GLTexture::setWrapT(param); return *this; }
  GLTexture2D& setMinFilter(GLint param) { GLTexture::setMinFilter(param); return *this; }
  GLTexture2D& setMagFilter(GLint param) { GLTexture::setMagFilter(param); return *this; }
  GLTexture2D& setMinLod(GLfloat param) { GLTexture::setMinLod(param); return *this; }
  GLTexture2D& setMaxLod(GLfloat param) { GLTexture::setMaxLod(param); return *this; }
  GLTexture2D& setBaseLevel(GLint param) { GLTexture::setBaseLevel(param); return *this; }
  GLTexture2D& setMaxLevel(GLint param) { GLTexture::setMaxLevel(param); return *this; }
  GLTexture2D& setLodBias(GLfloat param) { GLTexture::setLodBias(param); return *this; }
  GLTexture2D& setGenerateMipmap(GLboolean param) { GLTexture::setGenerateMipmap(param); return *this; }
  GLTexture2D& bind() { GLTexture::bind(); return *this; }

  GLTexture2D& setImage(const Array2<Vec3uc>& image,GLint level=0,GLenum internalFormat=GL_RGB8,GLenum format=GL_RGB)
  {
    bind();
    glTexImage2D(GL_TEXTURE_2D,level,internalFormat,image.width(),image.height(),0,format,GL_UNSIGNED_BYTE,image.data());
    return *this;
  }

  GLTexture2D& setImage(const Array2<Vec4uc>& image,GLint level=0,GLenum internalFormat=GL_RGBA8,GLenum format=GL_RGBA)
  {
    bind();
    glTexImage2D(GL_TEXTURE_2D,level,internalFormat,image.width(),image.height(),0,format,GL_UNSIGNED_BYTE,image.data());
    return *this;
  }
};

class GLTexture3D : public GLTexture
{
public:
  GLTexture3D() : GLTexture(GL_TEXTURE_3D) {};
  
  ~GLTexture3D() {}

  GLTexture3D& setParameter(GLenum name,GLint param) { GLTexture::setParameter(name,param); return *this; }
  GLTexture3D& setParameter(GLenum name,GLfloat param) { GLTexture::setParameter(name,param); return *this; }
  GLTexture3D& setWrapS(GLint param) { GLTexture::setWrapS(param); return *this; }
  GLTexture3D& setWrapT(GLint param) { GLTexture::setWrapT(param); return *this; }
  GLTexture3D& setWrapR(GLint param) { GLTexture::setWrapR(param); return *this; }
  GLTexture3D& setMinFilter(GLint param) { GLTexture::setMinFilter(param); return *this; }
  GLTexture3D& setMagFilter(GLint param) { GLTexture::setMagFilter(param); return *this; }
  GLTexture3D& setMinLod(GLfloat param) { GLTexture::setMinLod(param); return *this; }
  GLTexture3D& setMaxLod(GLfloat param) { GLTexture::setMaxLod(param); return *this; }
  GLTexture3D& setBaseLevel(GLint param) { GLTexture::setBaseLevel(param); return *this; }
  GLTexture3D& setMaxLevel(GLint param) { GLTexture::setMaxLevel(param); return *this; }
  GLTexture3D& setLodBias(GLfloat param) { GLTexture::setLodBias(param); return *this; }
  GLTexture3D& setGenerateMipmap(GLboolean param) { GLTexture::setGenerateMipmap(param); return *this; }
  GLTexture3D& bind() { GLTexture::bind(); return *this; }

};

class GLTextureCubeMap : public GLTexture
{
public:
  GLTextureCubeMap() : GLTexture(GL_TEXTURE_CUBE_MAP) {};
  
  ~GLTextureCubeMap() {}

  GLTextureCubeMap& setParameter(GLenum name,GLint param) { GLTexture::setParameter(name,param); return *this; }
  GLTextureCubeMap& setParameter(GLenum name,GLfloat param) { GLTexture::setParameter(name,param); return *this; }
  GLTextureCubeMap& setWrapS(GLint param) { GLTexture::setWrapS(param); return *this; }
  GLTextureCubeMap& setWrapT(GLint param) { GLTexture::setWrapT(param); return *this; }
  GLTextureCubeMap& setMinFilter(GLint param) { GLTexture::setMinFilter(param); return *this; }
  GLTextureCubeMap& setMagFilter(GLint param) { GLTexture::setMagFilter(param); return *this; }
  GLTextureCubeMap& setMinLod(GLfloat param) { GLTexture::setMinLod(param); return *this; }
  GLTextureCubeMap& setMaxLod(GLfloat param) { GLTexture::setMaxLod(param); return *this; }
  GLTextureCubeMap& setBaseLevel(GLint param) { GLTexture::setBaseLevel(param); return *this; }
  GLTextureCubeMap& setMaxLevel(GLint param) { GLTexture::setMaxLevel(param); return *this; }
  GLTextureCubeMap& setLodBias(GLfloat param) { GLTexture::setLodBias(param); return *this; }
  GLTextureCubeMap& setGenerateMipmap(GLboolean param) { GLTexture::setGenerateMipmap(param); return *this; }
  GLTextureCubeMap& bind() { GLTexture::bind(); return *this; }
  
  GLTextureCubeMap& setImage(GLenum target,const Array2<Vec3uc>& image,GLint level=0,GLenum internalFormat=GL_RGB8,GLenum format=GL_RGB)
  {
    bind();
    glTexImage2D(target,level,internalFormat,image.width(),image.height(),0,format,GL_UNSIGNED_BYTE,image.data());
    return *this;
  }

  GLTextureCubeMap& setImages(const Array2<Vec3uc>& imagePX,
                              const Array2<Vec3uc>& imageNX,
                              const Array2<Vec3uc>& imagePY,
                              const Array2<Vec3uc>& imageNY,
                              const Array2<Vec3uc>& imagePZ,
                              const Array2<Vec3uc>& imageNZ,
                              GLint level=0,GLenum internalFormat=GL_RGB8,GLenum format=GL_RGB)
  {
    setImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X,imagePX,level,internalFormat,format);
    setImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,imageNX,level,internalFormat,format);
    setImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,imagePY,level,internalFormat,format);
    setImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,imageNY,level,internalFormat,format);
    setImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,imagePZ,level,internalFormat,format);
    setImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,imageNZ,level,internalFormat,format);
    return *this;
  }
};

class GLProgram
{
public:
  GLProgram() : initialized(false) {}
  
  ~GLProgram()
  {
    if (initialized)
    {
      glDetachShader(p,v);
      glDetachShader(p,f);
      glDeleteProgram(p);
    }
  }

  bool load(const char* vertexShaderFileName,const char* fragmentShaderFileName)
  {
    if (!initialized) init();
    
    char* vs = NULL;
    char* fs = NULL;

    vs = textFileRead(vertexShaderFileName);
    fs = textFileRead(fragmentShaderFileName);
  
    if (vs==NULL || fs==NULL) return false;
    
    const char* vv = vs;
    const char* ff = fs;
  
    glShaderSource(v,1,&vv,NULL);
    glShaderSource(f,1,&ff,NULL);
  
    delete vs;
    delete fs;
  
    glCompileShader(v);
    glCompileShader(f);
  
    printShaderInfoLog(v);
    printShaderInfoLog(f);
  
    glLinkProgram(p);
    printProgramInfoLog(p);
  
    return true;
  }

  GLProgram& use()
  {
    glUseProgram(p);
    return *this;
  }
  
  GLProgram& setUniform(const char* name,GLint v0)
  {
    glUniform1i(glGetUniformLocation(p,name),v0);
    return *this;
  }

  GLProgram& setUniform(const char* name,GLfloat v0)
  {
    glUniform1f(glGetUniformLocation(p,name),v0);
    return *this;
  }
  
  GLProgram& setUniform(const char* name,GLfloat v0,GLfloat v1)
  {
    setUniform(name,Vec2f(v0,v1));
    return *this;
  }
  
  GLProgram& setUniform(const char* name,GLfloat v0,GLfloat v1,GLfloat v2)
  {
    setUniform(name,Vec3f(v0,v1,v2));
    return *this;
  }
  
  GLProgram& setUniform(const char* name,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3)
  {
    setUniform(name,Vec4f(v0,v1,v2,v3));
    return *this;
  }

  GLProgram& setUniform(const char* name,const Vec2f& v)
  {
    glUniform2f(glGetUniformLocation(p,name),v(0),v(1));
    return *this;
  }
  
  GLProgram& setUniform(const char* name,const Vec3f& v)
  {
    glUniform3f(glGetUniformLocation(p,name),v(0),v(1),v(2));
    return *this;
  }
  
  GLProgram& setUniform(const char* name,const Vec4f& v)
  {
    glUniform4f(glGetUniformLocation(p,name),v(0),v(1),v(2),v(3));
    return *this;
  }
  
  GLProgram& setUniform(const char* name,const Mat2x2f& m)
  {
    glUniformMatrix2fv(glGetUniformLocation(p,name),1,1,m.constRowMajorData());
    return *this;
  }
  
  GLProgram& setUniform(const char* name,const Mat3x3f& m)
  {
    glUniformMatrix3fv(glGetUniformLocation(p,name),1,1,m.constRowMajorData());
    return *this;
  }

  GLProgram& setUniform(const char* name,const Mat4x4f& m)
  {
    glUniformMatrix4fv(glGetUniformLocation(p,name),1,1,m.constRowMajorData());
    return *this;
  }

  GLProgram& bindTexture(const char* name,GLTexture& texture,GLint unit)
  {
    setUniform(name,unit);
    glActiveTexture(GL_TEXTURE0+unit);
    texture.bind();
    return *this;
  }
  
  /* 
  template<typename T> GLProgram& set(const char* name,const T& value)
  {
    return setUniform(name,value);
  }
  */
  
  GLint getAttribLocation(const char* name)
  {
    return glGetAttribLocation(p,name);
  }

private:
  GLuint v;
  GLuint f;
  GLuint p;

  bool initialized;

  void init()
  {
    p = glCreateProgram();
    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);
    glAttachShader(p,v);
    glAttachShader(p,f);   
    
    initialized = true; 
  };

  char* textFileRead(const char* fileName)
  {
    FILE *fp;
    char *content = NULL;

    int count=0;

    if (fileName != NULL) 
    {
      fp = fopen(fileName,"rt");

      if (fp != NULL)
      {        
        fseek(fp, 0, SEEK_END);
        count = ftell(fp);
        rewind(fp);

        if (count > 0)
        {
          content = new char[count+1];
          count = fread(content,sizeof(char),count,fp);
          content[count] = '\0';
        }
        fclose(fp);
      }
    }  
    
    return content;
  }
  
  void printShaderInfoLog(GLuint obj)
  {
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    
    glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&infologLength);
    
    if (infologLength > 1)
    {
      infoLog = (char *)malloc(infologLength);
      glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
      printf("%s\n",infoLog);
      free(infoLog);
    }
  }
  
  void printProgramInfoLog(GLuint obj)
  {
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    
    glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&infologLength);
    
    if (infologLength > 1)
    {
      infoLog = (char *)malloc(infologLength);
      glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
      printf("%s\n",infoLog);
      free(infoLog);
    }
  }
};

#endif
