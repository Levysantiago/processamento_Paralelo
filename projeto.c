#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define VSIZE 128
#define MSIZE 256
#define NSIZE 512

//Projeto5: Retorna a soma dos elementos de cada linha de uma matriz
double* somaMatrizxLinha(double *A, int msize, int nsize);

int main( int argc, char** argv )
{
  int vSize, mSize, nSize;
  double *x;
  double *A;
  double *V;

  int i, j;

  if(argc > 1)
    vSize = atoi(argv[1]);
  else
    vSize = VSIZE;

  if(argc > 2)
    mSize = atoi(argv[2]);
  else
    mSize = MSIZE;

  if(argc > 3)
    nSize = atoi(argv[3]);
  else
    nSize = NSIZE;

  x = (double *) malloc(vSize * sizeof(double));
  A = (double *) malloc(mSize * nSize * sizeof(double));
  srandom(123456789);

  for(i = 0; i < vSize; i++)
    x[i] = ((double)random() / RAND_MAX)*2.0 - 1.0;

  //Percorrer a matriz por linhas
  for(i = 0; i < mSize; i++)
    for(j = 0; j < nSize; j++)
      A[i*nSize + j] = ((double)random() / RAND_MAX)*2.0 - 1.0;

  //Percorrer a matriz por colunas
  for(j = 0; j < nSize; j++)
    for(i = 0; i < mSize; i++)
      A[j*mSize + i] = ((double)random() / RAND_MAX)*2.0 - 1.0;

  for(i = 0; i < mSize*nSize; i++)
    A[i] = ((double)random() / RAND_MAX)*2.0 - 1.0;

  V = somaMatrizxLinha(A, mSize, nSize); 
  
  free(x);
  free(A);
  return 0;
}

double* somaMatrizxLinha(double *A, int msize, int nsize){
    int i, j;
    double soma = 0.0;
    double *vet;

    vet = malloc(nsize * sizeof(double));
    if(vet = NULL){
        printf("Erro na alocacao.");
        exit(-1);
    }

    for(i = 0; i < msize; i++){
      for(j = 0; j < nsize; j++){
        soma += A[i*nsize + j];
      }
      vet[i] = soma;
      soma = 0.0;
    }

    return vet;
}
