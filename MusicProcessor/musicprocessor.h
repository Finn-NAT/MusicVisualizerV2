#ifndef MUSICPROCESSOR_H
#define MUSICPROCESSOR_H

#include <stdint.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

// Constants
#define MP_BUFFER_SIZE 1024
#define MP_SAMPLE_RATE 44100
#define MP_FFT_SIZE 1024
#define MP_MAX_FREQ_BINS 80

// Error codes
typedef enum {
    MP_SUCCESS = 0,
    MP_ERROR_INIT = -1,
    MP_ERROR_DEVICE = -2
} mp_result_t;

typedef enum {
    MP_STATE_IDLE = 0,
    MP_STATE_RECORDING,
    MP_STATE_ERROR
} mp_state_t;

// Configuration structure
typedef struct {
    int sample_rate;
    int channels;
    int fft_size;
    const char* device_name;
} mp_config_t;


// Public API functions

/**
 * Initialize the music processor with default configuration
 * @return MP_SUCCESS on success, error code on failure
 */
mp_result_t mp_init(void);

/**
 * Initialize the music processor with custom configuration
 * @param config Configuration structure
 * @return MP_SUCCESS on success, error code on failure
 */
mp_result_t mp_init_with_config(const mp_config_t* config);

/**
 * Start real-time audio processing
 * @param callback Function to call with FFT data (can be NULL)
 * @param user_data User data to pass to callback
 * @return MP_SUCCESS on success, error code on failure
 */
mp_result_t mp_start_recording(void);

/**
 * Stop audio processing
 * @return MP_SUCCESS on success, error code on failure
 */
mp_result_t mp_stop_recording(void);

/**
 * Internal processing function (to be run in a separate thread)
 */
void processing_function(void);

/**
 * Get the latest magnitude spectrum data
 * @return Pointer to array of magnitude values (size: fft_size/2 + 1)
 */
float* get_magnitude_data(void);

/**
 * Cleanup and free all resources
 */
void mp_deinit(void);

/**
 * Get default configuration
 * @return Default configuration structure
 */
mp_config_t mp_get_default_config(void);

#ifdef __cplusplus
}
#endif

#endif // MUSICPROCESSOR_H