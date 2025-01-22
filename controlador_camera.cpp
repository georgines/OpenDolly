#include "Sistema.h"
/*#include <stdio.h>
  #include "pico/stdlib.h"
  #include "Oled.h"
  #include "Acionador.h"
  #include "hardware/gpio.h"
  #include "ControleMotorVelocidadeFixa.h"
  #include "ControleAnalogico.h"

  typedef ControleMotorVelocidadeFixa Motor;
  typedef ControleAnalogico Analogico;

  //botao A e B
  #define PINO_A 5
  #define PINO_B 6
  //pinos tela
  #define PINO_SDA 14
  #define PINO_SCL 15
  //joystick
  #define PINO_JOYSTICK_X 27
  #define CANAL_JOYSTICK_X 0 // ADC0
  #define PINO_JOYSTICK_Y 26
  #define CANAL_JOYSTICK_Y 1 // ADC1
  #define PINO_JOYSTICK_SW 22
  #define VALOR_MIN_SAIDA -1
  #define VALOR_MAX_SAIDA 1
  //driver motor
  #define PINO_STEP_MOTOR_X 2
  #define PINO_DIR_MOTOR_X 3
  #define VELOCIDADE_MOTOR_X 500.0f
  #define POSICAO_MIN_MOTOR_X 0
  #define POSICAO_ATUAL_MOTOR_X 200
  #define POSICAO_MAX_MOTOR_X 400

  #define PINO_STEP_MOTOR_Y 4
  #define PINO_DIR_MOTOR_Y 8
  #define VELOCIDADE_MOTOR_Y 500.0f
  #define POSICAO_MIN_MOTOR_Y 0
  #define POSICAO_ATUAL_MOTOR_Y 200
  #define POSICAO_MAX_MOTOR_Y 400

  #define POSICAO_MAXIMA 400
  #define POSICAO_MINIMA 0
  //memoria
  #define MEMORIA_MAXIMA 3
  //menu
  #define MODO_LIVRE 0
  #define MODO_MEMORIA 1
  //linhas menu
  #define LINHA_TITULO 0
  #define LINHA_POSX 20
  #define LINHA_POSY 32
  #define LINHA_MEMORIA 44
  #define LINHA_MENU 56

  struct PosicaoMotor {
  int motorX;
  int motorY;
  };

  struct Sistema {
  Motor &MotorX;
  Motor &MotorY;
  Analogico &joystick_X;
  Analogico &joystick_Y;
  Oled &tela;
  Acionador &botaoA;
  Acionador &botaoB;
  Acionador &botaoJoystick;
  PosicaoMotor *memorias;
  bool *memoriasGravadas;
  uint8_t &memoriaSelecionada;
  int32_t &ultimaPosicaoX;
  int32_t &ultimaPosicaoY;
  };*/

// Funções de exibição
void atualizarExibicaoMemoria(Sistema &sistema, const char *mensagem ) {
  if (mensagem) {
    sistema.tela.imprimir(0, LINHA_MEMORIA, "Mem: %d %s", sistema.memoriaSelecionada + 1, mensagem);
  } else {
    sistema.tela.limparAreaLinha(48, LINHA_MEMORIA);
    sistema.tela.imprimir(0, LINHA_MEMORIA, "Mem: %d", sistema.memoriaSelecionada + 1);
  }
  sistema.tela.renderizarDisplay();
}

void exibirMensagemTemporariaNaMemoria(Sistema &sistema, const char *mensagem, uint32_t duracao_ms) {
  atualizarExibicaoMemoria(sistema, mensagem);
  sleep_ms(duracao_ms);
  atualizarExibicaoMemoria(sistema);
}

void atualizarExibicaoCodenadas(Sistema &sistema, int x, int y) {
  sistema.tela.limparAreaLinha(56, LINHA_POSX);
  sistema.tela.limparAreaLinha(56, LINHA_POSY);
  sistema.tela.imprimir(0, LINHA_POSX, "Des X: %d", x);
  sistema.tela.imprimir(0, LINHA_POSY, "Ang Y: %d",  y);
  sistema.tela.renderizarDisplay();
}

