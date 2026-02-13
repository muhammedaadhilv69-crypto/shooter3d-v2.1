#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STBI_ONLY_BMP

#include "stb_image.h"

// Minimal implementation for demo purposes
// In production, use the full stb_image.h implementation

static const char *failure_reason = NULL;

STBIDEF const char *stbi_failure_reason(void) {
    return failure_reason;
}

STBIDEF void stbi_image_free(void *retval_from_stbi_load) {
    free(retval_from_stbi_load);
}

STBIDEF void stbi_set_flip_vertically_on_load(int flag_true_if_should_flip) {
    // Stub
}

STBIDEF stbi_uc *stbi_load_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *channels_in_file, int desired_channels) {
    failure_reason = "Image loading not implemented in minimal version";
    return NULL;
}

STBIDEF stbi_uc *stbi_load(char const *filename, int *x, int *y, int *channels_in_file, int desired_channels) {
    failure_reason = "File loading not implemented in minimal version";
    return NULL;
}

STBIDEF int stbi_info(char const *filename, int *x, int *y, int *comp) {
    failure_reason = "Info not implemented in minimal version";
    return 0;
}
