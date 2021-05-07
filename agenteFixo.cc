#include "agenteFixo.h"

/* Construtor da classe */ 

FixedAgent::FixedAgent() : Agent(PT_FIXED_AGENT) {
	x_ = -1;
	y_ = -1;
	nodeID_ = -1;
	dist_ = -1;
	xCalcFixo_ = -1;
	yCalcFixo_ = -1;
}

 
/* A Função dist calcula a distância entre dois pontos. Ela é usada para calcular a distância entre um nó fixo
e um móvel*/
inline double distance( double x1, double y1, double x2, double y2 ) {
	return sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
}


int FixedAgent::command(int argc, const char*const* argv) {

// Comando: attach-node - Define o No' ao qual o Agente estah associado
	
	if (strcasecmp (argv[1], "attach-node") == 0) {

		assert(node_ == NULL);
		TclObject *obj; 
		if ((obj = TclObject::lookup(argv[2])) == 0) return TCL_ERROR;
		node_ = (MobileNode *) obj;                             // Seta o no' atual
		nodeID_ = node_->nodeid();
		ErroRssi = atof(argv[3]); 
		node_->getLoc(&x_, &y_, &z_);			// Acessa Informacoes de Localizacao
		printf("\n%d  \t%f \t%f  \n",nodeID_,x_,y_);    //Para realizar o calculo da cobertura total
		//printf("sdfsdf %lf ",ErroRssi);
	return TCL_OK;                                          // Retorna ao TCL
	}
	
// Se o comando nao for reconhecido por este agente, envia para a classe
	return (Agent::command(argc, argv));
}

void FixedAgent::recv(Packet* packet, Handler*) {

	node_->getLoc(&x_, &y_, &z_);			// Acessa Informacoes de Localizacao	
	
	xCalcFixo_ = Random::normal(x_,ErroRssi);			//Erro de RSSI
	yCalcFixo_ = Random::normal(y_,ErroRssi);			//Erro de RSSI
	
/* Acessa os headers do novo Pacote */
	hdr_cmn *cmh = HDR_CMN(packet);		// Acessa o Header Common
	// As proximas quatro linhas mudam a cor do no' no NAM
	char tmpTcl[100];
	sprintf(tmpTcl, "$ns_ at %f \"$node_(%d) color black\"", Scheduler::instance().clock() + 0.000001, node_->nodeid());
	Tcl& tcl = Tcl::instance();
	tcl.eval(tmpTcl);  
	/*Tah com o nome FIXED, mas esse cabecalho eh usado tanto para fixo como para movel*/
  	if ( cmh->ptype_ == PT_FIXED_AGENT ) {    		// Verifica o tipo de pacote
		hdr_mfAgents *mfh = HDR_AGENTS(packet);		// Acessa o Header do pacote enviado pelo nó móvel
		if ( mfh->nodeID() == node_->nodeid() ) return; // ? Recebi um pacote meu, retorna 
		
		//printf( "\nDADOS DO PACOTE:\n" );
		//printf( "\nNe: %d \t Xe: %f \t Ye: %f \t IDp: %d\n", mfh->nodeID_, mfh->x_, mfh->y_ ,mfh->packageID_ );
		//printf("\n");		
		//printf( "\tIDp: %d\n", mfh->packageID_ );		
		//printf( "Nr: %d \t Xr: %f \t Yr: %f\n", nodeID_, x_, y_ );
      	
		printf( "\n%d \t %f \t %f \t %d\n", nodeID_, x_, y_ , mfh->packageID_ );
/*   
		printf( "\tDistacia p/ o nodo movel: %f\n",( distance(x_ , y_ , mfh->x_, mfh->y_));

		printf( "\t%d \t x: %f \t y: %f", , ,  );
		printf( "\n %d", mfh->packageID_ );
		
		printf( "%d", mfh->packageID_ );
		printf( "\n %d %f %f %f %f\n", nodeID_, x_ ,y_ ,mfh->x_,mfh->y_ ); //Original
		printf( "\n %d %f %f %f %f %f %f \n", nodeID_, x_ ,y_,xCalcFixo_, yCalcFixo_ ,mfh->x_,mfh->y_);
		printf( "\t%f\n", distance(x_ , y_ , mfh->x_, mfh->y_) );
         
*/
      }
}


