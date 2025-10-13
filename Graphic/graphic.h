/**
 * @file graphic.h
 * @brief Graphics interface using LVGL with SDL2 driver
 */

#ifndef GRAPHIC_H
#define GRAPHIC_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"
#include "lv_drivers/sdl/sdl.h"
#include <stdbool.h>
#include <stdint.h>

/*********************
 *      DEFINES
 *********************/
#define GRAPHIC_HOR_RES     1280
#define GRAPHIC_VER_RES     720
#define GRAPHIC_COLOR_DEPTH 32

#define PROJECT_PATH "S/home/bmo/Documents/MusicVisualizerV2/Graphic/images_src/"

/* Display buffer size in pixels */
#define GRAPHIC_DISP_BUF_SIZE (GRAPHIC_HOR_RES * GRAPHIC_VER_RES / 10)

/**********************
 *      TYPEDEFS
 **********************/

/**
 * @brief Graphics initialization status
 */
typedef enum {
    GRAPHIC_OK = 0,
    GRAPHIC_ERR_INIT_FAILED,
    GRAPHIC_ERR_SDL_INIT_FAILED,
    GRAPHIC_ERR_DISP_INIT_FAILED,
    GRAPHIC_ERR_INDEV_INIT_FAILED,
    GRAPHIC_ERR_ALREADY_INIT
} graphic_result_t;

/**
 * @brief Graphics configuration structure
 */
typedef struct {
    uint16_t hor_res;           /**< Horizontal resolution */
    uint16_t ver_res;           /**< Vertical resolution */
    uint8_t  color_depth;       /**< Color depth in bits */
} graphic_config_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * @brief Initialize graphics system with default configuration
 * @return GRAPHIC_OK on success, error code otherwise
 */
graphic_result_t graphic_init(void);

/**
 * @brief Initialize graphics system with custom configuration
 * @param config Pointer to configuration structure
 * @return GRAPHIC_OK on success, error code otherwise
 */
graphic_result_t graphic_init_with_config(const graphic_config_t* config);

/**
 * @brief Get default graphics configuration
 * @return Default configuration structure
 */
graphic_config_t graphic_get_default_config(void);

/**
 * @brief Deinitialize graphics system
 */
void graphic_deinit(void);

/**
 * @brief Check if graphics system is initialized
 * @return true if initialized, false otherwise
 */
bool graphic_is_initialized(void);

/**
 * @brief Get main display object
 * @return Pointer to display object or NULL if not initialized
 */
lv_disp_t* graphic_get_display(void);

/**
 * @brief Run graphics task (should be called periodically)
 * This function handles LVGL timer tasks and should be called in main loop
 */
void graphic_task_handler(void);

/**
 * @brief Force screen refresh
 */
void graphic_refresh(void);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GRAPHIC_H */
