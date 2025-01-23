#pragma once
#include "Sistema.h"

// Funções de exibição
void atualizarExibicaoMemoria(Sistema &sistema, const char *mensagem)
{
  if (mensagem)
  {
    sistema.tela.imprimir(0, LINHA_3, "Mem: %d %s", sistema.indiceMemoriaSelecionada + 1, mensagem);
  }
  else
  {
    sistema.tela.limparAreaLinha(48, LINHA_3);
    sistema.tela.imprimir(0, LINHA_3, "Mem: %d", sistema.indiceMemoriaSelecionada + 1);
  }
  sistema.tela.renderizarDisplay();
}

void exibirMensagemTemporariaNaMemoria(Sistema &sistema, const char *mensagem, uint32_t duracao_ms)
{
  atualizarExibicaoMemoria(sistema, mensagem);
  sleep_ms(duracao_ms);
  atualizarExibicaoMemoria(sistema);
}

void atualizarExibicaoCodenadas(Sistema &sistema, int x, int y)
{
  sistema.tela.limparAreaLinha(56, LINHA_1);
  sistema.tela.limparAreaLinha(56, LINHA_2);
  sistema.tela.imprimir(0, LINHA_1, "Des X: %d mm", sistema.ultimaPosicaoX);
  sistema.tela.imprimir(0, LINHA_2, "Ang Y: %d Graus", sistema.ultimoAnguloY);
  sistema.tela.renderizarDisplay();
}

void exibirModoComCoordenadas(Sistema &sistema, uint8_t modo, int x, int y)
{
  const char *modos[4][3] = {
      {"   Modo Livre   ", "A:Selec   B:Grav"},
      {"  Modo Memoria  ", "A:Selec   B:Exec"},
      {" Zerando eixo X ", "A:Zerar         "},
      {" Zerando eixo Y ", "         B:Zerar"}};
  sistema.tela.limpar();
  sistema.tela.imprimir(0, LINHA_TITULO, "%s", modos[modo][0]);
  atualizarExibicaoCodenadas(sistema, x, y);
  sistema.tela.imprimir(0, LINHA_MENU, "%s", modos[modo][1]);
  atualizarExibicaoMemoria(sistema);
  sistema.tela.renderizarDisplay();
}

void exibirMensagemTemporaria(Sistema &sistema, const char *mensagem, uint32_t duracao_ms)
{
  sistema.tela.limpar();
  sistema.tela.imprimir(0, 0, mensagem);
  sistema.tela.renderizarDisplay();
  sleep_ms(duracao_ms);
}

void exibirInstrucoes(Sistema &sistema)
{
  const char *paginas[][4] = {

    {"  Instrucoes P1 ", "Botao Joystick: ", "- Se chama J    ", "- Alterna Modos "},
    {"  Instrucoes P2 ", "Modo Live:      ", "- Movimenta pelo", "joystick        "},
    {"  Instrucoes P3 ", "Modo Memoria:   ", "- Movimenta pela", "pos. da memoria "},
    {"  Instrucoes P4 ", "Botoes:         ", "A: Funcao A     ", "B: Funcao B     "}
  };

  int total_paginas = sizeof(paginas) / sizeof(paginas[0]);
  int pagina_atual = 0;

  while (true)
  {

    sistema.tela.limpar();
    sistema.tela.imprimir(0, LINHA_TITULO, paginas[pagina_atual][0]);
    sistema.tela.imprimir(0, LINHA_1, paginas[pagina_atual][1]);
    sistema.tela.imprimir(0, LINHA_2, paginas[pagina_atual][2]);
    sistema.tela.imprimir(0, LINHA_3, paginas[pagina_atual][3]);
    sistema.tela.imprimir(0, LINHA_MENU, "J: Proximo");
    sistema.tela.renderizarDisplay();    

    while (!sistema.botaoJoystick.verificarAcionamento())
    {
      sleep_ms(10);
    }
    
    acionarBuzzer(sistema);
    pagina_atual++;
    if (pagina_atual >= total_paginas)
    {
      break;
    }
  }
}

void exibirAnimacaoDeInicio(Sistema &sistema)
{
  const char *estados[] = {"Iniciando", "Iniciando.", "Iniciando..", "Iniciando..."};
  for (int i = 0; i < 12; i++)
  {
    sistema.tela.limparAreaLinha(72, LINHA_TITULO);
    sistema.tela.imprimir(0, 0, estados[i % 4]);
    sistema.tela.renderizarDisplay();
    sleep_ms(300);
  }
}