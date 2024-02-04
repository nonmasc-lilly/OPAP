#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPAPERR(caller, error) fprintf(stderr, "OPAPERR: %s: %s\n",\
        caller, error)
#define OBOOL int
#define OVBO  unsigned int
#define OSHADER unsigned int
#define OTEXTURE unsigned int
#define ODISPLAY unsigned char
#define OTRUE 1
#define OFALSE 0
#define COORD unsigned int

#define COLOR unsigned int

typedef struct o_pap {
    GLFWwindow *winhandle;
    unsigned int width, resx;
    unsigned int height, resy;
    OBOOL dt_ishandling;
    OBOOL is_closed;
    OSHADER dt_shader;
    OVBO  dt_vao, dt_vbo, dt_ebo;
    OTEXTURE dt_texture;
    ODISPLAY *pixel_display;
    OBOOL needs_updating;
} OPAP_T;

OSHADER default_shader();

OPAP_T create_opap(unsigned int width, unsigned int height,
        unsigned int resolutionx, unsigned int resolutiony,
        const char *name);
void set_opap_shader(OPAP_T *opap, OSHADER shader);
void start_handle_opap(OPAP_T *opap);
void end_handle_opap(OPAP_T *opap);
void set_opap_coord(OPAP_T *opap, COORD x, COORD y, COLOR color);
COLOR get_opap_coord(OPAP_T *opap, COORD x, COORD Y);

