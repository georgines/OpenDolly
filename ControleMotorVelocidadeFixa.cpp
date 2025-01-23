#include "ControleMotorVelocidadeFixa.h"

ControleMotorVelocidadeFixa::ControleMotorVelocidadeFixa(uint8_t pinoStep, uint8_t pinoDirecao, float velocidade)
    : pinoStep(pinoStep), pinoDirecao(pinoDirecao), velocidade(velocidade), passoAtual(0), passoAlvo(0), emMovimento(false)
{
  this->intervaloEntrePassos = 1000000.0 / velocidade;
  gpio_init(this->pinoStep);
  gpio_set_dir(this->pinoStep, GPIO_OUT);
  gpio_init(this->pinoDirecao);
  gpio_set_dir(this->pinoDirecao, GPIO_OUT);
}

void ControleMotorVelocidadeFixa::aplicarPassoDestino(int32_t passoAlvo)
{
  if (this->passoAlvo != passoAlvo)
  {
    this->passoAlvo = passoAlvo;
    this->emMovimento = true;
  }
}

void ControleMotorVelocidadeFixa::atualizar()
{
  if (!this->emMovimento)
  {
    return;
  }

  uint64_t tempoAtual = time_us_64();
  if (tempoAtual - this->ultimoTempoPasso >= static_cast<uint64_t>(this->intervaloEntrePassos))
  {
    this->realizarPasso();
    this->ultimoTempoPasso = tempoAtual;

    if (this->passoAtual == this->passoAlvo)
    {
      this->emMovimento = false;
    }
  }
}

int32_t ControleMotorVelocidadeFixa::obterPassoAtual()
{
  return this->passoAtual;
}

void ControleMotorVelocidadeFixa::definirPassoAtual(int32_t passo)
{
  this->passoAtual = passo;
}

bool ControleMotorVelocidadeFixa::motorEmMovimento()
{
  return this->emMovimento;
}

void ControleMotorVelocidadeFixa::realizarPasso()
{
  if (this->passoAtual < this->passoAlvo)
  {
    gpio_put(this->pinoDirecao, true);
    this->passoAtual++;
  }
  else if (this->passoAtual >= this->passoAlvo)
  {
    gpio_put(this->pinoDirecao, false);
    this->passoAtual--;
  }

  gpio_put(this->pinoStep, true);
  sleep_us(20);
  gpio_put(this->pinoStep, false);
}
