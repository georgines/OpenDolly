#pragma once
#include "Sistema.h"
#include "auxiliarControleMotor.h"
#include "auxiliarConversore.h"

// Funções para joystick
void controlarMotoresPeloJoystick(Sistema &sistema)
{
    if (sistema.joystick_X.houveAlteracao())
    {
        int32_t valorX = static_cast<int32_t>(sistema.joystick_X.lerValorAtual());
        int32_t passo_min = calcularPassosParaDeslocamento(DESLOCAMENTO_MIN_MOTOR_X);
        int32_t passo_maximo = calcularPassosParaDeslocamento(DESLOCAMENTO_MAXIMO_MOTOR_X);
        moverMotorLivre(&sistema.motorX, &valorX, passo_min, passo_maximo, 'X');
    }

    if (sistema.joystick_Y.houveAlteracao())
    {
        int32_t valorY = static_cast<int32_t>(sistema.joystick_Y.lerValorAtual());
        int32_t angulo_min = calcularPassosParaInclinacao(ANGULACAO_MIN_MOTOR_Y);
        int32_t angulo_max = calcularPassosParaInclinacao(ANGULACAO_MAXIMO_MOTOR_Y);
        moverMotorLivre(&sistema.motorY, &valorY, angulo_min, angulo_max, 'Y');
    }
}