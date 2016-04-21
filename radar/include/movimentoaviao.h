#ifndef __ESTRUTURA_MOVIMENTO_AVIAO_H__
#define __ESTRUTURA_MOVIMENTO_AVIAO_H__

#include "aviao.h"

typedef struct {
  float pontoInicialAviao[3];
  float pontoCentroRadar[3];
  float pontoCatetos[3];
  float pontoHipotenusa[3];

  float vetorCatetoAdjacente[3];
  float vetorCatetoOposto[3];

  float tamanhoHipotenusa;
  float tamanhoCatetoAdjacente;
  float tamanhoCatetoOposto;

  float anguloTeta;
}movimentoAviao;

void calculandoProximoPonto( aviao *pombo, movimentoAviao *movimentoPombo );
char* strInfPombo( aviao *pombo, movimentoAviao *movimentoPombo );

#endif
