#include "ControleMotorVelocidadeFixa.h"

ControleMotorVelocidadeFixa::ControleMotorVelocidadeFixa(uint8_t pinoStep, uint8_t pinoDirecao, float velocidade)
  : pinoStep(pinoStep), pinoDirecao(pinoDirecao), velocidade(velocidade), posicaoAtual(0), posicaoAlvo(0), emMovimento(false) {
  this->intervaloEntrePassos = 1000000.0 / velocidade;
  gpio_init(this->pinoStep);
  gpio_set_dir(this->pinoStep, GPIO_OUT);
  gpio_init(this->pinoDirecao);
  gpio_set_dir(this->pinoDirecao, GPIO_OUT);
}

void ControleMotorVelocidadeFixa::configurarDestino(int32_t posicaoAlvo) {
  if (this->posicaoAlvo != posicaoAlvo) {
    this->posicaoAlvo = posicaoAlvo;
    this->emMovimento = true;
  }
}

void ControleMotorVelocidadeFixa::atualizar() {
  if (!this->emMovimento) {
    return;
  }

  uint64_t tempoAtual = time_us_64();
  if (tempoAtual - this->ultimoTempoPasso >= static_cast<uint64_t>(this->intervaloEntrePassos)) {
    this->realizarPasso();
    this->ultimoTempoPasso = tempoAtual;

    if (this->posicaoAtual == this->posicaoAlvo) {
      this->emMovimento = false;
    }
  }

}

int32_t ControleMotorVelocidadeFixa::obterPosicaoAtual() {
  return this->posicaoAtual;
}

void ControleMotorVelocidadeFixa::setarPosicaoAtual(int32_t posicao) {
  this->posicaoAtual = posicao;
}


bool ControleMotorVelocidadeFixa::motorEmMovimento() {
  return this->emMovimento;
}

void ControleMotorVelocidadeFixa::realizarPasso() {
  if (this->posicaoAtual < this->posicaoAlvo) {
    gpio_put(this->pinoDirecao, true);
    this->posicaoAtual++;
  } else if (this->posicaoAtual >= this->posicaoAlvo) {
    gpio_put(this->pinoDirecao, false);
    this->posicaoAtual--;
  }

  gpio_put(this->pinoStep, true);
  sleep_us(20); // Pulso mínimo para o STEP
  gpio_put(this->pinoStep, false);
}
