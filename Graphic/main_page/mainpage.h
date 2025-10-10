#ifndef MAINPAGE_H
#define MAINPAGE_H

#include "../lvgl/lvgl.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SUBPAGES 10

typedef struct {
    lv_obj_t *main_container;               // Main container object
    lv_obj_t *subpage_list;                 // List widget to contain subpage containers
} mainpage_t;

/**
 * @brief Initialize main page with container list
 * @param parent Parent object to create main page in
 */
void mainpage_create(lv_obj_t *parent);

#ifdef __cplusplus
}
#endif

#endif /* MAINPAGE_H */