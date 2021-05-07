#include <stdio.h>
#include <stdlib.h>

int ordena(double x[], double y[],int n)
{
   int i, j,aux,aux2;


	for ( j = 1 ; j < n ; j++){
		aux = x[j];
		aux2 = y[j];
		i = j - 1;
     
	while((i>=0) && (x[i]>aux)){      
		x[i+1] = x[i];
		y[i+1] = y[i];
		i = i - 1;
	}
		x[i+1] = aux;
		y[i+1] = aux2;
	}
	return 0;
}
//------------------------------------------- Funcoes Lagrange -------------------------------
/*
Funcao que faz o calculo da interpolacao pelo Polinomio de Lagrange 
Entradas : valor a ser interpolado, o grau do polinomio,o vetor das abcissas, vetor das ordenadas
Saida :Valor interpolado
*/

void polinomioLagrange(double xValor, int n, double x[], double y[]){	
    
int i, j;
double mult;
double l = 0;

	for(i = 0 ; i < n ; i++){
	
		mult = y[i];           
	
		for(j=0;j<n;j++){
		
			if(i!=j)
	
				mult = (mult*((xValor - x[j])/(x[i] - x[j])));
			

		}

	l = l + mult;	

	}	
	printf("\t\n%lf  %lf \n",xValor,l);
     
}    
             


//-----------------------------------Fim das Funcoes utilizadas por Lagrange-------------

int TrataDados(FILE *fp){

#define TAM 4

int id[TAM];
double x[TAM];
double y[TAM];
double x2,y2;
int i,id2;
	
	//while(!feof(fp)){
		
		for (i=0; i < TAM -1 ; i++){
		
			fscanf(fp,"%d",&id[i]);
			fscanf(fp,"%lf",&x[i]);		
			fscanf(fp,"%lf",&y[i]);
			
		}
		
		fscanf(fp,"%d",&id2);		
		fscanf(fp,"%lf",&x2);
		fscanf(fp,"%lf",&x2);
		
		fscanf(fp,"%d",&id[TAM -1]);
		fscanf(fp,"%lf",&x[TAM -1]);		
		fscanf(fp,"%lf",&y[TAM -1]);
			

		ordena(x,y,4);
		
		//for(i=0;i<TAM;i++){
		//	printf("\n %d - %lf - %lf",id[i],x[i],y[i]);
			
	//	}	
		
		polinomioLagrange(x2,3,x,y);
	//}
			
}

int main(int argc, char *argv[]){
	
	FILE *fp;   
	fp = fopen(argv[1],"r");

 	//freopen("entrada","r",stdin);	 
 	//freopen("rastreamento","w",stdout);	 
	TrataDados(fp);

	return 0;		
	
}
