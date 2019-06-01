#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "quicksort.h"
#include <stdbool.h>

#define RAM 5
#define NUM_NUMS 16
#define NUM_CAMINHOS 3

// Cria um arquivo temporario chamado nome com tam numeros aleatorios. 
// Os numeros gerados são mostrados na tela. 
void cria_arq_rand(char *nome, int tam){
  FILE *arq;
  int num,j=0;
  arq = fopen(nome,"wb"); 

  for (int i = 0; i < tam; i++) {
    num = random()%100;
    fwrite(&num, sizeof(num), 1, arq);
    printf("%d:%d\t",j++,num);
    if (j%RAM==0) 
      printf("\n");
  }
  printf("\n");
  printf("\n");
  fflush(arq); // garante que tudo foi gravado antes de fechar
  fclose(arq);
}

// Le um aquivo com numeros inteiros e mostra-os na tela.
void le_arq(char *nome){
  FILE *arq;
  int num;
  int i=0;

  arq = fopen(nome,"rb"); 

  while( 1 == fread(&num,sizeof(num),1,arq)){
    printf("%d:%d\t",i++,num);
    
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


void fecha_arqs_temps(int inicio, int fim, FILE **arqs){
  for (int i =inicio; i < fim; i++){
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
  int buffer2[RAM];
  int bufferPrint[NUM_NUMS];
  int vet_aux[NUM_NUMS];
  int lidos = 0, iK = -1;

  // Abre os arquivos criados no distribui
  abre_arqs_temp(0, NUM_CAMINHOS, nome_arq, arqstemp, "rb");

  int num_cam_novo = 0;
  bool whileFlag = 1, j = 0;
  int val = 0, k = 0, maior, menor;

  char *nome_arq_temp = malloc(sizeof(char)*strlen(nome_arq)+3); // suporta ate .99 arquivos
  
  while (whileFlag){
    
    for(int j = 0; j < RAM; j++){
      fread(&buffer[j], sizeof(int), 1, arqstemp[num_cam_novo]);
    }

    if(num_cam_novo+1 < NUM_CAMINHOS){
      for(int j = 0; j < RAM; j++){
        fread(&buffer2[j], sizeof(int), 1, arqstemp[num_cam_novo+1]);
      }

      for(int j = 0; j < RAM; j++){

        if(buffer[j] < buffer2[j]){
          vet_aux[k] = buffer[j];
          vet_aux[k+1] = buffer2[j];
        }else{
          
          vet_aux[k] = buffer2[j];
          vet_aux[k+1] = buffer[j];
        }

        k+=2;
        
      }

    }else{
      for(int j = 0; j < RAM; j++){
        vet_aux[k] = buffer[j];
        k++;
      }
    
    }

    num_cam_novo+=2;

    if(num_cam_novo > NUM_CAMINHOS){
      break;
      whileFlag = 0;
    } 
 
  }
  fecha_arqs(NUM_CAMINHOS, arqstemp);
  abre_arqs_temp(NUM_CAMINHOS, NUM_CAMINHOS+1, nome_arq, arqstemp, "wb");
    
  quick_sort(vet_aux,NUM_NUMS);

  fwrite(vet_aux, sizeof(int)*(NUM_NUMS), 1, arqstemp[NUM_CAMINHOS]);

    for(int j = 0; j < NUM_NUMS; j++){
      fread(&bufferPrint[j], sizeof(int), 1, arqstemp[NUM_CAMINHOS]);
    }
  
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
  char nome_arq[]="teste.arq";
   int bufferPrint[NUM_NUMS];
  char *nome_arq_temp = malloc(sizeof(char)*strlen(nome_arq)+3); // suporta ate .99 arquivos
  cria_arq_rand(nome_arq, NUM_NUMS);
  distribui(NUM_CAMINHOS, nome_arq);

  for (int i=0; i<NUM_CAMINHOS+1; i++){
    sprintf(nome_arq_temp, "%s.%d", nome_arq, i);
    printf("------- %s:\n", nome_arq_temp);
    le_arq(nome_arq_temp);
  }

}