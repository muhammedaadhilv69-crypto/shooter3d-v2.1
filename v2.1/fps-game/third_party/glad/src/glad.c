#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glad/glad.h>

struct gladGLversionStruct GLVersion;

// Define all function pointers
#define GLAD_FUNCTION_POINTER(type, name) type name = NULL;
#include "glad/glad_func_ptrs.h"
#undef GLAD_FUNCTION_POINTER

int gladLoadGLLoader(GLADloadproc load) {
    if(load == NULL) return 0;
    
    // Load OpenGL function pointers
    #define GLAD_FUNCTION_POINTER(type, name) name = (type)load(#name);
    #include "glad/glad_func_ptrs.h"
    #undef GLAD_FUNCTION_POINTER
    
    GLVersion.major = 3;
    GLVersion.minor = 3;
    
    return 1;
}
