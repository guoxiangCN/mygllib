#include "mygllib_canvas.h"

namespace mygl {

#define NORM_ALPHA(x) ((x)/255.f)

static inline RGBA applyBlend(GlBlendContext *ctx, RGBA src_color, RGBA dest_color) { 
    RGBA output; 
    float srcAlpha = NORM_ALPHA(src_color.m_a) *0.5;
    output.m_r = src_color.m_r * srcAlpha + dest_color.m_r * (1 - srcAlpha);
    output.m_g = src_color.m_g * srcAlpha + dest_color.m_g * (1 - srcAlpha);
    output.m_b = src_color.m_b * srcAlpha + dest_color.m_b * (1 - srcAlpha);
    output.m_a = src_color.m_a * srcAlpha + dest_color.m_a * (1 - srcAlpha);
    return output;
}

void Canvas::DrawLineV1(Point2D pt1, Point2D pt2, RGBA color) {
    double k = (pt1.x < pt2.x) ? ((pt2.y - pt1.y) / (pt2.x - pt1.x))
                               : ((pt1.y - pt2.y) / (pt1.x - pt2.x));
    double b = pt1.y - (k * pt1.x);

    int begin = pt1.x;
    int end = pt2.x;
    if (begin > end) {
        std::swap(begin, end);
    }

    for (int x = begin; x <= end; x++) {
        double y = k * x + b;
        DrawPoint(Point2D(x, y), color);
    }
}

void Canvas::DrawLineV2(Point2D pt1, Point2D pt2, RGBA color) {
    double k = (pt2.x - pt1.x != 0) ? ((pt2.y - pt1.y) / (pt2.x - pt1.x))
                                    : 0;  // 计算斜率
    double b = pt1.y - k * pt1.x;         // 计算截距

    int x1 = pt1.x;
    int y1 = pt1.y;
    int x2 = pt2.x;
    int y2 = pt2.y;

    // 判断斜率是否大于 1
    if (abs(k) <= 1) {
        // 以 x 为步长
        if (x1 > x2) {
            std::swap(x1, x2);
        }
        for (int x = x1; x <= x2; x++) {
            double y = k * x + b;
            DrawPoint(Point2D(x, static_cast<int>(y + 0.5)),
                      color);  // 四舍五入
        }
    } else {
        // 以 y 为步长
        if (y1 > y2) {
            std::swap(y1, y2);
        }
        for (int y = y1; y <= y2; y++) {
            double x = (y - b) / k;
            DrawPoint(Point2D(static_cast<int>(x + 0.5), y),
                      color);  // 四舍五入
        }
    }
}

void Canvas::DrawLineV3(Point2D pt1, Point2D pt2, RGBA color) {
    int disY = abs(pt2.y - pt1.y);
    int disX = abs(pt2.x - pt1.x);

    int xNow = pt1.x;
    int yNow = pt1.y;

    int stepX = (pt1.x < pt2.x) ? 1 : -1;
    int stepY = (pt1.y < pt2.y) ? 1 : -1;

    int totalStep = disX;
    bool useXStep = true;

    if (disX < disY) {  // k = (disY/disX)
        totalStep = disY;
        useXStep = false;
        std::swap(disX, disY);
    }

    int p = 2 * disY - disX;

    for (int i = 0; i < totalStep; i++) {
        DrawPoint(Point2D(xNow, yNow), color);

        if (p >= 0) {
            if (useXStep) {
                yNow += stepY;
            } else {
                xNow += stepX;
            }

            p = p - 2 * disX;
        }

        if (useXStep) {
            xNow += stepX;
        } else {
            yNow += stepY;
        }

        p = p + 2 * disY;
    }
}

void Canvas::DrawTriangleV1(Point2D pt1, Point2D pt2, Point2D pt3) {
    // 构建最小矩形包围
    int left = std::min({pt1.x, pt2.x, pt3.x});
    int top = std::max({pt1.y, pt2.y, pt3.y});
    int right = std::max({pt1.x, pt2.x, pt3.x});
    int bottom = std::min({pt1.y, pt2.y, pt3.y});

    // 屏幕裁剪
    left = left < 0 ? 0 : left;
}

void Canvas::DrawImage(int x, int y, Image* image) {
    for (int u = 0; u < image->GetWidth(); u++) {
        for (int v = 0; v < image->GetHeight(); v++) {
            RGBA srcColor = image->GetColor(u, v);
            RGBA destColor = GetColor(x + u, y + v);
            if (m_flags & GL_BLEND) {
                RGBA finalColor = applyBlend(&m_blend_ctx,srcColor, destColor);
                DrawPoint(Point2D(x + u, y + v), finalColor);
            } else {
                DrawPoint(Point2D(x + u, y + v), srcColor);
            }
            
        }
    }
}
}  // namespace mygl