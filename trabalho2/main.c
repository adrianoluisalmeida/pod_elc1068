#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include <time.h>


//RANDOM NUMBER
int randomRange(int min, int max){
    return ( rand() % ( max - min ) ) + min;
}

//IMPRIME O VETOR
void printVector(int *vector, int start, int end){
    int i;
    for(i = start; i <= end; i++){
        printf("%d ", vector[i]);
    }
}

/*
* Tabela de verdade da técnica proposta
*
* 0 and 0 -> 0 = 0
* 1 and 1 -> 1 = 1
* 0 and 1 -> \0 = 2
* 1 and 0 -> \1 = 3
*/
int compress(int val1, int val2){
    if(val1 == 0 && val2 == 0)
        return 0;
    else if(val1 == 1 && val2 == 1)
        return 1;
    else if(val1 == 0 && val2 == 1)
        return 2;
    else if(val1 == 1 && val2 == 0)
        return 3;
    else{
        printf("Error"); exit(0);
    }
}

//TAMANHO DO VETOR PRECISA SER SEMPRE MULTIPLO DE 2
bool multiplo_de_dois(int valor){
    if(valor%2 == 0){
        return true;
    }else{
        return false;
    }
}

// GERA ARRAY ALEATORIO
// O vetor aleatório é gerado a partir de um random de 0 ou 1

int RandomArray(int *array, int sizeVet){
     int i;
     for(i = 0; i < sizeVet; i++){
         int random = randomRange(0,2);
         array[i] = random;
     }
     return *array;
}

//GERA O "Y" A PARTIR DO ARRAY
int GeraYRandom(int *y,int sizeVet, int *array){
    int i;
    for( i = 0; i < sizeVet; i++){
        int random = randomRange(0, sizeVet);
        y[i] = array[random];
    }
    return *y;
}

//GERA O VETOR "Y" NAO ALEATORIO
// Vetor Y é gerado a partir do vetor ARRAY não aleatório
int GeraY(int *y,int sizeVet, int *array){
    int i;
    for( i = 0; i < sizeVet; i++){
        y[i] = array[i];
        printf("%d",y[i]);
    }
    return *y;
}

// PREEECHER VETOR CASO O USUARIO NAO QUEIRA GERAR ALEATORIO
int PreencheArray(int *array, int sizeVet){
    int i, val;
    for(i = 0; i < sizeVet; i++){
        printf("VALOR %d:", i+1);
        scanf("%d", &val);
        array[i] = val;
    }
    return *array;
}

// GERA O VETOR Z
// Vetor Z é resultado do ( Xor X and Y ) 
int VetorZ(int *z, int sizeVet, int *x, int *y){
    //vector z generate
    int i;
    for(i = 0; i < sizeVet; i++){
        int or = x[i]|x[i];
        int and = (or != y[i]) ? 1 : 0;
        z[i] = and;
    }
    
    printf("\nVector z:\n");
    printVector(z, 0, sizeVet-1);
    return *z;
}

// RESULTADO DA COMPACTACAO
int Resultado(int *result, int sizeVet, int *z){
    int j = 0;
    int i;

    for(i = 0; i < sizeVet; i+=2){
         result[j] = compress(z[i], z[i+1]);
         j++;
    }

    printf("\nResult:\n");
    printVector(result, 0, ((sizeVet-1)/2));
    return *result;
}

// Imprime os vetores finais, array, x, y
void print_valor_dos_vetores(int *array, int *x, int *y, int sizeVet){

    printf("Vector random (array): \n");
    printVector(array, 0, sizeVet-1);

    printf("\nVector x (array): \n");
    printVector(x, 0, sizeVet-1);
    
    printf("\nVector y (array): \n");
    printVector(y, 0, sizeVet-1);
}


// PRINCIPAL
int main(){

    int sizeVet, gerar;
    int array[sizeVet];
    int x[20] = {1,0,1,1,0,1,0,1,0,0,1,1,0,0,1,1,0,0,1,0};
    int y[sizeVet]; //y gerado apartir do array
    int z[sizeVet]; // vetor criptografado
    int result[sizeVet/2]; // vetor compressado

    //int array[20] = {0,1,0,1,0,0,1,1,0,1,1,0,1,0,1,0,1,0,1,0}; // para teste igual ao artigo
    //int y[20] = {0,1,0,1,0,0,1,1,0,1,1,0,1,0,1,0,1,0,1,0};  // para teste igual ao artigo
    
    //int array[];

    srand(time(NULL));

    //ENTRADA TAMANHO DO VETOR
    printf("Informe o tamanho do vetor (multiplo de 2): ");
    scanf("%d", &sizeVet);

    if(!multiplo_de_dois(sizeVet)){
        printf("\n\n\n O VALOR DIGITADO NAO Eh MULTIPLO DE 2\n\n\n");
    }else{
          printf(" DESEJA GERAR VALORES ALEATORIOS ?(1-sim 2-nao)");
          scanf("%d", &gerar);

          if(gerar==1){
             //random array
             RandomArray(array,sizeVet);
             GeraYRandom(y,sizeVet,array);
             print_valor_dos_vetores(array,x,y,sizeVet);
             VetorZ(z,sizeVet,x,y);
             Resultado(result,sizeVet,z);

          }else if(gerar==2){
              PreencheArray(array,sizeVet);
              GeraY(y,sizeVet,array);
              print_valor_dos_vetores(array,x,y,sizeVet);
              VetorZ(z,sizeVet,x,y);
              Resultado(result,sizeVet,z);
          }
    }
}
