#!/bin/bash

for ((i = 1; i <= 10; i++)) do
	ns simulacao-cobertura-teste.tcl $i 10 100 1 5 > $i-SAIDA-10.txt
	ns simulacao-cobertura-teste.tcl $i 20 100 1 5 > $i-SAIDA-20.txt
	ns simulacao-cobertura-teste.tcl $i 30 100 1 5 > $i-SAIDA-30.txt
	ns simulacao-cobertura-teste.tcl $i 40 100 1 5 > $i-SAIDA-40.txt
	ns simulacao-cobertura-teste.tcl $i 50 100 1 5 > $i-SAIDA-50.txt
	ns simulacao-cobertura-teste.tcl $i 60 100 1 5 > $i-SAIDA-60.txt
	ns simulacao-cobertura-teste.tcl $i 70 100 1 5 > $i-SAIDA-70.txt
	ns simulacao-cobertura-teste.tcl $i 80 100 1 5 > $i-SAIDA-80.txt
	ns simulacao-cobertura-teste.tcl $i 90 100 1 5 > $i-SAIDA-90.txt
	ns simulacao-cobertura-teste.tcl $i 100 100 1 5 > $i-SAIDA-100.txt
done
	
