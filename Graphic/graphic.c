/**
 * @file graphic.c
 * @brief Graphics interface implementation using LVGL with SDL2 driver
 */

/*********************
 *      INCLUDES
 *********************/
#include "graphic.h"
#include "lv_conf.h"
#include "lv_drv_conf.h"
#include "lv_drivers/sdl/sdl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*********************
 *      DEFINES
 *********************/
#define GRAPHIC_TASK_PERIOD_MS  5   /* LVGL task handler period in ms */

/**********************
 *  STATIC VARIABLES
 **********************/
static bool g_graphic_initialized = false;
static lv_disp_t* g_display = NULL;

/* Display buffer */
static lv_disp_draw_buf_t g_disp_buffer;
static lv_color_t* g_buffer = NULL;

/* Driver structures */
static lv_disp_drv_t g_disp_drv;

/* Configuration */
static graphic_config_t g_config;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static graphic_result_t graphic_init_lvgl(void);
static graphic_result_t graphic_init_display(void);
static graphic_result_t graphic_init_input_devices(void);
static void graphic_cleanup(void);

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

graphic_result_t graphic_init(void)
{
    graphic_config_t default_config = graphic_get_default_config();
    return graphic_init_with_config(&default_config);
}

graphic_result_t graphic_init_with_config(const graphic_config_t* config)
{
    if (g_graphic_initialized) {
        return GRAPHIC_ERR_ALREADY_INIT;
    }
    
    if (config == NULL) {
        return GRAPHIC_ERR_INIT_FAILED;
    }
    
    /* Store configuration */
    memcpy(&g_config, config, sizeof(graphic_config_t));
    
    /* Initialize LVGL */
    graphic_result_t result = graphic_init_lvgl();
    if (result != GRAPHIC_OK) {
        graphic_cleanup();
        return result;
    }
    
    /* Initialize SDL2 */
    sdl_init();
    
    /* Initialize display */
    result = graphic_init_display();
    if (result != GRAPHIC_OK) {
        graphic_cleanup();
        return result;
    }
    
    /* Initialize input devices */
    result = graphic_init_input_devices();
    if (result != GRAPHIC_OK) {
        graphic_cleanup();
        return result;
    }
    
    g_graphic_initialized = true;
    return GRAPHIC_OK;
}

graphic_config_t graphic_get_default_config(void)
{
    graphic_config_t config = {
        .hor_res = GRAPHIC_HOR_RES,
        .ver_res = GRAPHIC_VER_RES,
        .color_depth = GRAPHIC_COLOR_DEPTH,
    };
    return config;
}

void graphic_deinit(void)
{
    if (!g_graphic_initialized) {
        return;
    }
    
    graphic_cleanup();
    g_graphic_initialized = false;
}

bool graphic_is_initialized(void)
{
    return g_graphic_initialized;
}

lv_disp_t* graphic_get_display(void)
{
    return g_display;
}

void graphic_task_handler(void)
{
    if (!g_graphic_initialized) {
        return;
    }
    
    /* Handle LVGL tasks */
    lv_timer_handler();
}

void graphic_refresh(void)
{
    if (!g_graphic_initialized || g_display == NULL) {
        return;
    }
    
    /* Force refresh of the display */
    lv_obj_invalidate(lv_scr_act());
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static graphic_result_t graphic_init_lvgl(void)
{
    /* Initialize LVGL */
    lv_init();

    /* Allocate display buffers */
    size_t buf_size = GRAPHIC_DISP_BUF_SIZE * sizeof(lv_color_t);
    
    g_buffer = (lv_color_t*)malloc(buf_size);
    if (g_buffer == NULL) {
        return GRAPHIC_ERR_INIT_FAILED;
    }
    lv_disp_draw_buf_init(&g_disp_buffer, g_buffer, NULL, GRAPHIC_DISP_BUF_SIZE);
    
    return GRAPHIC_OK;
}

static graphic_result_t graphic_init_display(void)
{
    /* Initialize display driver */
    lv_disp_drv_init(&g_disp_drv);
    
    /* Set display resolution */
    g_disp_drv.hor_res = g_config.hor_res;
    g_disp_drv.ver_res = g_config.ver_res;
    
    /* Set display buffer */
    g_disp_drv.draw_buf = &g_disp_buffer;
    
    /* Set flush callback */
    g_disp_drv.flush_cb = sdl_display_flush;
    
    /* Register display driver */
    g_display = lv_disp_drv_register(&g_disp_drv);
    if (g_display == NULL) {
        return GRAPHIC_ERR_DISP_INIT_FAILED;
    }
    
    return GRAPHIC_OK;
}

static graphic_result_t graphic_init_input_devices(void)
{
    /* Initialize mouse input device */
    static lv_indev_drv_t indev_drv_mouse;
    lv_indev_drv_init(&indev_drv_mouse);
    indev_drv_mouse.type = LV_INDEV_TYPE_POINTER;
    indev_drv_mouse.read_cb = sdl_mouse_read;
    lv_indev_drv_register(&indev_drv_mouse);
    
    /* Initialize mouse wheel input device */
    static lv_indev_drv_t indev_drv_mousewheel;
    lv_indev_drv_init(&indev_drv_mousewheel);
    indev_drv_mousewheel.type = LV_INDEV_TYPE_ENCODER;
    indev_drv_mousewheel.read_cb = sdl_mousewheel_read;
    lv_indev_drv_register(&indev_drv_mousewheel);
    
    /* Initialize keyboard input device */
    static lv_indev_drv_t indev_drv_kb;
    lv_indev_drv_init(&indev_drv_kb);
    indev_drv_kb.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv_kb.read_cb = sdl_keyboard_read;
    lv_indev_drv_register(&indev_drv_kb);
    
    return GRAPHIC_OK;
}

static void graphic_cleanup(void)
{
    /* Free display buffers */
    if (g_buffer) {
        free(g_buffer);
        g_buffer = NULL;
    }
    
    /* Reset variables */
    g_display = NULL;
}
