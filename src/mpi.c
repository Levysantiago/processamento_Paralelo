#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MSIZE 256
#define NSIZE 512

//Projeto5: Retorna a soma dos elementos de cada linha de uma matriz
double* somaMatrizxLinha(double *A, int mSize, int nSize, int rank, int quantProc);

int main(int argc, char** argv) {
  int i, rank, quantProc;
  int mSize, nSize, mxn;
  double *A;
  double *result;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &quantProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

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
  result = somaMatrizxLinha(A, mSize, nSize, rank, quantProc);

  if(rank == 0){
    //Imprimindo o resultado 
    printf("Resultado: | ");
    for(i = 0;i < mSize; i++){
      printf("%f | ", result[i]);
    }
    printf("\n");

    free(result);
  }

  free(A);

  MPI_Finalize();
  
  return 0;
}

double* somaMatrizxLinha(double *A, int mSize, int nSize, int rank, int quantProc){
  int i, j, jStart;
  double soma;
  double *result;
  MPI_Status status;

  if(rank == 0){
    //Alocando o vetor resultado
    result = (double *) malloc(mSize * sizeof(double));
    if(result == NULL){
      printf("Erro na alocacao.");
      MPI_Finalize();
      exit(-1);
    }
  }

  //Percorrendo a matriz em forma de vetor
  for(i = 0; i < mSize; i++){
    if(i % quantProc == rank){
      soma = 0.0;
      jStart = nSize*i;
      for(j = jStart; j < jStart+nSize; j++){
        soma += A[j];
      }
      if(rank != 0){
        MPI_Send(&i, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&soma, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
      }
    }
    if(rank == 0){
      result[i] = soma;
      for(j = 1;j < quantProc;j++){
        MPI_Recv(&i, 1, MPI_INT, j, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&result[i], 1, MPI_DOUBLE, j, 0, MPI_COMM_WORLD, &status);
      }
    }
  }

  return result;
}