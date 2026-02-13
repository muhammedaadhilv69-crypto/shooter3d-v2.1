/*
 * Minimal GLAD-style OpenGL 3.3 Core loader
 * Uses SDL2's GL function loader under the hood.
 * This replaces a full glad generation for portability.
 */
#ifndef GLAD_H
#define GLAD_H

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#ifdef __APPLE__
  #include <OpenGL/gl3.h>
#else
  #include <GL/gl.h>
#endif

#include <stddef.h>
#include <stdint.h>

/* ---- Types not always defined in old gl.h ---- */
#ifndef GLAPIENTRY
  #ifdef _WIN32
    #define GLAPIENTRY __stdcall
  #else
    #define GLAPIENTRY
  #endif
#endif

typedef void   (GLAPIENTRY *PFNGLGENVERTEXARRAYSPROC)(GLsizei, GLuint*);
typedef void   (GLAPIENTRY *PFNGLBINDVERTEXARRAYPROC)(GLuint);
typedef void   (GLAPIENTRY *PFNGLDELETEVERTEXARRAYSPROC)(GLsizei, const GLuint*);
typedef void   (GLAPIENTRY *PFNGLGENBUFFERSPROC)(GLsizei, GLuint*);
typedef void   (GLAPIENTRY *PFNGLBINDBUFFERPROC)(GLenum, GLuint);
typedef void   (GLAPIENTRY *PFNGLDELETEBUFFERSPROC)(GLsizei, const GLuint*);
typedef void   (GLAPIENTRY *PFNGLBUFFERDATAPROC)(GLenum, GLsizeiptr, const void*, GLenum);
typedef void   (GLAPIENTRY *PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint);
typedef void   (GLAPIENTRY *PFNGLDISABLEVERTEXATTRIBARRAYPROC)(GLuint);
typedef void   (GLAPIENTRY *PFNGLVERTEXATTRIBPOINTERPROC)(GLuint, GLint, GLenum, GLboolean, GLsizei, const void*);
typedef GLuint (GLAPIENTRY *PFNGLCREATESHADERPROC)(GLenum);
typedef void   (GLAPIENTRY *PFNGLDELETESHADERPROC)(GLuint);
typedef void   (GLAPIENTRY *PFNGLSHADERSOURCEPROC)(GLuint, GLsizei, const GLchar* const*, const GLint*);
typedef void   (GLAPIENTRY *PFNGLCOMPILESHADERPROC)(GLuint);
typedef void   (GLAPIENTRY *PFNGLGETSHADERIVPROC)(GLuint, GLenum, GLint*);
typedef void   (GLAPIENTRY *PFNGLGETSHADERINFOLOGPROC)(GLuint, GLsizei, GLsizei*, GLchar*);
typedef GLuint (GLAPIENTRY *PFNGLCREATEPROGRAMPROC)(void);
typedef void   (GLAPIENTRY *PFNGLDELETEPROGRAMPROC)(GLuint);
typedef void   (GLAPIENTRY *PFNGLATTACHSHADERPROC)(GLuint, GLuint);
typedef void   (GLAPIENTRY *PFNGLLINKPROGRAMPROC)(GLuint);
typedef void   (GLAPIENTRY *PFNGLGETPROGRAMIVPROC)(GLuint, GLenum, GLint*);
typedef void   (GLAPIENTRY *PFNGLGETPROGRAMINFOLOGPROC)(GLuint, GLsizei, GLsizei*, GLchar*);
typedef void   (GLAPIENTRY *PFNGLUSEPROGRAMPROC)(GLuint);
typedef GLint  (GLAPIENTRY *PFNGLGETUNIFORMLOCATIONPROC)(GLuint, const GLchar*);
typedef void   (GLAPIENTRY *PFNGLUNIFORM1IPROC)(GLint, GLint);
typedef void   (GLAPIENTRY *PFNGLUNIFORM1FPROC)(GLint, GLfloat);
typedef void   (GLAPIENTRY *PFNGLUNIFORM3FVPROC)(GLint, GLsizei, const GLfloat*);
typedef void   (GLAPIENTRY *PFNGLUNIFORM4FVPROC)(GLint, GLsizei, const GLfloat*);
typedef void   (GLAPIENTRY *PFNGLUNIFORMMATRIX4FVPROC)(GLint, GLsizei, GLboolean, const GLfloat*);
typedef void   (GLAPIENTRY *PFNGLACTIVETEXTUREPROC)(GLenum);
typedef void   (GLAPIENTRY *PFNGLGENERATEMIPMAPPROC)(GLenum);
typedef void   (GLAPIENTRY *PFNGLBLENDEQUATIONPROC)(GLenum);
typedef void   (GLAPIENTRY *PFNGLBLENDFUNCSEPARATEPROC)(GLenum, GLenum, GLenum, GLenum);

