#pragma once
#include "Sistema.h"

// Funções de loop
void loopZerarPosicaoMotorX(Sistema &sistema)
{
  exibirModoComCoordenadas(sistema, MODO_ZERAR_X);
  moverMotorSentidoAntihorario(&sistema.motorX);
  while (true)
  {
    if (sistema.botaoA.verificarAcionamento())
    {
      acionarBuzzer(sistema);
      sistema.motorX.definirPassoAtual(calcularPassosParaDeslocamento(DESLOCAMENTO_MIN_MOTOR_X));
      break;
    }
    sistema.motorX.atualizar();

    monitorarMotoresExibirCordenadas(sistema);
    sleep_ms(10);
  }
}

void loopZerarAnguloMotorY(Sistema &sistema)
{
  exibirModoComCoordenadas(sistema, MODO_ZERAR_Y);
  moverMotorSentidoAntihorario(&sistema.motorY);

  while (true)
  {
    bool botao_b_pressionado = sistema.botaoB.verificarAcionamento();
    if (botao_b_pressionado)
    {
      acionarBuzzer(sistema);
      sistema.motorY.definirPassoAtual(calcularPassosParaInclinacao(ANGULACAO_MIN_MOTOR_Y));
      break;
    }
    sistema.motorY.atualizar();
    monitorarMotoresExibirCordenadas(sistema);
    sleep_ms(10);
  }

  sistema.motorY.aplicarPassoDestino(calcularPassosParaInclinacao(ANGULACAO_ATUAL_MOTOR_Y));
  while (true)
  {
    bool motorParado = !sistema.motorY.motorEmMovimento();
    if (motorParado)
    {
      break;
    }
    sistema.motorY.atualizar();
    monitorarMotoresExibirCordenadas(sistema);
    sleep_ms(10);
  }
}

void loopModoLivre(Sistema &sistema)
{

  exibirModoComCoordenadas(sistema, MODO_LIVRE, sistema.ultimaPosicaoX, sistema.ultimoAnguloY);

  sistema.motorX.aplicarPassoDestino(sistema.motorX.obterPassoAtual());
  sistema.motorY.aplicarPassoDestino(sistema.motorY.obterPassoAtual());

  while (true)
  {

    atualizarPosicoesMotores(sistema);
    controlarMotoresPeloJoystick(sistema);

   //printf("valor: %d\r\n", (int32_t)sistema.joystick_X.lerValorAtual());

    bool botao_a_pressionado = sistema.botaoA.verificarAcionamento();
    bool botao_b_pressionado = sistema.botaoB.verificarAcionamento();
    bool botao_joystick_pressionado = sistema.botaoJoystick.verificarAcionamento();

    if (botao_a_pressionado)
    {
      acionarBuzzer(sistema);
      selecionarMemoriaModoLivre(sistema);
    }

    if (botao_b_pressionado)
    {
      acionarBuzzer(sistema);
      gravarMemoriaAtual(sistema);
    }

    if (botao_joystick_pressionado)
    {
      acionarBuzzer(sistema);
      break;
    }

    sleep_ms(10);
  }
}

void loopModoMemoria(Sistema &sistema)
{
  bool nenhuma_mamoria_foi_gravada = !selecionarMenorIndiceMemoriaValida(sistema);
  if (nenhuma_mamoria_foi_gravada)
  {
    exibirMensagemTemporaria(sistema, "Nenhuma memoria gravada", 1000);
    return;
  }

  exibirModoComCoordenadas(sistema, MODO_MEMORIA, sistema.ultimaPosicaoX, sistema.ultimoAnguloY);

  while (true)
  {

    atualizarPosicoesMotores(sistema);

    bool botao_a_pressionado = sistema.botaoA.verificarAcionamento();
    bool botao_b_pressionado = sistema.botaoB.verificarAcionamento();
    bool botao_joystick_pressionado = sistema.botaoJoystick.verificarAcionamento();

    if (botao_a_pressionado)
    {
      acionarBuzzer(sistema);
      selecionarMemoriaModoMemoria(sistema);
    }

    if (botao_b_pressionado)
    {
      acionarBuzzer(sistema);
      lerMemoriaApensSeFoiGravada(sistema);
    }

    if (botao_joystick_pressionado)
    {
      acionarBuzzer(sistema);
      break;
    }

    sleep_ms(10);
  }
}