#pragma once
#include "Sistema.h"
#include "auxiliarExibicao.h"

// Funções de controle de motores
void moverMotorLivre(Motor *motor, float *valor, uint32_t passo_min, uint32_t passo_maximo, const char eixo)
{

  // printf("Valor eixo %c: %f\r\n", eixo, *valor);
  if (*valor > 0.8f)
  {
    motor->aplicarPassoDestino(passo_maximo);
  }
  if (*valor < -0.8f)
  {
    motor->aplicarPassoDestino(passo_min);
  }
  if (*valor > -0.5f && *valor < 0.5f)
  {
    motor->aplicarPassoDestino(motor->obterPassoAtual());
  }
  return;
}

bool houveAlteracaoPosicaoMotores(Sistema &sistema)
{
  return sistema.ultimaPosicaoX != calcularPosicaoAtual(sistema.motorX.obterPassoAtual()) ||
         sistema.ultimoAnguloY != calcularAnguloAtual(sistema.motorY.obterPassoAtual());
}

void monitorarMotoresExibirCordenadas(Sistema &sistema)
{
  if (houveAlteracaoPosicaoMotores(sistema))
  {
    sistema.ultimaPosicaoX = calcularPosicaoAtual(sistema.motorX.obterPassoAtual());
    sistema.ultimoAnguloY = calcularAnguloAtual(sistema.motorY.obterPassoAtual());

    int32_t passo_x = calcularPassosParaDeslocamento(sistema.ultimaPosicaoX);
    int32_t passo_y = calcularPassosParaInclinacao(sistema.ultimoAnguloY);

    // if (!sistema.motorX.motorEmMovimento() && !sistema.motorY.motorEmMovimento()) {
    atualizarExibicaoCodenadas(sistema, sistema.ultimaPosicaoX, sistema.ultimoAnguloY);
    //}
    printf("Posicao atual motor X: %d\tPasso: %d\r\n", sistema.ultimaPosicaoX, passo_x);
    printf("Angulo atual motor Y: %d\tPasso: %d\r\n", sistema.ultimoAnguloY, passo_y);
  }
}

void atualizarPosicoesMotores(Sistema &sistema)
{
  sistema.motorX.atualizar();
  sistema.motorY.atualizar();

  monitorarMotoresExibirCordenadas(sistema);
}

void moverMotorSentidoHorario(Motor *motor)
{
  motor->definirPassoAtual(0);
  motor->aplicarPassoDestino(100000);
}

void moverMotorSentidoAntihorario(Motor *motor)
{
  motor->definirPassoAtual(0);
  motor->aplicarPassoDestino(-100000);
}
