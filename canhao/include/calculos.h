//bib para fazer todos os calculos necessarios
// - calculo de velocidade do aviao
// - calculo da interseccao da reta do aviao com o raio maximo de alcance do canhao
// - tempo em que o projetil vai acertar o alvo
// - verificacao da colisao
// - calculo azimuth
// - calculo angulo no eixo z

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

typedef struct {
	double pos_antx, pos_anty, pos_antz;
	double pos_atux, pos_atuy, poz_atuz;
	double velocidade;
	double tempo_ini, tempo_atu;
}aviao;

double calcula_velocidade(aviao *pomba);

