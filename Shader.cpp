#include "Shader.h"
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVersionFunctionsFactory>
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    QOpenGLFunctions_3_3_Core* f = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(QOpenGLContext::currentContext());

    // vertex Shader
    vertex = f->glCreateShader(GL_VERTEX_SHADER);
    f->glShaderSource(vertex, 1, &vShaderCode, NULL);
    f->glCompileShader(vertex);
    // print compile errors if any
    f->glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        f->glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // similiar for Fragment Shader
    // vertex Shader
    fragment = f->glCreateShader(GL_FRAGMENT_SHADER);
    f->glShaderSource(fragment, 1, &fShaderCode, NULL);
    f->glCompileShader(fragment);
    // print compile errors if any
    f->glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        f->glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // shader Program
    ID = f->glCreateProgram();
    f->glAttachShader(ID, vertex);
    f->glAttachShader(ID, fragment);
    f->glLinkProgram(ID);
    // print linking errors if any
    f->glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        f->glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    f->glDeleteShader(vertex);
    f->glDeleteShader(fragment);
}
void Shader::use()
{
    QOpenGLFunctions_3_3_Core* f = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(QOpenGLContext::currentContext());
    f->glUseProgram(ID);
}
void Shader::setBool(const std::string& name, bool value) const
{
    QOpenGLFunctions_3_3_Core* f = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(QOpenGLContext::currentContext());
    f->glUniform1i(f->glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
    QOpenGLFunctions_3_3_Core* f = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(QOpenGLContext::currentContext());
    f->glUniform1i(f->glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
    QOpenGLFunctions_3_3_Core* f = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(QOpenGLContext::currentContext());
    f->glUniform1f(f->glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const
{
    QOpenGLFunctions_3_3_Core* f = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(QOpenGLContext::currentContext());
    f->glUniformMatrix4fv(f->glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec(const std::string& name,int size, float* value) const
{
    QOpenGLFunctions_3_3_Core* f = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(QOpenGLContext::currentContext());
    f->glUniform3fv(f->glGetUniformLocation(ID, name.c_str()), size, value);
}
void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
    QOpenGLFunctions_3_3_Core* f = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(QOpenGLContext::currentContext());
    f->glUniform3fv(f->glGetUniformLocation(ID, name.c_str()),1,&value[0]);
}