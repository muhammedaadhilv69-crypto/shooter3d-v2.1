/*
 * Minimal OpenGL 3.3 Core function loader using SDL2_GL_GetProcAddress
 */
#include "glad/glad.h"
#include <stdio.h>

/* ---- Function pointer definitions ---- */
PFNGLGENVERTEXARRAYSPROC         glad_glGenVertexArrays         = NULL;
PFNGLBINDVERTEXARRAYPROC         glad_glBindVertexArray         = NULL;
PFNGLDELETEVERTEXARRAYSPROC      glad_glDeleteVertexArrays      = NULL;
PFNGLGENBUFFERSPROC              glad_glGenBuffers              = NULL;
PFNGLBINDBUFFERPROC              glad_glBindBuffer              = NULL;
PFNGLDELETEBUFFERSPROC           glad_glDeleteBuffers           = NULL;
PFNGLBUFFERDATAPROC              glad_glBufferData              = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glad_glDisableVertexAttribArray = NULL;
PFNGLVERTEXATTRIBPOINTERPROC     glad_glVertexAttribPointer     = NULL;
PFNGLCREATESHADERPROC            glad_glCreateShader            = NULL;
PFNGLDELETESHADERPROC            glad_glDeleteShader            = NULL;
PFNGLSHADERSOURCEPROC            glad_glShaderSource            = NULL;
PFNGLCOMPILESHADERPROC           glad_glCompileShader           = NULL;
PFNGLGETSHADERIVPROC             glad_glGetShaderiv             = NULL;
PFNGLGETSHADERINFOLOGPROC        glad_glGetShaderInfoLog        = NULL;
PFNGLCREATEPROGRAMPROC           glad_glCreateProgram           = NULL;
PFNGLDELETEPROGRAMPROC           glad_glDeleteProgram           = NULL;
PFNGLATTACHSHADERPROC            glad_glAttachShader            = NULL;
PFNGLLINKPROGRAMPROC             glad_glLinkProgram             = NULL;
PFNGLGETPROGRAMIVPROC            glad_glGetProgramiv            = NULL;
PFNGLGETPROGRAMINFOLOGPROC       glad_glGetProgramInfoLog       = NULL;
PFNGLUSEPROGRAMPROC              glad_glUseProgram              = NULL;
PFNGLGETUNIFORMLOCATIONPROC      glad_glGetUniformLocation      = NULL;
PFNGLUNIFORM1IPROC               glad_glUniform1i               = NULL;
PFNGLUNIFORM1FPROC               glad_glUniform1f               = NULL;
PFNGLUNIFORM3FVPROC              glad_glUniform3fv              = NULL;
PFNGLUNIFORM4FVPROC              glad_glUniform4fv              = NULL;
PFNGLUNIFORMMATRIX4FVPROC        glad_glUniformMatrix4fv        = NULL;
PFNGLACTIVETEXTUREPROC           glad_glActiveTexture           = NULL;
PFNGLGENERATEMIPMAPPROC          glad_glGenerateMipmap          = NULL;
PFNGLBLENDEQUATIONPROC           glad_glBlendEquation           = NULL;
PFNGLBLENDFUNCSEPARATEPROC       glad_glBlendFuncSeparate       = NULL;

#define LOAD(name) glad_##name = (PFN##name##PROC) SDL_GL_GetProcAddress(#name); \
    if (!glad_##name) { fprintf(stderr, "WARNING: Could not load " #name "\n"); }

int gladLoadGL(void) {
    LOAD(glGenVertexArrays)
    LOAD(glBindVertexArray)
    LOAD(glDeleteVertexArrays)
    LOAD(glGenBuffers)
    LOAD(glBindBuffer)
    LOAD(glDeleteBuffers)
    LOAD(glBufferData)
    LOAD(glEnableVertexAttribArray)
    LOAD(glDisableVertexAttribArray)
    LOAD(glVertexAttribPointer)
    LOAD(glCreateShader)
    LOAD(glDeleteShader)
    LOAD(glShaderSource)
    LOAD(glCompileShader)
    LOAD(glGetShaderiv)
    LOAD(glGetShaderInfoLog)
    LOAD(glCreateProgram)
    LOAD(glDeleteProgram)
    LOAD(glAttachShader)
    LOAD(glLinkProgram)
    LOAD(glGetProgramiv)
    LOAD(glGetProgramInfoLog)
    LOAD(glUseProgram)
    LOAD(glGetUniformLocation)
    LOAD(glUniform1i)
    LOAD(glUniform1f)
    LOAD(glUniform3fv)
    LOAD(glUniform4fv)
    LOAD(glUniformMatrix4fv)
    LOAD(glActiveTexture)
    LOAD(glGenerateMipmap)
    LOAD(glBlendEquation)
    LOAD(glBlendFuncSeparate)

    if (!glad_glGenVertexArrays || !glad_glCreateShader || !glad_glCreateProgram) {
        fprintf(stderr, "ERROR: Failed to load essential OpenGL 3.3 functions.\n");
        fprintf(stderr, "Make sure your GPU supports OpenGL 3.3 and drivers are up-to-date.\n");
        return 0;
    }
    return 1;
}
