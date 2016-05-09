#include "calculos.h"

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
	
	double tamanhoHipotenusa      = 0 + (pomba->velocidade * (tempoAtual - tempo));
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
	azimuth = atan( (p->x - pomba->pos_atux)/(p->y - pomba->pos_atuy) );
	return azimuth;
}

double calcula_angulo_disparo(projetil *p, double *ponto, double tempo){
	long double v2 = p->velocidade;
	long double v4 = p->velocidade;

	printf("%Lf %Lf\n", v2, v4);

	double x = sqrt((ponto[0] * ponto[0]) + (ponto[1] + ponto[1]));

	double teta = atan((v2 + sqrt(((v4 - p->aceleracao) * (p->aceleracao * pow(x, 2) + 2 * ponto[3] * v2))))/p->velocidade * x);

	printf("%lf\n", teta);

	return teta;
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

	if(sqrt(pow(hipo_xy, 2) + pow(dist_z, 2)) < 2.0)
		return 1;
	else
		return 0;
}