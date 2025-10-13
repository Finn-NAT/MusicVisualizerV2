#include "musicprocessor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavutil/opt.h>
#include <libavutil/channel_layout.h>
#include <libavutil/samplefmt.h>
#include <libavutil/time.h>
#include "kissfft/kiss_fft.h"
#include "kissfft/kiss_fftr.h"
#include "ring_buffer.h"

// Internal data structure
typedef struct {
    kiss_fftr_cfg fft_cfg;
    ring_buffer_t* ring_buffer;
    float* input_buffer;
    kiss_fft_cpx* output_buffer;
    float* magnitude;
    mp_state_t state;
    mp_config_t config;
    
    // FFmpeg related
    AVFormatContext* input_fmt_ctx;
    int audio_stream_index;
} mp_processor_t;

// Global processor instance
static mp_processor_t g_processor = {0};
static int g_initialized = 0;

// Internal function declarations
static void process_fft(void);
static void convert_samples_to_float(AVPacket* packet, float* output, int* num_samples);
static int setup_audio_input(void);
static void display_spectrum(void);

// Get default configuration
mp_config_t mp_get_default_config(void) {
    mp_config_t config = {
        .sample_rate = MP_SAMPLE_RATE,
        .channels = 1,
        .fft_size = MP_FFT_SIZE,
        .device_name = "default"
    };
    return config;
}

// Initialize with default config
mp_result_t mp_init(void) {
    mp_config_t default_config = mp_get_default_config();
    return mp_init_with_config(&default_config);
}

// Initialize with custom config
mp_result_t mp_init_with_config(const mp_config_t* config) {
    if (!config) {
        return MP_ERROR_INIT;
    }
    
    if (g_initialized) {
        mp_deinit();
    }
    
    // Copy configuration
    g_processor.config = *config;
    
    // Initialize FFT
    g_processor.fft_cfg = kiss_fftr_alloc(config->fft_size, 0, NULL, NULL);
    if (!g_processor.fft_cfg) {
        fprintf(stderr, "Could not initialize Kiss FFT\n");
        return MP_ERROR_INIT;
    }
    
    // Allocate buffers
    g_processor.input_buffer = (float*)calloc(config->fft_size, sizeof(float));
    g_processor.output_buffer = (kiss_fft_cpx*)calloc(config->fft_size/2 + 1, sizeof(kiss_fft_cpx));
    g_processor.magnitude = (float*)calloc(config->fft_size/2 + 1, sizeof(float));
    
    if (!g_processor.input_buffer || !g_processor.output_buffer || 
        !g_processor.magnitude) {
        fprintf(stderr, "Unable to allocate memory for FFT\n");
        mp_deinit();
        return MP_ERROR_INIT;
    }
    
    g_processor.state = MP_STATE_IDLE;
    g_processor.input_fmt_ctx = NULL;
    g_processor.audio_stream_index = -1;
    
    // Initialize FFmpeg
    avdevice_register_all();

    g_processor.ring_buffer = malloc(sizeof(ring_buffer_t));
    bool check = ring_buffer_init(g_processor.ring_buffer, config->fft_size); 
    printf("Ring buffer init check: %d\n", check);
    
    g_initialized = 1;
    printf("Music Processor initialized successfully (FFT_SIZE: %d)\n", 
           config->fft_size);
    
    return MP_SUCCESS;
}

// Start recording
mp_result_t mp_start_recording(void) {
    if (!g_initialized) {
        return MP_ERROR_INIT;
    }

    if (g_processor.state == MP_STATE_RECORDING) {
        return MP_SUCCESS; // Already recording
    }
    
    // Setup audio input
    if (setup_audio_input() != 0) {
        return MP_ERROR_DEVICE;
    }

    g_processor.state = MP_STATE_RECORDING;
    
    printf("Music processor started recording\n");
    return MP_SUCCESS;
}

// Stop recording
mp_result_t mp_stop_recording(void) {
    if (!g_initialized || g_processor.state != MP_STATE_RECORDING) {
        return MP_SUCCESS;
    }

    g_processor.state = MP_STATE_IDLE;

    // Close input
    if (g_processor.input_fmt_ctx) {
        avformat_close_input(&g_processor.input_fmt_ctx);
    }
    
    printf("Music processor stopped recording\n");
    return MP_SUCCESS;
}

// Cleanup
void mp_deinit(void) {
    if (!g_initialized) {
        return;
    }
    
    // Stop recording if active
    mp_stop_recording();
    
    // Free FFT resources
    if (g_processor.fft_cfg) {
        kiss_fftr_free(g_processor.fft_cfg);
        g_processor.fft_cfg = NULL;
    }
    
    free(g_processor.input_buffer);
    free(g_processor.output_buffer);
    free(g_processor.magnitude);

    ring_buffer_free(g_processor.ring_buffer);
    g_processor.input_buffer = NULL;
    g_processor.output_buffer = NULL;
    g_processor.magnitude = NULL;

    g_initialized = 0;
    printf("Music processor deinitialized\n");
}

