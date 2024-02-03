#include "opap.h"

OSHADER default_shader() {
    OBOOL success;
    char infolog[512];
    static OSHADER shader = 0;
    const char *shader_src[2] = {
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"
        "out vec2 TexCoord;\n"
        "uniform vec2 scale;"
        "void main() {\n"
        "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "  TexCoord = aTexCoord;\n"
        "}\n",

        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec2 TexCoord;\n"
        "uniform sampler2D Texture;\n"
        "void main() {\n"
        "  FragColor = texture(Texture, vec2(TexCoord.x, -TexCoord.y));\n"
        "}\n"};
    if(!shader) {
        OSHADER vshader, fshader;
        vshader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vshader, 1, &shader_src[0], NULL);
        glCompileShader(vshader);
        glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(vshader, 512, NULL, infolog);
            OPAPERR("default_shader compile failure (v)", infolog);
            return 0;
        }
        fshader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fshader, 1, &shader_src[1], NULL);
        glCompileShader(fshader);
        glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(fshader, 512, NULL, infolog);
            OPAPERR("default_shader compile failure (f)", infolog);
            return 0;
        }
        shader = glCreateProgram();
        glAttachShader(shader, vshader);
        glAttachShader(shader, fshader);
        glLinkProgram(shader);
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shader, 512, NULL, infolog);
            OPAPERR("default_shader link failure", infolog);
        }
        glDeleteShader(vshader);
        glDeleteShader(fshader);
    }
    return shader;
}

OPAP_T create_opap(unsigned int width, unsigned int height,
        unsigned int resolutionx, unsigned int resolutiony,
        const char *name) {
    const float obuf[] = {
         1,  1, 0,    1, 1,
         1, -1, 0,    1, 0,
        -1, -1, 0,    0, 0,
        -1,  1, 0,    0, 1};
    const unsigned int oind[] = {
        0, 1, 3,
        1, 2, 3
    };
    static int initiated = 0;
    OPAP_T ret = {0};
    if(!initiated) {
        glfwInit();
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    ret.winhandle = glfwCreateWindow(width, height, name, NULL, NULL);
    ret.width = width;
    ret.height = height;
    ret.is_closed = OFALSE;
    if(ret.winhandle == NULL) {
        OPAPERR("create_opap failure", "glfwCreateWindow failed");
        return ret;
    }
    if(!initiated) {
        glfwMakeContextCurrent(ret.winhandle);
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            OPAPERR("create_opap failure", "gladLoadGLLoader failed");
            return ret;
        }
        glViewport(0, 0, width, height);
    }
    ret.dt_shader = default_shader();
    ret.resx = resolutionx;
    ret.resy = resolutiony;
    glUseProgram(ret.dt_shader);
    glUniform2f(glGetUniformLocation(ret.dt_shader, "scale"), (
        (float)ret.resx/(float)ret.width),((float)ret.resy/(float)ret.height));
    glGenVertexArrays(1, &ret.dt_vao);
    glBindVertexArray(ret.dt_vao);
    glGenBuffers(1, &ret.dt_vbo);
    glGenBuffers(1, &ret.dt_ebo);
    glGenTextures(1, &ret.dt_texture);
    glBindBuffer(GL_ARRAY_BUFFER, ret.dt_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(obuf), obuf, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret.dt_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(oind), oind, GL_DYNAMIC_DRAW);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ret.dt_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    ret.pixel_display = calloc(1,ret.resx * ret.resy * 4);
    if(ret.pixel_display) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ret.resx, ret.resy, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, ret.pixel_display);
    } else {
        OPAPERR("create_opap failure", "failed to calloc pixel_display");
    }
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float),
        (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0); 
    return ret;
}

void set_opap_shader(OPAP_T *opap, OSHADER shader) {
    opap->dt_shader = shader;
}

void start_handle_opap(OPAP_T *opap) {
    if(opap->dt_ishandling) {
        OPAPERR("start_handle_opap warning", "opap is already handling");
    }
    opap->dt_ishandling = OTRUE;
    glfwSwapBuffers(opap->winhandle);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(opap->dt_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, opap->dt_texture);
    glUseProgram(opap->dt_shader);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void end_handle_opap(OPAP_T *opap) {
    if(!opap->dt_ishandling) {
        OPAPERR("end_handle_opap warning", "opap was never handling");
    }
    if(opap->needs_updating) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, opap->dt_texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, opap->resx,
            opap->resy, GL_RGBA, GL_UNSIGNED_BYTE,
            opap->pixel_display);
        opap->needs_updating = OFALSE;
    }
    opap->dt_ishandling = OFALSE;
    glfwPollEvents();
    opap->is_closed = glfwWindowShouldClose(opap->winhandle);
}

void set_opap_coord(OPAP_T *opap, COORD x, COORD y, COLOR color) {
    opap->pixel_display[3+(x + y*opap->resx)*4] = ((unsigned char*)&color)[0];
    opap->pixel_display[2+(x + y*opap->resx)*4] = ((unsigned char*)&color)[1];
    opap->pixel_display[1+(x + y*opap->resx)*4] = ((unsigned char*)&color)[2];
    opap->pixel_display[0+(x + y*opap->resx)*4] = ((unsigned char*)&color)[3];
}

COLOR get_opap_coord(OPAP_T *opap, COORD x, COORD y) {
    return ((COLOR*)opap->pixel_display)[x+y*opap->resx];
}

