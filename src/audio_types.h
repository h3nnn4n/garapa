#ifndef AUDIO_TYPES_H
#define AUDIO_TYPES_H

#include <stdint.h>

#include "SDL.h"

#define BUFFER_SIZE 1024
#define SAMPLE_RATE 44100

typedef struct {
    uint8_t enable;
    uint8_t sweep_enable;
    uint8_t sweep_timer;
    uint8_t sweep_negate_calcd;
    uint16_t frequency_sh;
    uint8_t sweep_period;
    uint8_t sweep_direction;
    uint8_t sweep_shift;

    uint8_t wave_pattern_duty;
    uint8_t wave_pattern_index;
    uint16_t length;
    uint8_t length_enable;
    uint8_t volume;
    uint8_t volume_envl_timer;
    uint8_t volume_envl_initial;
    uint8_t volume_envl_direction;
    uint8_t volume_envl_period;
    uint16_t frequency;
    uint16_t timer;
} _channel1;

typedef struct {
    uint8_t enable;

    uint8_t wave_pattern_duty;
    uint8_t wave_pattern_index;
    uint16_t length;
    uint8_t length_enable;
    uint8_t volume;
    uint8_t volume_envl_timer;
    uint8_t volume_envl_initial;
    uint8_t volume_envl_direction;
    uint8_t volume_envl_period;

    uint16_t frequency;
    uint16_t timer;
} _channel2;

typedef struct {
    uint8_t enable;
    uint8_t dac_enable;

    uint16_t length;
    uint8_t length_enable;

    uint8_t volume;
    uint16_t frequency;
    uint16_t timer;

    uint8_t wave_ram[16];
    uint8_t wave_ram_buffer;
    uint8_t wave_ram_buffer_position;
} _channel3;

typedef struct {
    uint8_t enable;

    uint16_t length;
    uint8_t length_enable;

    uint8_t volume;
    uint8_t volume_envl_timer;
    uint8_t volume_envl_initial;
    uint8_t volume_envl_direction;
    uint8_t volume_envl_period;

    uint8_t shift;
    uint8_t width;
    uint8_t divisor;
    uint8_t lfsr;

    uint16_t timer;
} _channel4;

typedef struct {
    _channel1 ch1;
    _channel2 ch2;
    _channel3 ch3;
    _channel4 ch4;

    uint16_t buffer[BUFFER_SIZE * 2];
    uint16_t buffer_index;

    uint32_t sample_timer;

    uint8_t enable; // master enable
    uint8_t frame_seq_step; // current step for the frame sequencer

    uint8_t left_vin_enable;
    uint8_t right_vin_enable;

    uint8_t left_volume;
    uint8_t right_volume;

    uint8_t ch1_left_enable;
    uint8_t ch2_left_enable;
    uint8_t ch3_left_enable;
    uint8_t ch4_left_enable;

    uint8_t ch1_right_enable;
    uint8_t ch2_right_enable;
    uint8_t ch3_right_enable;
    uint8_t ch4_right_enable;

    SDL_AudioSpec want, have;
    SDL_AudioDeviceID dev;
} _apu;

#endif /* AUDIO_TYPES_H */
