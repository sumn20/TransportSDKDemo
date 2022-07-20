//
// Created by Sumn on 2022/7/20.
//

#include "OpenGlRender.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>

#include <android/native_window_jni.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <log_util.h>

#define GET_STR(x) #x

const char *vertexShader = GET_STR(

        attribute vec4 aPosition; //顶点坐标，在外部获取传递进来

        attribute vec2 aTexCoord; //材质（纹理）顶点坐标

        varying vec2 vTexCoord;   //输出的材质（纹理）坐标，给片元着色器使用
        void main() {
            //纹理坐标转换，以左上角为原点的纹理坐标转换成以左下角为原点的纹理坐标，
            // 比如以左上角为原点的（0，0）对应以左下角为原点的纹理坐标的（0，1）
            vTexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
            gl_Position = aPosition;
        }
);
const char *fragYUV420P = GET_STR(

        precision mediump float;    //精度

        varying vec2 vTexCoord;     //顶点着色器传递的坐标，相同名字opengl会自动关联

        uniform sampler2D yTexture; //输入的材质（不透明灰度，单像素）

        uniform sampler2D uTexture;

        uniform sampler2D vTexture;
        void main() {
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture, vTexCoord).r; // y分量
            // 因为UV的默认值是127，所以我们这里要减去0.5（OpenGLES的Shader中会把内存中0～255的整数数值换算为0.0～1.0的浮点数值）
            yuv.g = texture2D(uTexture, vTexCoord).r - 0.5; // u分量
            yuv.b = texture2D(vTexture, vTexCoord).r - 0.5; // v分量
            // yuv转换成rgb，两种方法，一种是RGB按照特定换算公式单独转换
            // 另外一种是使用矩阵转换
            rgb = mat3(1.0, 1.0, 1.0,
                       0.0, -0.39465, 2.03211,
                       1.13983, -0.58060, 0.0) * yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb, 1.0);
        }
);
//加入三维顶点数据 两个三角形组成正方形
static float vers[] = {
        1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
};
//加入材质坐标数据
static float txts[] = {
        1.0f, 0.0f, //右下
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0, 1.0
};


OpenGlRender::OpenGlRender(ANativeWindow *nwin) {
    LOGCATI("OpenGlRender()");
    p_win = nwin;
    //1 EGL display创建和初始化
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        LOGCATE("eglGetDisplay failed!");
        return;
    }
    if (EGL_TRUE != eglInitialize(display, 0, 0)) {
        LOGCATE("eglInitialize failed!");
        return;
    }
    //2 surface
    //2-1 surface窗口配置
    //输出配置

    EGLint configNum;
    EGLint configSpec[] = {
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE
    };
    if (EGL_TRUE != eglChooseConfig(display, configSpec, &config, 1, &configNum)) {
        LOGCATE("eglChooseConfig failed!");
        return;
    }
    //创建surface
    winsurface = eglCreateWindowSurface(display, config, p_win, 0);
    if (winsurface == EGL_NO_SURFACE) {
        LOGCATE("eglCreateWindowSurface failed!");
        return;
    }
    //3 context 创建关联的上下文
    const EGLint ctxAttr[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
    };
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttr);
    if (context == EGL_NO_CONTEXT) {
        LOGCATE("eglCreateContext failed!");
        return;
    }
    //纹理的修改和显示
    buf[3] = {0};
    buf[0] = new unsigned char[width * height];
    buf[1] = new unsigned char[width * height / 4];
    buf[2] = new unsigned char[width * height / 4];
    LOGCATI("EGL Init Success!");
}

OpenGlRender::~OpenGlRender() {
    LOGCATI("~OpenGlRender()");
    delete buf[0];
    delete buf[1];
    delete buf[2];
    width = NULL;
    height = NULL;
    length = NULL;
    if (p_win) {
        ANativeWindow_release(p_win);
    }
}

