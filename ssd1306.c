#include "ssd1306.h"       // Inclui o arquivo de cabeçalho para declarações e configurações
#include "hardware/i2c.h"  // Biblioteca do Raspberry Pi Pico para comunicação I2C
#include "pico/stdlib.h"   // Biblioteca padrão do Pico para GPIO e outras funções básicas

#include <stdlib.h>
#include <string.h>        

void i2c_init_custom() {
    i2c_init(I2C_PORT, 400 * 1000); // Inicializa I2C com frequência de 400 kHz
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C); // Configura o pino SDA como função I2C
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C); // Configura o pino SCL como função I2C
    gpio_pull_up(SDA_PIN); // Habilita resistor pull-up no pino SDA
    gpio_pull_up(SCL_PIN); // Habilita resistor pull-up no pino SCL
}

void ssd1306_send_command(uint8_t command) {
    uint8_t data[] = {0x00, command}; // 0x00 indica que é um comando
    i2c_write_blocking(I2C_PORT, 0x3C, data, sizeof(data), false); // Envia o comando ao endereço do display (0x3C)
}

void ssd1306_init() { // isso daqui eu não faço ideia de nada, so aceitei
    ssd1306_send_command(0xAE); // Display OFF
    ssd1306_send_command(0xD5); // Set Display Clock Divide Ratio
    ssd1306_send_command(0x80); // Frequência padrão
    ssd1306_send_command(0xA8); // Set Multiplex Ratio
    ssd1306_send_command(0x3F); // Multiplex para 64 linhas
    ssd1306_send_command(0xD3); // Set Display Offset
    ssd1306_send_command(0x00); // Sem deslocamento
    ssd1306_send_command(0x40); // Set Display Start Line para 0
    ssd1306_send_command(0x8D); // Ativa Charge Pump
    ssd1306_send_command(0x14); // Habilita
    ssd1306_send_command(0x20); // Define modo de endereçamento
    ssd1306_send_command(0x00); // Modo horizontal
    ssd1306_send_command(0xA1); // Segment Re-map (coluna 127 mapeada para SEG0)
    ssd1306_send_command(0xC8); // COM Output Scan Direction (invertido)
    ssd1306_send_command(0xDA); // Set COM Pins Hardware Configuration
    ssd1306_send_command(0x12); // Configuração padrão
    ssd1306_send_command(0x81); // Define contraste
    ssd1306_send_command(0x7F); // Valor de contraste (127)
    ssd1306_send_command(0xD9); // Define período de pré-carga
    ssd1306_send_command(0xF1); // Valor padrão
    ssd1306_send_command(0xDB); // Define nível de deseleção VCOMH
    ssd1306_send_command(0x40); // Nível padrão
    ssd1306_send_command(0xA4); // Define display como "seguindo o conteúdo da RAM"
    ssd1306_send_command(0xA6); // Display em modo normal (não invertido)
    ssd1306_send_command(0xAF); // Display ON
}


void ssd1306_update(uint8_t *display) {
    for (uint8_t page = 0; page < 8; page++) { // Cada página contém 8 linhas de pixels
        ssd1306_send_command(0xB0 + page); // Seleciona a página
        ssd1306_send_command(0x00);        // Define endereço de coluna inferior
        ssd1306_send_command(0x10);        // Define endereço de coluna superior

        uint8_t data[129]; // 1 byte para controle e 128 bytes para pixels
        data[0] = 0x40; // 0x40 indica que estamos enviando dados
        memcpy(&data[1], &display[page * DISPLAY_WIDTH], DISPLAY_WIDTH); // Copia uma página do buffer

        i2c_write_blocking(I2C_PORT, 0x3C, data, sizeof(data), false); // Envia dados para o display
    }
}

void ssd1306_draw_pixel(int x, int y, bool on, uint8_t *display) {
    if (x < 0 || x >= DISPLAY_WIDTH || y < 0 || y >= DISPLAY_HEIGHT) return; // Ignora pixels fora do limite
    if (on)
        display[x + (y / 8) * DISPLAY_WIDTH] |= (1 << (y % 8)); // Liga o pixel
    else
        display[x + (y / 8) * DISPLAY_WIDTH] &= ~(1 << (y % 8)); // Desliga o pixel
}

// algoritmo de Bresenham

void ssd1306_draw_line(int x0, int y0, int x1, int y1, bool on, uint8_t *display) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1; // Delta X e direção de incremento
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1; // Delta Y e direção de incremento
    int err = dx + dy, e2; // Erro inicial e variável auxiliar

    while (1) {
        ssd1306_draw_pixel(x0, y0, on, display); // Desenha pixel na posição atual
        if (x0 == x1 && y0 == y1) break; // Termina se chegou ao ponto final
        e2 = 2 * err;
        if (e2 >= dy)  err += dy, x0 += sx;  // Ajusta erro e incrementa X
        if (e2 <= dx)  err += dx, y0 += sy;  // Ajusta erro e incrementa Y
    }
}