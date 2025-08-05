#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdio_uart.h"
#include "AHT10.h"
#include "u8g2pico.h"

#define UART_ID         uart1
#define UART_BAUD_RATE  115200
#define UART_TX_PIN     8
#define UART_RX_PIN     9

#define AHT10_I2C_PORT i2c0
#define AHT10_SDA_PIN 0
#define AHT10_SCL_PIN 1
#define AHT10_ADDRESS 0x38

#define DISPLAY_I2C_PORT i2c1
#define DISPLAY_SDA_PIN 14
#define DISPLAY_SCL_PIN 15
#define DISPLAY_I2C_ADDRESS 0x3C

void inicializarDisplay(u8g2pico_t *u8g2);
void exibirDados(u8g2pico_t *u8g2, float temperatura, float umidade);

int main()
{
    stdio_uart_init_full(UART_ID, UART_BAUD_RATE, UART_TX_PIN, UART_RX_PIN);
    
    printf("\n=== INICIALIZANDO SISTEMA ===\n");
    printf("UART configurado: %d baud\n", UART_BAUD_RATE);
    printf("Pinos UART: TX=%d, RX=%d\n", UART_TX_PIN, UART_RX_PIN);

    aht10_t aht10;
    u8g2pico_t u8g2;

    printf("Iniciando AHT10...\n");

    iniciarAHT10(&aht10, AHT10_I2C_PORT, AHT10_SDA_PIN, AHT10_SCL_PIN, AHT10_ADDRESS);
    printf("AHT10 iniciado com sucesso!\n");

    inicializarDisplay(&u8g2);
    printf("Display inicializado com sucesso!\n");

    printf("Sistema pronto! Iniciando leituras...\n");
    printf("=====================================\n");

    while (true)
    {
        if (disponivel(&aht10))
        {
            float temperatura = obterTemperaturaCelsius(&aht10);
            float umidade = obterUmidadeRelativa(&aht10);
            
            printf("Temperatura: %.2f°C\tUmidade: %.2f%%\n", temperatura, umidade);
            exibirDados(&u8g2, temperatura, umidade);
            
            if (umidade > 70.0f)
            {
                printf("ALERTA: Umidade alta! (%.2f%%)\n", umidade);
            }
            if (temperatura < 20.0f)
            {
                printf("ALERTA: Temperatura baixa! (%.2f°C)\n", temperatura);
            }
        }
        else
        {
            printf("Erro: Sensor AHT10 não disponível\n");
        }
        sleep_ms(1000);
    }
}

void inicializarDisplay(u8g2pico_t *u8g2)
{
    u8g2_Setup_ssd1306_i2c_128x64_noname_f_pico(u8g2, DISPLAY_I2C_PORT, DISPLAY_SDA_PIN, DISPLAY_SCL_PIN, U8G2_R0, DISPLAY_I2C_ADDRESS);
    u8g2_InitDisplay(u8g2);
    u8g2_SetPowerSave(u8g2, 0);
    u8g2_ClearBuffer(u8g2);
    u8g2_SendBuffer(u8g2);
}

void exibirDados(u8g2pico_t *u8g2, float temperatura, float umidade)
{
    u8g2_ClearBuffer(u8g2);
    u8g2_SetFont(u8g2, u8g2_font_ncenB08_tr);

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "Temp: %.2f C", temperatura);
    u8g2_DrawStr(u8g2, 0, 10, buffer);

    snprintf(buffer, sizeof(buffer), "Umid: %.2f %%", umidade);
    u8g2_DrawStr(u8g2, 0, 20, buffer);

    if (umidade > 70.0f)
    {
        u8g2_DrawStr(u8g2, 0, 30, "Alerta: Alta Umid!");
    }
    if (temperatura < 20.0f)
    {
        u8g2_DrawStr(u8g2, 0, 40, "Alerta: Baixa temp!");
    }

    u8g2_SendBuffer(u8g2);
}
