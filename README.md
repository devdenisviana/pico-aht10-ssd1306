# Projeto AHT10 com Raspberry Pi Pico

Este projeto demonstra a leitura de temperatura e umidade usando o sensor AHT10 conectado a um Raspberry Pi Pico. Os dados são exibidos em um display OLED SSD1306 128x64 via I²C usando a biblioteca U8G2Pico e também enviados por UART para um terminal.

## Recursos

- Leitura de temperatura em graus Celsius (AHT10)
- Leitura de umidade relativa em porcentagem (AHT10)
- Visualização em OLED 128x64 (SSD1306) via U8G2Pico
- Alertas de alta umidade (>70%) e baixa temperatura (<20°C)
- Saída de dados via UART

## Estrutura do Projeto

```
AHT10/
├── AHT10.h            # Definições e protótipos de funções do sensor
├── AHT10.cpp          # Implementação das funções de comunicação com o sensor
├── main.cpp           # Código principal: inicializa sensor, display e loop de leitura
├── CMakeLists.txt     # Configuração do build com CMake e Pico SDK
├── lib/u8g2pico/      # Biblioteca U8G2Pico para displays SSD1306
├── pico_sdk_import.cmake
└── build/             # Pasta gerada pelo CMake (artefatos de compilação)
```

## Requisitos

- Raspberry Pi Pico ou Pico W
- Sensor AHT10 (I²C)
- Display OLED SSD1306 128x64 (I²C)
- Cabos para conexão I²C e alimentação
- Versão 2.1.1 do Pico SDK
- CMake >= 3.13
- Toolchain GCC ARM (incluído no SDK)
- VS Code (opcional) com extensão Raspberry Pi Pico

## Conexões de Hardware

| Dispositivo         | Pico GPIO | Função  |
|---------------------|-----------:|---------|
| AHT10 VCC           | 3V3        | Alimentação  |
| AHT10 GND           | GND        | Terra        |
| AHT10 SDA           | GP0        | SDA (I²C0)   |
| AHT10 SCL           | GP1        | SCL (I²C0)   |
| SSD1306 VCC         | 3V3        | Alimentação  |
| SSD1306 GND         | GND        | Terra        |
| SSD1306 SDA         | GP14       | SDA (I²C1)   |
| SSD1306 SCL         | GP15       | SCL (I²C1)   |
| UART TX (opcional)  | GP8        | UART1 TX     |
| UART RX (opcional)  | GP9        | UART1 RX     |

## Instalação e Build

```bash
# Clone o repositório (ou copie os arquivos)
cd AHT10

# Crie pasta de build e entre nela
mkdir build && cd build

# Configure o projeto com CMake
cmake ..

# Compile o firmware
cmake --build .
```

### Pelo VS Code
1. Abra a pasta `AHT10` no VS Code.
2. Garanta que o Pico SDK esteja configurado (`pico_sdk_import.cmake`).
3. Utilize a task `Compile Project` (Build) e `Run Project` ou `Flash` para gravar no microcontrolador.

## Uso

1. Conecte o Pico ao PC via USB.
2. Compile e grave o firmware.
3. Abra um terminal serial (115200 baud) para visualizar os dados.
4. O display OLED mostrará temperatura e umidade, com alertas visuais.

## Funções Principais (AHT10)

- `iniciarAHT10(aht10_t*, i2c_inst_t*, uint8_t sda, uint8_t scl, uint8_t endereco)` – Inicializa I²C e configurações.
- `calibrarAht10(aht10_t*)` – Envia sequência de calibração.
- `enviarComandoMedicaoAht10(aht10_t*)` – Inicia uma medição.
- `lerDadosAht10(aht10_t*, uint8_t* buf)` – Lê bytes brutos do sensor.
- `processarDadosAht10(aht10_t*, uint8_t* buf)` – Extrai temperatura e umidade.
- `obterTemperaturaCelsius(aht10_t*)` – Retorna temperatura em °C.
- `obterUmidadeRelativa(aht10_t*)` – Retorna umidade relativa em %.
- `disponivel(aht10_t*)` – Verifica se a nova leitura está pronta.
- `resetarAht10(aht10_t*)` – Soft reset do sensor.

## Bibliotecas Utilizadas

- [U8G2Pico](https://github.com/georgines/u8g2picohttps://github.com/georgines/u8g2pico) – Wrapper U8G2 para Pico (SSD1306)

## Licença

Este projeto está licenciado sob a licença MIT. Veja o arquivo LICENSE para mais detalhes.

---
Desenvolvido por Georgines Bezerra Pereira © 2025
