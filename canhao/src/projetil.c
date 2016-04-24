#include "projetil.h"

projetil *projetil_aloca(double x, double y, double z, double angulo_z, double angulo_xy, double velocidade_inicial){
	projetil *p = malloc(sizeof(projetil));

	p->x_inicial = x;
	p->y_inicial = y;
	p->z_inicial = z;

	p->x = x;
	p->y = y;
	p->z = z;

	p->aceleracao = -9.8;

	p->velocidade_z = velocidade_inicial * sin(angulo_z)
	p->velocidade_x = velocidade_inicial * cos(angulo_z) * sin(angulo_xy)
	p->velocidade_y = velocidade_inicial * cos(angulo_z) * cos(angulo_xy)

	struct timeval tv;
	gettimeofday(&tv, NULL);
	p->tempo = 0;

	return p;
}

void atualizar_posicao(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	double tempo_atual = tv.tv_sec + tv.tv_usec/1e6;

	double tempo = p->tempo_de_disparo - tempo_atual;

	p->x = p->x_inicial + (p->velocidade_x * tempo);
	p->y = p->y_inicial + (p->velocidade_y * tempo);
	p->z = p->z_inicial + (p->velocidade_z * tempo) + (p->aceleracao * (tempo*tempo))/2;
}