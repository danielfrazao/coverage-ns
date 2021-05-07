
#ifndef ns_agenteFixo_h
#define ns_agenteFixo_h

#include "agent.h"
#include "packet.h"
#include "ip.h"
#include "scheduler.h" //para os gerenciadores de eventos
#include "mobilenode.h"
#include "random.h"
#include "agenteMovel.h"

class FixedAgent : public Agent {
	/* Metodos e Propriedades Publicas*/
	public:						
		FixedAgent();                              	// Metodo Construtor
  		int  command(int argc, const char*const* argv); // Metodo que executa comandos enviados pelo TCL
  		void recv(Packet*, Handler*); 			// Metodo para manipulacao de pacotes que chegam
  														
  	/* Metodos e Propriedades Privadas (protegidas) */	
	protected:
		int nodeID_;		// Id do nó
  		double x_;		// Posicoes X, Y e Z do nó
  		double y_;
  		double z_;
  		double dist_;		// distância do nó fixo para o móvel
  		double xCalcFixo_;
  		double yCalcFixo_;
  		double ErroRssi;
  		MobileNode *node_;	// Ponteiro para o No do Agente, utilizado para saber Posicao deste No'
		/* Se vamos usarmos vamos ter que modificar ?!
  		int debug_;		 Nivel de Debug do Agente */
};

/*
static class MobileAgentHeaderClass : public PacketHeaderClass {// Classe do Pacote
	public:
		MobileAgentHeaderClass() : PacketHeaderClass("PacketHeader/MobileAgent", sizeof(hdr_MobileAgent)) {
    			bind_offset(&hdr_MobileAgent::offset_);
  		}
} class_MobileAgent_hdr;
*/
static class FixedAgentClass : public TclClass {            // Classe do FixedAgent
	public:
  		FixedAgentClass() : TclClass("Agent/FixedAgent") {}
		TclObject* create(int, const char*const*) {
    			return (new FixedAgent());
		}
} class_FixedAgent;

#endif /* ns_agenteFixoSAUIM_h */
