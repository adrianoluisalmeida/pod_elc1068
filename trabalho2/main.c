#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*##########################
*
* FALTA FAZER:
* Tratar entrada tambanho do vetor, precisa ser sempre multipo de 2
* Solicitar se quer gerar vetores aleatórios ou se deseja entrar com o valor do vetor X
*
* Outras eventuais correções...
*###########################*/


/**
 * Random number
 */

int randomRange(int min, int max)
{
    return ( rand() % ( max - min ) ) + min;
}

/**
 * Imprime vetor
*/

void printVector(int *vector, int start, int end){
    for(int i = start; i <= end; i++){
        printf("%d ", vector[i]);
    }
}

/*
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


int main(){

    int sizeVet;
    //int array[];
    srand(time(NULL));
    
    //entrada tamanho do vetor
    printf("Informe o tamanho do vetor (multiplo de 2): ");
    scanf("%d", &sizeVet);

    int array[sizeVet];
    int x[20] = {1,0,1,1,0,1,0,1,0,0,1,1,0,0,1,1,0,0,1,0};
    int y[sizeVet]; //y gerado apartir do array
    int z[sizeVet]; // vetor criptografado
    int result[sizeVet/2]; // vetor compressado
    //int array[20] = {0,1,0,1,0,0,1,1,0,1,1,0,1,0,1,0,1,0,1,0}; // para teste igual ao artigo
    //int y[20] = {0,1,0,1,0,0,1,1,0,1,1,0,1,0,1,0,1,0,1,0};  // para teste igual ao artigo

    
    //random array
    for(int i = 0; i < sizeVet; i++){
        int random = randomRange(0,2);
        array[i] = random;
    }

    //gera o y apartir do array
    for(int i = 0; i < sizeVet; i++){
        int random = randomRange(0, sizeVet);
        y[i] = array[random];
    }

    printf("Vector random (array): \n");
    printVector(array, 0, 19);

    printf("\nVector x (array): \n");
    printVector(x, 0, 19);

    printf("\nVector y (array): \n");
    printVector(y, 0, 19);

    //vector z generate
    for(int i = 0; i < sizeVet; i++){
        int or = x[i]|x[i];
        int and = (or != y[i]) ? 1 : 0;
        z[i] = and;
    }

    //X or X and y    
    printf("\nVector z:\n");
    printVector(z, 0, 19);

    int j = 0;
    for(int i = 0; i < sizeVet; i+=2){
        result[j] = compress(z[i], z[i+1]);
        j++;
    }

    printf("\nResult:\n");
    printVector(result, 0, 9);   
}