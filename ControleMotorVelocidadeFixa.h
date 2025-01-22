#pragma once

#include "pico/stdlib.h"
#include <stdio.h>

class ControleMotorVelocidadeFixa {
  public:
    ControleMotorVelocidadeFixa(uint8_t pinoStep, uint8_t pinoDirecao, float velocidade);

    void aplicarPassoDestino(int32_t passoAlvo);
    void atualizar();
    int32_t obterPassoAtual();
    void definirPassoAtual(int32_t passo);
    bool motorEmMovimento();

  private:
    void realizarPasso();

    uint8_t pinoStep;
    uint8_t pinoDirecao;

    float velocidade; // Velocidade fixa em passos por segundo
    float intervaloEntrePassos;

    int32_t passoAtual;
    int32_t passoAlvo;

    uint64_t ultimoTempoPasso;
    bool emMovimento;
};

