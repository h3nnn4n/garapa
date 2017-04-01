#ifndef FILE_CONTROL_H
#define FILE_CONTROL_H

typedef struct {
    int seed;
    int frames;
    int can_write;
} _file_control;

_file_control* get_file_control_pointer();
void reset_file_control();
void set_can_write_file_control();
void bump_file_control();
void init_file_control();
void check_stop_condition();
int can_write_file_control();
int get_frame_number_file_control();
int get_seed_file_control();

#endif /* FILE_CONTROL_H */
