#include "ControleAnalogico.h"

float ControleAnalogico::mapearFloat(float valor, float entrada_min, float entrada_max, float saida_min, float saida_max) {
    return (valor - entrada_min) * (saida_max - saida_min) / (entrada_max - entrada_min) + saida_min;
}

float ControleAnalogico::lerValorAtual() {
    adc_select_input(this->canal);
    uint16_t valorAdc = adc_read();
    return this->mapearFloat(valorAdc, this->entradaMin, this->entradaMax, this->saidaMin, this->saidaMax);
}

ControleAnalogico::ControleAnalogico(uint8_t pino_adc, uint8_t canal_adc, float saida_min, float saida_max)
    : pino(pino_adc), canal(canal_adc), entradaMin(0.0f), entradaMax(4095.0f), saidaMin(saida_min), saidaMax(saida_max), ultimoValor(0.0f) {
    
    adc_gpio_init(this->pino);
    adc_select_input(this->canal);
    adc_init();
}

bool ControleAnalogico::houveAlteracao() {
    float valorAtual = this->lerValorAtual();

    if (valorAtual != this->ultimoValor) {
        this->ultimoValor = valorAtual;
        return true;
    }

    return false;
}

float ControleAnalogico::lerAlteracao() {
    return this->lerValorAtual();
}
