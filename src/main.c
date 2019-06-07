/*Ordenador multithread
*
*Autora: Mariana Chen 
* Ultima modificação: 07Jun2019
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define MAX_THREADS 2
#define MAX_ENTRADAS 100000 
#define ATIVO 1
#define INATIVO 0

typedef struct {

	int *vetor;
	int comeco;
	int fim; 	

} thread_args;

pthread_mutex_t trava;
pthread_mutex_t trava1;
pthread_mutex_t trava2;
pthread_t workers[MAX_THREADS]; 
int disponivel[MAX_THREADS]; //disponibilidade de uma thread
int entradas[MAX_ENTRADAS];

void merge(int vetor[],int comeco, int meio,int fim);
void mergeSort(int vetor[],int comeco, int fim);
void *worker(void *arg);


int main(){


/*Leitura da entrada*/
	int numero;
	numero = '\0';
	int contador = 0;
	while(scanf("%d ", &numero) != EOF){
		entradas[contador] = numero;
		//printf("Recebi: %d\n", entradas[contador]);		
		contador++; //conta quantos numeros serao tratados 
   	}
	//printf("contei %d numeros \n",contador);
	
/*Threads*/
	thread_args *send_args;
	send_args = (thread_args*)malloc(sizeof(thread_args));

	thread_args *send_args2;
	send_args2 = (thread_args*)malloc(sizeof(thread_args));

	int aux = contador/2;
//Thread esquerda
	send_args ->vetor = entradas;
	send_args ->comeco = 0;
	send_args ->fim = aux-1;
	
	
	pthread_mutex_lock(&trava);
	disponivel[0] = ATIVO;
	pthread_create(&(workers[0]), NULL, worker, (void*) send_args);
	pthread_mutex_unlock(&trava);

//Thread direita
	
	send_args2 ->vetor = entradas;
	send_args2 ->comeco = aux;
	send_args2 ->fim = contador-1;
	

	pthread_mutex_lock(&trava);
	disponivel[1] = ATIVO;
	pthread_create(&(workers[1]), NULL, worker, (void*) send_args2);
	pthread_mutex_unlock(&trava);


	/*Verificar se todas as threads terminaram de processar*/
	for(int i=0;i<MAX_THREADS;i++){
		if(disponivel[i] == ATIVO){
			pthread_join(workers[i],NULL);
		}
	}
	
	/*Juntar vetores*/

	//printf("Juntou?\n");
	merge(entradas,0,aux,contador-1);
	


	/*Imprimir na tela*/ 
	for(int i=0;i<contador;i++){
		printf("%d",entradas[i]);
		if(i != contador-1){
		printf(" ");
		}
	}
	printf("\n");
	//printf("\n FIM \n");

 return 0;
} 

void merge(int vetor[],int comeco, int meio,int fim){

	//printf("Intercalar\n");
	int tam = fim-comeco+1;
	int *aux;
	aux = (int*)malloc(tam*sizeof(int));

	int in1=comeco;
	int in2=meio;
	int fim1=in2-1;
	int au=0;

	while(in1<=fim1 && in2<=fim){
		if (vetor[in1]<vetor[in2]){
			aux[au++] = vetor[in1++];
		}
		else{
			aux[au++] = vetor[in2++];
		}
	}
	while(in1<=fim1){
		aux[au++] = vetor[in1++];
	}
	while(in2<=fim){
		aux[au++] = vetor[in2++];
	}

	for(int i=0;i<au;i++){
		vetor[i+comeco]=aux[i];
	}
}

void mergeSort (int vetor[],int comeco, int fim){
	int meio;
	if(comeco<fim){
		meio=(comeco+fim)/2;
		mergeSort(vetor,comeco,meio);
		mergeSort(vetor,meio+1,fim);
		merge(vetor,comeco,meio+1,fim);  
	}
}

void *worker(void *arg){

	thread_args *info = (thread_args *)arg;
	pthread_mutex_lock(&trava);
//	printf("Merge\n");
	mergeSort(info->vetor,info->comeco, info->fim);
	/*printf("Antes de juntar -> comeco: %d:", info->comeco);
	for(int i=info->comeco;i<info->fim;i++){
	printf("%d ", info->vetor[i]);
	}
	printf("\n");*/

	disponivel[0] = INATIVO;
	pthread_mutex_unlock(&trava);
	
	return NULL;

}
