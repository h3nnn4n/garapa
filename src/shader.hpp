#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
  public:
    unsigned int ID;

    std::string vertexPath;
    std::string fragmentPath;
    std::string geometryPath;

    bool useGeometryShader;

    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);

    void use();

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, float* value) const;
    void setVec3(const std::string &name, float v1, float v2, float v3) const;
    void setMatrix4(const std::string &name, float* value) const;

    void reload_changes();

  private:
    int inotify_fd;

    void checkCompileErrors(GLuint shader, std::string type, std::string extra);
    void load(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
};

#endif
