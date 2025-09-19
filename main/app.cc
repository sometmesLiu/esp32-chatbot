#include <cstdio>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h" 
#include "sd_card.h"
#include <string.h>
#include <bits/shared_ptr.h>
#include "wav_recorder.h"
#include "audio_es7210.h"

char buffer[100];

extern "C" void app_main(void)
{
    auto sdFile = std::make_shared<SdCard>();
    ESP_ERROR_CHECK(sdFile->open("vvv.wav","w+"));
    // // 通过构造函数注入依赖
    WavRecorder recorder(sdFile);
    //
    AudioEs7210 audioEs7210 = AudioEs7210();
    
    // // 开始录音
    i2s_chan_handle_t i2sChan = audioEs7210.audio_es7210_init();
    recorder.record(i2sChan, 10);

    sdFile->close();
}
