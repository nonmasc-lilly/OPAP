#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef OPAP_H
#define OPAP_H

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
#define UCODE unsigned int
#define ASCII unsigned char
#define BYTE  unsigned char
#define COLOR unsigned int

typedef enum func_modes {
    OFGET, OFSET
} FUNCMODE;

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
void opap_key_callback(GLFWwindow *window, unsigned int codepoint);
COLOR get_opap_coord(OPAP_T *opap, COORD x, COORD Y);
ASCII opap_consume_key(OPAP_T *opap, UCODE *dump);
void opap_mkey_callback(GLFWwindow *win, int key, int scancode,
    int action, int mods);
void opap_key_handling(GLFWwindow *win, ASCII *io, FUNCMODE mode);
void opap_special_key_handling(GLFWwindow *win, UCODE *io, FUNCMODE mode);

#endif
