# =============================================================================
# Definicao das Funcoes ..
# =============================================================================
proc debugMsg {proc msg} {                                  ;# Funcao para mostrar mensagens de Debug
    puts stderr "\[$proc\] $msg"                            ;# Mostra a mensagem de Debug
}

proc stop {} {                                              ;# Funcao executada ao termino da Simulacao
    debugMsg "stop" "Terminando Simulacao .."               ;# Mensagem de Termino
    global ns_ traceFd namFd                                ;# Variaveis globais
    close $traceFd                                          ;# Fecha Arquivo de Trace
    close $namFd                                            ;# Fecha Arquivo de Nam
}

# =============================================================================
# Definicao de Algumas Opcoes ..
# =============================================================================
debugMsg "Simulacao" "Iniciando Simulacao .."

debugMsg "Simulacao" "  Definindo algumas Opcoes .."

if {$argc  == 5} {
    set opt(numNos)   [lindex $argv 0]                      ;# Numero de Nos tirado do Argv
    set opt(noDens)   [lindex $argv 1]                      ;# Densidade dos Nos no Campo de Sensores
    set opt(numTest)  [lindex $argv 2]                      ;# Numero do teste atual
    set opt(tempo_resched) [lindex $argv 3];		    
    set opt(ErroRssi) [lindex $argv 4]		            ;# Erro de Rssi
} else {
    debugMsg "Simulacao" "    Sintaxe: ns simulacao-localiza.tcl <NumNos:156> <Densidade: 0.03> <NumTest:1> <TempoResched: 1.5>" 
    debugMsg "Simulacao" "        Ex: ns simulacao-localiza.tcl 156 0.03 1 1.5 > Saida.log";
    exit
}

Phy/WirelessPhy set RXThresh_ 8.5457e-07;                   # 15m

set opt(noChan)       Channel/WirelessChannel;              # Channel type
set opt(noProp)       Propagation/FreeSpace;                # Radio-propagation model
set opt(noNetif)      Phy/WirelessPhy;                      # Network interface type
set opt(noMac)        Mac/802_11;                           # Mac protocol
set opt(noIfq)        Queue/DropTail/PriQueue;              # Interface queue type
set opt(noLl)         LL;                                   # Link layer type
set opt(noAnt)        Antenna/OmniAntenna;                  # Antenna type
set opt(noIfqlen)     50;                                   # Max packet in ifq
set opt(noEngmodel)   EnergyModel;                          # Energy model
set opt(noRouting)    AODV;                                 # Routing protocol
set opt(filters)      noFilter;                             # Old name for twophasepull filter
set opt(noTxPower)    0.045;                                # Transmit power in Watts
set opt(noRxPower)    0.024;                                # Reception power in Watts
set opt(noIdlePower)  0.000;                                # Idle power in Watts
set opt(noIniteng)    100.0;                                # Initial energy
#set opt(tmSimul)      100;                                  # Simulation time
set opt(traceFile)    "OutSimulacao.tr";                    # Trace filename
set opt(namFile)      "OutSimulacao.nam";                   # Nam filename
set opt(xTam)         [ expr int ( sqrt($opt(numNos)/$opt(noDens)) ) ]; # X dimension of the topography
set opt(yTam)         [ expr int ( sqrt($opt(numNos)/$opt(noDens)) ) ]; # Y dimension of the topography
set opt(zTam)         2.5;                                  # Z dimension of the topography

set opt(posDist)      [ expr $opt(xTam)/ceil(sqrt($opt(numNos))) ]; # Espa??amento entre os n??s
set opt(posErr)       3;                                    # Erro de espa??amento entre os n??s
set opt(noRouting)    DumbAgent;                            # Routing Agent
set opt(tmSimul)      500;                                   # Simulation time


# =============================================================================
# Configuracao da Simulacao ..
# =============================================================================

set ns_ [new Simulator]                                     ;# Creates a simulater instance ..

