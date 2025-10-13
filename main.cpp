#include "Graphic/graphic.h"
#include "Graphic/main_page/mainpage.h"
#include "Graphic/sub_pages/subpage.h"
#include "Graphic/lvgl/lvgl.h"
#include "MusicProcessor/musicprocessor.h"
#include <stdio.h>
#include <unistd.h>

#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

pthread_mutex_t lvgl_mutex;
subpage_value_t value;

void* thread0(void* arg) {
    processing_function();
    usleep(10000); /* 10ms */
    return NULL;
}

void* thread1(void* arg) {
    while (1) {
        pthread_mutex_lock(&lvgl_mutex); 
        if (MusicVisualizerPage && MusicVisualizerPage->state == SUB_PAGE_INIT) {
            MusicVisualizerPage->sub_page_main_function(&value);
        } else if(MusicVisualizerPage && MusicVisualizerPage->state == SUB_PAGE_DEINIT){
            MusicVisualizerPage->sub_page_deinit();
        }
        pthread_mutex_unlock(&lvgl_mutex); 
        usleep(50000); /* 50ms */
    }
    return NULL;
}


int main(void)
{  
    /* Initialize graphics with default configuration */
    graphic_result_t result = graphic_init();
    if (result != GRAPHIC_OK) {
        printf("Error: Failed to initialize graphics: %d\n", result);
        return -1;
    }
    
    /* Create test UI */
    mainpage_create(lv_scr_act());
    mp_init();
    value.value = get_magnitude_data();
    printf("Magnitude data pointer: %f\n", value.value[0]);
    mp_start_recording();

    pthread_mutex_init(&lvgl_mutex, NULL);

    const int NUM_THREADS = 2;
    pthread_t threads[NUM_THREADS];
    pthread_create(&threads[0], NULL, thread0, NULL);
    pthread_create(&threads[1], NULL, thread1, NULL);

    /* Main loop */
    printf("Starting main loop... (Press Ctrl+C to exit)\n");
    while (1) {
        lv_tick_inc(5);
        /* Handle LVGL tasks */
        pthread_mutex_lock(&lvgl_mutex); 
        graphic_task_handler();
        pthread_mutex_unlock(&lvgl_mutex);

        /* Sleep for a short time to avoid 100% CPU usage */
        usleep(5000); /* 5ms */
    }
    
    /* Cleanup (never reached in this example) */
    pthread_join(threads[0], NULL);
    pthread_cancel(threads[0]);
    pthread_join(threads[1], NULL);
    pthread_cancel(threads[1]);
    graphic_deinit();
    printf("Graphics system deinitialized.\n");
    
    return 0;
}