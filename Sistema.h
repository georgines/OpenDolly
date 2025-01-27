#pragma once

#include "pico/stdlib.h"
#include "Oled.h"
#include "Acionador.h"
#include "ControleMotorVelocidadeFixa.h"
#include "ControleAnalogico.h"
#include "PWM.h"

#define SIMULADOR 0
#define PLACA_DE_DESENVOLVIMENTO 1

// alterna o joystick de acordo com o ambiente de execução
#define AMBIENTE_EXECUCAO PLACA_DE_DESENVOLVIMENTO

// joystick
#if AMBIENTE_EXECUCAO == SIMULADOR
#define PINO_JOYSTICK_X 27
#define CANAL_JOYSTICK_X 0 // ADC0
#define VALOR_MIN_SAIDA_X 10
#define VALOR_MAX_SAIDA_X -10
#define PINO_JOYSTICK_Y 26
#define CANAL_JOYSTICK_Y 1 // ADC1
#define VALOR_MIN_SAIDA_Y -10
#define VALOR_MAX_SAIDA_Y 10
#elif AMBIENTE_EXECUCAO == PLACA_DE_DESENVOLVIMENTO
#define PINO_JOYSTICK_X 27
#define CANAL_JOYSTICK_X 1 // ADC1
#define VALOR_MIN_SAIDA_X -10
#define VALOR_MAX_SAIDA_X 10
#define PINO_JOYSTICK_Y 26
#define CANAL_JOYSTICK_Y 0 // ADC0
#define VALOR_MIN_SAIDA_Y -10
#define VALOR_MAX_SAIDA_Y 10
#endif

// botao A, B, J
#define PINO_BOTAO_A 5
#define PINO_BOTAO_B 6
#define PINO_BOTAO_JOYSTICK 22
// pinos tela
#define PINO_SDA_TELA 14
#define PINO_SCL_TELA 15

// driver motor
#define PINO_STEP_MOTOR_X 2
#define PINO_DIR_MOTOR_X 3
#define VELOCIDADE_MOTOR_X 500.0f
#define DESLOCAMENTO_MIN_MOTOR_X 0
#define DESLOCAMENTO_ATUAL_MOTOR_X 100
#define DESLOCAMENTO_MAXIMO_MOTOR_X 2000

#define PINO_STEP_MOTOR_Y 4
#define PINO_DIR_MOTOR_Y 8
#define VELOCIDADE_MOTOR_Y 500.0f
#define ANGULACAO_MIN_MOTOR_Y -45
#define ANGULACAO_ATUAL_MOTOR_Y 0
#define ANGULACAO_MAXIMO_MOTOR_Y 45

// memoria
#define MEMORIA_MAXIMA 3
// menu
#define MODO_LIVRE 0
#define MODO_MEMORIA 1
#define MODO_ZERAR_X 2
#define MODO_ZERAR_Y 3

// linhas menu
#define LINHA_TITULO 0
#define LINHA_1 20
#define LINHA_2 32
#define LINHA_3 44
#define LINHA_MENU 56

// buzzer
#define PINO_BUZZER 10
#define WRAP_BUZZER 4095            // Configurado para 1 kHz
#define DIVISOR_CLOCK_BUZZER 30.52f // Configurado para 1 kHz
#define DUTY_INICIAL_BUZZER 0
#define DUTY_BUZZER 50
#define DUTY_DESLIGAR_BUZZER DUTY_INICIAL_BUZZER
#define TEMPO_BUZZER_MS 100

// calculo de deslocamento motor
// diametro da polia ideal: d = 31.83mm
// perimetro aproximado da polia PER = PI*d = 100mm
// passo para uma volta completa = 200
// incrimentos por passos = PER/200 = 0.5mm/passo
#define INCREMENTO_POR_PASSO_X 0.5f // deslocamento
#define PASSOS_POR_GRAU_Y 2         // rotacao

// Tipos de controle
typedef ControleMotorVelocidadeFixa Motor;
typedef ControleAnalogico Analogico;

// Estruturas de dados
struct PosicaoMotor
{
    int32_t posicaoX;
    int32_t anguloY;
};

struct Sistema
{
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
    uint8_t &indiceMemoriaSelecionada;
    int32_t &ultimaPosicaoX;
    int32_t &ultimoAnguloY;
    PWM &buzzer;
    volatile absolute_time_t inicioAtivacao;
    volatile bool ativo;
};

// Protótipos das funções organizados por responsabilidades

// funcoes buzzer
absolute_time_t pegarTempoAbsolutoAtual();
void inicializarContadorDeTempoDoBuzzer(Sistema &sistema, repeating_timer &timer);
bool desabilitarBuzzer(struct repeating_timer *t);
void acionarBuzzer(Sistema &sistema);

// Funções de exibição
void atualizarExibicaoMemoria(Sistema &sistema, const char *mensagem = nullptr);
void exibirMensagemTemporariaNaMemoria(Sistema &sistema, const char *mensagem, uint32_t duracao_ms);
void atualizarExibicaoCodenadas(Sistema &sistema, int x, int y);
void exibirModoComCoordenadas(Sistema &sistema, uint8_t modo, int x = 0, int y = 0);
void exibirMensagemTemporaria(Sistema &sistema, const char *mensagem, uint32_t duracao_ms);
void exibirInstrucoes(Sistema &sistema);
void exibirAnimacaoDeInicio(Sistema &sistema);

// Funções de controle de memória
bool selecionarMenorIndiceMemoriaValida(Sistema &sistema);
void selecionarMemoriaModoLivre(Sistema &sistema);
void selecionarMemoriaModoMemoria(Sistema &sistema);
void gravarMemoriaAtual(Sistema &sistema);
void lerMemoriaAtual(Sistema &sistema);
void lerMemoriaApensSeFoiGravada(Sistema &sistema);

// Funções de controle de motores
void moverMotorLivre(Motor *motor, float *valor, uint32_t passo_min, uint32_t passo_maximo, const char eixo = ' ');
bool houveAlteracaoPosicaoMotores(Sistema &sistema);
void monitorarMotoresExibirCordenadas(Sistema &sistema);
void atualizarPosicoesMotores(Sistema &sistema);
void moverMotorSentidoHorario(Motor *motor);
void moverMotorSentidoAntihorario(Motor *motor);

// conversores de posicao angulo e passo
int32_t calcularPassosParaDeslocamento(int32_t distancia);
int32_t calcularPassosParaInclinacao(int32_t angulo);
int32_t calcularPosicaoAtual(int32_t passo_atual);
int32_t calcularAnguloAtual(int32_t passo_atual);

// Funções para joystick
void controlarMotoresPeloJoystick(Sistema &sistema);

// Funções de loop
void loopZerarPosicaoMotorX(Sistema &sistema);
void loopZerarAnguloMotorY(Sistema &sistema);
void loopModoLivre(Sistema &sistema);
void loopModoMemoria(Sistema &sistema);