set  traceFd    [open $opt(traceFile) w]                    ;# Trace support ..
$ns_ trace-all  $traceFd                                    ;# Trace support ..
set  namFd [open $opt(namFile) w]                           ;# Network animator (Nam) support ..
$ns_ namtrace-all-wireless $namFd $opt(xTam) $opt(xTam)     ;# Network animator (Nam) support ..

set topo [new Topography]                                   ;# Criates a topography object
$topo load_flatgrid $opt(xTam) $opt(xTam)                   ;# Defines the topologia
set god_ [create-god $opt(numNos)]                          ;# Criates the general operations director (GOD)

# Configuration of the new nodes

set noChan_1_ [new $opt(noChan)]
$ns_ node-config \
    -adhocRouting  $opt(noRouting)  \
    -diffusionFilter $opt(filters)  \
    -llType        $opt(noLl)       \
    -macType       $opt(noMac)      \
    -ifqType       $opt(noIfq)      \
    -ifqLen        $opt(noIfqlen)   \
    -antType       $opt(noAnt)      \
    -propType      $opt(noProp)     \
    -phyType       $opt(noNetif)    \
    -channel       $noChan_1_       \
    -topoInstance  $topo            \
    -agentTrace    ON               \
    -routerTrace   ON               \
    -macTrace      OFF              \
    -movementTrace OF               \
    -energyModel   $opt(noEngmodel) \
    -initialEnergy $opt(noIniteng)  \
    -txPower       $opt(noTxPower)  \
    -rxPower       $opt(noRxPower)  \
    -idlePower     $opt(noIdlePower)

# =============================================================================
# Criacao dos Nos ..
# =============================================================================

# Gera os No's
set random [new RNG];
$random seed 0;

	set node_(0) [$ns_ node 0];  	
	set AgenteMovel_(0) [new Agent/AgenteMovel];
	$ns_ attach-agent $node_(0) $AgenteMovel_(0);
	$AgenteMovel_(0) attach-node $node_(0); 
	$god_  new_node $node_(0);                             # Inclui o no' no God (General Operations)
    	$ns_ initial_node_pos $node_(0) 2; 
	$node_(0) color red;

for {set i 1} {$i < $opt(numNos) } {incr i} {
    set node_($i) [$ns_ node $i];                           # Cria um novo noh
    $node_($i) random-motion 0;                             # Disable random motion
    $node_($i) color black;
    # Proximas linhas definem a posicao do no campo de sensores
    set xTemp [ expr ($opt(posDist)/2) + ( int($i)/int(sqrt($opt(numNos))) * $opt(posDist)) + [$random normal 0 $opt(posErr)]];
    set yTemp [ expr $opt(posDist) + ( int($i)%int(sqrt($opt(numNos))) * $opt(posDist)) + [$random normal 0 $opt(posErr)]];
    
    if { $xTemp < 0 } { set xTemp 0; }
    if { $yTemp < 0 } { set yTemp 0; }
    if { $xTemp > $opt(xTam) } { set xTemp $opt(xTam); }
    if { $yTemp > $opt(xTam) } { set yTemp $opt(xTam); }
    
    $node_($i) set X_ $xTemp;                               # Set the X position of the node
    $node_($i) set Y_ $yTemp;                               # Set the Y position of the node
    $node_($i) set Z_ 0.0;                                  # Set the Z position of the node

	# Cria o agente MyFlooding propriamente dito (antes foi a configuracao do no')

    set FixedAgent_($i) [new Agent/FixedAgent];             # Cria uma instanca do Protocolo FixedAgent
    $ns_ attach-agent $node_($i) $FixedAgent_($i);
    $FixedAgent_($i) attach-node $node_($i) $opt(ErroRssi); 
    $god_ new_node $node_($i);                             # Inclui o no' no God (General Operations)
    $ns_ initial_node_pos $node_($i) 3;                     # Seta a posicao (e tamanho) inicial no Nam

}


# =============================================================================
# Movimenta????o do n?? M??vel
# =============================================================================

set opt(StepNum) $opt(tmSimul);
#indica para qual ponto (x_ini, y_ini) o n?? m??vel se movimentar?? primeiro
set x_ini [expr int(rand()*100 ) % 10]; 
set y_ini [expr int(rand()*100 ) % 10];

