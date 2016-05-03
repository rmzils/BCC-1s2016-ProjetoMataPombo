#include <stdlib.h>

typedef struct {
	double pos_x, pos_y, pos_z;
	double p_inter_x, p_inter_y, p_inter_z;
	double tempo_inter;
}canhao;

canhao *canhao_aloca(double pos_x, double pos_y, double pos_z){
	canhao *c = malloc(sizeof(canhao));

	c->pos_x = pos_x;
	c->pos_y = pos_y;
	c->pos_z = pos_z;

	return c;
}

void atualiza_inter(canhao *c, double pos_x, double pos_y, double pos_z, double tempo){
	c->pos_inter_x = pos_x;
	c->pos_inter_y = pos_y;
	c->pos_inter_z = pos_z;

	c->tempo_inter = tempo;
}