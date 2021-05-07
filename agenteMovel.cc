#include "agenteMovel.h"  

int hdr_mfAgents::offset_ ;		// offset eh um membro estatico da estrutura hdr_AgenteMovel

/* Construtor da classe AgenteMovel */
 
AgenteMovel::AgenteMovel() : Agent(PT_MOBILE_AGENT), timer_(this) {
// Como o id e as coordenadas do nó ainda não foram identificados atribuímos o valor -1 
	nodeID_ = -1;
	x_ = -1;
	y_ = -1; 
	z_ = -1;
	packageID_ = -1;
}


void TimerMovel::expire( Event *e ) {
	a_->broadcast();
}
 
int AgenteMovel::command(int argc, const char*const* argv) {
	
	//float packet_resched;
	// Comando: broadcast - Faz com que o no' atual faça um broadcast
	if (strcmp(argv[1], "broadcast") == 0) {
    
	// Seta a posicao Perfeita do No' Sink
		// nodeID_ = node_->nodeid(); ocorre um erro quando se descomenta esse comando
		time_resched = atof(argv[2]);
		printf("\n%f\n",time_resched);		
		broadcast();				// Envia o pacote de posicao
		
		return TCL_OK;					// Retorna OK ao TCL
	
	}

// Comando: attach-node - Define o No' ao qual o Agente estah associado
	if ( strcasecmp (argv[1], "attach-node") == 0 ) {
		assert(node_ == NULL);
		TclObject *obj;
		if ((obj = TclObject::lookup(argv[2])) == 0) return TCL_ERROR;
			node_ = (MobileNode *) obj;			// Seta o no' atual
			nodeID_ = node_->nodeid();
			// As proximas quatro linhas mudam a cor do no' no NAM
			char tmpTcl[100];
			sprintf(tmpTcl, "$ns_ at %f \"$node_(%d) color red\"", Scheduler::instance().clock() + 0.000001, node_->nodeid());
			Tcl& tcl = Tcl::instance();
			tcl.eval(tmpTcl);

	return TCL_OK;						// Retorna ao TCL
	
	}

// Se o comando nao for reconhecido por este agente, envia para a classe
	return (Agent::command(argc, argv));
}

/* Metodo que envia um pacote por broadcast */
void AgenteMovel::broadcast() {
// As proximas quatro linhas mudam a cor do no' no NAM
	packageID_++;
// Cria um Novo Pacote para ser Enviado
	Packet *packet = allocpkt();                              // Aloca memoria para o pacote

// Acessa os headers do novo Pacote
	hdr_cmn *cmh = HDR_CMN(packet);                           // Acesso ao Header Common
	hdr_ip  *iph = HDR_IP(packet);                            // Acesso ao Header do IP
	hdr_mfAgents *mah = HDR_AGENTS(packet);                 // Acessa o Header do Pacote do APS de Beacon Position

// Preenche o Pacote com as informacoes necessarias de Roteamento
	cmh->direction() = hdr_cmn::DOWN;                         // Direcao do Pacote
	cmh->addr_type_  = NS_AF_INET;                            // Endereco IP
	cmh->next_hop_   = IP_BROADCAST;                          // Sera feito um Broadcast
	cmh->ptype_      = PT_FIXED_AGENT;                       // Tipo de Pacote = MyFlooding
	iph->dport()     = iph->sport();                          // Porta de Destino
	iph->daddr()     = IP_BROADCAST;                          // Sera feito um Broadcast

// Preenche o Pacote com as informacoes do no'
	node_->getLoc(&x_, &y_, &z_);			// Acessa Informacoes de Localizacao
	mah->x()     = x_;                                // Seta a posicao X do nó no Pacote a ser enviado
	mah->y()     = y_;                                // Seta a posicao Y do nó no Pacote a ser enviado
	mah->packageID() = packageID_;
	mah->nodeID()    = nodeID_; 
	RNG random;
	Scheduler::instance().schedule(target_, packet, random.uniform(0, 0)); 
	timer_.resched(time_resched);
}
