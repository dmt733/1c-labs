#include <math.h>
#include <stdio.h>

#define IMAGE_HEIGHT 1080
#define IMAGE_WIDTH 1920

struct point_f { float x_, y_; };
struct color { float r_, g_, b_; };

struct point_f get_bezier_point(float t, const struct point_f p[4]) {
    float a = 1 - t, a2 = a*a, a3 = a2*a, t2 = t*t, t3 = t2*t;
    struct point_f pt;
    pt.x_ = a3*p[0].x_ + 3*a2*t*p[1].x_ + 3*a*t2*p[2].x_ + t3*p[3].x_;
    pt.y_ = a3*p[0].y_ + 3*a2*t*p[1].y_ + 3*a*t2*p[2].y_ + t3*p[3].y_;
    return pt;
}

float dist(const struct point_f *a, const struct point_f *b) {
    float dx = a->x_ - b->x_, dy = a->y_ - b->y_;
    return sqrt(dx*dx + dy*dy);
}

int main() {
    int SEG = 100;
    struct point_f pts[SEG];
    
    // Уникальная кривая Безье
    struct point_f cp[4] = {{0.1f, 0.8f}, {0.3f, 0.2f}, {0.7f, 0.9f}, {0.9f, 0.3f}};
    struct color curve_color = {0.95f, 0.2f, 0.3f};
    
    for (int i = 0; i < SEG; i++)
        pts[i] = get_bezier_point((float)i/(SEG-1), cp);
    
    printf("P3\n%d %d\n255\n", IMAGE_WIDTH, IMAGE_HEIGHT);
    
    for (int j = 0; j < IMAGE_HEIGHT; j++) {
        for (int i = 0; i < IMAGE_WIDTH; i++) {
            struct point_f p = {(float)i/IMAGE_WIDTH, (float)j/IMAGE_HEIGHT};
            
            // Находим минимальное расстояние до кривой
            float d = 1.0f;
            for (int k = 0; k < SEG; k++) {
                float dd = dist(&p, &pts[k]);
                if (dd < d) d = dd;
            }
            
            // Смешиваем фон с цветом кривой
            float w = exp(-d*d/(2*0.1f*0.1f));
            struct color bg = {0.92f, 0.85f, 0.71f};
            
            // Уникальный цветовой эффект
            float hue = (p.x_ + p.y_) * 0.5f;
            struct color extra = {hue, 0.5f + 0.5f*cosf(p.x_*10), 0.5f + 0.5f*sinf(p.y_*10)};
            
            struct color c = {
                bg.r_*(1-w) + curve_color.r_*w + extra.r_*0.1f,
                bg.g_*(1-w) + curve_color.g_*w + extra.g_*0.1f,
                bg.b_*(1-w) + curve_color.b_*w + extra.b_*0.1f
            };
            
            printf("%d %d %d\n", 
                (int)(c.r_*255.999f), 
                (int)(c.g_*255.999f), 
                (int)(c.b_*255.999f));
        }
    }
    return 0;
}