#include "PWM.h"

PWM::PWM(uint8_t pino, uint16_t wrap, float divisor_clock)
    : pino(pino), wrap(wrap), divisor_clock(divisor_clock), slice(0) {}

void PWM::iniciar(uint16_t duty_inicial) {
    gpio_set_function(pino, GPIO_FUNC_PWM);
    slice = pwm_gpio_to_slice_num(pino);
    pwm_set_clkdiv(slice, divisor_clock);
    pwm_set_wrap(slice, wrap);
    modificarDuty(duty_inicial);
    habilitar();
}

void PWM::habilitar() {
    pwm_set_enabled(slice, true);
}

void PWM::desabilitar() {
    pwm_set_enabled(slice, false);
}

void PWM::habilitarInterrupcao() {
    pwm_set_irq_enabled(slice, true);
}

void PWM::modificarDuty(uint16_t duty) {
    uint16_t duty_convertido = PWM::mapearDuty(duty, 0, 100, 0, wrap);
    pwm_set_gpio_level(pino, duty_convertido);
}

void PWM::modificarDuty(uint8_t pino, uint16_t wrap, uint16_t duty) {
    uint16_t duty_convertido = PWM::mapearDuty(duty, 0, 100, 0, wrap);
    pwm_set_gpio_level(pino, duty_convertido);
}

void PWM::limparInterrupcao(uint16_t slice) {
    pwm_clear_irq(slice);
}

uint16_t PWM::pegarSlice(uint8_t pino) {
    return pwm_gpio_to_slice_num(pino);
}

uint16_t PWM::mapearDuty(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
    if (in_max == in_min) {
        return 0;
    }
    uint16_t result = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    if (result > out_max) {
        return out_max;
    }
    return result;
}
