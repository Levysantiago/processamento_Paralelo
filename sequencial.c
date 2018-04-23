#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MSIZE 256
#define NSIZE 512

//Projeto5: Retorna a soma dos elementos de cada linha de uma matriz
double* somaMatrizxLinha(double *A, int msize, int nsize);

int main( int argc, char** argv )
{
	int mSize, nSize, mxn;
	double *A;
	double *result;
	int i, j;

	//Definindo a quantidade de linhas
	if(argc > 1){
		mSize = atoi(argv[1]);
	}
	else{
		mSize = MSIZE;
	}

	//Definindo a quantidade de colunas
	if(argc > 2){
		nSize = atoi(argv[2]);
	}
	else{
		nSize = NSIZE;
	}

	//Alocando a matriz como forma de um vetor
	A = (double *) malloc(mSize * nSize * sizeof(double));
	if(A == NULL){
		printf("Erro na alocacao.");
        exit(-1);
	}
	
	//Definindo semente
	srandom(123456789);

	//Preenchendo a matriz com valores aleatórios, dado a semente
	mxn = mSize*nSize;
	for(i = 0; i < mxn; i++){
		A[i] = ((double)random() / RAND_MAX)*2.0 - 1.0;
	}

	//Realizando o cálculo
	result = somaMatrizxLinha(A, mSize, nSize);

	//Imprimindo o resultado
	/*printf("Resultado: | ");
	for(i = 0;i < mSize; i++){
		printf("%f | ", result[j]);
	}
	printf("\n");*/

	free(A);
	free(result);
	return 0;
}

double* somaMatrizxLinha(double *A, int mSize, int nSize){
    int i, j, jStart;
    double soma;
    double *result;

    //Alocando o vetor resultado
    result = (double *) malloc(mSize * sizeof(double));
    if(result == NULL){
        printf("Erro na alocacao.");
        exit(-1);
    }

    //Percorrendo a matriz em forma de vetor
    for(i = 0; i < mSize; i++){
    	soma = 0.0;
    	jStart = nSize*i;
		for(j = jStart; j < jStart+nSize; j++){
			soma += A[j];
		}
		result[i] = soma;
	}

    return result;
}