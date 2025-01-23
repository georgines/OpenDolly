#pragma once
#include "Sistema.h"
#include "auxiliarExibicao.h"
#include "auxiliarControleMotor.h"
#include "auxiliarConversore.h"

// Funções de controle de memória
bool selecionarMenorIndiceMemoriaValida(Sistema &sistema)
{
  for (int i = 0; i < MEMORIA_MAXIMA; i++)
  {
    if (sistema.memoriasGravadas[i])
    {
      sistema.indiceMemoriaSelecionada = i;
      return true;
    }
  }
  return false;
}

void selecionarMemoriaModoLivre(Sistema &sistema)
{
  sistema.indiceMemoriaSelecionada++;
  if (sistema.indiceMemoriaSelecionada >= MEMORIA_MAXIMA)
  {
    sistema.indiceMemoriaSelecionada = 0;
  }
  atualizarExibicaoMemoria(sistema);
}

void selecionarMemoriaModoMemoria(Sistema &sistema)
{
  do
  {
    sistema.indiceMemoriaSelecionada++;
    if (sistema.indiceMemoriaSelecionada >= MEMORIA_MAXIMA)
    {
      sistema.indiceMemoriaSelecionada = 0;
    }
  } while (!sistema.memoriasGravadas[sistema.indiceMemoriaSelecionada]);

  atualizarExibicaoMemoria(sistema);
}

void gravarMemoriaAtual(Sistema &sistema)
{
  sistema.memorias[sistema.indiceMemoriaSelecionada].posicaoX = sistema.ultimaPosicaoX;
  sistema.memorias[sistema.indiceMemoriaSelecionada].anguloY = sistema.ultimoAnguloY;
  sistema.memoriasGravadas[sistema.indiceMemoriaSelecionada] = true;
  exibirMensagemTemporariaNaMemoria(sistema, "Gravado!", 500);
}

void lerMemoriaAtual(Sistema &sistema)
{
  if (sistema.memoriasGravadas[sistema.indiceMemoriaSelecionada])
  {
    int32_t passo_motor_x = calcularPassosParaDeslocamento(sistema.memorias[sistema.indiceMemoriaSelecionada].posicaoX);
    int32_t passo_motor_y = calcularPassosParaInclinacao(sistema.memorias[sistema.indiceMemoriaSelecionada].anguloY);
    sistema.motorX.aplicarPassoDestino(passo_motor_x);
    sistema.motorY.aplicarPassoDestino(passo_motor_y);
    exibirMensagemTemporariaNaMemoria(sistema, "Movendo!", 500);
  }
}

void lerMemoriaApensSeFoiGravada(Sistema &sistema)
{
  bool memoria_atual_nao_gravada = !sistema.memoriasGravadas[sistema.indiceMemoriaSelecionada];
  if (memoria_atual_nao_gravada)
    return;
  lerMemoriaAtual(sistema);
}