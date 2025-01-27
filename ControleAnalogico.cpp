#include "ControleAnalogico.h"

float ControleAnalogico::mapearFloat(float valor, float entrada_min, float entrada_max, float saida_min, float saida_max) {
    return (valor - entrada_min) * (saida_max - saida_min) / (entrada_max - entrada_min) + saida_min;
}

float ControleAnalogico::lerValorAtual() {
    adc_select_input(this->canal);
    uint16_t valorAdc = adc_read();
    return this->mapearFloat(valorAdc, this->entradaMin, this->entradaMax, this->saidaMin, this->saidaMax);
}

float ControleAnalogico::aplicarFiltro(float novoValor) {
    // Adiciona o novo valor ao buffer
    this->bufferLeituras.push_back(novoValor);

    // Remove o valor mais antigo se o buffer exceder o tamanho definido
    if (this->bufferLeituras.size() > this->tamanhoFiltro) {
        this->bufferLeituras.erase(this->bufferLeituras.begin());
    }

    // Calcula a média dos valores no buffer
    float soma = 0.0f;
    for (float valor : this->bufferLeituras) {
        soma += valor;
    }

    return soma / this->bufferLeituras.size();
}

ControleAnalogico::ControleAnalogico(uint8_t pino_adc, uint8_t canal_adc, float saida_min, float saida_max, size_t tamanho_filtro, float limiar_tolerancia)
    : pino(pino_adc), canal(canal_adc), entradaMin(0.0f), entradaMax(4095.0f), saidaMin(saida_min), saidaMax(saida_max), ultimoValor(0.0f), tamanhoFiltro(tamanho_filtro), limiar(limiar_tolerancia) {
    
    adc_gpio_init(this->pino);
    adc_select_input(this->canal);
    adc_init();
    bufferLeituras.reserve(this->tamanhoFiltro);
}

bool ControleAnalogico::houveAlteracao() {
    float valorAtual = this->lerValorAtual();
    //float valorAtual = this->aplicarFiltro(this->lerValorAtual());

    // Verifica se a mudança ultrapassa o limiar
    if (abs(valorAtual - this->ultimoValor) > this->limiar) {
        this->ultimoValor = valorAtual;
        return true;
    }

    return false;
}

float ControleAnalogico::lerAlteracao() {
    return this->ultimoValor;
}
