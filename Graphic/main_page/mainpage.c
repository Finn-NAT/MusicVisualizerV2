#include "mainpage.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../music_visualizer_pages/mvpage.h"
#include <graphic.h>

// Global mainpage instance
mainpage_t g_mainpage;
// Project path constant
// static const char* PROJECT_PATH = "S/home/bmo/Documents/MusicVisualizerV2/Graphic/images_src/";
static uint32_t item_id_counter = 1;

/**********************
 * STATIC PROTOTYPES  
 **********************/
static void container_click_event_cb(lv_event_t *e);
static void mainpage_add_subpage(const char *title, const char *context, const char *img_path);

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * @brief Initialize main page with container list
 */
void mainpage_create(lv_obj_t *parent)
{
    // Initialize structure
    memset(&g_mainpage, 0, sizeof(mainpage_t));
    
    // Create main container 
    g_mainpage.main_container = lv_obj_create(parent);
    lv_obj_set_size(g_mainpage.main_container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(g_mainpage.main_container, lv_color_hex(0x2c2c2c), 0);
    lv_obj_clear_flag(g_mainpage.main_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_all(g_mainpage.main_container, 0, 0);
    
    // Create list widget to contain subpage containers
    g_mainpage.subpage_list = lv_list_create(g_mainpage.main_container);
    lv_obj_set_size(g_mainpage.subpage_list, LV_PCT(100), LV_PCT(100));
    lv_obj_center(g_mainpage.subpage_list);
    
    // Set scroll properties
    lv_obj_set_scroll_dir(g_mainpage.subpage_list, LV_DIR_VER);             
    lv_obj_clear_flag(g_mainpage.subpage_list, LV_OBJ_FLAG_SCROLL_ELASTIC);  
    lv_obj_set_scrollbar_mode(g_mainpage.subpage_list, LV_SCROLLBAR_MODE_AUTO); 
    
    // Set list properties
    lv_obj_set_style_pad_all(g_mainpage.subpage_list, 30, 0);              
    lv_obj_set_style_pad_row(g_mainpage.subpage_list, 20, 0);              
    lv_obj_set_style_bg_opa(g_mainpage.subpage_list, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(g_mainpage.subpage_list, LV_OPA_TRANSP, 0);     
    lv_obj_set_style_outline_opa(g_mainpage.subpage_list, LV_OPA_TRANSP, 0);

    mainpage_add_subpage("Basic Music Visualizer", "This is the basic of music visualizer with a simple bar layout. \
                                                \nYou can customize it or get example for another Type of Music Visualizer.", 
                                                "basic_visual_image.png");
    mainpage_add_subpage("Subpage 2", "This is the context for subpage 2.", "test.png");
    mainpage_add_subpage("Subpage 3", "This is the context for subpage 3.", "test.png");
    mainpage_add_subpage("Subpage 4", "This is the context for subpage 4.", "test.png");
    mainpage_add_subpage("Subpage 5", "This is the context for subpage 5.", "test.png");
    mainpage_add_subpage("Subpage 6", "This is the context for subpage 6.", "test.png");
    mainpage_add_subpage("Subpage 7", "This is the context for subpage 7.", "test.png");
    mainpage_add_subpage("Subpage 8", "This is the context for subpage 8.", "test.png");
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * @brief Event callback khi click vào container item
 */
static void container_click_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *container = lv_event_get_target(e);
    
    if (code == LV_EVENT_CLICKED) {
        // Lấy dữ liệu từ user_data
        uint32_t *item_id = (uint32_t*)lv_obj_get_user_data(container);
        
        if (item_id) {
            printf("=== ITEM CLICKED ===\n");
            printf("Item ID: %u\n", *item_id);
            printf("Container clicked!\n");
            printf("===================\n");
        }
        SetSubpage(0);
        MusicVisualizerPage -> sub_page_init(g_mainpage.main_container);
    }
} 

/**
 * @brief Add a new subpage to the list
 */
static void mainpage_add_subpage(const char *title, const char *context, const char *img_path)
{
    char image_path[256];
    snprintf(image_path, sizeof(image_path), "%s%s", PROJECT_PATH, img_path);
    // printf("Image path: %s\n", image_path);

    lv_obj_t *container;
    // Create container for this subpage in the list
    container = lv_obj_create(g_mainpage.subpage_list);
    lv_obj_set_size(container, LV_PCT(100), 120);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);

    // Style the subpage container
    lv_obj_set_style_bg_color(container, lv_color_hex(0x404040), 0);
    lv_obj_set_style_border_width(container, 1, 0);
    lv_obj_set_style_border_color(container, lv_color_hex(0x666666), 0);
    lv_obj_set_style_radius(container, 30, 0);

    // Tạo unique ID cho item và lưu vào user_data
    uint32_t *item_id = malloc(sizeof(uint32_t));
    *item_id = item_id_counter++;
    lv_obj_set_user_data(container, item_id);

    // Thêm event callback cho container
    lv_obj_add_event_cb(container, container_click_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *img;
    // Add image button
    img = lv_img_create(container);                                   
    lv_obj_set_style_bg_opa(img, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(img, lv_color_white(), 0);
    lv_obj_align(img, LV_ALIGN_LEFT_MID, 10, 0);
    lv_obj_set_style_radius(img, 10, 0);

    lv_img_set_src(img, image_path);


    // Add title label
    lv_obj_t *title_label = lv_label_create(container);
    lv_label_set_text(title_label, title);
    lv_obj_set_style_text_color(title_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_16, 0);
    lv_obj_align_to(title_label, img, LV_ALIGN_OUT_RIGHT_TOP, 10, 10);

    // Add context label
    lv_obj_t *context_label = lv_label_create(container);
    lv_label_set_text(context_label, context);
    lv_obj_set_style_text_color(context_label, lv_color_hex(0xaaaaaa), 0);
    lv_obj_set_style_text_font(context_label, &lv_font_montserrat_14, 0);
    lv_obj_align_to(context_label, title_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
}


