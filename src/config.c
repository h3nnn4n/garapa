#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "config.h"

static _config config;

_config *get_config() { return &config; }

void set_config(_config *new_config) { memcpy(&config, new_config, sizeof(_config)); }

int get_config_flag(char *flag_name) {
    if (strcmp("enable_video", flag_name) == 0) {
        /*printf("flag %s: %d\n", flag_name, config.enable_video);*/
        return config.enable_video;
    }

    if (strcmp("enable_audio", flag_name) == 0) {
        /*printf("flag %s: %d\n", flag_name, config.enable_audio);*/
        return config.enable_audio;
    }

    if (strcmp("run_test_roms", flag_name) == 0) {
        /*printf("flag %s: %d\n", flag_name, config.run_test_roms);*/
        return config.run_test_roms;
    }

    assert(0 && "invalid config flag!");
    return 0;
}

char *get_config_value_s(char *key_name) {
    if (strcmp("rom_name", key_name) == 0) {
        return config.rom_name;
    }

    if (strcmp("python_filename", key_name) == 0) {
        return config.python_filename;
    }

    if (strcmp("python_function", key_name) == 0) {
        return config.python_function;
    }

    assert(0 && "invalid config value!");
    return 0;
}