void exibirCoordenadas(Sistema &sistema, uint8_t modo, int x, int y) {
  const char *modos[2][3] = {
    {"   Modo Livre   ", "A:Selec B:Grav"},
    {"  Modo Memoria  ", "A:Selec B:Exec"}
  };
  sistema.tela.limpar();
  sistema.tela.imprimir(0, LINHA_TITULO, "%s", modos[modo][0]);
  atualizarExibicaoCodenadas(sistema, x, y);
  sistema.tela.imprimir(0, LINHA_MENU, "%s", modos[modo][1]);
  atualizarExibicaoMemoria(sistema);
  sistema.tela.renderizarDisplay();
}

void exibirMensagemTemporaria(Sistema &sistema, const char *mensagem, uint32_t duracao_ms) {
  sistema.tela.limpar();
  sistema.tela.imprimir(0, 0, mensagem);
  sistema.tela.renderizarDisplay();
  sleep_ms(duracao_ms);
}

void exibirInstrucoes(Sistema &sistema) {
  const char *paginas[][4] = {
    {"Instrucoes P1", "Botao Joystick:", "- Se chama J", "- Alterna Modos"},
    {"Instrucoes P2", "Modo Live:", "- Mov. pelo Joy", ""},
    {"Instrucoes P3", "Modo Memoria:", "- Mov. pela MEM", ""},
    {"Instrucoes P4", "Botoes:", "A: Funcao A", "B: Funcao B"}
  };

  int total_paginas = sizeof(paginas) / sizeof(paginas[0]);
  int pagina_atual = 0;

  while (true) {
    sistema.tela.limpar();
    sistema.tela.imprimir(0, 0, paginas[pagina_atual][0]);  // Título
    sistema.tela.imprimir(0, 20, paginas[pagina_atual][1]); // Linha 1
    sistema.tela.imprimir(0, 32, paginas[pagina_atual][2]); // Linha 2
    sistema.tela.imprimir(0, 44, paginas[pagina_atual][3]); // Linha 3
    sistema.tela.imprimir(0, 56, "J: Proximo");
    sistema.tela.renderizarDisplay();

    while (!sistema.botaoJoystick.verificarAcionamento()) {
      sleep_ms(10);
    }

    pagina_atual++;
    if (pagina_atual >= total_paginas) {
      break;
    }
  }
}

void exibirAnimacaoDeInicio(Sistema &sistema) {
  const char *estados[] = {"Iniciando", "Iniciando.", "Iniciando..", "Iniciando..."};
  for (int i = 0; i < 12; i++) {
    sistema.tela.limpar();
    sistema.tela.imprimir(0, 0, estados[i % 4]);
    sistema.tela.renderizarDisplay();
    sleep_ms(300);
  }
}

void exibirModo(Sistema &sistema, const char *modo, const char *instrucaoA, const char *instrucaoB, bool exibir_tudo) {
  sistema.tela.limpar();
  sistema.tela.imprimir(0, 0, modo);
  sistema.tela.imprimir(0, 20, "A: %s", instrucaoA);
  sistema.tela.imprimir(0, 32, "B: %s", instrucaoB);
  if (exibir_tudo) {
    sistema.tela.imprimir(0, 56, "J: Alt. Modo");
    atualizarExibicaoMemoria(sistema);
    return;
  }
  sistema.tela.renderizarDisplay();
}

bool selecionarMenorIndiceMemoriaValida(Sistema &sistema) {
  for (int i = 0; i < MEMORIA_MAXIMA; i++) {
    if (sistema.memoriasGravadas[i]) {
      sistema.memoriaSelecionada = i;
      return true;
    }
  }
  return false;
}

// Funções de controle de memória
void selecionarMemoriaModoLivre(Sistema &sistema) {
  sistema.memoriaSelecionada++;
  if (sistema.memoriaSelecionada >= MEMORIA_MAXIMA) {
    sistema.memoriaSelecionada = 0;
  }
  atualizarExibicaoMemoria(sistema);
}

