#pragma once

#include <stdio.h>
#include <math.h>
#include <vector>
#include "pico/stdlib.h"
#include "hardware/adc.h"

class ControleAnalogico {
  private:
    uint8_t pino;
    uint8_t canal;
    float entradaMin, entradaMax, saidaMin, saidaMax;
    float ultimoValor;
    std::vector<float> bufferLeituras;
    size_t tamanhoFiltro;
    float limiar; // Limiar de tolerância para mudanças

    float mapearFloat(float valor, float entrada_min, float entrada_max, float saida_min, float saida_max);

  public:
    float lerValorAtual();
    ControleAnalogico(uint8_t pino_adc, uint8_t canal_adc, float saida_min, float saida_max, size_t tamanho_filtro = 5, float limiar_tolerancia = 0.5f);
    bool houveAlteracao();
    float lerAlteracao();
    float aplicarFiltro(float novoValor);
};