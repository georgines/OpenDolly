#pragma once
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

class PWM {
public:
    PWM(uint8_t pino, uint16_t wrap, float divisor_clock)
        : pino(pino), wrap(wrap), divisor_clock(divisor_clock), slice(0) {}

    void iniciar(uint16_t duty_inicial) {
        gpio_set_function(pino, GPIO_FUNC_PWM);
        slice = pwm_gpio_to_slice_num(pino);
        pwm_set_clkdiv(slice, divisor_clock);
        pwm_set_wrap(slice, wrap);
        modificarDuty(duty_inicial);
        habilitar();
    }

    void habilitar() {
        pwm_set_enabled(slice, true);
    }

    void desabilitar() {
        pwm_set_enabled(slice, false);
    }

    void habilitarInterrupcao() {      
        pwm_set_irq_enabled(slice, true);
    }

    /*void habilitarInterrupcaoComCallback(void (*callback)(void) = nullptr) {
        if (!callback) return;
        pwm_set_irq_enabled(slice, true);
        irq_set_exclusive_handler(PWM_IRQ_WRAP, callback);
        irq_set_enabled(PWM_IRQ_WRAP, true);
    }*/

    void modificarDuty(uint16_t duty) {
        uint16_t duty_convertido = PWM::mapearDuty(duty, 0, 100, 0, wrap);
        pwm_set_gpio_level(pino, duty_convertido);
    }

    static void modificarDuty(uint8_t pino, uint16_t wrap, uint16_t duty) {
        uint16_t duty_convertido = PWM::mapearDuty(duty, 0, 100, 0, wrap);
        pwm_set_gpio_level(pino, duty_convertido);
    }

    static void limparInterrupcao(uint16_t slice) {
        pwm_clear_irq(slice);
    }

    static uint16_t pegarSlice(uint8_t pino) {
        return pwm_gpio_to_slice_num(pino);
    }

    static uint16_t mapearDuty(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
        if (in_max == in_min) {
            return 0;
        }
        uint16_t result = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        if (result > out_max) {
            return out_max;
        }
        return result;
    }

private:
    uint16_t slice;
    uint8_t pino;
    uint16_t wrap;
    float divisor_clock;
};
