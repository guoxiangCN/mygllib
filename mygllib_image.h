#pragma once

#include "mygllib_types.h"

namespace mygl {

class Image {
public:
    Image(int width, int height, byte* data)
        : m_width(width), m_height(height), m_data(nullptr) {
        if (data) {
            auto data_ptr = new RGBA[width * height];
            memcpy(data_ptr, data, sizeof(RGBA) * height * width);
            m_data.reset(data_ptr);
        }
    }

    ~Image() = default;

    int GetWidth() const { return m_width; }

    int GetHeight() const { return m_height; }

    RGBA GetColor(int x, int y) const {
        if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
            return RGBA(0, 0, 0, 0);
        }
        return m_data[static_cast<size_t>(y * m_width + x)];
    }

private:
    int m_width;
    int m_height;
    std::unique_ptr<RGBA[]> m_data;
};

/**
 * @brief Read a image from a pic file(jpg jpeg...)
 * @param path
 * @return
 */
Image* ReadImageFromFile(const std::string& path);
}  // namespace mygl