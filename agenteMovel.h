
#ifndef ns_agenteMovel_h
#define ns_agenteMovel_h

#include "agent.h"
#include "packet.h"
#include "ip.h"
#include "scheduler.h" //para os gerenciadores de eventos
#include "mobilenode.h"
#include "random.h"
#include "timer-handler.h"

//#include "fwTable.h"

#define HDR_AGENTS(p) (hdr_mfAgents::access(p)) // acessa o pacote p.

/* Obs: Em uma struct todos os membros (tanto de dados como funções) são públicos por default */

struct hdr_mfAgents { 	// Header do pacote dos agentes fixo e móvel
	int nodeID_; 		// id do nó que enviou o pacote
	int packageID_;		// id do pacote
	double x_; 		// posição x do nó
	double y_;		// posição y do nó
	double time_resched;
	
	/* Métodos para acessar os dados do pacote */
	int& nodeID() { return (nodeID_); }
	int& packageID() { return (packageID_); }
	double& x() { return (x_); }
	double& y() { return (y_); }

	/* Metodos de Acesso ao Header */
  	static int offset_;	//   Utilizado pelo PacketHeaderManager
  	
  	inline static int& offset() { return offset_; }
  	
  	inline static hdr_mfAgents* access(const Packet* p) {
    		return (hdr_mfAgents*) p->access(offset_);
    	}
};


class AgenteMovel;

class TimerMovel : public TimerHandler {
	public:
		TimerMovel ( AgenteMovel *a ) : TimerHandler() { a_ = a ; }
	protected:
		virtual void expire( Event *e );
		AgenteMovel *a_ ;
		
};


class AgenteMovel : public Agent {
	public:						// Metodos e Propriedades Publicas
	AgenteMovel();                              	// Metodo Construtor
  	int  command(int argc, const char*const* argv); // Metodo que executa comandos enviados pelo TCL
  	
	void broadcast();  				// envia pacote por broadcast (cria esse novo pacote)
  	
  	/* Metodos e Propriedades Privadas (protegidas) */
	
	protected:
	
	int    nodeID_;		// Id do nó
  	double x_;		// Posicao X do nó
  	double y_;          	// Posicao Y do nó
  	double z_;
	double time_resched;;
  	int packageID_;		// id do pacote    
  	MobileNode *node_;	// Ponteiro para o No do Agente, utilizado para saber Posicao deste No'
	
	TimerMovel timer_;
	
	/* Se vamos usarmos vamos ter que modificar ?!
  	int debug_;		 Nivel de Debug do Agente */
};





static class AgenteMovelHeaderClass : public PacketHeaderClass {// Classe do Pacote
	public:
		AgenteMovelHeaderClass() : PacketHeaderClass("PacketHeader/AgenteMovel", sizeof(hdr_mfAgents)) {
    			bind_offset(&hdr_mfAgents::offset_);
  		}
} class_AgenteMovel_hdr;

static class AgenteMovelClass : public TclClass {            // Classe do MobileAgent
	public:
  		AgenteMovelClass() : TclClass("Agent/AgenteMovel") {}
		TclObject* create(int, const char*const*) {
    			return (new AgenteMovel());
  		}
} class_AgenteMovel;

#endif /* ns_agenteMovelSAUIM_h */
