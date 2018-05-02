/** 
*
* Projeto 5: Retorna a soma dos elementos de cada linha de uma matriz
*
**/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

//Definições
#define MSIZE 256
#define NSIZE 512
#define N_THREADS 4
#define QTD_REPETICOES_POR_FUNCAO 3

/**
*	Um ponteiro para função que recebe um ponteiro a double (matriz) e dois inteiros (linha e
* coluna da matriz.
**/
typedef double* (*funcPointer)(double *A, int msize, int nsize);

/**
*	Uma função para liberar a memória que foi alocada para os ponteiros. Além de dar um free,
* ela seta o ponteiro como NULL para que futuramente o ponteiro não seja utilizado novamente.
**/
void freeSeguro(double **);

double absDouble(double x);

double compResults(double *vet1, double *vet2, int tam);

/**
*	Essa função recebe uma outra função como parâmetro, seus parâmetros e quantidade de vezes
* que essa função recebida será executada. O functionTime executa a função 'n' vezes, calcula
* o tempo de cada repetição e retorna a média de todos os resultados.
**/
double functionTime(funcPointer somaMatrizxLinha, double *A, int mSize, int nSize, double **result, int repeticao);

/**
*	Essa é a função que calcula a soma dos elementos de cada linha de uma matriz paralelamente.
**/
double* somaMatrizxLinhaParalelo(double *A, int msize, int nsize);

/**
*	Essa é a função que calcula a soma dos elementos de cada linha de uma matriz sequencialmente.
**/
double* somaMatrizxLinhaSequencial(double *A, int msize, int nsize);

int main( int argc, char** argv )
{
	int mSize, nSize, mxn, i, j;
	double *A = NULL, *resultParalel = NULL, *resultSeq = NULL;
	double speedup, eficiencia, diferencaMax;
	double tempoParalelo, tempoSequencial, tempoTotal;

	tempoTotal = omp_get_wtime();

	//Definindo a quantidade de linhas
	if(argc > 1){
		mSize = atoi(argv[1]);
	}
	else{
		mSize = MSIZE;
	}

	//Definindo a quantidade de colunas
	if(argc == 2){
		nSize = mSize;	
	}else if(argc > 2){
		nSize = atoi(argv[2]);
	}
	else{
		nSize = NSIZE;
	}

	//Para chegar em Gigabytes: (1024)^3 = 1073741824
	printf("\n Memória para a matriz:     %f Gigas\n\n", (double) (sizeof(double)*nSize*mSize)/(1073741824));

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

	//Calculando o tempo sequencial
	tempoSequencial = functionTime(&somaMatrizxLinhaSequencial, A, mSize, nSize, &resultSeq, QTD_REPETICOES_POR_FUNCAO);

	//Calculando o tempo paralelo
	tempoParalelo = functionTime(&somaMatrizxLinhaParalelo, A, mSize, nSize, &resultParalel, QTD_REPETICOES_POR_FUNCAO);

	//Comparando os vetores resultantes
	diferencaMax = compResults(resultSeq, resultParalel, mSize);

	//Desalocando os vetores
	freeSeguro(&resultSeq);
	freeSeguro(&resultParalel);

	//Calculando o speedup
	speedup = tempoSequencial / tempoParalelo;

	//Calculando a eficiência
	eficiencia = speedup / N_THREADS;

	printf(" Tempo Sequencial (Ts):     %f seg\n", tempoSequencial);
	printf(" Tempo Paralelo (Tp): \t    %f seg\n", tempoParalelo);
	printf(" Diferenca de valor máxima: %f\n\n", diferencaMax);
	printf(" Speedup (Sup): \t    %f Ts/Tp\n", speedup);
	printf(" Eficiencia: \t\t    %.2f%%\n\n", eficiencia*100);

	tempoTotal = omp_get_wtime() - tempoTotal;
	printf("\n Tempo total: \t\t    %f seg\n\n", tempoTotal);

	//Liberando a memória alocada pelos vetores
	freeSeguro(&A);
	return 0;
}

void freeSeguro(double **pointer){
	if(*pointer != NULL){
		free(*pointer);
	}
	*pointer = NULL;
}

double absDouble(double x){
	if(x < 0){
		x = x * -1.0;
	}

	return x;
}

double compResults(double *vet1, double *vet2, int tam){
	int i;
	double max, dif;

	max = absDouble(vet1[0] - vet2[0]);

	for(i = 1;i < tam;i++){
		dif = absDouble(vet1[i] - vet2[i]);
		if(dif > max){
			max = dif;
		}
	}

	return max;
}

double functionTime(funcPointer somaMatrizxLinha, double *A, int mSize, int nSize, double **result, int repeticao){
	double start, end, media = 0.0;
	int i;

	// Realizando chamadas à função 'repeticao' vezes e calculando a média desses tempos obtidos
	for(i = 0;i < repeticao; i++){
		start = omp_get_wtime();
		*result = somaMatrizxLinha(A, mSize, nSize);
		end = omp_get_wtime();

		if(i < repeticao-1){
			freeSeguro(result);
		}

		media += end - start;
	}

	media /= repeticao;

	return media;
}

double* somaMatrizxLinhaSequencial(double *A, int mSize, int nSize){
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

double* somaMatrizxLinhaParalelo(double *A, int mSize, int nSize){
    int i;
    double *result;

    //Alocando o vetor resultado
    result = (double *) malloc(mSize * sizeof(double));
    if(result == NULL){
        printf("Erro na alocacao.");
        exit(-1);
    }

    //Percorrendo a matriz em forma de vetor
    #pragma omp parallel num_threads(N_THREADS)
    {
    	int j, jStart;
    	
    	//Criando uma variável soma para cada thread
		double soma = 0.0;

    	//Percorrendo a matriz em forma de vetor
    	#pragma omp for schedule(static)
	    for(i = 0; i < mSize; i++){
	    	soma = 0.0;

	    	jStart = i*nSize;
			for(j = jStart; j < jStart+nSize; j++){
				soma += A[j];
			}
			result[i] = soma;
		}
    }

    return result;
}