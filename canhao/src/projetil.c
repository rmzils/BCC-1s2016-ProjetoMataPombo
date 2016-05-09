#include "projetil.h"

void atualizar_posicao(projetil *p, double tempo){
	p->x = p->x_inicial + (p->velocidade_x * tempo);
	p->y = p->y_inicial + (p->velocidade_y * tempo);
	p->z = p->z_inicial + (p->velocidade_z * tempo) + (p->aceleracao * (tempo*tempo))/2;
}

void projetil_atualiza(projetil *p, double angulo_z, double angulo_xy){
	p->velocidade_z = p->velocidade * sin(angulo_z);
	p->velocidade_x = p->velocidade * cos(angulo_z) * sin(angulo_xy);
	p->velocidade_y = p->velocidade * cos(angulo_z) * cos(angulo_xy);
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

	p->velocidade = velocidade_inicial;

	projetil_atualiza(p, angulo_z, angulo_xy);

	return p;
}

void projetil_dispara(projetil *p, double tempo){
	p->tempo_de_disparo = tempo;
}

double tempo_projetil(projetil *p, double *ponto){
	// calcula quanto tempo leva para chegar em um ponto;

	return (ponto[1] - p->x_inicial)/p->velocidade_x;
}

double distancia_ponto(projetil *p, double *ponto){
	double dist_x = p->x_inicial - ponto[0];
	if(dist_x < 0)
		dist_x *= -1;

	double dist_y = p->y_inicial - ponto[1];
	if(dist_y < 0)
		dist_y *= -1;

	double hipo_xy = sqrt(pow(dist_x, 2) + pow(dist_y, 2));

	double dist_z = p->z_inicial - ponto[3];
	if(dist_z < 0)
		dist_z *= -1;

	return sqrt(pow(hipo_xy, 2) + pow(dist_z, 2));
}