void selecionarMemoriaModoMemoria(Sistema &sistema) {
  do {
    sistema.memoriaSelecionada++;
    if (sistema.memoriaSelecionada >= MEMORIA_MAXIMA) {
      sistema.memoriaSelecionada = 0;
    }
  } while (!sistema.memoriasGravadas[sistema.memoriaSelecionada]);

  atualizarExibicaoMemoria(sistema);
}

void gravarMemoriaAtual(Sistema &sistema) {
  sistema.memorias[sistema.memoriaSelecionada].motorX = sistema.ultimaPosicaoX;
  sistema.memorias[sistema.memoriaSelecionada].motorY = sistema.ultimaPosicaoY;
  sistema.memoriasGravadas[sistema.memoriaSelecionada] = true;
  exibirMensagemTemporariaNaMemoria(sistema, "Gravado!", 500);
}


void lerMemoriaAtual(Sistema &sistema) {
  if (sistema.memoriasGravadas[sistema.memoriaSelecionada]) {
    sistema.MotorX.configurarDestino(sistema.memorias[sistema.memoriaSelecionada].motorX);
    sistema.MotorY.configurarDestino(sistema.memorias[sistema.memoriaSelecionada].motorY);
    exibirMensagemTemporariaNaMemoria(sistema, "Movendo!", 500);
  }
}

// Funções de controle de motores
void zerarReferencia(Sistema &sistema, const char *eixo) {
  sistema.tela.limpar();
  sistema.tela.imprimir(0, 0, "Zerando %s...", eixo);
  sistema.tela.renderizarDisplay();
  sleep_ms(1000);
}

void moverMotorLivre(Motor *motor, float *valor, uint32_t posicao_min, uint32_t posicao_max, const char eixo) {

  //printf("Valor eixo %c: %f\r\n", eixo, *valor);
  if (*valor > 0.8f) {
    motor->configurarDestino(posicao_max);
  }
  if (*valor < -0.8f) {
    motor->configurarDestino(posicao_min);
  }
  if (*valor > -0.5f && *valor < 0.5f) {
    motor->configurarDestino(motor->obterPosicaoAtual());
  }
  return;
}

bool houveAlteracaoPosicaoMotores(Sistema &sistema) {
  return sistema.ultimaPosicaoX != sistema.MotorX.obterPosicaoAtual() ||
         sistema.ultimaPosicaoY != sistema.MotorY.obterPosicaoAtual();
}

void atualizarPosicoesMotores(Sistema &sistema) {
  sistema.MotorX.atualizar();
  sistema.MotorY.atualizar();

  if (houveAlteracaoPosicaoMotores(sistema)) {
    sistema.ultimaPosicaoX = sistema.MotorX.obterPosicaoAtual();
    sistema.ultimaPosicaoY = sistema.MotorY.obterPosicaoAtual();
    if (!sistema.MotorX.motorEmMovimento() && !sistema.MotorY.motorEmMovimento()) {
      atualizarExibicaoCodenadas(sistema, sistema.ultimaPosicaoX, sistema.ultimaPosicaoY);
    }
    printf("Posicao atual motor X: %d\r\n", sistema.ultimaPosicaoX);
    printf("Posicao atual motor Y: %d\r\n", sistema.ultimaPosicaoY);
  }
}

void controlarMotoresPeloJoystick(Sistema &sistema) {
  if (sistema.joystick_X.houveAlteracao()) {
    float valorX = sistema.joystick_X.lerAlteracao();
    moverMotorLivre(&sistema.MotorX, &valorX, POSICAO_MIN_MOTOR_X, POSICAO_MAX_MOTOR_X, 'X');
  }

  if (sistema.joystick_Y.houveAlteracao()) {
    float valorY = sistema.joystick_Y.lerAlteracao();
    moverMotorLivre(&sistema.MotorY, &valorY, POSICAO_MIN_MOTOR_Y, POSICAO_MAX_MOTOR_Y, 'Y');
  }
}

// Funções de loop
void loopZerarPosicaoMotorX(Sistema &sistema) {
  exibirModo(sistema, "Zerando Motor X", "Zerar", "");
  while (true) {
    if (sistema.botaoA.verificarAcionamento()) {
      break;
    }
    sleep_ms(10);
  }
}

