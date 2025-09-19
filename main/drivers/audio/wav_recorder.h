#pragma once
#include "file_interface.h"
#include "driver/i2s_std.h"
#include <memory>  

class WavRecorder {
public:
    explicit WavRecorder(std::shared_ptr<FileInterface> fileInterface);

    esp_err_t record(i2s_chan_handle_t i2s_chan, uint16_t seconds);
    
private:
    std::shared_ptr<FileInterface> m_file; // 用智能指针持有对象
};
