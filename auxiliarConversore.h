#pragma once
#include "Sistema.h"

//conversores de posicao angulo e passo
int32_t calcularPassosParaDeslocamento(int32_t distancia)
{
  if (distancia == 0)
    return 0;
  return static_cast<int32_t>(distancia / INCREMENTO_POR_PASSO_X);
}

int32_t calcularPassosParaInclinacao(int32_t angulo)
{
  return static_cast<int32_t>(angulo * PASSOS_POR_GRAU_Y);
}

int32_t calcularPosicaoAtual(int32_t passo_atual)
{
  if (passo_atual == 0)
    return 0;
  return static_cast<int32_t>(passo_atual * INCREMENTO_POR_PASSO_X);
}

int32_t calcularAnguloAtual(int32_t passo_atual)
{
  if (passo_atual == 0)
    return 0;
  return static_cast<int32_t>(passo_atual / PASSOS_POR_GRAU_Y);
}