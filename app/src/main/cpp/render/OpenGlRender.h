//
// Created by Sumn on 2022/7/20.
//

#ifndef TRANSPORTSDKDEMO_OPENGLRENDER_H
#define TRANSPORTSDKDEMO_OPENGLRENDER_H

extern "C" {
#include <libavutil/frame.h>
}

#include <GLES2/gl2.h>
#include <EGL/egl.h>

class OpenGlRender {
public:
    OpenGlRender(ANativeWindow *nwin);

    ~OpenGlRender();

    int render(AVFrame *frame);

private:
    GLint InitShader(const char *code, GLint type);

private:
    ANativeWindow *p_win = NULL;
    int length = 0;
    int width = 368;
    int height = 640;
    unsigned char *buf[3]{};
    GLuint texts[3]{};
    EGLDisplay display;
    EGLSurface winsurface;
    EGLConfig config{};
    EGLContext context;


};


#endif //TRANSPORTSDKDEMO_OPENGLRENDER_H
