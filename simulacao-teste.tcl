#
# VLoc
#

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

if {$argc  == 3} {
    set opt(numNos)   [lindex $argv 0]                      ;# Numero de Nos tirado do Argv
    set opt(noDens)   [lindex $argv 1]                      ;# Densidade dos Nos no Campo de Sensores
    set opt(numTest)  [lindex $argv 2]                      ;# Numero do teste atual
} else {
    debugMsg "Simulacao" "    Sintaxe: ns simulacao-myflooding.tcl <NumNos:156> <Densidade: 0.03> <NumTest:1>" 
    debugMsg "Simulacao" "        Eg: ns simulacao-myflooding.tcl 156 0.03 1 > Saida.log";
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
set opt(tmSimul)      100;                                  # Simulation time
set opt(traceFile)    "OutSimulacao.tr";                    # Trace filename
set opt(namFile)      "OutSimulacao.nam";                   # Nam filename
set opt(xTam)         [ expr int ( sqrt($opt(numNos)/$opt(noDens)) ) ]; # X dimension of the topography
set opt(yTam)         [ expr int ( sqrt($opt(numNos)/$opt(noDens)) ) ]; # Y dimension of the topography
set opt(zTam)         2.5;                                  # Z dimension of the topography

set opt(posDist)      [ expr $opt(xTam)/ceil(sqrt($opt(numNos))) ]; # Espacamento entre os nohs
set opt(posErr)       3;                                    # Erro de espacamento entre os nohs
set opt(noRouting)    DumbAgent;                            # Routing Agent
set opt(tmSimul)      40;                                   # Simulation time

# =============================================================================
# Configuracao da Simulacao ..
# =============================================================================
debugMsg "Simulacao" "    Configuring the simulation .."   ;# Start message

debugMsg "Simulacao" "      - Simulator instance .."
set ns_ [new Simulator]                                     ;# Creates a simulater instance ..

debugMsg "Simulacao" "      - Trace support .. "
set  traceFd    [open $opt(traceFile) w]                    ;# Trace support ..
$ns_ trace-all  $traceFd                                    ;# Trace support ..
set  namFd [open $opt(namFile) w]                           ;# Network animator (Nam) support ..
$ns_ namtrace-all-wireless $namFd $opt(xTam) $opt(xTam)     ;# Network animator (Nam) support ..

debugMsg "Simulacao" "      - Topography .."
set topo [new Topography]                                   ;# Criates a topography object
$topo load_flatgrid $opt(xTam) $opt(xTam)                   ;# Defines the topologia
set god_ [create-god $opt(numNos)]                          ;# Criates the general operations director (GOD)

# Configuration of the new nodes
debugMsg "Simulacao" "      - Nodes creation .."
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
    -movementTrace ON               \
    -energyModel   $opt(noEngmodel) \
    -initialEnergy $opt(noIniteng)  \
    -txPower       $opt(noTxPower)  \
    -rxPower       $opt(noRxPower)  \
    -idlePower     $opt(noIdlePower)

# =============================================================================
# Criacao dos Nos ..
# =============================================================================

# Gera os Nohs
debugMsg "Simulacao" "   Criando Nos .."
set random [new RNG]
$random seed 0;

for {set i 0} {$i < $opt(numNos) } {incr i} {
    set node_($i) [$ns_ node $i];                           # Cria um novo noh
    $node_($i) random-motion 0;                             # Disable random motion
    $node_($i) color blue;                                  # Cor do No'

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
#    set myFlooding_($i) [new Agent/MyFlooding];             # Cria uma instanca do Protocolo MyFlooding
#    $ns_ attach-agent $node_($i) $myFlooding_($i);          # Atacha o MyFlooding ao No'
#    $myFlooding_($i) attach-node $node_($i);                # Seta o ID do No' dentro do Agente MyFlooding
    $god_  new_node $node_($i);                             # Inclui o no' no God (General Operations)
    $ns_ initial_node_pos $node_($i) 3;                     # Seta a posicao (e tamanho) inicial no Nam
}

#$ns_ at 0.001 "$myFlooding_(0) set-sink";                   # Seta o no' zero como Sink


# =============================================================================
# Agendamentos e Execucao da Simulacao ..
# =============================================================================
debugMsg "Simulacao" "   Scheduling the simulation events .."
	set now [$ns_ now]
	puts "dsad   $now " 
debugMsg "Simulacao" "      Finalization events .."

$ns_ at $opt(tmSimul) "$ns_ nam-end-wireless $opt(tmSimul)";#   - Tell nam the simulation stop time
$ns_ at $opt(tmSimul) "stop";                               #   - Agenda o Stop

$ns_ at $opt(tmSimul).0004 "debugMsg \"Simulacao\" \"Ending NS ..\" ; $ns_ halt"

puts $traceFd "M 0.0 nn $opt(numNos) x $opt(xTam) y $opt(xTam)"
puts $traceFd "M 0.0 topo $topo \"\" \"\" seed 0.0"
puts $traceFd "M 0.0 prop $opt(noProp) ant $opt(noAnt)"

debugMsg "Simulacao" "    Initializing simulation .."
$ns_ run;                                                   # Inicia a simulacao
