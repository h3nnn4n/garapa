#include "shader.hpp"
#include "shader_c.h"

extern "C" {
  Shader* newShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
    return new Shader(vertexPath, fragmentPath, geometryPath);
  }

#ifndef __APPLE__
  void Shader_reload_changes(Shader* shader) {
    shader->reload_changes();
  }
#endif

  unsigned int Shader_get_id(Shader* shader) {
    return shader->ID;
  }

  void Shader_use(Shader* shader) {
    shader->use();
  }

  void Shader_set_bool(Shader* shader, const char* name, int value) {
    shader->setBool(name, value);
  }

  void Shader_set_int(Shader* shader, const char* name, int value) {
    shader->setInt(name, value);
  }

  void Shader_set_float(Shader* shader, const char* name, float value) {
    shader->setFloat(name, value);
  }

  void Shader_set_vec3(Shader* shader, const char* name, float* value) {
    shader->setVec3(name, value);
  }

  void Shader_set_vec3f(Shader* shader, const char* name, float v1, float v2, float v3) {
    shader->setVec3(name, v1, v2, v3);
  }

  void Shader_set_matrix4(Shader* shader, const char* name, float *value) {
    shader->setMatrix4(name, value);
  }

  void Shader_destroy(Shader* shader) {
    delete shader;
  }
}
