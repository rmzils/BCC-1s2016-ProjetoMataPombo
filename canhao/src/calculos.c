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

void calcula_pos_aviao(double *ponto, aviao *pomba, double angulo, double tempo){
	double vx = pomba->velocidade * sin(angulo);
	double vy = pomba->velocidade * cos(angulo);

	ponto[0] = pomba->pos_atux + (vx * tempo);
	ponto[1] = pomba->pos_atuy + (vy * tempo);
	ponto[2] = pombo->pos_atuz;
}

void ponto_alvo(double *ponto, aviao *pomba, double tempo){
	//pegar coefeciente angular e linear

	double a = (pomba->pos_atuy - pomba->pos_anty)/(pomba->pos_atux - pomba->pos_antx);

	double b = pomba->pos_atuy - (a * pomba->pos_atux);

	// econtrar o quadrante da reta

	int quadrante = 0;

	// calcular o angulo

	double angulo;

	double angulo_real;

	if(quadrante == 0)
		angulo_real = angulo;
	else if(quadrante == 1)
		angulo_real = 180 - angulo;
	else if(quadrante == 2)
		angulo_real = 270 - angulo;
	else
		angulo_real = 360 - angulo;
	
	calcula_pos_aviao(ponto, pomba, angulo_real, tempo);
}

double calcula_azemuth(){

}

double calcula_angulo_disparo(projetil *p, double *ponto, double tempo){
	long double v2 = p->velocidade;
	long double v4 = p->velocidade;

	printf("%lf %lf\n", v2, v4);

	double x = sqrt((ponto[0] * ponto[0]) + (ponto[1] + ponto[1]));

	double teta = atan((v2 + sqrt(((v4 - p->aceleracao) * (p->aceleracao * pow(x, 2) + 2 * ponto[3] * v2))))/p->velocidade * x);

	printf("%lf\n", teta);

	return teta;
}

int colisao(projetil *p, aviao *pomba, double tempo){
	//atualiza posicao pomba
}