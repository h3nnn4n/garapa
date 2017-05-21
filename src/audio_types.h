#ifndef AUDIO_TYPES_H
#define AUDIO_TYPES_H

#define BUFFER_SIZE 1024

typedef struct {

} _channel1;

typedef struct {

} _channel2;

typedef struct {

} _channel3;

typedef struct {

} _channel4;

typedef struct {
    _channel1 ch1;
    _channel2 ch2;
    _channel3 ch3;
    _channel4 ch4;

    uint16_t buffer[BUFFER_SIZE * 2];
    uint8_t buffer_index;

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

    uint8_t ch1_rigth_enable;
    uint8_t ch2_rigth_enable;
    uint8_t ch3_rigth_enable;
    uint8_t ch4_rigth_enable;
} _apu;

#endif /* AUDIO_TYPES_H */