// Internal functions implementation

static int setup_audio_input(void) {
    AVInputFormat *input_format = av_find_input_format("alsa");
    if (!input_format) {
        fprintf(stderr, "ALSA input format not found\n");
        return -1;
    }
    
    AVDictionary *options = NULL;
    char sample_rate_str[16];
    char channels_str[16];
    
    snprintf(sample_rate_str, sizeof(sample_rate_str), "%d", g_processor.config.sample_rate);
    snprintf(channels_str, sizeof(channels_str), "%d", g_processor.config.channels);
    
    av_dict_set(&options, "sample_rate", sample_rate_str, 0);
    av_dict_set(&options, "channels", channels_str, 0);
    
    int ret = avformat_open_input(&g_processor.input_fmt_ctx, g_processor.config.device_name, input_format, &options);
    if (ret < 0) {
        char error_str[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(ret, error_str, sizeof(error_str));
        fprintf(stderr, "Cannot open audio device: %s\n", error_str);
        av_dict_free(&options);
        return -1;
    }
    
    ret = avformat_find_stream_info(g_processor.input_fmt_ctx, NULL);
    if (ret < 0) {
        char error_str[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(ret, error_str, sizeof(error_str));
        fprintf(stderr, "Cannot find stream info: %s\n", error_str);
        av_dict_free(&options);
        return -1;
    }
    
    // Find audio stream
    for (unsigned int i = 0; i < g_processor.input_fmt_ctx->nb_streams; i++) {
        if (g_processor.input_fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            g_processor.audio_stream_index = i;
            break;
        }
    }
    
    if (g_processor.audio_stream_index == -1) {
        fprintf(stderr, "Cannot find audio stream\n");
        av_dict_free(&options);
        return -1;
    }
    
    av_dict_free(&options);
    return 0;
}

void processing_function(void) {
    AVPacket packet;
    float audio_samples[MP_BUFFER_SIZE];
    
    while (g_processor.state == MP_STATE_RECORDING) {
        int ret = av_read_frame(g_processor.input_fmt_ctx, &packet);
        if (ret < 0) {
            if (ret == AVERROR(EAGAIN)) {
                continue;
            }
            fprintf(stderr, "Frame reading error\n");
            break;
        }
        
        if (packet.stream_index == g_processor.audio_stream_index) {
            int num_samples;
            convert_samples_to_float(&packet, audio_samples, &num_samples);
            int count_debug = ring_buffer_write(g_processor.ring_buffer, audio_samples, num_samples);
            //printf("Wrote %f samples to ring buffer\n", g_processor.ring_buffer->buffer[g_processor.ring_buffer->current]);
            process_fft();
        }
        
        av_packet_unref(&packet);
    }
}

static void process_fft(void) {
    
    // Perform FFT
    ring_buffer_read_all(g_processor.ring_buffer, g_processor.input_buffer);
    kiss_fftr(g_processor.fft_cfg, g_processor.input_buffer, g_processor.output_buffer);
    
    // Calculate magnitude spectrum
    float max_mag = 0.0f;
    for (int i = 0; i < g_processor.config.fft_size/2 + 1; i++) {
        float real = g_processor.output_buffer[i].r;
        float imag = g_processor.output_buffer[i].i;
        g_processor.magnitude[i] = sqrtf(real*real + imag*imag);
        if (g_processor.magnitude[i] > max_mag) {
            max_mag = g_processor.magnitude[i];
        }
    }

    //display_spectrum() ;
    
}

static void convert_samples_to_float(AVPacket* packet, float* output, int* num_samples) {
    int16_t* input_samples = (int16_t*)packet->data;
    *num_samples = packet->size / sizeof(int16_t);
    
    for (int i = 0; i < *num_samples; i++) {
        output[i] = input_samples[i] / 32768.0f;
    }
}

static void display_spectrum() {
    printf("\r");
    
    // Find max value to normalize
    float max_val = 0;
    for (int i = 1; i < 40; i++) { // Ignore bin 0 (DC component)
        if (g_processor.magnitude[i] > max_val) {
            max_val = g_processor.magnitude[i];
        }
    }
    
    // Display bars
    printf("Spectrum: [");
    for (int i = 1; i < 40; i++) {
        float normalized = (max_val > 0) ? (g_processor.magnitude[i] / max_val) : 0;
        int bar_height = (int)(normalized * 12);
        if (bar_height >= 12) printf("█");
        else if (bar_height >= 11) printf("▇");
        else if (bar_height >= 10) printf("▆");
        else if (bar_height >= 9) printf("▅");
        else if (bar_height >= 8) printf("▄");
        else if (bar_height >= 6) printf("▃");
        else if (bar_height >= 5) printf("▂");
        else if (bar_height >= 4) printf("▁");
        else printf("_");
    }
    printf("] Max: %.0f", max_val);
    fflush(stdout);
}

float* get_magnitude_data(void) {
    return g_processor.magnitude;
}