/* ---- Function pointer declarations ---- */
extern PFNGLGENVERTEXARRAYSPROC         glad_glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC         glad_glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC      glad_glDeleteVertexArrays;
extern PFNGLGENBUFFERSPROC              glad_glGenBuffers;
extern PFNGLBINDBUFFERPROC              glad_glBindBuffer;
extern PFNGLDELETEBUFFERSPROC           glad_glDeleteBuffers;
extern PFNGLBUFFERDATAPROC              glad_glBufferData;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glad_glDisableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC     glad_glVertexAttribPointer;
extern PFNGLCREATESHADERPROC            glad_glCreateShader;
extern PFNGLDELETESHADERPROC            glad_glDeleteShader;
extern PFNGLSHADERSOURCEPROC            glad_glShaderSource;
extern PFNGLCOMPILESHADERPROC           glad_glCompileShader;
extern PFNGLGETSHADERIVPROC             glad_glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC        glad_glGetShaderInfoLog;
extern PFNGLCREATEPROGRAMPROC           glad_glCreateProgram;
extern PFNGLDELETEPROGRAMPROC           glad_glDeleteProgram;
extern PFNGLATTACHSHADERPROC            glad_glAttachShader;
extern PFNGLLINKPROGRAMPROC             glad_glLinkProgram;
extern PFNGLGETPROGRAMIVPROC            glad_glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC       glad_glGetProgramInfoLog;
extern PFNGLUSEPROGRAMPROC              glad_glUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC      glad_glGetUniformLocation;
extern PFNGLUNIFORM1IPROC               glad_glUniform1i;
extern PFNGLUNIFORM1FPROC               glad_glUniform1f;
extern PFNGLUNIFORM3FVPROC              glad_glUniform3fv;
extern PFNGLUNIFORM4FVPROC              glad_glUniform4fv;
extern PFNGLUNIFORMMATRIX4FVPROC        glad_glUniformMatrix4fv;
extern PFNGLACTIVETEXTUREPROC           glad_glActiveTexture;
extern PFNGLGENERATEMIPMAPPROC          glad_glGenerateMipmap;
extern PFNGLBLENDEQUATIONPROC           glad_glBlendEquation;
extern PFNGLBLENDFUNCSEPARATEPROC       glad_glBlendFuncSeparate;

/* ---- Macros to redirect calls ---- */
#define glGenVertexArrays         glad_glGenVertexArrays
#define glBindVertexArray         glad_glBindVertexArray
#define glDeleteVertexArrays      glad_glDeleteVertexArrays
#define glGenBuffers              glad_glGenBuffers
#define glBindBuffer              glad_glBindBuffer
#define glDeleteBuffers           glad_glDeleteBuffers
#define glBufferData              glad_glBufferData
#define glEnableVertexAttribArray glad_glEnableVertexAttribArray
#define glDisableVertexAttribArray glad_glDisableVertexAttribArray
#define glVertexAttribPointer     glad_glVertexAttribPointer
#define glCreateShader            glad_glCreateShader
#define glDeleteShader            glad_glDeleteShader
#define glShaderSource            glad_glShaderSource
#define glCompileShader           glad_glCompileShader
#define glGetShaderiv             glad_glGetShaderiv
#define glGetShaderInfoLog        glad_glGetShaderInfoLog
#define glCreateProgram           glad_glCreateProgram
#define glDeleteProgram           glad_glDeleteProgram
#define glAttachShader            glad_glAttachShader
#define glLinkProgram             glad_glLinkProgram
#define glGetProgramiv            glad_glGetProgramiv
#define glGetProgramInfoLog       glad_glGetProgramInfoLog
#define glUseProgram              glad_glUseProgram
#define glGetUniformLocation      glad_glGetUniformLocation
#define glUniform1i               glad_glUniform1i
#define glUniform1f               glad_glUniform1f
#define glUniform3fv              glad_glUniform3fv
#define glUniform4fv              glad_glUniform4fv
#define glUniformMatrix4fv        glad_glUniformMatrix4fv
#define glActiveTexture           glad_glActiveTexture
#define glGenerateMipmap          glad_glGenerateMipmap
#define glBlendEquation           glad_glBlendEquation
#define glBlendFuncSeparate       glad_glBlendFuncSeparate

/* ---- Constants not always in old gl.h ---- */
#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER           0x8892
#endif
#ifndef GL_ELEMENT_ARRAY_BUFFER
#define GL_ELEMENT_ARRAY_BUFFER   0x8893
#endif
#ifndef GL_STATIC_DRAW
#define GL_STATIC_DRAW            0x88B4
#endif
#ifndef GL_DYNAMIC_DRAW
#define GL_DYNAMIC_DRAW           0x88E8
#endif
#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER          0x8B31
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER        0x8B30
#endif
#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS         0x8B81
#endif
#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS            0x8B82
#endif
#ifndef GL_INFO_LOG_LENGTH
#define GL_INFO_LOG_LENGTH        0x8B84
#endif
#ifndef GL_TEXTURE0
#define GL_TEXTURE0               0x84C0
#endif
#ifndef GL_GENERATE_MIPMAP
#define GL_GENERATE_MIPMAP        0x8191
#endif

/* ---- Loader function ---- */
int gladLoadGL(void);

#endif /* GLAD_H */
