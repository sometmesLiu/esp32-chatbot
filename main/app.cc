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

    auto audioEs7210 = std::make_shared<AudioEs7210>();

    // // 通过构造函数注入依赖
    WavRecorder recorder(sdFile, audioEs7210);
    recorder.record(10);
    
    sdFile->close();
}
