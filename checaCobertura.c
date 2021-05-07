/* Codigo para verificar a cobertura (global/sink)*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define RAIO 15
#define LIMITE_BUFFER 100
void main(int argc, const char*const* argv){
/*Variaveis X,Y sao o tamanho do grid e numNos, o numero de nos da rede*/
float X,Y;
int numNos;
int buffer = 0;
/*Matrizes de cobertura*/
int **cobertura;
int **coberturaComBuffer;

/*Intervalo de tempo*/
int t0,t;

/*Tempo de retransmissao de pacote*/
float tempoRtt;

int i,j,a,b,nCelula,pisoX,tetoX,pisoY,tetoY,idNoR,idPacote;
int pisoXb,tetoXb,pisoYb,tetoYb;
float x,y,pCorbGlobal,pCorbSink,pCorbSinkBuffer,pCorbRelativa,pCorbRelativaBuffer,nCorbGlobal,nCorbSink,nCorbSinkBuffer;

/*Variaveis para para reconhecimento da topologia da rede*/
float *x_,*y_;
int *nodeID_;

/*Estrutura que representa o buffer do noh movel (mula)*/
struct bufferNoMovel {
   int nodeID,idPacote;
   float x,y;
};

struct bufferNoMovel bufferMovel[100];

/*Variaveis para trabalhar com arquivo*/
FILE *fp;
fp = fopen("SAIDA.txt","rt");

/*Variaveis recebem intervalo de tempo que eh passada como parametro*/
t0 = atoi(argv[1]);
t = atoi(argv[2]);
printf("Tempo Inicial: %d\n\nTempo Final: %d\n\n",t0,t);

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

	/*Calculo da cobertura dos pacotes que chegam ao sink com buffer limitado e ilimitado*/
	while(!feof(fp)){
		fscanf(fp,"%d",&idNoR);
		fscanf(fp,"%f",&x);
		fscanf(fp,"%f",&y);
		fscanf(fp,"%d",&idPacote);
	
		/*Calculo da cobertura dos pacotes que chegam ao sink*/
		pisoX = (x-RAIO);
		tetoX = (x+RAIO);
		pisoY = (y-RAIO);
		tetoY = (y+RAIO);

		if((pisoX)<(0))
			pisoX = 0;
		if((tetoX)>(X))
			tetoX = (X-1);
		if((pisoY)<(0))
			pisoY = 0;
		if((tetoY)>(Y))
			tetoY = (Y-1);
	
		i=pisoX;
		j=pisoY;

		for (j=pisoY;j<tetoY;j++){	
			for(i=pisoX;i<tetoX;i++){
				cobertura[i][j] = 1;
			}
		}
		/*Fim - Calculo da cobertura dos pacotes que chegam ao sink*/

		/*Calculo da cobertura dos pacotes que chegam ao sink com buffer*/
		if((buffer<LIMITE_BUFFER)&&(idPacote>=t0*tempoRtt)&&(idPacote<=t*tempoRtt)){
			/*Preenche o buffer do noh Movel*/
			bufferMovel[buffer].nodeID = idNoR;
			bufferMovel[buffer].x = x;
			bufferMovel[buffer].y = y;
			bufferMovel[buffer].idPacote = idPacote;

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

			for (j=pisoYb;j<tetoYb;j++){	
				for(i=pisoXb;i<tetoXb;i++){
					coberturaComBuffer[i][j] = 1;
				}
			}
		}

		buffer++;
		/*Fim - Calculo da cobertura dos pacotes que chegam ao sink com buffer*/	

	}
	
	/*Verifica a porcentagem area coberta reconhecida pelo sink com buffer ilimitado*/
	nCorbSink = 0;

	for(i=0; i<nCelula; i++){	
		for(j=0; j<nCelula; j++){				
			if(cobertura[i][j]>0)
				nCorbSink++;
		}
	}

	pCorbSink = ((nCorbSink*100)/(nCelula*nCelula));
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
	printf("\nCobertura Sink em um Intervalo de Tempo com Buffer Limitado: %.2f \n",pCorbSinkBuffer);
	/*Fim - Verifica a porcentagem de area coberta reconhecida pelo sink com buffer limitado*/

	/*Gera a cobertura relativa recebida pelo Sink com Buffer Ilimitado, considerando a cobertura global*/
	pCorbRelativa = ((100*nCorbSink)/nCorbGlobal);
	//printf("\nCobertura Relativa Global/SinkBufferIlimitado: %.2f \n",pCorbRelativa);
	/*Fim - Gera a cobertura relativa recebida pelo Sink com Buffer Ilimitado, considerando a cobertura global*/	


	/*Gera a cobertura relativa recebida pelo Sink com Buffer limitado, considerando a cobertura global*/
	pCorbRelativaBuffer = ((100*nCorbSinkBuffer)/nCorbGlobal);
	printf("\nCobertura Relativa em um Intervalo de Tempo Global/SinkBufferLimitado: %.2f \n",pCorbRelativaBuffer);
	/*Fim - Gera a cobertura relativa recebida pelo Sink com Buffer limitado, considerando a cobertura global*/	
}
