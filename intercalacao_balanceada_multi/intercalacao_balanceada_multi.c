#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "quicksort.h"

#define RAM 5
#define NUM_NUMS 16
#define NUM_CAMINHOS 3

// Cria um arquivo temporario chamado nome com tam numeros aleatorios. 
// Os numeros gerados sÃ£o mostrados na tela. 
void cria_arq_rand(char *nome, int tam){
  FILE *arq;
  int num,j=0;
  arq = fopen(nome,"wb"); 
  int vet[NUM_NUMS] = {32,45,10,13,8,2,50,81,9,1,11,12,4,24,17,90};

  for (int i = 0; i < tam; i++) {
    num = random()%100;
    //fwrite(&num, sizeof(num), 1, arq);
    fwrite(&vet[i], sizeof(vet[i]), 1, arq);
    //printf("%d:%d\t",j++,num);
    printf("%d\t",vet[i],j++);
    if (j%RAM==0) 
      printf("\n");
  }
  printf("\n");
  printf("\n");
//  fflush(arq); // garante que tudo foi gravado antes de fechar
  fclose(arq);
}

// Le um aquivo com numeros inteiros e mostra-os na tela.
void le_arq(char *nome){
  FILE *arq;
  int num;
  int i=0;

  arq = fopen(nome,"rb"); 

  while( 1 == fread(&num,sizeof(num),1,arq)){
  //  printf("%d:%d\t",i++,num);
    printf("%d",i++,num);
    if (i%RAM==0) 
      printf("\n");
  }
  printf("\n");
  if (!feof(arq)) 
    printf("Houston!\n\n"); // teoricamente nunca devemos ver isso!
}

void fecha_arqs(int num, FILE **arqs){
  for (int i =0; i < num; i++){
    //fflush(arqs[i]); // garante que tudo foi gravado antes de fechar
    fclose(arqs[i]);
  }
}

void abre_arqs_temp(int inicio, int fim, char *nome_arq, FILE **arqstemp, char *como){
  char nome_arq_temp[20];
  for (int i=inicio;i<fim;i++){
    // cria nome do arq temporario e abre
    //printf("NEW FILE -> %d\n ", i);
    sprintf(nome_arq_temp,"%s.%d",nome_arq,i);
    arqstemp[i]=fopen(nome_arq_temp, como);
  }
}

void intercalacao_balanceada(FILE **arqstemp, char *nome_arq){
  //NUM_CAMINHOS
  int i = 0, iBuffer;
  int buffer[RAM];
  int lidos = 0, iK = -1;
  // Abre os arquivos criados no distribui
  abre_arqs_temp(0, NUM_CAMINHOS, nome_arq, arqstemp, "rb");

  //Cria novos arquivos
  abre_arqs_temp(NUM_CAMINHOS, NUM_CAMINHOS*2, nome_arq, arqstemp, "wb");

  // Salva valores no buffer até ficar cheio
  for(int k = 0; k < NUM_CAMINHOS; k++){
    
    iBuffer = 0;

    while( iBuffer <  RAM && (lidos = fread(&(buffer[iBuffer]), sizeof(int), 1, arqstemp[i])) ){
     
      i++;
      i %= NUM_CAMINHOS; // limitação arquivos
      iBuffer++; // próx elemento buffer
    }

    //ordenação do buffer
    quick_sort(buffer, RAM);
    
    // salva novo arquivo
    fwrite(buffer, sizeof(int), RAM, arqstemp[k+NUM_CAMINHOS]);
    for(int a=0; a<RAM; a++) 
      printf(" %i ",buffer[a]);
    
    printf("\n");
  }

  fecha_arqs(NUM_CAMINHOS*2, arqstemp);
}

void distribui(int num_caminhos, char *nome_arq){
  FILE *arq = fopen(nome_arq,"rb");
  FILE **arqstemp = malloc(sizeof(FILE *) * num_caminhos);
  int buffer[RAM], i=0, lidos;
  
  abre_arqs_temp(0,num_caminhos,nome_arq,arqstemp,"wb"); //abre args temporarios

  while( RAM == (lidos = fread(&buffer, sizeof(int), RAM, arq))){ // le o arquivo de entrada para a RAM
    quick_sort(buffer,RAM); // ordena
    fwrite(buffer, sizeof(int)*RAM, 1, arqstemp[i]); // escreve para o arquivo temporario atual
    i++; 
    i %= num_caminhos; // incrementa o arquivo temporario
  }
  // trata os ultimos numeros do arquivo
  quick_sort(buffer,lidos); // ordena
  fwrite(buffer, sizeof(int)*lidos, 1, arqstemp[i]); // escreve para o arquivo temporario atual
  fecha_arqs(num_caminhos, arqstemp);

  // TODO 
  intercalacao_balanceada(arqstemp, nome_arq);
}

int main(){
  //srand(time(NULL));
  char nome_arq[]="teste.arq";
  char *nome_arq_temp = malloc(sizeof(char)*strlen(nome_arq)+3); // suporta ate .99 arquivos
  cria_arq_rand(nome_arq, NUM_NUMS);
  distribui(NUM_CAMINHOS, nome_arq);

  for (int i=0; i<NUM_CAMINHOS; i++){
    sprintf(nome_arq_temp, "%s.%d", nome_arq, i);
    printf("------- %s:\n", nome_arq_temp);
    le_arq(nome_arq_temp);
  }
}
