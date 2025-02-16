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

namespace mygl {

class Canvas {
private:
    int m_width;
    int m_height;
    RGBA* m_buffer;

public:
    Canvas(int width, int height, void* buffer) {
        assert(width > 0 && height > 0);
        m_width = width;
        m_height = height;
        m_buffer = reinterpret_cast<RGBA*>(buffer);
    }

    ~Canvas() = default;

    Canvas(const Canvas&) = delete;
    Canvas& operator=(const Canvas&) = delete;

    void Clear() {
        if (m_buffer) {
            memset(m_buffer, 0, sizeof(RGBA) * m_width * m_height);
        }
    }

    /**
     * @brief ���Ƶ�
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
     * ����б�ʽؾ෨����ֱ��(���ڸ�������������Ͳ�������ᵼ�²����ϵ�(����),
     * �����޷�������)
     * @param pt1
     * @param pt2
     * @param color
     */
    void DrawLineV1(Point2D pt1, Point2D pt2, RGBA color);

    /**
     * @brief  ����б�ʽؾ෨����ֱ�ߵ��Ż�:
       �� |k| <= 1 ʱ���� x Ϊ���������� y ���������룻
       �� |k| > 1 ʱ���� y Ϊ���������� x ���������룻
     * @param pt1
     * @param pt2
     * @param color
     */
    void DrawLineV2(Point2D pt1, Point2D pt2, RGBA color);

    /**
     * @brief Brensenhamֱ�߻��� (���⸡��������)
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     * @param color
     */
    void DrawLineV3(Point2D pt1, Point2D pt2, RGBA color);

    // TODO: Xiaolin Wu �㷨��֧�ֿ����

    /**
     *
     * @brief ʹ��ɨ�����㷨����������(Ч�ʵ�)
     * @param pt1
     * @param pt2
     * @param pt3
     */
    void DrawTriangleV1(Point2D pt1, Point2D pt2, Point2D pt3);

    /**
     * TODO
     * @brief ����������
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
