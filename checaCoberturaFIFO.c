/* Codigo para verificar a cobertura (global/sink)*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define RAIO 15

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

/*Vai ter problema quando tentar remover quando tiver apenas a informacao de 1 noh, mas no nosso caso isso nao ocorre, entao segue em frente*/
int primeiroFila(tipoFila F, int *nodeID, int *idPacote, float *x, float *y){
	if(filaVazia(F)) return 0;
	*nodeID = F.inicio->nodeID;
	*idPacote = F.inicio->idPacote;
	*x = F.inicio->x;
	*y = F.inicio->y;
	return 1;
}

int removeFila(tipoFila *F, int *nodeID, int *idPacote, float *x, float *y){
	tipoNoFila *aux;   
	if (filaVazia(*F)) return 0;	/* fila vazia  */  
	primeiroFila(*F,nodeID,idPacote,x,y);
	if (F->inicio == F->fim)	/*Se existir somente um nó na fila, apontamos o final da fila para NULL*/
		F->fim = NULL;   
	aux = F->inicio;		/*Atribuímos o nó retornado da fila ao nó auxiliar*/    
	F->inicio = F->inicio->prox;	/*O início da fila apontará para o próximo nó do que estamos removendo*/
	free(aux);                      
	return 1;   
} 

/*Fim - Funcoes e estruturas para a FILA*/

