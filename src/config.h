#pragma once

typedef struct {
    int enable_video;
    int enable_audio;
    int run_test_roms;

    char *rom_name;
    char *python_filename;
} _config;

_config *get_config();
void     set_config(_config *new_config);
int      get_config_flag(char *flag_name);
char *   get_config_value_s(char *key_name);
