#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "file_control.h"
#include "other_window.h"
#include "types.h"
#include "memory.h"

static _file_control file_control;

static void clear_files() {
    char name[256];
    for (int i = 0; i < get_frame_number_file_control(); ++i) {
        sprintf(name, "frame_%016d_%016d.bmp", get_seed_file_control(), i);
        /*printf("Deleting %s\n", name);*/
        unlink(name);
    }
}

_file_control* get_file_control_pointer() {
    return &file_control;
}

void reset_file_control() {
    clear_files();

    file_control.frames    = 0;
    file_control.can_write = 0;
}

void set_can_write_file_control(){
    file_control.can_write = 1;
}

int can_write_file_control(){
    return file_control.can_write;
}

void bump_file_control() {
    file_control.frames ++;
}

int get_frame_number_file_control(){
    return file_control.frames;
}

void init_file_control() {
    reset_file_control();
    file_control.seed = time(NULL);
}

int get_seed_file_control(){
    return file_control.seed;
}

void check_stop_condition() {
    int a = get_cpu_pointer()->mem_controller.memory[0x9951];
    int b = get_cpu_pointer()->mem_controller.memory[0x9950];
    int c = get_cpu_pointer()->mem_controller.memory[0x994f];
    int d = get_cpu_pointer()->mem_controller.memory[0x994e];

    a = a == 0x2f ? 0 : a;
    b = b == 0x2f ? 0 : b;
    c = c == 0x2f ? 0 : c;
    d = d == 0x2f ? 0 : d;

    int best =  a  * 1 + b * 10 + c * 100 + d * 1000;

    printf("Finished game with %4d lines cleared\n", best);

    if ( best > 411 ) {
        exit(0);
    }
}
