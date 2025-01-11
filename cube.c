#include "ssd1306.h"
#include "pico/stdlib.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define CENTER_X (DISPLAY_WIDTH / 2)
#define CENTER_Y (DISPLAY_HEIGHT / 2)

#define CUBE_SIZE 15

typedef struct {
    float x, y, z;
} Point3D;

Point3D cube_vertices[8] = {
    {-CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE},
    { CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE},
    { CUBE_SIZE,  CUBE_SIZE, -CUBE_SIZE},
    {-CUBE_SIZE,  CUBE_SIZE, -CUBE_SIZE},
    {-CUBE_SIZE, -CUBE_SIZE,  CUBE_SIZE},
    { CUBE_SIZE, -CUBE_SIZE,  CUBE_SIZE},
    { CUBE_SIZE,  CUBE_SIZE,  CUBE_SIZE},
    {-CUBE_SIZE,  CUBE_SIZE,  CUBE_SIZE},
};

int cube_edges[12][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Face traseira
    {4, 5}, {5, 6}, {6, 7}, {7, 4}, // Face frontal
    {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Conexões entre as faces
};

void project_3d_to_2d(Point3D point, int *x, int *y) {
    float scale = 64 / (64 + point.z); // Ajuste de perspectiva
    *x = (int)(CENTER_X + point.x * scale);
    *y = (int)(CENTER_Y - point.y * scale);
}

void rotate_x(Point3D *point, float angle) {
    float y = point->y * cos(angle) - point->z * sin(angle);
    float z = point->y * sin(angle) + point->z * cos(angle);
    point->y = y;
    point->z = z;
}

// Rotação no eixo Y
void rotate_y(Point3D *point, float angle) {
    float x = point->x * cos(angle) + point->z * sin(angle);
    float z = -point->x * sin(angle) + point->z * cos(angle);
    point->x = x;
    point->z = z;
}

// Rotação no eixo Z
void rotate_z(Point3D *point, float angle) {
    float x = point->x * cos(angle) - point->y * sin(angle);
    float y = point->x * sin(angle) + point->y * cos(angle);
    point->x = x;
    point->y = y;
}

void draw_cube(float angle_x, float angle_y, float angle_z, uint8_t *display) {
    Point3D transformed_vertices[8];
    for (int i = 0; i < 8; i++) {
        transformed_vertices[i] = cube_vertices[i];

        // Aplicar rotações em cada eixo
        rotate_x(&transformed_vertices[i], angle_x);
        rotate_y(&transformed_vertices[i], angle_y);
        rotate_z(&transformed_vertices[i], angle_z);
    }

    for (int i = 0; i < 12; i++) {
        int x0, y0, x1, y1;
        project_3d_to_2d(transformed_vertices[cube_edges[i][0]], &x0, &y0);
        project_3d_to_2d(transformed_vertices[cube_edges[i][1]], &x1, &y1);
        ssd1306_draw_line(x0, y0, x1, y1, true, display);
    }
}

int main() {
    stdio_init_all();
    i2c_init_custom();
    ssd1306_init();

    uint8_t display[128 * 64 / 8] = {};

    float angle_x = 0.0, angle_y = 0.0, angle_z = 0.0;

    while (true) {
        // Limpa o buffer
        memset(display, 0, sizeof(display));

        // Desenha o cubo rotacionado
        draw_cube(angle_x, angle_y, angle_z, display);
        ssd1306_update(display);

        // Atualiza os ângulos de rotação
        angle_x += 0.05;
        angle_y += 0.03; // Velocidade diferente para criar efeito mais dinâmico
        angle_z += 0.02;

        if (angle_x > 2 * M_PI) angle_x -= 2 * M_PI;
        if (angle_y > 2 * M_PI) angle_y -= 2 * M_PI;
        if (angle_z > 2 * M_PI) angle_z -= 2 * M_PI;

        sleep_ms(50); // Controle da velocidade de rotação
    }
}