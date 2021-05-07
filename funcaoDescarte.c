#include <stdio.h>
#include <stdlib.h>

struct bufferNoMovel {
   int nodeID,idPacote;
   float x,y;
};


typedef struct bufferNoMovel bufferMovel;

int numOcorrencia(bufferMovel *buffer, int N, int valor){
int i,cont;
cont = 0;
int aux = 0;
	for(i=0;i < N;i++){
		if(buffer[i].nodeID == valor){
			cont++; aux = i;
		}
	}

	if(cont == 0)
		return -1;
	else{
		 return aux;
	}
	
}

int main(){
int aux;
	bufferMovel vet[5];
	vet[0].nodeID = 1;
	vet[1].nodeID = 1;
	vet[2].nodeID = 2;
	vet[3].nodeID = 4;
	vet[4].nodeID = 3;
	aux = numOcorrencia(vet,5,1);
	printf("\nvalor : %d ",aux);
}
