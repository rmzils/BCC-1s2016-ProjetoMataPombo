#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

double tempo(){
 struct timeval tv;
 char buffer[30];
 time_t curtime;

 gettimeofday(&tv, NULL);

 curtime=tv.tv_sec;
 strftime(buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));
 printf("%s%d\n",buffer,tv.tv_usec);

 printf("segundos: %lf\t \n", tv.tv_sec + tv.tv_usec/1e6 );

 return tv.tv_sec + tv.tv_usec/1e6;
}

int main(void){
  double star, fim, diferenca;

  star = tempo();

  printf("DIPARADO! \n");
  for(int i=0;i<30000;i++);
  printf("FIM LOOP!\n");

  fim = tempo();

  diferenca = fim - star;
  printf("Diferenca: %lf\t \n",diferenca);

  return 0;
}
