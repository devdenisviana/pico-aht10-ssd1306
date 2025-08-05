#include "AHT10.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

float obterTemperaturaCelsius(aht10_t *aht10)
{
    return ((aht10->amostragem_temperatura * 200.0f) / (1 << 20)) - 50.0f;
}

float obterUmidadeRelativa(aht10_t *aht10)
{
    return (aht10->amostragem_umidade * 100.0f) / (1 << 20);
}

void enviarComandoMedicaoAht10(aht10_t *aht10)
{
    uint8_t cmd[3] = {
        AHT10_CMD_MEASURE,
        AHT10_CMD_MEASURE_ARG,
        AHT10_CMD_MEASURE_ARG2};
    enviarComandos(aht10, cmd, sizeof(cmd));
}

void lerDadosAht10(aht10_t *aht10, uint8_t *buf)
{
    i2c_read_blocking(aht10->i2c, aht10->endereco, buf, 6, false);
}

void processarDadosAht10(aht10_t *aht10, uint8_t *buf)
{
    aht10->amostragem_umidade = ((uint32_t)buf[1] << 12) | ((uint32_t)buf[2] << 4) | ((buf[3] >> 4) & 0x0F);
    aht10->amostragem_temperatura = (((uint32_t)(buf[3] & 0x0F)) << 16) | ((uint32_t)buf[4] << 8) | buf[5];
}

bool disponivel(aht10_t *aht10)
{
    aht10->leitura_disponivel = false;

    enviarComandoMedicaoAht10(aht10);
    sleep_ms(80);
    uint8_t buf[6];
    lerDadosAht10(aht10, buf);
    processarDadosAht10(aht10, buf);
    aht10->leitura_disponivel = true;
    return aht10->leitura_disponivel;
}

void calibrarAht10(aht10_t *aht10)
{
    uint8_t cmd[3] = {
        AHT10_CMD_CALIBRATION,
        AHT10_CMD_CALIBRATION_ARG,
        AHT10_CMD_CALIBRATION_ARG2};

    enviarComandos(aht10, cmd, sizeof(cmd));
}

void resetarAht10(aht10_t *aht10)
{
    uint8_t cmd[3] = {
        AHT10_CMD_SOFT_RESET,
        AHT10_CMD_SOFT_RESET_ARG,
        AHT10_CMD_SOFT_RESET_ARG2};
    enviarComandos(aht10, cmd, sizeof(cmd));   
    sleep_ms(20);
}

void enviarComandos(aht10_t *aht10, const uint8_t *comandos, size_t tamanho)
{
    i2c_write_blocking(aht10->i2c, aht10->endereco, comandos, tamanho, false);
}

void iniciarAHT10(aht10_t *aht10, i2c_inst_t *i2c, uint8_t sda, uint8_t scl, uint8_t endereco)
{
    i2c_init(i2c, 100000);
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);

    aht10->i2c = i2c;
    aht10->sda = sda;
    aht10->scl = scl;
    aht10->endereco = endereco;
    aht10->leitura_disponivel = false;
    aht10->amostragem_temperatura = 0;
    aht10->amostragem_umidade = 0;
    resetarAht10(aht10);
    sleep_ms(20);
    calibrarAht10(aht10);
}