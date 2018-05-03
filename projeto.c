/** 
*
* Projeto 5: Retorna a soma dos elementos de cada linha de uma matriz
* Feito por: Levy Santiago e Adson Cardoso
*
* Compilando: $ gcc projeto.c -o projeto -fopenmp -lm
*
**/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

//Quantidade padrão de linhas da matriz
#define MSIZE 256

//Quantidade padrão de colunas da matriz
#define NSIZE 512

//Quantidade de threads a serem usadas no código em paralelo
#define N_THREADS 4

//Quantidade de repetições para a chamada das funções em sequencial e paralelo 
#define QTD_REPETICOES_POR_FUNCAO 10

/**
*	Um ponteiro para função que recebe um ponteiro a double (matriz) e dois inteiros (linha e
* coluna da matriz.
**/
typedef double* (*funcPointer)(double *A, int msize, int nsize);

/*
*	Um struct para armazenar os dois tipos de medidas a serem realizadas
*
*/
typedef struct medidas{
	double tempo;
	double desvioPadrao;
}t_medidas;

/**
*	Uma função para liberar a memória que foi alocada para os ponteiros. Além de dar um free,
* ela seta o ponteiro como NULL para que futuramente o ponteiro não seja utilizado novamente.
**/
void freeSeguro(void **);

/*
* Uma função para retornar o valor absoluto de um double.
*
*/
double absDouble(double x);

/*
*	Uma função que compara os valores de dois vetores de mesmo tamanho calculando as diferenças
* absolutas em cada posição dos vetores, e retorna a máxima diferença resultante.
*
*/
double compResults(double *vet1, double *vet2, int tam);

/**
*	Essa função recebe uma outra função como parâmetro, uma matriz, a linha e coluna dessa matriz
* o vetor de resultado como ponteiro para ponteiro, e a quantidade de vezes que essa função recebida 
* como parâmetro será executada. O functionTime executa a função 'n' vezes, calcula o tempo de cada 
* repetição, retorna a média de todos os resultados e o desvio padrão a partir de um ponteiro do 
* tipo t_medidas.
**/
t_medidas * functionTime(funcPointer somaMatrizxLinha, double *A, int mSize, int nSize, double **result, int repeticao);

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
	double tempoTotal;
	t_medidas *medidasParalelo, *medidasSequencial;

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
	printf("\n Memória para a matriz:     *%f* Gigas\n\n", (double) (sizeof(double)*nSize*mSize)/(1073741824));

	//Alocando a matriz como forma de um vetor
	A = (double *) malloc(mSize * nSize * sizeof(double));
	if(A == NULL){
		printf("\nErro na alocacao.\n");
        exit(-1);
	}
	
	//Definindo semente
	srandom(123456789);

	//Preenchendo a matriz com valores aleatórios, dado a semente
	mxn = mSize*nSize;
	for(i = 0; i < mxn; i++){
		A[i] = ((double)random() / RAND_MAX)*2.0 - 1.0;
	}

	//Calculando o tempo e desvio padrão sequencial
	medidasSequencial = functionTime(&somaMatrizxLinhaSequencial, A, mSize, nSize, &resultSeq, QTD_REPETICOES_POR_FUNCAO);

	//Calculando o tempo e desvio padrão paralelo
	medidasParalelo = functionTime(&somaMatrizxLinhaParalelo, A, mSize, nSize, &resultParalel, QTD_REPETICOES_POR_FUNCAO);

	//Comparando os vetores resultantes
	diferencaMax = compResults(resultSeq, resultParalel, mSize);

	//Desalocando os vetores
	freeSeguro((void*) &resultSeq);
	freeSeguro((void*) &resultParalel);

	//Calculando o speedup
	speedup = medidasSequencial->tempo / medidasParalelo->tempo;

	//Calculando a eficiência
	eficiencia = speedup / N_THREADS;

	printf(" Ordem da matriz:           *%dx%d*\n", mSize, nSize);
	printf(" Tempo Sequencial (Ts):     *%f* seg\n", medidasSequencial->tempo);
	printf(" Tempo Paralelo (Tp): \t    *%f* seg\n", medidasParalelo->tempo);
	printf(" Desvio Padrão Sequencial:  *%f*\n", medidasSequencial->desvioPadrao);
	printf(" Desvio Padrão Paralelo:    *%f*\n\n", medidasParalelo->desvioPadrao);
	printf(" Diferenca de valor máxima: *%f*\n\n", diferencaMax);
	printf(" Speedup: \t\t    *%f* Ts/Tp\n", speedup);
	printf(" Eficiencia: \t\t    *%.2f*%%\n\n", eficiencia*100);

	//Desalocando os espaços de memória das medidas
	freeSeguro((void*) &medidasParalelo);
	freeSeguro((void*) &medidasSequencial);

	//Liberando a memória alocada pelos vetores
	freeSeguro((void*) &A);

	//Obtendo tempo final do programa
	tempoTotal = omp_get_wtime() - tempoTotal;
	printf("\n Tempo total: \t\t    *%f* seg\n", tempoTotal);
	printf("\n ***\n");

	return 0;
}

