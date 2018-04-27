#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

//Definições
#define MSIZE 256
#define NSIZE 512
#define N_THREADS 4
#define QTD_REPETICOES_POR_FUNCAO 3

/** 
*
* Projeto 5: Retorna a soma dos elementos de cada linha de uma matriz
*
**/

typedef double* (*funcPointer)(double *A, int msize, int nsize);

/**
*	Uma função para liberar a memória que foi alocada para os ponteiros. Além de dar um free,
* ela seta o ponteiro como NULL para que futuramente o ponteiro não seja utilizado novamente.
**/
void freeSeguro(double **);

/**
*	Essa função recebe uma outra função como parâmetro, seus parâmetros e quantidade de vezes
* que essa função recebida será executada. O functionTime executa a função 'n' vezes, calcula
* o tempo de cada repetição e retorna a média de todos os resultados.
**/
double functionTime(funcPointer somaMatrizxLinha, double *A, int mSize, int nSize, int repeticao);

double* somaMatrizxLinhaParalelo(double *A, int msize, int nsize);

double* somaMatrizxLinhaSequencial(double *A, int msize, int nsize);

int main( int argc, char** argv )
{
	int mSize, nSize, mxn, i, j;
	double *A = NULL, *result = NULL;
	double speedup, eficiencia;
	double tempoParalelo, tempoSequencial;

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

	// (1024)^3 = 1073741824
	printf("\n Memória para a matriz:   %f Gigas\n\n", (double) (sizeof(double)*nSize*mSize)/(1073741824));

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
	tempoSequencial = functionTime(&somaMatrizxLinhaSequencial, A, mSize, nSize, QTD_REPETICOES_POR_FUNCAO);

	//Calculando o tempo paralelo
	tempoParalelo = functionTime(&somaMatrizxLinhaParalelo, A, mSize, nSize, QTD_REPETICOES_POR_FUNCAO);

	//Calculando o speedup
	speedup = tempoSequencial / tempoParalelo;

	//Calculando a eficiência
	eficiencia = speedup / N_THREADS;

	printf(" Tempo Sequencial: \t  %f\n", tempoSequencial);
	printf(" Tempo Paralelo: \t  %f\n\n", tempoParalelo);
	printf(" Speedup: \t\t  %f\n", speedup);
	printf(" Eficiencia: \t\t  %f\n\n", eficiencia);

	//Imprimindo o resultado
	/*printf("Resultado: | ");
	for(i = 0;i < mSize; i++){
		printf("%f | ", result[i]);
	}
	printf("\n");*/

	freeSeguro(&A);
	freeSeguro(&result);
	return 0;
}

void freeSeguro(double **pointer){
	if(*pointer != NULL){
		free(*pointer);
	}
	*pointer = NULL;
}

double functionTime(funcPointer somaMatrizxLinha, double *A, int mSize, int nSize, int repeticao){
	double start, end, media = 0.0, *result;
	int i;

	// Realizando chamadas à função 'repeticao' vezes e calculando a média desses tempos obtidos
	for(i = 0;i < repeticao; i++){
		start = omp_get_wtime();
		result = somaMatrizxLinha(A, mSize, nSize);
		end = omp_get_wtime();

		freeSeguro(&result);
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
    int i, j, nSizeLessOne = nSize - 1, tam = mSize*nSize;
    double *result;
    //Criando uma variável soma para cada thread
	double soma = 0.0;
	//Criando um contador de linha para cada thread
	int count = 0;

    //Alocando o vetor resultado
    result = (double *) malloc(mSize * sizeof(double));
    if(result == NULL){
        printf("Erro na alocacao.");
        exit(-1);
    }

    //Percorrendo a matriz em forma de vetor
    #pragma omp parallel num_threads(N_THREADS)
    {
    	//Criando uma variável soma para cada thread
		double soma = 0.0;
		//Criando um contador de linha para cada thread
		int count = 0;

		#pragma omp for schedule(dynamic, nSize)
		for(i = 0; i < tam; i++){
			//Somando os valores de uma linha
			soma += A[i];
			//Incrementando contador
			count++;
			//Se chegar no fim da linha
			if(count > nSizeLessOne){
				//Inserindo as somas de acordo com os índices adequados
				result[i/nSize] = soma;
				//Reiniciando soma e count
				count = 0;
				soma = 0.0;
			}
		}	
    }

    return result;
}