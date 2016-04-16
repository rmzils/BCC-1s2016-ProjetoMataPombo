#ifndef __ESTRUTURA_AVIAO_H__
#define __ESTRUTURA_AVIAO_H__

typedef struct {
  float posX, posY, posZ;
  float velocidade;
  int altitude;
  float anguloDirecao;
  int tipoTrajetoria;
  time_t tempoDeVoo;
}aviao;

#endif
