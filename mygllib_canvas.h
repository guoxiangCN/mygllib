/**
 * DrawLine: https://zhuanlan.zhihu.com/p/678524194
 */
#pragma once

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>

#include "mygllib_image.h"
#include "mygllib_types.h"
#include "mygllib_blend.h"

namespace mygl {

enum {
    GL_BLEND = 1, // 开启颜色混合(类似opengl::BL_BLEND)
};

class Canvas {
private:
    int m_width;
    int m_height;
    RGBA* m_buffer;
    
    uint64_t m_flags;
    GlBlendContext m_blend_ctx;

public:
    Canvas(int width, int height, void* buffer) {
        assert(width > 0 && height > 0);
        m_width = width;
        m_height = height;
        m_buffer = reinterpret_cast<RGBA*>(buffer);
        m_flags = 0;
    }

    ~Canvas() = default;

    Canvas(const Canvas&) = delete;
    Canvas& operator=(const Canvas&) = delete;

    /**
     * @brief 清空整个画布
     */
    void Clear() {
        if (m_buffer) {
            memset(m_buffer, 0, sizeof(RGBA) * m_width * m_height);
        }
    }

    /**
     * @brief 启用特性
     * @param feature 如GL_BLEND
     */
    void Enable(uint64_t feature) {
        m_flags |= feature;
    }

    /**
     * @brief 禁用特性
     * @param feature 如GL_BLEND
     */
    void Disable(uint64_t feature) {
        m_flags &= (~feature);
    }

    /**
     * @brief 设置blend的混合因子
     * @param sfactor 
     * @param dfactor 
     */
    void glBlendFunc(GLenum sfactor, GLenum dfactor) {
        // TODO: 检查是否有效.
        // Ref: https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/mesa/main/blend.c?ref_type=heads#L127
        m_blend_ctx.blend_src = sfactor;
        m_blend_ctx.blend_dest = dfactor;
    }

    /**
     * @brief 获取画布上指定坐标的已有颜色(背景色)
     * @param x 
     * @param y 
     * @return 
     */
    RGBA GetColor(int x, int y) {
        if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
            return COLOR_UNDEFINED;
        }

        return m_buffer[y * m_width + x];
    }

    /**
     * @brief 绘制点
     * @param p
     * @param color
     */
    void DrawPoint(Point2D p, RGBA color) {
        int x = p.x;
        int y = p.y;

        if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
            return;
        }

        m_buffer[y * m_width + x] = color;
    }

    /**
     * @brief
     * 基于斜率截距法绘制直线(由于浮点数精度问题和步长问题会导致产生断点(虚线),
     * 此外无法处理垂线)
     * @param pt1
     * @param pt2
     * @param color
     */
    void DrawLineV1(Point2D pt1, Point2D pt2, RGBA color);

    /**
     * @brief  基于斜率截距法绘制直线的优化:
       当 |k| <= 1 时，以 x 为步长，计算 y 并四舍五入；
       当 |k| > 1 时，以 y 为步长，计算 x 并四舍五入；
     * @param pt1
     * @param pt2
     * @param color
     */
    void DrawLineV2(Point2D pt1, Point2D pt2, RGBA color);

    /**
     * @brief Brensenham直线绘制 (避免浮点数计算)
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     * @param color
     */
    void DrawLineV3(Point2D pt1, Point2D pt2, RGBA color);

    // TODO: Xiaolin Wu 算法，支持抗锯齿

    /**
     *
     * @brief 使用扫描线算法绘制三角形(效率低)
     * @param pt1
     * @param pt2
     * @param pt3
     */
    void DrawTriangleV1(Point2D pt1, Point2D pt2, Point2D pt3);

    /**
     * TODO
     * @brief 绘制三角形
     * @param pt1
     * @param pt2
     * @param pt3
     */
    void DrawTriangleV2(Point2D pt1, Point2D pt2, Point2D pt3) {}

    /**
     * @brief
     * @param x
     * @param y
     * @param image
     */
    void DrawImage(int x, int y, Image* image);
    void DrawImage(Point2D pt, Image* image) { DrawImage(pt.x, pt.y, image); }
};

}  // namespace mygl
