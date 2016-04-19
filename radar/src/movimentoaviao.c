#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

#include "definicoes.h"
#include "estatistica.h"
#include "aviao.h"

//------------------------------------------------------------------------------
float modulo(float num){
  if (num < 0) {
    num = num * (-1);
  }
  return num;
}

//------------------------------------------------------------------------------
void calcularPontoAdjacente( aviao *pombo, movimentoAviao *movimentoPombo ){
  movimentoPombo->pontoHipotenusa[0] = movimentoPombo->pontoCatetos[0] + movimentoPombo->tamanhoCatetoOposto * movimentoPombo->vetorCatetoOposto[0];
  movimentoPombo->pontoHipotenusa[1] = movimentoPombo->pontoCatetos[1] + movimentoPombo->tamanhoCatetoOposto * movimentoPombo->vetorCatetoOposto[1];
}

//------------------------------------------------------------------------------
void calcularVetorCatetoOposto( aviao *pombo, movimentoAviao *movimentoPombo ){
  if ( pombo->anguloDirecao > 0 ) {
    movimentoPombo->vetorCatetoOposto[0] = movimentoPombo->vetorCatetoAdjacente[0];
    movimentoPombo->vetorCatetoOposto[1] = -(movimentoPombo->vetorCatetoAdjacente[1]);
  }
  else if ( pombo->anguloDirecao < 0 ) {
    movimentoPombo->vetorCatetoOposto[0] = -(movimentoPombo->vetorCatetoAdjacente[0]);
    movimentoPombo->vetorCatetoOposto[1] = movimentoPombo->vetorCatetoAdjacente[1];
  }
  else {
    movimentoPombo->vetorCatetoOposto[0] = movimentoPombo->vetorCatetoAdjacente[0];
    movimentoPombo->vetorCatetoOposto[1] = movimentoPombo->vetorCatetoAdjacente[1];
  }
  calcularPontoAdjacente( pombo, movimentoPombo );
}

//------------------------------------------------------------------------------
void calcularPontoCatetos( aviao *pombo, movimentoAviao *movimentoPombo ){
  movimentoPombo->pontoCatetos[0] = movimentoPombo->pontoInicialAviao[0] + movimentoPombo->tamanhoCatetoAdjacente * movimentoPombo->vetorCatetoAdjacente[0];
  movimentoPombo->pontoCatetos[1] = movimentoPombo->pontoInicialAviao[1] + movimentoPombo->tamanhoCatetoAdjacente * movimentoPombo->vetorCatetoAdjacente[1];
  calcularVetorCatetoOposto( pombo, movimentoPombo );
}

//------------------------------------------------------------------------------
void calcularVetorCatetoAdjacente( aviao *pombo, movimentoAviao *movimentoPombo ){
  movimentoPombo->vetorCatetoAdjacente[0] = ((movimentoPombo->pontoInicialAviao[0] - POSXRADAR )/(modulo(movimentoPombo->pontoInicialAviao[0] - POSXRADAR)));
  movimentoPombo->vetorCatetoAdjacente[1] = ((movimentoPombo->pontoInicialAviao[1] - POSYRADAR )/(modulo(movimentoPombo->pontoInicialAviao[1] - POSYRADAR)));
  calcularPontoCatetos( pombo,movimentoPombo );
}

//------------------------------------------------------------------------------
void calculoTamanhoTriangulo( aviao *pombo, movimentoAviao *movimentoPombo ){
  movimentoPombo->tamanhoHipotenusa      = 0 + (pombo->velocidade * pombo->tempoDeVoo);
  movimentoPombo->tamanhoCatetoAdjacente = movimentoPombo->tamanhoHipotenusa * cos( movimentoPombo->anguloTeta );
  movimentoPombo->tamanhoCatetoOposto    = movimentoPombo->tamanhoHipotenusa * sin( movimentoPombo->anguloTeta );
  calcularVetorCatetoAdjacente( pombo, movimentoPombo );
}

//------------------------------------------------------------------------------
void calculandoProximoPonto( aviao *pombo, movimentoAviao *movimentoPombo ){
   calculoTamanhoTriangulo( pombo, movimentoPombo );
}

//------------------------------------------------------------------------------
