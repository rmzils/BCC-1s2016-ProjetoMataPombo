#ifndef __ESTRUTURA_AVIAO_H__
#define __ESTRUTURA_AVIAO_H__

typedef struct {
  float posX, posY, posZ;
  float velocidade;
  int altitude;
  float anguloDirecao;
  int tipoTrajetoria;
  float tempoMudancaTrajetoria;
  float tempoDeVoo;
  int atacou;
  char primeiraVez;
}aviao;

#endif
