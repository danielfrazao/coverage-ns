#include <stdio.h>
#include <stdlib.h>

/*Estrutura que representa o buffer do noh movel (mula)*/
struct bufferNoMovel {
   int nodeID,idPacote;
   float x,y;
};

/*Funcoes e estruturas para a FILA*/
/*Definicao da estrutura do noh da fila*/
typedef struct noFila {
	int nodeID,idPacote;
	float x,y;
	struct noFila *prox;
}tipoNoFila;

/*Definicao da estrutura do tipo fila */
typedef struct tipoFila {
	tipoNoFila *inicio;
	tipoNoFila *fim;	
} tipoFila;

/*Funcao para cria um fila vazia*/
void criaFila(tipoFila *F){
	F->inicio = F->fim = NULL;
}

int filaVazia(tipoFila F){
	if((F.inicio == NULL) && (F.fim == NULL)) 
		return 1;   
	else return 0;   
}  


int insereFila(tipoFila *F, int nodeID, int idPacote, float x, float y){
	tipoNoFila *novo;
	novo = (tipoNoFila*)malloc(sizeof(tipoNoFila));
	if (novo ==  NULL) 
		return 0;	/* Erro ao alocar o novo nó */ 
	novo->nodeID = nodeID;
	novo->idPacote = idPacote;
	novo->x = x;
	novo->y = y;
	novo->prox = NULL;	/*Atribuição do valor e apontando o próximo para NULL*/
	if(filaVazia(*F))
		F->inicio = novo;	/* Se a fila estiver vazia, o novo nó sera o primeiro da fila*/
	else
		F->fim->prox = novo;	/*Senão, o ponteiro para o final da fila apontará para o novo nó*/
	F->fim = novo;
	return 1;
  
}	

int primeiroFila(tipoFila F, int *nodeID, int *idPacote, float *x, float *y){
	if(filaVazia(F)) return 0;
	*nodeID = (F.inicio)->nodeID;
	*idPacote = F.inicio->idPacote;
	*x = F.inicio->x;
	*y = F.inicio->y;
	return 1;
}

int removeFila(tipoFila *F, int *nodeID, int *idPacote, float *x, float *y){
	tipoNoFila *aux;   
	if (filaVazia(*F))return 0;	/* fila vazia  */  
	primeiroFila(*F,nodeID,idPacote,x,y);
	if (F->inicio == F->fim)	/*Se existir somente um nó na fila, apontamos o final da fila para NULL*/
		F->fim = NULL;   
	aux = F->inicio;		/*Atribuímos o nó retornado da fila ao nó auxiliar*/    
	F->inicio = (F->inicio)->prox;	/*O início da fila apontará para o próximo nó do que estamos removendo*/
	free(aux);                      
	return 1;   
} 

/*Fim - Funcoes e estruturas para a FILA*/


int main(){   
	tipoFila fila;
	tipoFila aux                                     
	int nodeID,idPacote,i;
	float x,y;   
	criaFila(&fila);      
	criaFila(&aux);
	insereFila(&fila ,1,1,10,10);
	insereFila(&fila ,2,2,20,20);
	removeFila(&fila ,&nodeID,&idPacote,&x,&y);			
	insereFila(&fila ,3,3,30,30);			
	printf("\n%d - %d - %f - %f\n", fila.inicio->nodeID,fila.inicio->idPacote,fila.inicio->x,fila.inicio->y);  
	removeFila(&fila ,&nodeID,&idPacote,&x,&y); 
	printf("\n%d - %d - %f - %f\n", fila.inicio->nodeID,fila.inicio->idPacote,fila.inicio->x,fila.inicio->y);  
	aux = fila->inicio;
	fila->inicio = (fila->inicio)->prox;	
	while((fila.inicio->prox != NULL)){   
		printf("\nValor : %d\n",i);
		i++;	
	}   

	return 0;   
}  

