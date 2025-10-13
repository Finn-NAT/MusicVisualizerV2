#ifndef SUBPAGE_H
#define SUBPAGE_H

#include "../lvgl/lvgl.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BAR_NUMBER 100

typedef enum
{
    SUB_PAGE_RET_OK     = 0,
    SUB_PAGE_RET_FAIL,                    
} sub_page_err_code;

typedef enum
{
    SUB_PAGE_INIT     = 0,
    SUB_PAGE_DEINIT,
    SUB_PAGE_IDLE
} sub_page_state_t;

typedef struct subpage_value_t{
    float* value;
} subpage_value_t;

typedef struct subpage_t{
    lv_obj_t *container;
    lv_obj_t *back_container;
    lv_obj_t *title_label;

    sub_page_err_code (*sub_page_init)(lv_obj_t *parent);
    sub_page_err_code (*sub_page_deinit)(void);
    sub_page_err_code (*sub_page_main_function)(subpage_value_t *value);

    uint16_t subpage_size;
    sub_page_state_t state;
} subpage_t;

typedef struct musicvisualtest_subpage_t{
    subpage_t base;
    lv_obj_t *music_bar[BAR_NUMBER];
} musicvisualtest_subpage_t;

extern subpage_t *MusicVisualizerPage;
extern musicvisualtest_subpage_t MusicVisualizerTestPage;

sub_page_err_code SetSubpage(uint16_t index);

#ifdef __cplusplus
}
#endif

#endif // SUBPAGE_H