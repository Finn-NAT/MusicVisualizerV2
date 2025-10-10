#include "subpage.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

sub_page_err_code MusicVisualizerTest_sub_page_init(lv_obj_t *parent){
    if (!MusicVisualizerPage || !parent) return SUB_PAGE_RET_FAIL;
    
    // Cast to your specific subpage type
    musicvisualtest_subpage_t *music_subpage = (musicvisualtest_subpage_t *)MusicVisualizerPage;
    
    // Tạo container chính cho visualizer
    lv_obj_t *main_container = lv_obj_create(parent);
    lv_obj_set_size(main_container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(main_container, lv_color_hex(0x1a1a1a), 0);
    lv_obj_clear_flag(main_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(main_container, 0, 0);
    lv_obj_set_style_pad_all(main_container, 20, 0);

    // Store the container in the base structure
    music_subpage->base.container = main_container;

    int32_t container_width = 1240 - 40; // trừ padding
    int32_t bar_width = (container_width - (80 - 1) * 2) / 80;       // 2px gap giữa các bar
    int32_t bar_height_max = 700 - 40;  // chiều cao tối đa
    printf("Container width: %d, Bar width: %d, Bar height max: %d\n", container_width, bar_width, bar_height_max);

    // Tạo 80 bars
    for (int i = 0; i < 80; i++) {
        //printf("Creating bar %d\n", i);
        // Tạo bar object
        music_subpage->music_bar[i] = lv_obj_create(main_container);
        lv_obj_set_size(music_subpage->music_bar[i], bar_width, 10);  // Chiều cao ban đầu = 10
        
        // Vị trí bar
        int32_t x_pos = i * (bar_width + 2);  // 2px gap
        lv_obj_set_pos(music_subpage->music_bar[i], x_pos, bar_height_max - bar_height_max/2 - 10);
        
        // Style cho bar
        lv_obj_set_style_bg_color(music_subpage->music_bar[i], lv_color_hex(0x00ff00), 0);
        lv_obj_set_style_border_width(music_subpage->music_bar[i], 0, 0);
        lv_obj_set_style_radius(music_subpage->music_bar[i], 2, 0);
        lv_obj_clear_flag(music_subpage->music_bar[i], LV_OBJ_FLAG_SCROLLABLE);
        
        // Gradient color effect
        if (i < 30) {
            lv_obj_set_style_bg_color(music_subpage->music_bar[i], lv_color_hex(0x00ff00), 0);
        } else if (i < 60) {
            lv_obj_set_style_bg_color(music_subpage->music_bar[i], lv_color_hex(0xffff00), 0);
        } else {
            lv_obj_set_style_bg_color(music_subpage->music_bar[i], lv_color_hex(0xff0000), 0);
        }
    }

    music_subpage->base.is_initialized = 1;

    return SUB_PAGE_RET_OK;
}
sub_page_err_code MusicVisualizerTest_sub_page_deinit(void){
    if (!MusicVisualizerPage) return SUB_PAGE_RET_FAIL;

    // Cast to your specific subpage type
    musicvisualtest_subpage_t *music_subpage = (musicvisualtest_subpage_t *)MusicVisualizerPage;

    // Xóa container chính (tự động xóa tất cả bars)
    if (music_subpage->base.container) {
        lv_obj_del(music_subpage->base.container);
        music_subpage->base.container = NULL;
        music_subpage->base.back_container = NULL;
        music_subpage->base.title_label = NULL;
    }

    free(music_subpage);
    MusicVisualizerPage = NULL;
    
    return SUB_PAGE_RET_OK;
}
sub_page_err_code MusicVisualizerTest_sub_page_main_function(subpage_value_t *value){
    // //if (!MusicVisualizerPage || !value) return SUB_PAGE_RET_FAIL;
    // if (!MusicVisualizerPage) return SUB_PAGE_RET_FAIL;

    // // Cast to your specific subpage type
    // musicvisualtest_subpage_t *music_subpage = (musicvisualtest_subpage_t *)MusicVisualizerPage;

    // // Simulate music data (thay thế bằng dữ liệu âm thanh thực)
    // static int frame_counter = 0;
    // frame_counter++;

    // int32_t container_height = lv_obj_get_height(music_subpage->base.container) - 40;

    // // Cập nhật chiều cao của từng bar dựa trên dữ liệu âm thanh
    // for (int i = 0; i < 80; i++) {
    //     if (music_subpage->music_bar[i]) {
    //         // Tạo dữ liệu giả (thay thế bằng FFT data thực)
    //         float amplitude = (sin((frame_counter + i * 10) * 0.1) + 1.0f) * 0.5f; // 0-1
    //         amplitude += (rand() % 50) / 100.0f; // Thêm noise
    //         if (amplitude > 1.0f) amplitude = 1.0f;
            
    //         // Tính chiều cao bar
    //         int32_t bar_height = (int32_t)(amplitude * container_height * 0.8f);
    //         if (bar_height < 5) bar_height = 5;  // Chiều cao tối thiểu
            
    //         // Cập nhật kích thước và vị trí
    //         lv_obj_set_height(music_subpage->music_bar[i], bar_height);
    //         lv_obj_set_y(music_subpage->music_bar[i], container_height - bar_height);
    //     }
    // }

    //if (!MusicVisualizerPage || !value) return SUB_PAGE_RET_FAIL;
    if (!MusicVisualizerPage) return SUB_PAGE_RET_FAIL;

    // Cast to your specific subpage type
    musicvisualtest_subpage_t *music_subpage = (musicvisualtest_subpage_t *)MusicVisualizerPage;

    // Simulate music data (thay thế bằng dữ liệu âm thanh thực)
    static int frame_counter = 0;
    frame_counter++;

    int32_t container_height = lv_obj_get_height(music_subpage->base.container) - 40;

    // Cập nhật chiều cao của từng bar dựa trên dữ liệu âm thanh
    for (int i = 0; i < 80; i++) {
        if (music_subpage->music_bar[i]) {
            value->value[i] /= 300.0f; //
            
            // Tính chiều cao bar
            int32_t bar_height = (int32_t)(value->value[i] * (container_height/2) * 0.8f);
            if (bar_height < 5) bar_height = 5;  // Chiều cao tối thiểu
            
            // Cập nhật kích thước và vị trí
            lv_obj_set_height(music_subpage->music_bar[i], bar_height);
            lv_obj_set_y(music_subpage->music_bar[i], container_height/2 - bar_height);
        }
    }
    
    return SUB_PAGE_RET_OK;
}

musicvisualtest_subpage_t MusicVisualizerTestPage = {
    .music_bar = {NULL},
    .base = {
        .sub_page_init = MusicVisualizerTest_sub_page_init,
        .sub_page_deinit = MusicVisualizerTest_sub_page_deinit,
        .sub_page_main_function = MusicVisualizerTest_sub_page_main_function,
        .subpage_size = sizeof(musicvisualtest_subpage_t),
        .is_initialized = 0,
    },
};