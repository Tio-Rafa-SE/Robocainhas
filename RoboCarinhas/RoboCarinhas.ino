/*
  RoboCarinhas
  Robo K10 + DFR0548 que anda em quatro direcoes e muda de carinha a cada uma.
  Base: exemplo que ja funciona (sem voz, sem Serial).
*/

#include <TR0548_K10.h>
#include "unihiker_k10.h"

TR0548_K10  robo;
UNIHIKER_K10 k10;
uint8_t      screen_dir = 2;

// ---------- Ajustes ----------
const int VELOCIDADE  = 255;    // 0 a 255
const int TEMPO_FRENTE   = 4000; // ms
const int TEMPO_TRAS     = 4000; // ms
const int TEMPO_ESQUERDA = 2500; // ms
const int TEMPO_DIREITA  = 2500; // ms
const int TEMPO_PAUSA    = 1000; // ms parado entre os movimentos

const uint32_t PRETO  = 0x000000;
const uint32_t BRANCO = 0xFFFFFF;

// ======================= Desenho =======================

// Olho branco com pupila (dx, dy deslocam a pupila) e brilhinho
void olho(int cx, int cy, int dx, int dy, uint32_t corPupila) {
  k10.canvas->canvasCircle(cx, cy, 28, BRANCO, BRANCO, true);
  k10.canvas->canvasCircle(cx + dx, cy + dy, 13, corPupila, corPupila, true);
  k10.canvas->canvasCircle(cx + dx - 4, cy + dy - 5, 4, BRANCO, BRANCO, true);
}

// Boca curva feita de bolinhas. curva > 0 = sorriso, curva < 0 = boca triste
void boca(int cx, int cy, int meiaLarg, int curva, uint32_t cor) {
  const int n = 24;
  for (int i = 0; i <= n; i++) {
    float t = -1.0f + 2.0f * i / n;
    int x = cx + (int)(t * meiaLarg);
    int y = cy + (int)(curva * (1.0f - t * t));
    k10.canvas->canvasCircle(x, y, 4, cor, cor, true);
  }
}

void bochechas(uint32_t cor) {
  k10.canvas->canvasCircle(40, 160, 12, cor, cor, true);
  k10.canvas->canvasCircle(200, 160, 12, cor, cor, true);
}

void sobrancelha(int x1, int y1, int x2, int y2, uint32_t cor) {
  for (int i = -2; i <= 2; i++) k10.canvas->canvasLine(x1, y1 + i, x2, y2 + i, cor);
}

// 0: Parado, 1: Frente, 2: Tras, 3: Esquerda, 4: Direita
void desenharExpressao(int estado) {
  // Limpa a tela (fundo preto), igual ao exemplo que funciona
  k10.canvas->canvasRectangle(0, 0, 240, 320, PRETO, PRETO, true);

  const int EY = 100, EX1 = 80, EX2 = 160;

  switch (estado) {
    case 0: // Parado: olhar tranquilo e boca reta
      olho(EX1, EY, 0, 0, 0x3366FF);
      olho(EX2, EY, 0, 0, 0x3366FF);
      k10.canvas->canvasRectangle(90, 195, 60, 8, 0x9E9E9E, 0x9E9E9E, true);
      break;

    case 1: // Frente: olhando pra cima, sorrisao e bochechas
      olho(EX1, EY, 0, -10, 0x00C853);
      olho(EX2, EY, 0, -10, 0x00C853);
      bochechas(0xFF6F91);
      boca(120, 175, 55, 40, 0x00E676);
      break;

    case 2: // Tras: olhando pra baixo, sobrancelhas preocupadas e boca em "o"
      olho(EX1, EY, 0, 10, 0xFF6D00);
      olho(EX2, EY, 0, 10, 0xFF6D00);
      sobrancelha(50, 52, 100, 66, 0xFF6D00);
      sobrancelha(190, 52, 140, 66, 0xFF6D00);
      k10.canvas->canvasCircle(120, 205, 22, 0xFF6D00, 0xFF6D00, true);
      k10.canvas->canvasCircle(120, 205, 13, PRETO, PRETO, true);
      break;

    case 3: // Esquerda: olha pro lado, sobrancelha levantada e sorriso torto
      olho(EX1, EY, -14, 0, 0x2979FF);
      olho(EX2, EY, -14, 0, 0x2979FF);
      sobrancelha(52, 56, 104, 52, 0x2979FF);
      boca(100, 185, 40, 22, 0x2979FF);
      break;

    case 4: // Direita: espelho da esquerda
      olho(EX1, EY, 14, 0, 0xD500F9);
      olho(EX2, EY, 14, 0, 0xD500F9);
      sobrancelha(136, 52, 188, 56, 0xD500F9);
      boca(140, 185, 40, 22, 0xD500F9);
      break;
  }

  k10.canvas->updateCanvas();
}

// ======================= Programa =======================

void setup() {
  k10.begin();
  k10.initScreen(screen_dir);
  k10.creatCanvas();
  robo.begin();
}

void loop() {
  // 1. FRENTE
  desenharExpressao(1);
  robo.frente(VELOCIDADE);
  delay(TEMPO_FRENTE);
  robo.stopAll();
  desenharExpressao(0);
  delay(TEMPO_PAUSA);

  // 2. TRAS
  desenharExpressao(2);
  robo.tras(VELOCIDADE);
  delay(TEMPO_TRAS);
  robo.stopAll();
  desenharExpressao(0);
  delay(TEMPO_PAUSA);

  // 3. ESQUERDA
  desenharExpressao(3);
  robo.esquerda(VELOCIDADE);
  delay(TEMPO_ESQUERDA);
  robo.stopAll();
  desenharExpressao(0);
  delay(TEMPO_PAUSA);

  // 4. DIREITA
  desenharExpressao(4);
  robo.direita(VELOCIDADE);
  delay(TEMPO_DIREITA);
  robo.stopAll();
  desenharExpressao(0);
  delay(TEMPO_PAUSA);
}
