#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

#include "definicoes.h"
#include "movimentoaviao.h"
#include "aviao.h"

//------------------------------------------------------------------------------
float converterGrausEmradianus(float grau){
  float radiano;
  radiano = grau * ( 3.14159265359 / 180 );
  return radiano;
}

float randomAnguloDirecaoPombo(){

  float anguloDirecao;

  do {
    anguloDirecao = rand() % 75;
  } while(anguloDirecao < 10);

  int sinal = rand() % 2;
  if (sinal == 1) {
    anguloDirecao = anguloDirecao * (-1);
  }

  anguloDirecao = converterGrausEmradianus(anguloDirecao);
  return anguloDirecao;
}

//------------------------------------------------------------------------------
void randomTrajetoriaAviao( aviao *pombo, movimentoAviao *movimentoPombo ){

  int tipoTrajetoria = (rand() % 2) + 1;

  if (tipoTrajetoria == 1) {
    pombo->posZ = 200;
    pombo->velocidade = 66.6667;

    pombo->anguloDirecao = 0;
    pombo->tipoTrajetoria = tipoTrajetoria;
  }
  else {
    pombo->posZ = 500;
    pombo->velocidade = 111.111;
    pombo->tipoTrajetoria = tipoTrajetoria;

    pombo->anguloDirecao = randomAnguloDirecaoPombo();
    movimentoPombo->anguloTeta = pombo->anguloDirecao;
  }
}

//------------------------------------------------------------------------------
void randomPosicaoAviao( aviao *pombo, movimentoAviao *movimentoPombo ){

  int angulo = (rand() % 360);

  angulo = converterGrausEmradianus(angulo);
  pombo->posX = RAIO * cos( angulo ) + POSXRADAR;
  pombo->posY = RAIO * sin( angulo ) + POSYRADAR;
  pombo->posZ = 0;

  movimentoPombo->pontoInicialAviao[0] = pombo->posX;
  movimentoPombo->pontoInicialAviao[1] = pombo->posY;

  movimentoPombo->pontoCentroRadar[0] = POSXRADAR;
  movimentoPombo->pontoCentroRadar[1] = POSYRADAR;

  time(&pombo->tempoDeVoo);

  // printf("angulo sorteado: %d \npos. ini. aviao X: %f - pos. ini. aviao X: %f \n",angulo, pombo->posX, pombo->posY );

  randomTrajetoriaAviao( pombo, movimentoPombo );
}


// //------------------------------------------------------------------------------
// float randomTempoMudancaTrajetoria(){
//
// }
// //------------------------------------------------------------------------------
