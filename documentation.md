# OPAP DOCUMENTATION

this describes every function struct and type opap creates

- OPAPERR(caller, error) : type macro
- + prints an error message formatted with ("OPAPERR: %s: %s\n", caller, error) to stderr
- OBOOL : type macro
- + resolves to int, for use in conditionals
- OTRUE : type macro
- + resolves to 1, for use with OBOOL
- OFALSE : type macro
- + resolves to 0, for use with OBOOL
- OSHADER : type macro
- + resolves to unsigned int, for use with opengl shaders
- OTEXTURE : type macro
- + resolves to unsigned int, for use with opengl textures
- ODISPLAY : type macro
- + resolves to unsigned char, for use as display buffer unit
- OVBO : type macro
- + resolves to unsigned int, for use with opengl buffers
- COORD : type macto
- + resolves to unsigned int, for use as x or y value in coord functions
- COLOR : type macro
- + resolves to unsigned int, for use as a 32 bit representation of four ODISPLAY units
- UCODE : type macro
- + resolves to unsigned int, for use as a UTF32 code
- ASCII : type macro
- + resolves to unsigned char, for use as an ASCII code
- BYTE : type macro
- + resolves to unsigned char, for use as byte memory
- FUNCMODE : type enum
- + a set of modes used by various state functions
```C
typedef enum func_modes {
    OFGET, /* get state */
    OFSET, /* set state */
} FUNCMODE;
```

- OPAP\_T : type struct
```C
typedef o_pap {
    GLFWwindow *winhandle;   /* GLFW window for use in OPAP functions */
    unsigned int width,      /* width of window in pixels */
                 height,     /* height of window in pixels */
                 resx,       /* width of the pixel_display in pixels */
                 resy;       /* height of the pixel_display in pixels */
    OBOOL dt_ishandling,     /* whether or not the opap is being handled currently */
          is_closed,         /* whether or not glfw has closed the window */
          needs_updating;    /* whether or not to redraw the pixel_display to winhandle */
    OSHADER dt_shader;       /* currently used shader */
    OVBO dt_vao,             /* OPENGL vao for use in OPAP functions */
         dt_vbo,             /* OPENGL vbo for use in OPAP functions stores the screen quad */
         dt_ebo;             /* OPENGL ebo for use in OPAP functions stores screen indices */
    OTEXTURE texture;        /* OPENGL texture for use in OPAP functions to show pixel_display*/
    ODISPLAY *pixel_display; /* buffer formatted with UNSIGNED BYTE RGBA that represents pixel information to draw to the screen */
} OPAP_T;
```
- + dt\_ indicates that an item musnt be changed by the developer, only by OPAP functions.

- create\_opap : type function
- + creates a properly configured OPAP for future use
```C
OPAP_T create_opap(
    unsigned int width,       /* width of screen in pixels */
    unsigned int height,      /* height of screen in pixels */
    unsigned int resolutionx, /* width of pixel display in pixels */
    unsigned int resolutiony, /* height of pixel display in pixels */
    const char *name          /* name of the window */
);
```

- set\_opap\_shader : type function
- + sets the dt\_shader of an opap
- + - warning: a shader of the opap must still implement the requirements of default\_shader
```C
void set_opap_shader(
    OPAP_T *opap,   /* opap to modify */
    OSHADER shader  /* shader to set opap to */
);
```

- default\_shader : type function
- + returns the OSHADER corresponding to the default\_shader
```C
OSHADER default_shader();
```
- + the default shader is comprised of two subshaders, the vertex and fragment shader
```glsl
// vertex shader
#version 330 core
layout (location = 0) in vec3 aPos; // from the screen quad vbo, must be handled
layout (location = 1) in vec2 aTexCoord; // from the texture object must be handled
out vec2 TexCoord; // must be returned for the fragment shader

uniform vec2 scale; // equal to (resx/width, resy/height)

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoord = aTexCoord;
}
```

```glsl
// fragment shader
#version 330 core
out vec4 FragColor; // return color, must be returned to render
in vec2 TexCoord; // texture coordinates, inputted by vector

uniform sampler2D Texture; // texture set by opap functions, must be handled

void main() {
    FragColor = texture(Texture, vec2(TexCoord.x, -TexCoord.y));
}
```

- start\_handle\_opap : type function
- + starts handling of opap and rendering, all changes to opap should occur during handling
```C
void start_handle_opap(
    OPAP_T *opap /* opap to handle */
);
```

- end\_handle\_opap
- + ends handling of opap and applies any changes to pixel\_display when opap\-\>needs\_updating is OTRUE
```
void end_handle_opap(
    OPAP_T *opap /* opap handled */
);
```

- set\_opap\_coord
- + sets the pixel in pixel\_display at resolved coordinate (x, y) to color
```
void set_opap_coord(
    OPAP_T *opap, /* opap to set pixel_display */
    COORD x,      /* x coordinate */
    COORD y,      /* y coordinate */
    COLOR color   /* RGBA color */
);
```

- get\_opap\_coord
- + gets the COLOR of pixel in pixel\_display at resolved coordinate (x, y)
```
COLOR get_opap_coord(
    OPAP_T *opap, /* opap to get pixel from */
    COORD x,      /* x coordinate */
    COORD y       /* y coordinate */
);
```

- opap\_key\_callback : type func
- + char callback for use by glfw
```C
void opap_key_callback(
    GLFWwindow *window,     /* window mounted */
    unsigned int codepoint  /* UTF32 codepoint */
);
```

- opap\_mkey\_callback : type func
- + key callback for use by glfw
```C
void opap_mkey_callback(
    GLFWwindow *window, /* window mounted */
    int key,            /* keycode */
    int scancode,       /* scancode */
    int action,         /* action type */
    int mods            /* modifier keys */
);
```

- opap\_key\_handling
- + handles a windows ascii key press
```C
void opap_key_handling(
    GLFWwindow *win, /* window to handle */
    ASCII *io,       /* io port, depends on mode */
    FUNCMODE mode    /* when OFGET sets *io to the
                        last handled key of win
                        when OFSET sets the handled
                        key of win to *io */
)
```

- opap\_special\_key\_handling
- + opap\_key\_handling but for UCODE keys
```C
void opap_special_key_handling(
    GLFWwindow *win, /* window to handle */
    UCODE *io,       /* io port, depends on mode */
    FUNCMODE mode    /* see opap_key_handling */
);
```

- opap\_consume\_key
- + gets the last handled key of an opaps winhandle, and then sets it to zero, returns the key gotten
```C
ASCII opap_consume_key(
    OPAP_T *opap, /* opap to act on */
    UCODE *dump,  /* is set to the last handled
                     special key, the special key
                     is also set to zero by
                     the end of the function */
);
```


### NOTES

- coordinates come in two kinds
- + global: coordinates of the screen, max being (opap.width-1, opap.height-1)
- + resolved: coordinates of the pixel\_display, max being (opap.resx-1, opap.resy-1)
- + + resolved coordinates are mapped to global coordinate, such that resolved coordinates still fill the screen
