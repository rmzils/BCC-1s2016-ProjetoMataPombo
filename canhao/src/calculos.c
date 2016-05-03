#include "calculos.h"
#include "projetil.h"

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
}

void ponto_alvo(double *ponto, avisao *pomba, double tempo){
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

double calcula_tempo_projetil(){

}


double calcula_azemuth(){

}

double calcula_angulo_disparo(){

}

