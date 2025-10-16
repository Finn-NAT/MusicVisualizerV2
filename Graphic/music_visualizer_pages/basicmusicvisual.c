#include "mvpage.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <graphic.h>
#include <lvgl/lvgl.h>

static void back_button_event_cb(lv_event_t *e);

mv_page_err_code MusicVisualizerTest_sub_page_init(lv_obj_t *parent){
    if (!MusicVisualizerPage || !parent) return MV_PAGE_RET_FAIL;

    basic_musicvisual_mv_page_t *music_subpage = (basic_musicvisual_mv_page_t *)MusicVisualizerPage;

    lv_obj_t *main_container = lv_obj_create(parent);
    music_subpage->base.container = main_container;

    lv_obj_t *back_container = lv_obj_create(main_container);
    music_subpage->base.back_container = back_container;

    lv_obj_t *main_title = lv_label_create(main_container);
    music_subpage->base.title_label = main_title;

/* ---------------------------------------------------------------------------------
* User Setup for Title Lable
* ---------------------------------------------------------------------------------*/
    lv_obj_set_size(main_title, 540, 70);
    lv_obj_set_style_bg_opa(main_title, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(main_title, LV_OPA_TRANSP, 0);
    lv_obj_set_style_outline_opa(main_title, LV_OPA_TRANSP, 0);
    lv_obj_clear_flag(main_title, LV_OBJ_FLAG_SCROLLABLE);
    lv_label_set_text(main_title, "- Music Visualizer -");
    lv_obj_set_style_text_color(main_title, lv_color_hex(0x4abc96), 0);
    lv_obj_set_style_text_font(main_title, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_align(main_title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(main_title, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_style_shadow_width(main_title, 12, 0);             
    lv_obj_set_style_shadow_color(main_title, lv_color_hex(0x000000), 0);
    lv_obj_set_style_shadow_opa(main_title, LV_OPA_40, 0);       
    lv_obj_set_style_shadow_ofs_x(main_title, 0, 0);              
    lv_obj_set_style_shadow_ofs_y(main_title, 6, 0);              
    lv_obj_set_style_shadow_spread(main_title, 0, 0);              


    lv_obj_set_style_pad_all(main_title, 0, 0);
/* ---------------------------------------------------------------------------------
* End Setup 
* ---------------------------------------------------------------------------------*/

/* ---------------------------------------------------------------------------------
* User Setup for Back Container
* ---------------------------------------------------------------------------------*/
    lv_obj_set_size(back_container, 150, 50);
    lv_obj_set_style_bg_opa(back_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(back_container, LV_OPA_TRANSP, 0);     
    lv_obj_set_style_outline_opa(back_container, LV_OPA_TRANSP, 0);
    lv_obj_clear_flag(back_container, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_pad_all(back_container, 0, 0);
    
    char image_path[256];
    snprintf(image_path, sizeof(image_path), "%s%s", PROJECT_PATH, "back_icon.png");

    lv_obj_t *back_img = lv_img_create(back_container);
    lv_img_set_src(back_img, image_path);
    lv_obj_align(back_img, LV_ALIGN_LEFT_MID, 8, 0); 
    
    lv_obj_t *back_label = lv_label_create(back_container);
    lv_obj_set_size(back_label, 85, 37);
    lv_obj_set_style_text_align(back_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(back_label, "Back");
    lv_obj_set_style_text_color(back_label, lv_color_hex(0x4abc96), 0);  
    lv_obj_set_style_text_font(back_label, &lv_font_montserrat_30, 0);   
    lv_obj_align_to(back_label, back_img, LV_ALIGN_OUT_RIGHT_MID, 5, 0); 

    lv_obj_t *back_label_top = lv_obj_create(back_label);
    lv_obj_remove_style_all(back_label_top);
    lv_obj_set_width(back_label_top, LV_PCT(100));
    lv_obj_set_height(back_label_top, 2);
    lv_obj_set_style_bg_color(back_label_top, lv_color_hex(0x4abc96), 0);
    lv_obj_set_style_bg_opa(back_label_top, LV_OPA_COVER, 0);
    lv_obj_align(back_label_top, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t *back_label_bottom = lv_obj_create(back_label);
    lv_obj_remove_style_all(back_label_bottom);
    lv_obj_set_width(back_label_bottom, LV_PCT(100));
    lv_obj_set_height(back_label_bottom, 2);
    lv_obj_set_style_bg_color(back_label_bottom, lv_color_hex(0x4abc96), 0);
    lv_obj_set_style_bg_opa(back_label_bottom, LV_OPA_COVER, 0);
    lv_obj_align(back_label_bottom, LV_ALIGN_BOTTOM_MID, 0, 0);
 
    lv_obj_add_flag(back_container, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_set_style_bg_opa(back_container, LV_OPA_20, LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(back_container, lv_color_hex(0x00ff00), LV_STATE_PRESSED);

    lv_obj_add_event_cb(back_container, back_button_event_cb, LV_EVENT_CLICKED, NULL);

/* ---------------------------------------------------------------------------------
* End Setup 
* ---------------------------------------------------------------------------------*/


/* ---------------------------------------------------------------------------------
* User Setup for each Visualizer Page 
* ---------------------------------------------------------------------------------*/
    lv_obj_set_size(main_container, GRAPHIC_HOR_RES, GRAPHIC_VER_RES);
    lv_obj_set_style_bg_color(main_container, lv_color_hex(0xffffff), 0);
    lv_obj_clear_flag(main_container, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_pad_all(main_container, 20, 0);

    int32_t bars_width = GRAPHIC_HOR_RES - 40; 
    int32_t bar_width = (bars_width - (BAR_NUMBER - 1) * 1) / BAR_NUMBER;      // 1px gap 
    int32_t bar_height_max = GRAPHIC_VER_RES - 250;  
    printf("Container width: %d, Bar width: %d, Bar height max: %d\n", bars_width, bar_width, bar_height_max);

    for (int i = 0; i < BAR_NUMBER; i++) {
        music_subpage->music_bar[i] = lv_obj_create(main_container);
        lv_obj_set_size(music_subpage->music_bar[i], bar_width, 10); 
        
        int32_t x_pos = i * (bar_width + 1);  // 1px gap
        lv_obj_set_pos(music_subpage->music_bar[i], x_pos, bar_height_max - bar_height_max/2 - 10);
        
        lv_obj_set_style_bg_color(music_subpage->music_bar[i], lv_color_hex(0x00ff00), 0);
        lv_obj_set_style_border_width(music_subpage->music_bar[i], 0, 0);
        lv_obj_set_style_radius(music_subpage->music_bar[i], 3, 0);
        lv_obj_clear_flag(music_subpage->music_bar[i], LV_OBJ_FLAG_SCROLLABLE);
        
        float hue = (1.0f - (float)i / (BAR_NUMBER - 1)) * 300.0f; 
        uint32_t color = lv_color_hsv_to_rgb((uint16_t)hue, 100, 100).full;
        lv_obj_set_style_bg_color(music_subpage->music_bar[i], lv_color_hex(color), 0);
    }
/* ---------------------------------------------------------------------------------
* End Setup 
* ---------------------------------------------------------------------------------*/

    music_subpage->base.state = MV_PAGE_INIT;

    return MV_PAGE_RET_OK;
}
mv_page_err_code MusicVisualizerTest_sub_page_deinit(void){
    if (!MusicVisualizerPage) return MV_PAGE_RET_FAIL;

    basic_musicvisual_mv_page_t *music_subpage = (basic_musicvisual_mv_page_t *)MusicVisualizerPage;

    if (music_subpage->base.container) {
        lv_obj_del(music_subpage->base.container);
        music_subpage->base.container = NULL;
        music_subpage->base.back_container = NULL;
        music_subpage->base.title_label = NULL;
        music_subpage->base.state = MV_PAGE_IDLE;
    }

    //MusicVisualizerPage = NULL;

    return MV_PAGE_RET_OK;
}

mv_page_err_code MusicVisualizerTest_sub_page_main_function(mv_value_t *value){
    if (!MusicVisualizerPage ) return MV_PAGE_RET_FAIL;

    basic_musicvisual_mv_page_t *music_subpage = (basic_musicvisual_mv_page_t *)MusicVisualizerPage;

/* ---------------------------------------------------------------------------------
* User Functions for Visualizer Page 
* ---------------------------------------------------------------------------------*/

    int32_t container_height = lv_obj_get_height(music_subpage->base.container) - 40;
    int32_t bar_height_max = GRAPHIC_VER_RES - 250;;

    for (int i = 0; i < BAR_NUMBER; i++) {
        if (music_subpage->music_bar[i]) {
            value->value[i] /= 100.0f;
            
            int32_t bar_height = (int32_t)(value->value[i] * (container_height/2));
            if (bar_height < 5) bar_height = 5; 
            bar_height = bar_height > bar_height_max ? bar_height_max : bar_height;

            lv_obj_set_height(music_subpage->music_bar[i], bar_height);
            lv_obj_set_y(music_subpage->music_bar[i], container_height/2 - bar_height/2);
        }
    }    
/* ---------------------------------------------------------------------------------
* End Setup 
* ---------------------------------------------------------------------------------*/

    return MV_PAGE_RET_OK;
}

basic_musicvisual_mv_page_t BasicMusicVisualizerPage = {
    .music_bar = {NULL},
    .base = {
        .sub_page_init = MusicVisualizerTest_sub_page_init,
        .sub_page_deinit = MusicVisualizerTest_sub_page_deinit,
        .sub_page_main_function = MusicVisualizerTest_sub_page_main_function,
        .subpage_size = sizeof(basic_musicvisual_mv_page_t),
        .state = MV_PAGE_IDLE,
    },
};


static void back_button_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    
    if (code == LV_EVENT_CLICKED) {
        printf("Back button clicked! Returning to main page...\n");

        MusicVisualizerPage->state = MV_PAGE_DEINIT;
    }
}