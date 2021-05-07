#include <stdio.h>
#include <stdlib.h>
struct no {
	float info;
	struct no* prox;
};

typedef struct no No;

struct pilha {
	No* prim;
};

typedef struct pilha Pilha;

Pilha* criaPilha(void){
	Pilha* p = (Pilha*) malloc(sizeof(Pilha));
	p->prim = NULL;
return p;
}

/* função auxiliar: insere no início */
No* ins_ini (No* l, float v){
	No* p = (No*) malloc(sizeof(No));
	p->info = v;
	p->prox = l;
return p;
}
/* função auxiliar: retira do início */
No* ret_ini (No* l){
	No* p = l->prox;
	free(l);
return p;
}

void push (Pilha *p, float v){
	p->prim = ins_ini(p->prim,v);
}

void pop (Pilha* p){
float v;
	if (vazia(p)){
		printf("Pilha vazia.\n");
	exit(1);
	/* aborta programa */
	}

	//v = p->prim->info;
	p->prim = ret_ini(p->prim);

//return v;
}

int vazia (Pilha* p) {
	return (p->prim==NULL);
}

int main(){
	Pilha *P;
	P = criaPilha();
	push(P, 1.2);
	push(P, 2.6);
	//pop(P);
	printf("Valor no topo: %.2f\n",P->prim->info);
}

