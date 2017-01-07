#ifndef AUTOMATED_TESTS_Hj
#define AUTOMATED_TESTS_H

typedef struct {
    char name[4096];
    uint32_t frames;
    uint32_t hash;
} _test_info;

typedef struct {
    uint32_t *test_bitmap;
    uint32_t  test_spent_frames;
    uint32_t  test_needed_frames;
    uint8_t   test_bitmap_inialized;
    uint8_t   test_enable;
} _test_control;

_test_control test_control;

uint8_t test_step         ( _test_control *t                               ) ;
void test_create_buffer   ( _test_control* t                               ) ;
void test_reset_buffer    ( _test_control* t                               ) ;
void test_free_buffer     ( _test_control* t                               ) ;
void test_write_to_buffer ( _test_control* t, uint16_t addr, uint32_t data ) ;
void test_run             (                                                ) ;
void test_run_adjust      (                                                ) ;


#endif /* AUTOMATED_TESTS_H */