void loopZerarPosicaoMotorY(Sistema &sistema) {
  exibirModo(sistema, "Zerando Motor Y", "", "Zerar");
  while (true) {
    if (sistema.botaoB.verificarAcionamento()) {
      break;
    }
    sleep_ms(10);
  }
}

void loopModoLivre(Sistema &sistema) {

  exibirCoordenadas(sistema, MODO_LIVRE, sistema.ultimaPosicaoX, sistema.ultimaPosicaoY);

  while (true) {

    atualizarPosicoesMotores(sistema);
    controlarMotoresPeloJoystick(sistema);

    if (sistema.botaoA.verificarAcionamento()) {
      selecionarMemoriaModoLivre(sistema);
    }

    if (sistema.botaoB.verificarAcionamento()) {
      gravarMemoriaAtual(sistema);
    }

    if (sistema.botaoJoystick.verificarAcionamento()) {
      break;
    }

    sleep_ms(10);
  }
}

void loopModoMemoria(Sistema &sistema) {

  if (!selecionarMenorIndiceMemoriaValida(sistema)) {
    exibirMensagemTemporaria(sistema, "Nenhuma memoria gravada", 1000);
    return;
  }

  exibirCoordenadas(sistema, MODO_MEMORIA, sistema.ultimaPosicaoX, sistema.ultimaPosicaoY);

  while (true) {

    atualizarPosicoesMotores(sistema);

    if (sistema.botaoA.verificarAcionamento()) {
      selecionarMemoriaModoMemoria(sistema);
    }

    if (sistema.botaoB.verificarAcionamento()) {
      if (sistema.memoriasGravadas[sistema.memoriaSelecionada]) {
        lerMemoriaAtual(sistema);
      }
    }

    if (sistema.botaoJoystick.verificarAcionamento()) {
      break;
    }

    sleep_ms(10);
  }
}



int main() {

  stdio_init_all();

  Oled tela(i2c1, PINO_SDA, PINO_SCL);
  tela.definirFonte(fonte_customizada);
  tela.inicializar();

  Acionador botaoJoystick(PINO_JOYSTICK_SW);
  Acionador botaoA(PINO_A);
  Acionador botaoB(PINO_B);

  Motor motorX(PINO_STEP_MOTOR_X, PINO_DIR_MOTOR_X, VELOCIDADE_MOTOR_X);
  Motor motorY(PINO_STEP_MOTOR_Y, PINO_DIR_MOTOR_Y, VELOCIDADE_MOTOR_Y);

  Analogico joystick_X(PINO_JOYSTICK_X, CANAL_JOYSTICK_X, VALOR_MIN_SAIDA, VALOR_MAX_SAIDA);
  Analogico joystick_Y(PINO_JOYSTICK_Y, CANAL_JOYSTICK_Y, VALOR_MIN_SAIDA, VALOR_MAX_SAIDA);

  motorX.setarPosicaoAtual(POSICAO_ATUAL_MOTOR_X);
  motorY.setarPosicaoAtual(POSICAO_ATUAL_MOTOR_Y);

  PosicaoMotor memorias[MEMORIA_MAXIMA] = {};
  bool memoriasGravadas[MEMORIA_MAXIMA] = {false, false, false};
  uint8_t memoriaSelecionada = 0;
  int32_t ultimaPosicaoX = 0;
  int32_t ultimaPosicaoY = 0;

  Sistema sistema = {
    motorX, motorY,
    joystick_X, joystick_Y,
    tela,
    botaoA, botaoB, botaoJoystick,
    memorias,
    memoriasGravadas,
    memoriaSelecionada,
    ultimaPosicaoX,
    ultimaPosicaoY
  };

  //exibirAnimacaoDeInicio(sistema);
  //exibirInstrucoes(sistema);
  loopZerarPosicaoMotorX(sistema);
  loopZerarPosicaoMotorY(sistema);

  while (true) {
    loopModoLivre(sistema);
    loopModoMemoria(sistema);
  }

  return 0;
}