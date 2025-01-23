#include "Sistema.h"
#include "auxilarLoop.h"
#include "auxiliarExibicao.h"
#include "auxiliarJoystick.h"
#include "auxiliarMemoria.h"
#include "auxiliarConversore.h"
#include "auxiliarControleMotor.h"
#include "PWM.h"


int main()
{

  stdio_init_all();

  Oled tela(i2c1, PINO_SDA, PINO_SCL);
  tela.definirFonte(fonte_customizada);
  tela.inicializar();
  tela.limpar();

  Acionador botaoJoystick(PINO_JOYSTICK_SW);
  Acionador botaoA(PINO_A);
  Acionador botaoB(PINO_B);

  Motor motorX(PINO_STEP_MOTOR_X, PINO_DIR_MOTOR_X, VELOCIDADE_MOTOR_X);
  Motor motorY(PINO_STEP_MOTOR_Y, PINO_DIR_MOTOR_Y, VELOCIDADE_MOTOR_Y);

  Analogico joystick_X(PINO_JOYSTICK_X, CANAL_JOYSTICK_X, VALOR_MIN_SAIDA, VALOR_MAX_SAIDA);
  Analogico joystick_Y(PINO_JOYSTICK_Y, CANAL_JOYSTICK_Y, VALOR_MIN_SAIDA, VALOR_MAX_SAIDA);

  motorX.definirPassoAtual(calcularPassosParaDeslocamento(DESLOCAMENTO_ATUAL_MOTOR_X));
  motorY.definirPassoAtual(calcularPassosParaInclinacao(ANGULACAO_ATUAL_MOTOR_Y));

  PosicaoMotor memorias[MEMORIA_MAXIMA] = {};
  bool memoriasGravadas[MEMORIA_MAXIMA] = {false, false, false};
  uint8_t indiceMemoriaSelecionada = 0;
  int32_t ultimaPosicaoX = 0;
  int32_t ultimoAnguloY = 0;

  Sistema sistema = {
      motorX, motorY,
      joystick_X, joystick_Y,
      tela,
      botaoA, botaoB, botaoJoystick,
      memorias,
      memoriasGravadas,
      indiceMemoriaSelecionada,
      ultimaPosicaoX,
      ultimoAnguloY};

  //exibirAnimacaoDeInicio(sistema);
  //exibirInstrucoes(sistema);
  loopZerarPosicaoMotorX(sistema);
  loopZerarPosicaoMotorY(sistema);

  while (true)
  {
    loopModoLivre(sistema);
    loopModoMemoria(sistema);
  }

  return 0;
}