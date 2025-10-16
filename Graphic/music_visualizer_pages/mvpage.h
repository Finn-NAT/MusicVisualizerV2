#ifndef MUSIC_VISUALIZER_PAGE_H
#define MUSIC_VISUALIZER_PAGE_H

#include "../lvgl/lvgl.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Define for Basic Music Visualizer */
#define BAR_NUMBER 412
/*************************************/

typedef enum
{
    MV_PAGE_RET_OK     = 0,
    MV_PAGE_RET_FAIL,                    
} mv_page_err_code;

typedef enum
{
    MV_PAGE_INIT     = 0,
    MV_PAGE_DEINIT,
    MV_PAGE_IDLE
} mv_page_state_t;

typedef struct mv_value_t{
    float* value;
} mv_value_t;

typedef struct mv_page_t{
    lv_obj_t *container;
    lv_obj_t *back_container;
    lv_obj_t *title_label;

    mv_page_err_code (*sub_page_init)(lv_obj_t *parent);
    mv_page_err_code (*sub_page_deinit)(void);
    mv_page_err_code (*sub_page_main_function)(mv_value_t *value);

    uint16_t subpage_size;
    mv_page_state_t state;
} mv_page_t;

extern mv_page_t *MusicVisualizerPage;

mv_page_err_code SetSubpage(uint16_t index);

/* Setup for Basic Music Visualizer */

typedef struct basic_musicvisual_mv_page_t{
    mv_page_t base;
    lv_obj_t *music_bar[BAR_NUMBER];
} basic_musicvisual_mv_page_t;

extern basic_musicvisual_mv_page_t BasicMusicVisualizerPage;

/************************************/

#ifdef __cplusplus
}
#endif

#endif // SUBPAGE_H