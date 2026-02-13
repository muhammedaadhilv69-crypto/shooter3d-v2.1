#ifndef STBI_INCLUDE_STB_IMAGE_H
#define STBI_INCLUDE_STB_IMAGE_H

#ifndef STBIDEF
#ifdef STB_IMAGE_STATIC
#define STBIDEF static
#else
#define STBIDEF extern
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char stbi_uc;
typedef unsigned short stbi_us;

STBIDEF stbi_uc *stbi_load_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *channels_in_file, int desired_channels);
STBIDEF stbi_uc *stbi_load(char const *filename, int *x, int *y, int *channels_in_file, int desired_channels);
STBIDEF stbi_uc *stbi_load_from_file(void *f, int *x, int *y, int *channels_in_file, int desired_channels);

STBIDEF stbi_us *stbi_load_16_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *channels_in_file, int desired_channels);
STBIDEF stbi_us *stbi_load_16(char const *filename, int *x, int *y, int *channels_in_file, int desired_channels);
STBIDEF stbi_us *stbi_load_16_from_file(void *f, int *x, int *y, int *channels_in_file, int desired_channels);

STBIDEF float *stbi_loadf_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *channels_in_file, int desired_channels);
STBIDEF float *stbi_loadf(char const *filename, int *x, int *y, int *channels_in_file, int desired_channels);
STBIDEF float *stbi_loadf_from_file(void *f, int *x, int *y, int *channels_in_file, int desired_channels);

STBIDEF void stbi_hdr_to_ldr_gamma(float gamma);
STBIDEF void stbi_hdr_to_ldr_scale(float scale);
STBIDEF void stbi_ldr_to_hdr_gamma(float gamma);
STBIDEF void stbi_ldr_to_hdr_scale(float scale);

STBIDEF int stbi_is_hdr_from_memory(stbi_uc const *buffer, int len);
STBIDEF int stbi_is_hdr(char const *filename);
STBIDEF int stbi_is_hdr_from_file(void *f);

STBIDEF const char *stbi_failure_reason(void);
STBIDEF void stbi_image_free(void *retval_from_stbi_load);
STBIDEF int stbi_info(char const *filename, int *x, int *y, int *comp);
STBIDEF int stbi_info_from_file(void *f, int *x, int *y, int *comp);
STBIDEF int stbi_info_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp);

STBIDEF void stbi_set_unpremultiply_on_load(int flag_true_if_should_unpremultiply);
STBIDEF void stbi_convert_iphone_png_to_rgb(int flag_true_if_should_convert);
STBIDEF void stbi_set_flip_vertically_on_load(int flag_true_if_should_flip);

STBIDEF char *stbi_zlib_decode_malloc_guesssize(const char *buffer, int len, int initial_size, int *outlen);
STBIDEF char *stbi_zlib_decode_malloc_guesssize_headerflag(const char *buffer, int len, int initial_size, int *outlen, int parse_header);
STBIDEF char *stbi_zlib_decode_malloc(const char *buffer, int len, int *outlen);
STBIDEF int stbi_zlib_decode_buffer(char *obuffer, int olen, const char *ibuffer, int ilen);
STBIDEF char *stbi_zlib_decode_noheader_malloc(const char *buffer, int len, int *outlen);
STBIDEF int stbi_zlib_decode_noheader_buffer(char *obuffer, int olen, const char *ibuffer, int ilen);

#ifdef __cplusplus
}
#endif

#endif // STBI_INCLUDE_STB_IMAGE_H
