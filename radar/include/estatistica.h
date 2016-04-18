#ifndef __ESTATISTICA_H__
#define __ESTATISTICA_H__

#include "definicoes.h"
#include "movimentoaviao.h"
#include "aviao.h"

void randomPosicaoAviao( aviao *pombo, movimentoAviao *movimentoPombo );
void randomTrajetoriaAviao( aviao *pombo, movimentoAviao *movimentoPombo );
void direcaoAoAlvo( aviao *pombo, movimentoAviao *movimentoPombo );
float randomTempoMudancaTrajetoria();

#endif
