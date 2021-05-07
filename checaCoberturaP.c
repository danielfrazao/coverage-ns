/* Codigo para verificar a cobertura (global/sink)*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define RAIO 15

struct bufferNoMovel {
   int nodeID,idPacote;
   float x,y;
};


typedef struct bufferNoMovel bufferMovel;

void main(int argc, const char*const* argv){
int nelem=0;
int z=0;
/*Variaveis X,Y sao o tamanho do grid e numNos, o numero de nos da rede*/
float X,Y;
int numNos;
int buffer = 0;
int aux = 0;
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
float x,y,pCorbGlobal,pCorbSink,pCorbSinkBuffer,pCorbRelativa,pCorbRelativaBuffer,nCorbGlobal,nCorbSink,nCorbSinkBuffer,nCorbA,nCorbB,pCorbB,pCorbA;

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
printf("Tempo Inicial: %d\n\nTempo Final: %d\n\n",t0,t);


/*Variaveis auxiliares do tipo noh*/
bufferMovel bufferMovel[LIMITE_BUFFER];
//bufferMovel bufferMovelAux[];

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
	/*Inicializacao da tabela de cobertura*/	
	for(i=0; i<nCelula; i++){	
		for(j=0; j<nCelula; j++){				
			coberturaComBuffer[i][j] = 0;
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
	
		if(buffer < LIMITE_BUFFER){
			bufferMovel[buffer].nodeID = idNoR;
			bufferMovel[buffer].x = x;
			bufferMovel[buffer].y = y;
			bufferMovel[buffer].idPacote = idPacote;
			nelem++;
		}

		if(buffer >= LIMITE_BUFFER){
			for(z=0;z<LIMITE_BUFFER;z++){
				pisoX = (bufferMovel[z].x-RAIO);
				tetoX = (bufferMovel[z].x+RAIO);
				pisoY = (bufferMovel[z].y-RAIO);
				tetoY = (bufferMovel[z].y+RAIO);
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
			}
		
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

			nCorbA = 0;
	
			for(i=0; i<nCelula; i++){	
				for(j=0; j<nCelula; j++){				
					if(cobertura[i][j]>0)
						nCorbA++;
				}
			}

			pCorbA = ((nCorbA*100)/(nCelula*nCelula));

				
			for(i=0; i<nCelula; i++){	
				for(j=0; j<nCelula; j++){				
					cobertura[i][j] = 0;
				}
			}	

			for(z=0;z<LIMITE_BUFFER;z++){
				pisoX = (bufferMovel[z].x-RAIO);
				tetoX = (bufferMovel[z].x+RAIO);
				pisoY = (bufferMovel[z].y-RAIO);
				tetoY = (bufferMovel[z].y+RAIO);
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
			}

			for(aux=0;i<LIMITE_BUFFER;aux++){

				pisoX = (bufferMovel[aux].x-RAIO);
				tetoX = (bufferMovel[aux].x+RAIO);
				pisoY = (bufferMovel[aux].y-RAIO);
				tetoY = (bufferMovel[aux].y+RAIO);

					if((pisoX)<(0))	/*Zera a tabela de cobertura para reutilizacao no calculo da cobertura do sink*/	
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
			
				nCorbB = 0;

					for(i=0; i<nCelula; i++){	
						for(j=0; j<nCelula; j++){				
							if(cobertura[i][j]>0)
								nCorbB++;
						}
					}
		
				pCorbB = ((nCorbB*100)/(nCelula*nCelula));

					//if(aux > 0){
					if(pCorbA>pCorbB){
						bufferMovel[aux].nodeID = idNoR;
						bufferMovel[aux].x = x;
						bufferMovel[aux].y = y;
						bufferMovel[aux].idPacote = idPacote;	
					break;					
					}
			
			}
		}
		buffer++;
	}//while

	if(nelem>LIMITE_BUFFER)
		nelem = LIMITE_BUFFER;
	for(aux=0;aux<nelem;aux++){
			pisoXb = (bufferMovel[aux].x-RAIO);
			tetoXb = (bufferMovel[aux].x+RAIO);
			pisoYb = (bufferMovel[aux].y-RAIO);
			tetoYb = (bufferMovel[aux].y+RAIO);

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
	printf("\nCobertura Sink em um Intervalo de Tempo com Buffer Pcorb: %.2f \n",pCorbSinkBuffer);
	/*Fim - Verifica a porcentagem de area coberta reconhecida pelo sink com buffer limitado*/

	/*Gera a cobertura relativa recebida pelo Sink com Buffer Ilimitado, considerando a cobertura global*/
	//pCorbRelativa = ((100*nCorbSink)/nCorbGlobal);
	//printf("\nCobertura Relativa Global/SinkBufferIlimitado: %.2f \n",pCorbRelativa);
	/*Fim - Gera a cobertura relativa recebida pelo Sink com Buffer Ilimitado, considerando a cobertura global*/	


	/*Gera a cobertura relativa recebida pelo Sink com Buffer limitado, considerando a cobertura global*/
	pCorbRelativaBuffer = ((100*nCorbSinkBuffer)/nCorbGlobal);
	printf("\nCobertura Relativa em um Intervalo de Tempo Global/SinkBufferPcorb: %.2f \n",pCorbRelativaBuffer);
	/*Fim - Gera a cobertura relativa recebida pelo Sink com Buffer limitado, considerando a cobertura global*/	
}
