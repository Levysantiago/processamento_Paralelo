/** 
*
* Projeto 5: Retorna a soma dos elementos de cada linha de uma matriz
* Feito por: Levy Santiago e Adson Cardoso
*
* Compilando: $ mpicc mpi.c -o mpi -lm
* Executando: & mpirun -np <num_processos> mpi <linhas> <colunas>
*
**/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Quantidade padrão de linhas da matriz
#define MSIZE 256

//Quantidade padrão de colunas da matriz
#define NSIZE 512

//Quantidade de repetições para a chamada das funções em sequencial e paralelo 
#define QTD_REPETICOES_POR_FUNCAO 20

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
void freeSeguro(void **pointer);

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
*	Essa é a função que calcula a soma dos elementos de cada linha de uma matriz sequencialmente.
**/
double* somaMatrizxLinha(double *A, int mSize, int nSize);

int main(int argc, char** argv) {
	int i, rank, quantProc;
	int mSize, nSize, mxn, peace_per_proc, lines_per_proc;
	double speedup, eficiencia, diferencaMax, tempo, aux;
	double *A, *A2;
	double *result, *result2, *resultSeq;
	t_medidas *ptr_medidas, *ptr_medidas2, *ptr_medidasSeq;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &quantProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(rank == 0){
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
	}

	if(rank == 0){
		//Para chegar em Gigabytes: (1024)^3 = 1073741824
		printf("\n Memória para a matriz:     *%f* Gigas\n\n", (double) (sizeof(double)*nSize*mSize)/(1073741824));

		//Alocando a matriz completa para o rank 0 como forma de um vetor
		A = (double *) malloc(mSize * nSize * sizeof(double));
		if(A == NULL){
			printf("Erro na alocacao.");
			exit(-1);
		}

		//Alocando o vetor resultado final
		result = (double *) malloc(mSize * sizeof(double));
		if(result == NULL){
			printf("Erro na alocacao.");
			exit(-1);
		}

		//Alocando o ponteiro para um tipo de medidas
		ptr_medidas = (t_medidas *) malloc(sizeof(t_medidas));
		if(ptr_medidas == NULL){
			printf("\nErro de alocação\n");
			exit(-1);
		}

		//Definindo semente
		srandom(123456789);

		//Preenchendo a matriz com valores aleatórios, dado a semente
		mxn = mSize*nSize;
		for(i = 0; i < mxn; i++){
			A[i] = ((double)random() / RAND_MAX)*2.0 - 1.0;
		}

		/*Calculando de forma sequencial para comparar os vetores e tempo*/
		ptr_medidasSeq = functionTime(&somaMatrizxLinha, A, mSize, nSize, &resultSeq, QTD_REPETICOES_POR_FUNCAO);

		tempo = MPI_Wtime();

		//Calculando a quantidade de linhas para cada processo
		lines_per_proc = mSize / quantProc;
		// Calculando as partes da matriz para cada processo
		peace_per_proc = lines_per_proc * nSize;
		
		tempo = MPI_Wtime() - tempo;
	}

	if(rank == 0){
		aux = MPI_Wtime();
	}

	//Repassando a quantidade de linhas e colunas da matriz para os outros ranks
	MPI_Bcast(&mSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&nSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
	//Repassando os cálculos dos pedaços da matriz e da quantidade de linhas por processo para os outros processos
	MPI_Bcast(&peace_per_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&lines_per_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);

	//Alocando a matriz para todos os ranks como forma de um vetor
	A2 = (double *) malloc(peace_per_proc * sizeof(double));
	if(A2 == NULL){
		printf("Erro na alocacao de A2.");
		exit(-1);
	}

	//Dividindo a matriz para os ranks igualmente
	MPI_Scatter(A, peace_per_proc, MPI_DOUBLE, A2, peace_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	if(rank == 0){
		aux = MPI_Wtime() - aux;
		printf(" %f \n", aux);
		tempo += aux;
	}

	//Calculando a soma de cada linha que cada processo pegou
	ptr_medidas2 = functionTime(&somaMatrizxLinha, A2, lines_per_proc, nSize, &result2, QTD_REPETICOES_POR_FUNCAO);
	//result2 = somaMatrizxLinha(A2, lines_per_proc, nSize);
	
	if(rank == 0){
		aux = MPI_Wtime();
	}
	//Juntando todas as partes dos vetores soma com o vetor soma do rank 0
	MPI_Gather(result2, lines_per_proc, MPI_DOUBLE, result, lines_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	if(rank == 0){
		aux = MPI_Wtime() - aux;
		tempo += aux;
	}
	MPI_Reduce(&ptr_medidas2->tempo, &ptr_medidas->tempo, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&ptr_medidas2->desvioPadrao, &ptr_medidas->desvioPadrao, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if(rank == 0){
		ptr_medidas->tempo += tempo;

		//Comparando os vetores resultantes
		diferencaMax = compResults(resultSeq, result, mSize);

		//Calculando o speedup
		speedup = ptr_medidasSeq->tempo / ptr_medidas->tempo;

		//Calculando a eficiência
		eficiencia = speedup / quantProc;

		printf(" Ordem da matriz:           *%dx%d*\n", mSize, nSize);
		printf(" Tempo Sequencial (Ts):     *%f* seg\n", ptr_medidasSeq->tempo);
		printf(" Tempo MPI (Tp): \t    *%f* seg\n", ptr_medidas->tempo);
		printf(" Desvio Padrão Sequencial:  *%f*\n", ptr_medidasSeq->desvioPadrao);
		printf(" Desvio Padrão MPI:         *%f*\n\n", ptr_medidas->desvioPadrao);
		printf(" Diferenca de valor máxima: *%f*\n\n", diferencaMax);
		printf(" Speedup: \t\t    *%f* Ts/Tp\n", speedup);
		printf(" Eficiencia: \t\t    *%.2f*%%\n\n", eficiencia*100);
	}

	//Desalocando os vetores
	if(rank == 0){
		freeSeguro((void*) &A);
		freeSeguro((void*) &result);
		freeSeguro((void*) &resultSeq);
		freeSeguro((void*) &ptr_medidasSeq);
		freeSeguro((void*) &ptr_medidas);
	}
	freeSeguro((void*) &A2);
	freeSeguro((void*) &result2);
	freeSeguro((void*) &ptr_medidas2);

	MPI_Finalize();

	return 0;
}//Fim_main()

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
		tempo = MPI_Wtime();
		*result = somaMatrizxLinha(A, mSize, nSize);
		tempo = MPI_Wtime() - tempo;

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
}//Fim_functionTime()

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