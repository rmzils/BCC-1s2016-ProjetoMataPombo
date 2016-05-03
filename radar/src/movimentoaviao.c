#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#include "definicoes.h"
#include "estatistica.h"
#include "aviao.h"

//------------------------------------------------------------------------------
double horarioPonto(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec/1e6;
}

//------------------------------------------------------------------------------
char* strInfPombo( aviao *pombo, movimentoAviao *movimentoPombo ){

  float horario;
  char *strMensagem = "";
  strMensagem = malloc( 100 * sizeof (char) );
  strMensagem[0] = '\0';
  char strPX[60], strPY[60], strPZ[60], strHorario[60];

  sprintf(strPX, "%.7f", pombo->posX);
  sprintf(strPY, "%.7f", pombo->posY);
  sprintf(strPZ, "%.7f", pombo->posZ);

  strcat(strMensagem, strPX);
  strcat(strMensagem, " ");
  strcat(strMensagem, strPY);
  strcat(strMensagem, " ");
  strcat(strMensagem, strPZ);
  strcat(strMensagem, " ");

  struct timeval tv;
	gettimeofday(&tv, NULL);
	horario = tv.tv_sec + tv.tv_usec/1e6;

  sprintf(strHorario, "%f", horario);
  strcat(strMensagem, strHorario);
  strcat(strMensagem, " ");

  return strMensagem;
}

//------------------------------------------------------------------------------
void calcularPontoAdjacente( aviao *pombo, movimentoAviao *movimentoPombo ){
  movimentoPombo->pontoHipotenusa[0] = movimentoPombo->pontoCatetos[0] + movimentoPombo->tamanhoCatetoOposto * movimentoPombo->vetorCatetoOposto[0];
  movimentoPombo->pontoHipotenusa[1] = movimentoPombo->pontoCatetos[1] + movimentoPombo->tamanhoCatetoOposto * movimentoPombo->vetorCatetoOposto[1];

  pombo->posX = movimentoPombo->pontoHipotenusa[0];
  pombo->posY = movimentoPombo->pontoHipotenusa[1];
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

  if ( movimentoPombo->anguloTeta == 0 ) {
    movimentoPombo->pontoHipotenusa[0] = movimentoPombo->pontoCatetos[0];
    movimentoPombo->pontoHipotenusa[1] = movimentoPombo->pontoCatetos[1];
    pombo->posX = movimentoPombo->pontoHipotenusa[0];
    pombo->posY = movimentoPombo->pontoHipotenusa[1];
    return;
  }

  calcularVetorCatetoOposto( pombo, movimentoPombo );
}

//------------------------------------------------------------------------------
void calcularVetorCatetoAdjacente( aviao *pombo, movimentoAviao *movimentoPombo ){
  float escalar = (sqrt((POSXRADAR - movimentoPombo->pontoInicialAviao[0])*(POSXRADAR - movimentoPombo->pontoInicialAviao[0]) + (POSYRADAR - movimentoPombo->pontoInicialAviao[1])*(POSYRADAR - movimentoPombo->pontoInicialAviao[1])));
  movimentoPombo->vetorCatetoAdjacente[0] = ((POSXRADAR - movimentoPombo->pontoInicialAviao[0])/(escalar));
  movimentoPombo->vetorCatetoAdjacente[1] = ((POSYRADAR - movimentoPombo->pontoInicialAviao[1])/(escalar));
  calcularPontoCatetos( pombo, movimentoPombo );
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