int OpenGlRender::render(AVFrame *frame) {
    if (width != frame->width) {
        width = frame->width;
    }
    if (height != frame->height) {
        height = frame->height;
    }
    //长度不变，不改变原始图像的宽高
    if (length == 0) {
        length = width * height;
    }
    // 解码得到YUV数据
    //把原始数据复制到申请的内存里面
    //Y数据
    for (int i = 0; i < height; i++) {
        memcpy(buf[0] + width * i,
               frame->data[0] + frame->linesize[0] * i,
               width);
    }
    //U数据
    for (int j = 0; j < height / 2; j++) {
        memcpy(buf[1] + width / 2 * j,
               frame->data[1] + frame->linesize[1] * j,
               width / 2);
    }
    //V数据
    for (int k = 0; k < height / 2; k++) {
        memcpy(buf[2] + width / 2 * k,
               frame->data[2] + frame->linesize[2] * k,
               width / 2);
    }

    if (EGL_TRUE != eglMakeCurrent(display, winsurface, winsurface, context)) {
        LOGCATE("eglMakeCurrent failed!");
        return -1;
    }
    //创建渲染程序
    GLint program = glCreateProgram();
    if (program == 0) {
        LOGCATE("glCreateProgram failed!");
        return -1;
    }
    //顶点和片元shader初始化
    //顶点shader初始化
    GLint vsh = InitShader(vertexShader, GL_VERTEX_SHADER);
    //片元yuv420 shader初始化
    GLint fsh = InitShader(fragYUV420P, GL_FRAGMENT_SHADER);
    //渲染程序中加入着色器代码
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);
    //链接程序
    glLinkProgram(program);
    GLint status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        LOGCATE("glLinkProgram failed!");
        return -1;
    }
    glUseProgram(program);
    GLuint apos = (GLuint) glGetAttribLocation(program, "aPosition");
    glEnableVertexAttribArray(apos);
    //传递顶点
    glVertexAttribPointer(apos, 3, GL_FLOAT, GL_FALSE, 12, vers);
    GLuint atex = (GLuint) glGetAttribLocation(program, "aTexCoord");
    glEnableVertexAttribArray(atex);
    glVertexAttribPointer(atex, 2, GL_FLOAT, GL_FALSE, 8, txts);
    //材质纹理初始化
    //设置纹理层
    glUniform1i(glGetUniformLocation(program, "yTexture"), 0); //对于纹理第1层
    glUniform1i(glGetUniformLocation(program, "uTexture"), 1); //对于纹理第2层
    glUniform1i(glGetUniformLocation(program, "vTexture"), 2); //对于纹理第3层
    //创建opengl纹理
    texts[3] = {0};
    //创建三个纹理
    glGenTextures(3, texts);
    //设置纹理属性
    glBindTexture(GL_TEXTURE_2D, texts[0]);
    //缩小的过滤器
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //设置纹理的格式和大小
    glTexImage2D(GL_TEXTURE_2D,
                 0,           //细节基本 0默认
                 GL_LUMINANCE,//gpu内部格式 亮度，灰度图
                 width, height, //拉升到全屏
                 0,             //边框
                 GL_LUMINANCE,//数据的像素格式 亮度，灰度图 要与上面一致
                 GL_UNSIGNED_BYTE, //像素的数据类型
                 NULL                    //纹理的数据
    );

    //设置纹理属性
    glBindTexture(GL_TEXTURE_2D, texts[1]);
    //缩小的过滤器
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //设置纹理的格式和大小
    glTexImage2D(GL_TEXTURE_2D,
                 0,           //细节基本 0默认
                 GL_LUMINANCE,//gpu内部格式 亮度，灰度图
                 width / 2, height / 2, //拉升到全屏
                 0,             //边框
                 GL_LUMINANCE,//数据的像素格式 亮度，灰度图 要与上面一致
                 GL_UNSIGNED_BYTE, //像素的数据类型
                 NULL                    //纹理的数据
    );

    //设置纹理属性
    glBindTexture(GL_TEXTURE_2D, texts[2]);
    //缩小的过滤器
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //设置纹理的格式和大小
    glTexImage2D(GL_TEXTURE_2D,
                 0,           //细节基本 0默认
                 GL_LUMINANCE,//gpu内部格式 亮度，灰度图
                 width / 2, height / 2, //拉升到全屏
                 0,             //边框
                 GL_LUMINANCE,//数据的像素格式 亮度，灰度图 要与上面一致
                 GL_UNSIGNED_BYTE, //像素的数据类型
                 NULL                    //纹理的数据
    );
    // Use the program object
    //激活第1层纹理,绑定到创建的opengl纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texts[0]);
    //替换纹理内容
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, buf[0]);
    //激活第2层纹理,绑定到创建的opengl纹理
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, texts[1]);
    //替换纹理内容
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width / 2, height / 2, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                    buf[1]);


    //激活第2层纹理,绑定到创建的opengl纹理
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, texts[2]);
    //替换纹理内容
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width / 2, height / 2, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                    buf[2]);

    //三维绘制
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //窗口显示
    eglSwapBuffers(display, winsurface);

    //内存回收
    glDeleteShader(vsh);
    glDeleteShader(fsh);
    glDeleteTextures(3, texts);
    glDeleteProgram(program);
    return 0;
}

GLint OpenGlRender::InitShader(const char *code, GLint type) {
    //创建shader
    GLint sh = glCreateShader(type);
    if (sh == 0) {
        LOGCATE("glCreateShader %d failed!", type);
        return 0;
    }
    //加载shader
    glShaderSource(sh,
                   1,    //shader数量
                   &code, //shader代码
                   0);   //代码长度
    //编译shader
    glCompileShader(sh);

    //获取编译情况
    GLint status;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        LOGCATE("glCompileShader failed!");
        return 0;
    }
    return sh;
}
