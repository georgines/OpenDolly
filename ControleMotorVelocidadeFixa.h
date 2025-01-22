#pragma once

#include "pico/stdlib.h"
#include <stdio.h>

class ControleMotorVelocidadeFixa {
  public:
    ControleMotorVelocidadeFixa(uint8_t pinoStep, uint8_t pinoDirecao, float velocidade);

    void configurarDestino(int32_t posicaoAlvo);
    void atualizar();
    int32_t obterPosicaoAtual();
    void setarPosicaoAtual(int32_t posicao);
    bool motorEmMovimento();

  private:
    void realizarPasso();

    uint8_t pinoStep;
    uint8_t pinoDirecao;

    float velocidade; // Velocidade fixa em passos por segundo
    float intervaloEntrePassos;

    int32_t posicaoAtual;
    int32_t posicaoAlvo;

    uint64_t ultimoTempoPasso;
    bool emMovimento;
};

