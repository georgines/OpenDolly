#pragma once
#include "Sistema.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

absolute_time_t pegarTempoAbsolutoAtual(){
   return get_absolute_time();
}

void inicializarContadorDeTempoDoBuzzer( Sistema &sistema, repeating_timer &timer) {   
    add_repeating_timer_ms(10, desabilitarBuzzer, &sistema, &timer);   
}