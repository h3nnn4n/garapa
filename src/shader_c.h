#ifndef SHADER_C_H
#define SHADER_C_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__APPLE__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmismatched-tags"
// It looks like this isnt supported by microsoft (class and struct with the
// same name).  Lets just silence it
typedef struct Shader  Shader;
#pragma GCC diagnostic pop
#else
typedef struct Shader  Shader;
#endif

Shader *newShader(const char *vertexPath, const char *fragmentPath, const char *geometryPath);

#ifndef __APPLE__
void Shader_reload_changes(Shader *shader);
#endif

unsigned int Shader_get_id(Shader *shader);

void Shader_use(Shader *shader);
void Shader_set_bool(Shader *shader, const char *name, int value);
void Shader_set_int(Shader *shader, const char *name, int value);
void Shader_set_float(Shader *shader, const char *name, float value);
void Shader_set_vec3(Shader *shader, const char *name, float *value);
void Shader_set_vec3f(Shader *shader, const char *name, float v1, float v2, float v3);
void Shader_set_matrix4(Shader *shader, const char *name, float *value);

void Shader_destroy(Shader *shader);

#ifdef __cplusplus
}
#endif

#endif
