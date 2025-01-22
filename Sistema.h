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

//calculo de deslocamento motor
//diametro da polia ideal: d = 31.83mm
//perimetro aproximado da polia PER = PI*d = 100mm
//passo para uma volta completa = 200
//incrimentos por passos = PER/200 = 0.5mm/passo
#define INCREMENTO_POR_PASSO_X 0.5f //rotacao
#define PASSOS_POR_GRAU_Y 2 //rotacao

// Tipos de controle
typedef ControleMotorVelocidadeFixa Motor;
typedef ControleAnalogico Analogico;

// Estruturas de dados
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
void moverMotorLivre(Motor *motor, float *valor, uint32_t posicao_min, uint32_t posicao_max, const char eixo = ' ');
bool houveAlteracaoPosicaoMotores(Sistema &sistema);
void atualizarPosicoesMotores(Sistema &sistema);

// Funções de loop
void loopZerarPosicaoMotorX(Sistema &sistema);
void loopZerarPosicaoMotorY(Sistema &sistema);
void loopModoLivre(Sistema &sistema);
void loopModoMemoria(Sistema &sistema);