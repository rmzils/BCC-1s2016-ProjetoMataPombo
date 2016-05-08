#ifndef __PROJETIL_H__
#define __PROJETIL_H__

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <math.h>

typedef struct {
	double x_inicial, y_inicial, z_inicial;
	double aceleracao;
	double velocidade_x, velocidade_y, velocidade_z;

	double tempo_de_disparo;

	double x, y, z;
}projetil;

projetil *projetil_aloca(double x, double y, double z, double angulo_z, double angulo_xy, double velocidade_inicial);
void atualizar_posicao(double tempo);
void projetil_atualiza(projetil *p, double angulo_z, double angulo_xy);
double tempo_projetil(projetil *p, double *ponto);
void projetil_dispara(projetil *p, double tempo);