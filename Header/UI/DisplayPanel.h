#pragma once
#include<QtOpenGLWidgets/qopenglwidget.h>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVersionFunctionsFactory>
#include "Shader.h"
using namespace std;
class DisplayPanel: public QOpenGLWidget
{   
    Q_OBJECT
public:
    DisplayPanel();
    ~DisplayPanel();
protected:
    void initializeGL() override;
    void paintGL() override;
    int shaderProgram;
    Shader* shader;
    GLuint VAO, VBO;
    void initTriangle();
    void drawTriangle();
};
