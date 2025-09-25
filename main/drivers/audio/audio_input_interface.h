#pragma once
#include <cstddef>
#include <esp_err.h>

class AudioInputInterface {
    public:
        virtual ~AudioInputInterface() = default;
        virtual esp_err_t enable() = 0;
        virtual esp_err_t disable() = 0;
        virtual esp_err_t read(void *dest, size_t size, size_t *bytes_read) = 0;
};
