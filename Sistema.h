#pragma once

#include "pico/stdlib.h"
#include "Oled.h"
#include "Acionador.h"
#include "ControleMotorVelocidadeFixa.h"
#include "ControleAnalogico.h"

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
#define DESLOCAMENTO_MIN_MOTOR_X 0
#define DESLOCAMENTO_ATUAL_MOTOR_X 100
#define DESLOCAMENTO_MAXIMO_MOTOR_X 200

#define PINO_STEP_MOTOR_Y 4
#define PINO_DIR_MOTOR_Y 8
#define VELOCIDADE_MOTOR_Y 500.0f
#define ANGULACAO_MIN_MOTOR_Y 0
#define ANGULACAO_ATUAL_MOTOR_Y 100
#define ANGULACAO_MAXIMO_MOTOR_Y 200

#define passo_maximoIMA 400
#define passo_minIMA 0
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

//calculo de deslocamento motor
//diametro da polia ideal: d = 31.83mm
//perimetro aproximado da polia PER = PI*d = 100mm
//passo para uma volta completa = 200
//incrimentos por passos = PER/200 = 0.5mm/passo
#define INCREMENTO_POR_PASSO_X 0.5f //deslocamento
#define PASSOS_POR_GRAU_Y 2 //rotacao

// Tipos de controle
typedef ControleMotorVelocidadeFixa Motor;
typedef ControleAnalogico Analogico;

// Estruturas de dados
struct PosicaoMotor {
    int32_t posicaoX;
    int32_t anguloY;
};

struct Sistema {
    Motor &motorX;
    Motor &motorY;
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
    int32_t &ultimoAnguloY;
};

// Protótipos das funções organizados por responsabilidades

// Funções de exibição
void atualizarExibicaoMemoria(Sistema &sistema, const char *mensagem = nullptr);
void exibirMensagemTemporariaNaMemoria(Sistema &sistema, const char *mensagem, uint32_t duracao_ms);
void atualizarExibicaoCodenadas(Sistema &sistema, int x, int y);
void exibirCoordenadas(Sistema &sistema, uint8_t modo, int x = 0, int y = 0);
void exibirMensagemTemporaria(Sistema &sistema, const char *mensagem, uint32_t duracao_ms);
void exibirInstrucoes(Sistema &sistema);
void exibirModo(Sistema &sistema, const char *modo, const char *instrucaoA, const char *instrucaoB, bool exibir_tudo = false);
void exibirAnimacaoDeInicio(Sistema &sistema);

// Funções de controle de memória
//bool selecionarMenorIndiceMemoriaValida(Sistema &sistema);
void selecionarMemoriaModoLivre(Sistema &sistema);
void selecionarMemoriaModoMemoria(Sistema &sistema);
void gravarMemoriaAtual(Sistema &sistema);
void lerMemoriaAtual(Sistema &sistema);

// Funções de controle de motores
void moverMotorLivre(Motor *motor, float *valor, uint32_t passo_min, uint32_t passo_maximo, const char eixo = ' ');
bool houveAlteracaoPosicaoMotores(Sistema &sistema);
void atualizarPosicoesMotores(Sistema &sistema);

int32_t calcularPassosParaDeslocamento(int32_t distancia);
int32_t calcularPassosParaInclinacao(int32_t angulo);
int32_t calcularPosicaoAtual(int32_t passo_atual);
int32_t calcularAnguloAtual(int32_t passo_atual);

// Funções de loop
void loopZerarPosicaoMotorX(Sistema &sistema);
void loopZerarPosicaoMotorY(Sistema &sistema);
void loopModoLivre(Sistema &sistema);
void loopModoMemoria(Sistema &sistema);