void main(int argc, const char*const* argv){

/*Variaveis X,Y sao o tamanho do grid e numNos, o numero de nos da rede*/
float X,Y;
int numNos;
int buffer = 0;
int elemFila = 0;
/*Matrizes de cobertura*/
int **cobertura;
int **coberturaComBuffer;

/*Intervalo de tempo*/
int t0,t;

int LIMITE_BUFFER;

/*Para fazer a remoção da fila*/
int nodeID_r,idPacote_r;
float x_r,y_r;

/*Tempo de retransmissao de pacote*/
float tempoRtt;

int i,j,a,b,nCelula,pisoX,tetoX,pisoY,tetoY,idNoR,idPacote;
int pisoXb,tetoXb,pisoYb,tetoYb;
float x,y,pCorbGlobal,pCorbSink,pCorbSinkBuffer,pCorbRelativa,pCorbRelativaBuffer,nCorbGlobal,nCorbSink,nCorbSinkBuffer;

/*Variaveis para para reconhecimento da topologia da rede*/
float *x_,*y_;
int *nodeID_;

/*Variaveis para trabalhar com arquivo*/
FILE *fp;
fp = fopen("SAIDA.txt","rt");

/*Variaveis recebem intervalo de tempo que eh passada como parametro*/
t0 = atoi(argv[1]);
t = atoi(argv[2]);
LIMITE_BUFFER = atoi(argv[3]);

/*Variaveis auxiliares do tipo noh*/
struct bufferNoMovel bufferMovel[LIMITE_BUFFER];

printf("Tempo Inicial: %d\n\nTempo Final: %d\n\n",t0,t);

/*Inicializa a fila para trabalhar com buffer FIFO*/
tipoFila fila;                                     
criaFila(&fila);

	/*Ler arquivo de entrada com os parametros (X,Y)-> Grid , numNos -> Numeros de Nohs da Rede excluindo nohs moveis, nCelula -> numero de celulas da tabela de cobertura*/
	fscanf(fp,"%f",&X);
	fscanf(fp,"%f",&Y);
	fscanf(fp,"%d",&numNos);
	nCelula = ceil(X);
	printf("X:%.2f Y:%.2f \n\nNumero de Nos:%d\n",X , Y, numNos);
	printf("\nNumero de Celulas : %d\n",nCelula);

	cobertura = (int**)malloc(sizeof(int*)*(nCelula));	//Matriz de cobertura
	coberturaComBuffer = (int**)malloc(sizeof(int*)*(nCelula));	//Matriz de cobertura com Buffer

	/*Alocacao de variaveis para checar cobertura global*/
	nodeID_ = (int*)malloc(sizeof(int*)*(numNos));		
	x_ = (float*)malloc(sizeof(float*)*(numNos));
	y_ = (float*)malloc(sizeof(float*)*(numNos));

	/*Alocacao da matriz de cobertura(sera usada tanto para verificar cobertura global e do sink*/
	for(i=0; i<nCelula; i++){					
		cobertura[i] = (int*)malloc(sizeof(int)*(nCelula));
		coberturaComBuffer[i] = (int*)malloc(sizeof(int)*(nCelula));	
	}

	/*Inicializacao da tabela de cobertura*/	
	for(i=0; i<nCelula; i++){	
		for(j=0; j<nCelula; j++){				
			cobertura[i][j] = 0;
		}
	}
	
	/*Leitura dos parametros do arquivo para calculo da cobertura global*/
	for(i=1;i<numNos;i++){
		fscanf(fp,"%d",&nodeID_[i]);
		fscanf(fp,"%f",&x_[i]);
		fscanf(fp,"%f",&y_[i]);
	}
	
	/*Ler o tempo de retransmissao*/
	fscanf(fp,"%f",&tempoRtt);
	printf("\nTempo de Rtt: %.2f\n",tempoRtt);
	/*Calculo da cobertura global*/
	for(i=1;i<numNos;i++){
		pisoX = (x_[i]-RAIO);
		tetoX = (x_[i]+RAIO);
		pisoY = (y_[i]-RAIO);
		tetoY = (y_[i]+RAIO);
	
		if((pisoX)<(0))
			pisoX = 0;
		if((tetoX)>(X))
			tetoX = (X-1);
		if((pisoY)<(0))
			pisoY = 0;
		if((tetoY)>(Y))
		tetoY = (Y-1);

		a=pisoX;
		b=pisoY;

		for (b=pisoY;b<tetoY;b++){	
			for(a=pisoX;a<tetoX;a++){
				cobertura[a][b] = 1;
			}
		}
	}

	nCorbGlobal = 0;
	for(i=0; i<nCelula; i++){	
		for(j=0; j<nCelula; j++){				
			if(cobertura[i][j]>0)
				nCorbGlobal++;
		}
	}

	pCorbGlobal = ((nCorbGlobal*100)/(nCelula*nCelula));
	printf("\nCobertura Global dos Nós da Rede: %.2f \n",pCorbGlobal);
	/*Fim do Calculo da cobertura global*/

	/*Zera a tabela de cobertura para reutilizacao no calculo da cobertura do sink*/
	for(i=0; i<nCelula; i++){	
		for(j=0; j<nCelula; j++){				
			cobertura[i][j] = 0;
		}
	}	

	
	while(!feof(fp)){
		fscanf(fp,"%d",&idNoR);
		fscanf(fp,"%f",&x);
		fscanf(fp,"%f",&y);
		fscanf(fp,"%d",&idPacote);
	
		/*Calculo da cobertura dos pacotes que chegam ao sink com buffer*/
		//if((buffer >= LIMITE_BUFFER) && (idPacote>=t0*tempoRtt) && (idPacote<=t*tempoRtt)){	/*Buffer esta cheio */
		if(buffer >= LIMITE_BUFFER){		
			removeFila(&fila ,&nodeID_r,&idPacote_r,&x_r,&y_r);			
			insereFila(&fila ,idNoR,idPacote,x,y);
		}								
		//if((buffer < LIMITE_BUFFER) && (idPacote>=t0*tempoRtt) && (idPacote<=t*tempoRtt)){	/*Buffer nao esta cheio*/
if(buffer < LIMITE_BUFFER){
			/*Insere os dados na FILA*/
			insereFila(&fila ,idNoR,idPacote,x,y);
			elemFila++;
		}
		
		buffer++;
	}
	
	if(LIMITE_BUFFER < elemFila) 
		elemFila = LIMITE_BUFFER; 

	for(buffer=0;buffer<elemFila;buffer++){
			/*Decidi passar o conteúdo da fila pra um vetor de estrutura por ser mais fácil de trabalhar*/   
			bufferMovel[buffer].nodeID = fila.inicio->nodeID;
			bufferMovel[buffer].x = fila.inicio->x;
			bufferMovel[buffer].y = fila.inicio->y;
			bufferMovel[buffer].idPacote = fila.inicio->idPacote;

			removeFila(&fila ,&nodeID_r,&idPacote_r,&x_r,&y_r); /*Aqui to desenfilerando os elementos da fila*/
			
			pisoXb = (bufferMovel[buffer].x-RAIO);
			tetoXb = (bufferMovel[buffer].x+RAIO);
			pisoYb = (bufferMovel[buffer].y-RAIO);
			tetoYb = (bufferMovel[buffer].y+RAIO);

			if((pisoXb)<(0))
				pisoXb = 0;
			if((tetoXb)>(X))
				tetoXb = (X-1);
			if((pisoYb)<(0))
				pisoYb = 0;
			if((tetoYb)>(Y))
				tetoYb = (Y-1);
	
			i=pisoXb;
			j=pisoYb;
			
			/*Aqui preenche a matriz de cobertura com o buffer FIFO*/
			for (j=pisoYb;j<tetoYb;j++){	
				for(i=pisoXb;i<tetoXb;i++){
					coberturaComBuffer[i][j] = 1; 
				}
			}
	}
	/*Verifica a porcentagem area coberta reconhecida pelo sink com buffer ilimitado*/
	nCorbSink = 0;

	for(i=0; i<nCelula; i++){	
		for(j=0; j<nCelula; j++){				
			if(cobertura[i][j]>0)
				nCorbSink++;
		}
	}

	//pCorbSink = ((nCorbSink*100)/(nCelula*nCelula));
	//printf("\nCobertura Sink com Buffer Ilimitado: %.2f \n",pCorbSink);
	/*Fim - Verifica a porcentagem area coberta reconhecida pelo sink com buffer ilimitado*/

	/*Verifica a porcentagem de area coberta reconhecida pelo sink com buffer limitado*/
	nCorbSinkBuffer = 0;
	for(i=0; i<nCelula; i++){	
		for(j=0; j<nCelula; j++){				
			if(coberturaComBuffer[i][j]>0)
				nCorbSinkBuffer++;
		}
	}
	
	pCorbSinkBuffer = ((nCorbSinkBuffer*100)/(nCelula*nCelula));
	printf("\nCobertura Sink em um Intervalo de Tempo com Buffer FIFO: %.2f \n",pCorbSinkBuffer);
	/*Fim - Verifica a porcentagem de area coberta reconhecida pelo sink com buffer limitado*/

	/*Gera a cobertura relativa recebida pelo Sink com Buffer Ilimitado, considerando a cobertura global*/
	//pCorbRelativa = ((100*nCorbSink)/nCorbGlobal);
	//printf("\nCobertura possivel da movimentação (buffer ilimitado): %.2f \n",pCorbRelativa);
	/*Fim - Gera a cobertura relativa recebida pelo Sink com Buffer Ilimitado, considerando a cobertura global*/	


	/*Gera a cobertura relativa recebida pelo Sink com Buffer limitado, considerando a cobertura global*/
	pCorbRelativaBuffer = ((100*nCorbSinkBuffer)/nCorbGlobal);
	printf("\nCobertura Relativa em um Intervalo de Tempo Global/SinkBufferFIFO: %.2f \n",pCorbRelativaBuffer);
	/*Fim - Gera a cobertura relativa recebida pelo Sink com Buffer limitado, considerando a cobertura global*/	
}
