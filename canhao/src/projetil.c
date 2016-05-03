#include "projetil.h"

void atualizar_posicao(double tempo){
	p->x = p->x_inicial + (p->velocidade_x * tempo);
	p->y = p->y_inicial + (p->velocidade_y * tempo);
	p->z = p->z_inicial + (p->velocidade_z * tempo) + (p->aceleracao * (tempo*tempo))/2;
}

void projetil_atualiza(projetil *p, double angulo_z, double angulo_xy){
	p->velocidade_z = velocidade_inicial * sin(angulo_z)
	p->velocidade_x = velocidade_inicial * cos(angulo_z) * sin(angulo_xy)
	p->velocidade_y = velocidade_inicial * cos(angulo_z) * cos(angulo_xy)
}

projetil *projetil_aloca(double x, double y, double z, double angulo_z, double angulo_xy, double velocidade_inicial){
	projetil *p = malloc(sizeof(projetil));

	p->x_inicial = x;
	p->y_inicial = y;
	p->z_inicial = z;

	p->x = x;
	p->y = y;
	p->z = z;

	p->aceleracao = -9.8;

	projetil_atualiza(p, angulo_z, angulo_xy);

	//struct timeval tv;
	//gettimeofday(&tv, NULL);
	//p->tempo = 0;

	return p;
}

double tempo_projetil(projetil *p, double *ponto){
	// calcula quanto tempo leva para chegar em um ponto;
}