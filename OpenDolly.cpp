#include "Sistema.h"
#include "auxilarLoop.h"
#include "auxiliarExibicao.h"
#include "auxiliarJoystick.h"
#include "auxiliarMemoria.h"
#include "auxiliarConversore.h"
#include "auxiliarControleMotor.h"
#include "auxiliarBuzzer.h"

int main()
{

  stdio_init_all();

  Oled tela(i2c1, PINO_SDA_TELA, PINO_SCL_TELA);
  tela.definirFonte(fonte_customizada);
  tela.inicializar();
  tela.limpar();

  PWM buzzer(PINO_BUZZER, WRAP_BUZZER, DIVISOR_CLOCK_BUZZER);
  buzzer.iniciar(DUTY_INICIAL_BUZZER);

  Acionador botaoJoystick(PINO_BOTAO_JOYSTICK);
  Acionador botaoA(PINO_BOTAO_A);
  Acionador botaoB(PINO_BOTAO_B);

  Motor motorX(PINO_STEP_MOTOR_X, PINO_DIR_MOTOR_X, VELOCIDADE_MOTOR_X);
  Motor motorY(PINO_STEP_MOTOR_Y, PINO_DIR_MOTOR_Y, VELOCIDADE_MOTOR_Y);

  Analogico joystick_X(PINO_JOYSTICK_X, CANAL_JOYSTICK_X, VALOR_MIN_SAIDA_X, VALOR_MAX_SAIDA_X);
  Analogico joystick_Y(PINO_JOYSTICK_Y, CANAL_JOYSTICK_Y, VALOR_MIN_SAIDA_Y, VALOR_MAX_SAIDA_Y);

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
      memorias, memoriasGravadas, indiceMemoriaSelecionada,
      ultimaPosicaoX, ultimoAnguloY,
      buzzer, pegarTempoAbsolutoAtual(), false};

  repeating_timer timer;

  acionarBuzzer(sistema);
  inicializarContadorDeTempoDoBuzzer(sistema, timer);

  exibirAnimacaoDeInicio(sistema);
  exibirInstrucoes(sistema);
  loopZerarPosicaoMotorX(sistema);
  loopZerarAnguloMotorY(sistema);

  while (true)
  {
    loopModoLivre(sistema);
    loopModoMemoria(sistema);
  }

  return 0;
}