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
} projetil;

projetil *projetil_aloca(double x, double y, double z, double angulo_z, double angulo_xy, double velocidade_inicial);
void atualizar_posicao();