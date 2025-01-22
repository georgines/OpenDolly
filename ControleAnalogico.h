#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

class ControleAnalogico {
  private:
    uint8_t pino;
    uint8_t canal;
    float entradaMin, entradaMax, saidaMin, saidaMax;
    float ultimoValor;

    float mapearFloat(float valor, float entrada_min, float entrada_max, float saida_min, float saida_max);
    float lerValorAtual();

  public:
    ControleAnalogico(uint8_t pino_adc, uint8_t canal_adc, float saida_min, float saida_max);
    bool houveAlteracao();
    float lerAlteracao();
};
