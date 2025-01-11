# Projeto: Cubo Animado no Display SSD1306 com Raspberry Pi Pico W

Este projeto demonstra a renderização de um cubo animado em um display OLED SSD1306, utilizando uma **Raspberry Pi Pico W**. A implementação foi feita em **C** com uma biblioteca personalizada para controle do display. O projeto foi desenvolvido e testado no simulador [Wokwi](https://wokwi.com/) e está totalmente funcional na placa física.

---

## 📋 **Conteúdo**
- [Introdução](#introdução)
- [Pré-requisitos](#pré-requisitos)
- [Estrutura do Projeto](#estrutura-do-projeto)
- [Configuração no Simulador Wokwi](#configuração-no-simulador-wokwi)
- [Configuração no Hardware](#configuração-no-hardware)
- [Compilação e Execução](#compilação-e-execução)
- [Funcionamento](#funcionamento)

---

## 📖 **Introdução**

Este projeto apresenta um exemplo prático do uso do display SSD1306 para criar uma animação de um cubo 3D rotacionando. Utilizando o barramento **I2C**, o sistema é capaz de exibir gráficos animados em um display de 128x64 pixels.

**Destaques do Projeto:**
- Uso de uma **biblioteca personalizada** para controle do SSD1306.
- **Animação do cubo** com renderização de linhas em um buffer.
- Testado no **simulador Wokwi** e na placa física Raspberry Pi Pico W.

---

## 🛠 **Pré-requisitos**

### Hardware
- Raspberry Pi Pico W.
- Display OLED SSD1306 (128x64 pixels, I2C).
- Conexões I2C:
  - **SDA**: GPIO14 (ou conforme configurado).
  - **SCL**: GPIO15 (ou conforme configurado).
- Resistores Pull-up de 4.7kΩ nos pinos SDA e SCL (normalmente já presentes no módulo SSD1306).

### Software
- **Pico SDK** instalado e configurado.
- Compilador GCC para ARM.
- Simulador [Wokwi](https://wokwi.com/).
- Biblioteca personalizada `ssd1306.h` e `ssd1306.c`.

---

## 📂 **Estrutura do Projeto**


### Arquivos Principais
- **`cube.c`**: Contém a lógica para desenhar e animar o cubo 3D, utilizando funções da biblioteca `ssd1306`.
- **`ssd1306.h` e `ssd1306.c`**: Implementação da biblioteca para controle do display SSD1306.

---

## 🖥 **Configuração no Simulador Wokwi**

1. Acesse o [Wokwi](https://wokwi.com/).
2. Importe o arquivo `wokwi-project.json` fornecido no projeto.
3. Certifique-se de que os pinos I2C do display estão conectados corretamente:
   - **SDA**: GPIO14.
   - **SCL**: GPIO15.
4. Clique em "Start Simulation" para rodar o projeto.

---

## 🔧 **Configuração no Hardware**

1. Conecte o display SSD1306 à Raspberry Pi Pico W:
   - **VCC**: 3.3V da Pico.
   - **GND**: GND da Pico.
   - **SDA**: GPIO14.
   - **SCL**: GPIO15.
2. Certifique-se de que o endereço I2C do display seja 0x3C (valor padrão para o SSD1306).

---

## ⚙ **Compilação e Execução**

### Passo 1: Configurar o Ambiente
Certifique-se de que o Pico SDK está configurado corretamente no seu sistema. Consulte a [documentação oficial](https://github.com/raspberrypi/pico-sdk) para mais informações.

### Passo 2: Compilar o Projeto
No diretório do projeto, execute os seguintes comandos:

```bash
mkdir build
cd build
cmake ..
make
```
## 🔄 Funcionamento
Após a inicialização, o display será preenchido com a animação de um cubo 3D rotacionando.

O desenho é renderizado no buffer local e enviado ao display via I2C.
Descrição da Animação:
O cubo é desenhado utilizando linhas conectando vértices 3D transformados para coordenadas 2D.
A rotação é feita aplicando matrizes de transformação nos eixos X, Y e Z.