#indica a dist??ncia entre uma movimenta????o e outra
set distancia 0.5;

#indica a velocidade da movimenta????o
set velocidade 11;

#indica a ??rea retangular de sensoreamento=================
set inf_x 0.0;
set inf_y 0.0;
set largura $opt(xTam); 
set altura $opt(yTam);
#==========================================================

#proc gera_trajetoria { {x_ini 0.0} {y_ini 0.0} {distancia 0.1} {velocidade 11} {numero_de_passos 90 } {inf_x 0.0} {inf_y 0.0} {largura 100.0} {altura 100.0} } {
	set m 2; #A vari??vel m indica que a cada m passos ser?? realizada uma movimenta????o. Onde um passo ?? um ponto gerado. 
	set a 0;
	set x $x_ini;
	set y $y_ini;
	set time_ 0;
	#for {set k 0} {[expr ($k < $numero_de_passos) && ($time_ < $opt(tmSimul)) ] } {incr k} {	
	for {set k 0} {$time_ < $opt(tmSimul)} {incr k} {
		
		#puts "x: $x y: $y\n";
		if { [expr $k % $m ] == 0 } {
			#Sortear um angulo que vai de +50 a -50 deste angulo
			set tmp [expr int( rand()*10000 ) % 51];
			if {[expr $tmp % 2 ]} { # [expr tmp % 2 ] gera um valor booleano randomicamente
				#a = (a + tmp)%360 
				set a [expr int($a + $tmp) % 360];
			} else {
				#a = (tmp <= a ? a-tmp : a+Trajectory.MAX_ANGLE-tmp );
				set a [expr ($tmp <= $a)? $a-$tmp : ($a + 360) - $tmp];
			}
			#Como usaremos a fun????o cosseno e essa tem como par??metro um valor em radianos teremos que converter 'a' de graus para radianos
			#Pelo motivo descrito acima usaremos a constante PI para realizar a convers??o
			set PI 3.1415926536;
			set rad_a [expr $a * $PI/180];
			set ca [expr cos($rad_a) * $distancia];
			set co [expr sin($rad_a) * $distancia];
            	}
            	#while ( ( x+ca <= limits.getX() || x+ca >= limits.getX()+limits.getWidth()  ) ||
            	while { [expr ( (($x + $ca) <= $inf_x) || ( ($x + $ca) >= ($inf_x + $largura)) ) || ( ( ($y + $co) <= $inf_y) || (($y + $co) >= ($inf_y + $altura)) )] } {
                	#a += Math.PI/4;
                	set a [expr $a + ($PI/4)];
                	set rad_a [expr $a * ($PI/180)]; #converte 'a' para radianos
                	set ca [expr cos($rad_a) * $distancia];
			set co [expr sin($rad_a) * $distancia];
            	}
            	set x [expr $x + $ca];
            	set y [expr $y + $co];
            	
            	set time_ [expr $k * 0.5] ;
		$ns_ at $time_ "$node_(0) setdest $x $y $velocidade";
	}
#}

puts "$opt(xTam) $opt(yTam)";
#gera_trajetoria $x_ini $y_ini $distancia 11 $numero_de_passos $inf_x $inf_y $largura $altura ;

#==============================================================================
	$ns_ at 1.0 "$AgenteMovel_(0) broadcast 5 $opt(tempo_resched) ";    # Seta o no' zero como Sink e passa o tempo de reenvio de pacote
# =============================================================================
# Agendamentos e Execucao da Simulacao ..
# =============================================================================

$ns_ at $opt(tmSimul) "$ns_ nam-end-wireless $opt(tmSimul)";#   - Tell nam the simulation stop time
$ns_ at $opt(tmSimul) "stop";                               #   - Agenda o Stop

$ns_ at $opt(tmSimul).0004 "debugMsg \"Simulacao\" \"Ending NS ..\" ; $ns_ halt"

$ns_ run;                                                   # Inicia a simulacao
