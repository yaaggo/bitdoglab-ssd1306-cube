#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h> // Fornece definições de tipos como uint8_t (inteiro sem sinal de 8 bits)
#include <stdbool.h> // Fornece o tipo bool para variáveis booleanas

// Define os pinos usados para comunicação I2C
#define SDA_PIN 14  // Pino GPIO usado como linha de dados (SDA)
#define SCL_PIN 15  // Pino GPIO usado como linha de clock (SCL)

// Define o canal I2C a ser usado (i2c0 ou i2c1)
#define I2C_PORT i2c1 // Estamos usando o canal I2C1

// Define as dimensões do display SSD1306
#define DISPLAY_WIDTH 128 // Largura do display em pixels
#define DISPLAY_HEIGHT 64 // Altura do display em pixels

/**
 * @brief Inicializa a comunicação I2C no canal definido (I2C_PORT).
 *        Configura os pinos SDA e SCL e define a frequência de operação.
 */
void i2c_init_custom();

/**
 * @brief Envia um comando para o display SSD1306 via I2C.
 * 
 * @param command O comando que será enviado ao display.
 *        Exemplos: 0xAE (Display OFF), 0xAF (Display ON), etc.
 */
void ssd1306_send_command(uint8_t command);

/**
 * @brief Inicializa o display SSD1306.
 *        Envia a sequência de comandos necessários para configurar
 *        o display em modo normal de operação.
 */
void ssd1306_init();

/**
 * @brief Atualiza o conteúdo do display com base em um buffer de pixels.
 * 
 * @param display Um ponteiro para o buffer de 128x64 pixels (1024 bytes).
 *        Cada bit no buffer representa um pixel (1 = ON, 0 = OFF).
 */
void ssd1306_update(uint8_t *display);

/**
 * @brief Desenha um pixel individual no buffer de exibição.
 * 
 * @param x Coordenada X do pixel (0 a 127).
 * @param y Coordenada Y do pixel (0 a 63).
 * @param on Indica se o pixel deve ser ligado (true) ou desligado (false).
 * @param display Ponteiro para o buffer de exibição onde o pixel será alterado.
 */
void ssd1306_draw_pixel(int x, int y, bool on, uint8_t *display);

/**
 * @brief Desenha uma linha no buffer de exibição usando o algoritmo de Bresenham.
 * 
 * @param x0 Coordenada X inicial da linha.
 * @param y0 Coordenada Y inicial da linha.
 * @param x1 Coordenada X final da linha.
 * @param y1 Coordenada Y final da linha.
 * @param on Indica se a linha deve ser ligada (true) ou apagada (false).
 * @param display Ponteiro para o buffer de exibição onde a linha será desenhada.
 */
void ssd1306_draw_line(int x0, int y0, int x1, int y1, bool on, uint8_t *display);

#endif // SSD1306_H
