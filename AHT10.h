#pragma once
#include "hardware/i2c.h"
#include "hardware/gpio.h"

#define AHT10_CMD_CALIBRATION 0xE1
#define AHT10_CMD_CALIBRATION_ARG 0x08
#define AHT10_CMD_CALIBRATION_ARG2 0x00
#define AHT10_CMD_MEASURE 0xAC
#define AHT10_CMD_MEASURE_ARG 0x33
#define AHT10_CMD_MEASURE_ARG2 0x00
#define AHT10_CMD_SOFT_RESET 0xBA
#define AHT10_CMD_SOFT_RESET_ARG 0x00
#define AHT10_CMD_SOFT_RESET_ARG2 0x00

typedef struct
{
    bool leitura_disponivel;
    uint32_t amostragem_temperatura;
    uint32_t amostragem_umidade;
    uint8_t endereco;
    i2c_inst_t *i2c;
    uint8_t sda;
    uint8_t scl;
} aht10_t;

float obterTemperaturaCelsius(aht10_t *aht10);
float obterUmidadeRelativa(aht10_t *aht10);
void enviarComandos(aht10_t *aht10, const uint8_t *comandos, size_t tamanho);
void iniciarAHT10(aht10_t *aht10, i2c_inst_t *i2c, uint8_t sda, uint8_t scl, uint8_t endereco);
void calibrarAht10(aht10_t *aht10);
void enviarComandoMedicaoAht10(aht10_t *aht10);
void lerDadosAht10(aht10_t *aht10, uint8_t *buf);
void processarDadosAht10(aht10_t *aht10, uint8_t *buf);
bool disponivel(aht10_t *aht10);
void resetarAht10(aht10_t *aht10);