void freeSeguro(void **pointer){
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
		//Calculando a diferença absoluta entre os valores das mesmas posições dos vetores
		dif = absDouble(vet1[i] - vet2[i]);
		if(dif > max){
			max = dif;
		}
	}

	return max;
}

t_medidas * functionTime(funcPointer somaMatrizxLinha, double *A, int mSize, int nSize, double **result, int repeticao){
	double media = 0.0, tempo, variancia = 0.0, desvio, tempos[repeticao];
	int i;
	t_medidas *ptr_medidas;

	//Alocando o ponteiro para um tipo de medidas
	ptr_medidas = (t_medidas *) malloc(sizeof(t_medidas));
	if(ptr_medidas == NULL){
		printf("\nErro de alocação\n");
		exit(-1);
	}

	// Realizando chamadas à função 'repeticao' vezes e calculando a média desses tempos obtidos
	for(i = 0;i < repeticao; i++){
		tempo = omp_get_wtime();
		*result = somaMatrizxLinha(A, mSize, nSize);
		tempo = omp_get_wtime() - tempo;

		if(i < repeticao-1){
			freeSeguro((void*) result);
		}

		media += tempo;

		tempos[i] = tempo;
	}
	media /= repeticao;

	for(i=0;i < repeticao;i++){
		//Calculando os desvios
		desvio = tempos[i] - media;
		//Calculando a variância com o quadrado dos desvios
		variancia += desvio*desvio;
	}
	//Finalizando o cálculo da variância
	variancia /= (repeticao-1);

	//Calculando o desvio padrão
	ptr_medidas->desvioPadrao = sqrt(variancia);
	//Setando a média dos tempos
	ptr_medidas->tempo = media;

	return ptr_medidas;
}

double* somaMatrizxLinhaSequencial(double *A, int mSize, int nSize){
    int i, j, jStart;
    double soma;
    double *result;

    //Alocando o vetor resultado
    result = (double *) malloc(mSize * sizeof(double));
    if(result == NULL){
        printf("\nErro na alocacao.\n");
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
        printf("\nErro na alocacao.\n");
        exit(-1);
    }

    //Percorrendo a matriz em forma de vetor
    #pragma omp parallel num_threads(N_THREADS)
    {
    	int j, jStart;
    	
    	//Criando uma variável soma para cada thread
		double soma = 0.0;

    	//Utilizando o schedule dynamic para realizar os somatórios
    	#pragma omp for schedule(dynamic)
	    for(i = 0; i < mSize; i++){
	    	soma = 0.0;

	    	//Calculando a posição da coluna
	    	jStart = i*nSize;
			for(j = jStart; j < jStart+nSize; j++){
				//Realizando a soma dos elementos da linha
				soma += A[j];
			}
			//Inserindo o resultado da soma no vetor resultado
			result[i] = soma;
		}
    }

    return result;
}