#include "calculos.h"
#define PI 3.14159265359

void calcula_velocidade(aviao *pomba){
	double difx = pomba->pos_antx - pomba->pos_atux;
	if(difx < 0)
		difx *= -1;

	double dify = pomba->pos_antx - pomba->pos_atux;
	if(difx < 0)
		difx *= -1;

	double hipo = sqrt((difx * difx) + (dify * dify));

	double tempo = pomba->tempo_atu - pomba->tempo_ini;

	pomba->velocidade = hipo/tempo;
}

void ponto_alvo(double *ponto, aviao *pomba, double tempo){
	double vetorCatetoAdjacente[2];
	double tempoAtual = pomba->tempo_atu;

	double tamanhoHipotenusa      = 0 + (pomba->velocidade * (tempo - tempoAtual));

	double tamanhoCatetoAdjacente = tamanhoHipotenusa * cos( 0 );
	float escalar = (sqrt((pomba->pos_atux - pomba->pos_antx)*(pomba->pos_atux - pomba->pos_antx) + (pomba->pos_atuy - pomba->pos_anty)*(pomba->pos_atuy - pomba->pos_anty)));
    
    vetorCatetoAdjacente[0] = ((pomba->pos_atux - pomba->pos_antx)/(escalar));
    vetorCatetoAdjacente[1] = ((pomba->pos_atuy - pomba->pos_anty)/(escalar));
	
	ponto[0] = pomba->pos_antx + tamanhoCatetoAdjacente * vetorCatetoAdjacente[0];
    ponto[1] = pomba->pos_anty + tamanhoCatetoAdjacente * vetorCatetoAdjacente[1];
    ponto[2] = pomba->pos_atuz;
}
double calcula_azemuth(projetil *p, aviao *pomba ){
	double azimuth;

	double c = pomba->pos_atux - p->x_inicial;
	double d = pomba->pos_atuy - p->y_inicial;

	azimuth = atan(c/d);

	//printf("c: %lf, d: %lf, azimuth: %lf\n", c, d, azimuth);

	if(c > 0 && d < 0)
		azimuth = (PI) - azimuth;
	else if(c < 0 && d < 0)
		azimuth = (PI) + azimuth;
	else if(c < 0 && d > 0)
		azimuth = (2*PI) - azimuth;

	//azimuth = atan(azimuth);

	//printf("azimuth: %lf, cos: %lf, sen: %lf\n", azimuth, cos(azimuth), sin(azimuth));

	return azimuth;
}

double calcula_angulo_disparo(projetil *p, double *ponto, double tempo){
	long double velocidade_kh = p->velocidade;

	long double v2 = pow(velocidade_kh,2);
	long double v4 = pow(velocidade_kh,4);

	long double dist_x = p->x_inicial - ponto[0];
	if(dist_x < 0)
		dist_x *= -1;

	long double dist_y = p->y_inicial - ponto[1];
	if(dist_y < 0)
		dist_y *= -1;

	long double pow_x = pow(dist_x, 2);
	long double pow_y = pow(dist_y, 2);

	long double x = sqrt(pow_x + pow_y);

	long double y = ponto[2];

	long double aceleracao_kh = -1 * p->aceleracao;

	//double teta = atan((v2 - sqrt(v4 - (aceleracao_kh * ((aceleracao_kh * pow(x, 2)) + (2 * y * v2)))))/(aceleracao_kh * x));

	double teta = atan((v2 - sqrt(v4 - (aceleracao_kh * ((aceleracao_kh * pow(x,2)) + (2 * y * v2)))))/(aceleracao_kh * x));

	//printf("sen:%lf cos:%lf\n", sin(teta), cos(teta));

	return teta;
}

double tempo_aviao(double *ponto, aviao *pomba){
	double dist_x = ponto[0] - pomba->pos_atux;
	if(dist_x < 0)
		dist_x *= -1;

	double dist_y = ponto[1] - pomba->pos_atuy;
	if(dist_y < 0)
		dist_y *= -1;

	double hipo = sqrt(pow(dist_x, 2) + pow(dist_y, 2));

	return hipo/pomba->velocidade;
}

int colisao(projetil *p, aviao *pomba, double tempo){
	//atualiza posicao pomba

	double dist_x = p->x - pomba->pos_atux;
	if(dist_x < 0)
		dist_x *= -1;

	double dist_y = p->y - pomba->pos_atuy;
	if(dist_y < 0)
		dist_y *= -1;

	double hipo_xy = sqrt(pow(dist_x, 2) + pow(dist_y, 2));

	double dist_z = p->z - pomba->pos_atuz;
	if(dist_z < 0)
		dist_z *= -1;

	double dist = sqrt(pow(hipo_xy, 2) + pow(dist_z, 2));

	//printf("%lf\n", dist);

	if(dist < 2.0)
		return 1;
	else
		return 0;
}

double ganbeta_dist(projetil *p, aviao *pomba, double tempo){
	double dist_x = p->x - pomba->pos_atux;
	if(dist_x < 0)
		dist_x *= -1;

	double dist_y = p->y - pomba->pos_atuy;
	if(dist_y < 0)
		dist_y *= -1;

	double hipo_xy = sqrt(pow(dist_x, 2) + pow(dist_y, 2));

	double dist_z = p->z - pomba->pos_atuz;
	if(dist_z < 0)
		dist_z *= -1;

	double dist = sqrt(pow(hipo_xy, 2) + pow(dist_z, 2));

	return dist;
}