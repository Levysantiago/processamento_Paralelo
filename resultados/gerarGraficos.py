import matplotlib.pyplot as plt
import numpy as np
from matplotlib import gridspec

def main():
	#*****************************************************************************************************************************
	#*****************************************************************************************************************************
	#*********************************************************** OMP *************************************************************
	#*****************************************************************************************************************************
	#*****************************************************************************************************************************

	ordemMatriz, ticks, tempoSequencial, tempoParalelo, desvioPadraoSequencial, desvioPadraoParalelo, speedup1, eficiencia1 = gerarParalelo()	

	#GRÁFICO DE TEMPO SEQUENCIAL X TEMPO PARALELO
	gs = gridspec.GridSpec(2, 1, height_ratios=[10, 1]) 
	plt.subplot(gs[0])
	plt.title("Tempo Sequencial X Tempo Paralelo (4 Threads)")
	plt.grid(True)
	plt.ylabel("Tempo (seg)")
	plt.xlabel("Ordem da Matriz")
	plt.xticks(ticks, ordemMatriz, fontsize=8, rotation=90)
	linha_sequencial, = plt.plot(tempoSequencial, label="Sequencial")
	linha_paralelo, = plt.plot(tempoParalelo, label="Paralelo")
	plt.legend(handles=[linha_sequencial, linha_paralelo])
	
	plt.savefig("graficos/tempo.png")
	#plt.show()

	#GRÁFICO DE DESVIO PADRÃO SEQUENCIAL X DESVIO PADRÃO PARALELO
	gs = gridspec.GridSpec(2, 1, height_ratios=[10, 1]) 
	plt.subplot(gs[0])
	plt.title("Desvio Padrão Sequencial X Desvio Padrão Paralelo (4 Threads)")
	plt.grid(True)
	plt.ylabel("Desvio Padrão")
	plt.xlabel("Ordem da Matriz")
	plt.xticks(ticks, ordemMatriz, fontsize=8, rotation=90)
	linha_sequencial, = plt.plot(desvioPadraoSequencial, label="Sequencial")
	linha_paralelo, = plt.plot(desvioPadraoParalelo, label="Paralelo")
	plt.legend(handles=[linha_sequencial, linha_paralelo])
	
	plt.savefig("graficos/desvioPadrao.png")
	#plt.show()

	#GRÁFICO DE SPEEDUP COMPARANDO 2, 4 E 8 THREADS
	gs = gridspec.GridSpec(2, 1, height_ratios=[10, 1]) 
	plt.subplot(gs[0])
	plt.title("Gráfico de SpeedUP")
	plt.grid(True)
	plt.ylabel("SpeedUP")
	plt.xlabel("Ordem da Matriz")
	plt.xticks(ticks, ordemMatriz, fontsize=8, rotation=90)
	
	linha2th, = plt.plot(speedup1[0], label="2 Threads")
	linha4th, = plt.plot(speedup1[1], label="4 Threads")
	linha8th, = plt.plot(speedup1[2], label="8 Threads")
	plt.legend(handles=[linha2th, linha4th, linha8th])
	
	plt.savefig("graficos/speedup.png")
	#plt.show()

	#GRÁFICO DE EFICIÊNCIA COMPARANDO 2, 4 E 8 THREADS
	gs = gridspec.GridSpec(2, 1, height_ratios=[10, 1]) 
	plt.subplot(gs[0])
	plt.title("Gráfico de Eficiência")
	plt.grid(True)
	plt.ylabel("Eficiência (%)")
	plt.xlabel("Ordem da Matriz")
	plt.xticks(ticks, ordemMatriz, fontsize=8, rotation=90)
	
	linha2th, = plt.plot(eficiencia1[0], label="2 Threads")
	linha4th, = plt.plot(eficiencia1[1], label="4 Threads")
	linha8th, = plt.plot(eficiencia1[2], label="8 Threads")
	plt.legend(handles=[linha2th, linha4th, linha8th])
	
	plt.savefig("graficos/eficiencia.png")
	#plt.show()

	#*****************************************************************************************************************************
	#*****************************************************************************************************************************
	#*********************************************************** MPI *************************************************************
	#*****************************************************************************************************************************
	#*****************************************************************************************************************************

	ordemMatriz, ticks, tempoSequencial, tempoParalelo, desvioPadraoSequencial, desvioPadraoParalelo, speedup2, eficiencia2 = gerarMPI()	

	#GRÁFICO DE TEMPO SEQUENCIAL X TEMPO PARALELO
	gs = gridspec.GridSpec(2, 1, height_ratios=[10, 1]) 
	plt.subplot(gs[0])
	plt.title("Tempo Sequencial X Tempo MPI (4 Processos)")
	plt.grid(True)
	plt.ylabel("Tempo (seg)")
	plt.xlabel("Ordem da Matriz")
	plt.xticks(ticks, ordemMatriz, fontsize=8, rotation=90)
	linha_sequencial, = plt.plot(tempoSequencial, label="Sequencial")
	linha_paralelo, = plt.plot(tempoParalelo, label="MPI")
	plt.legend(handles=[linha_sequencial, linha_paralelo])
	
	plt.savefig("graficos/tempoMPI.png")
	#plt.show()

	#GRÁFICO DE DESVIO PADRÃO SEQUENCIAL X DESVIO PADRÃO PARALELO
	gs = gridspec.GridSpec(2, 1, height_ratios=[10, 1]) 
	plt.subplot(gs[0])
	plt.title("Desvio Padrão Sequencial X Desvio Padrão MPI (4 Processos)")
	plt.grid(True)
	plt.ylabel("Desvio Padrão")
	plt.xlabel("Ordem da Matriz")
	plt.xticks(ticks, ordemMatriz, fontsize=8, rotation=90)
	linha_sequencial, = plt.plot(desvioPadraoSequencial, label="Sequencial")
	linha_paralelo, = plt.plot(desvioPadraoParalelo, label="MPI")
	plt.legend(handles=[linha_sequencial, linha_paralelo])
	
	plt.savefig("graficos/desvioPadraoMPI.png")
	#plt.show()

	#GRÁFICO DE SPEEDUP COMPARANDO 2, 4 E 8 THREADS
	gs = gridspec.GridSpec(2, 1, height_ratios=[10, 1]) 
	plt.subplot(gs[0])
	plt.title("Gráfico de SpeedUP")
	plt.grid(True)
	plt.ylabel("SpeedUP")
	plt.xlabel("Ordem da Matriz")
	plt.xticks(ticks, ordemMatriz, fontsize=8, rotation=90)
	
	linha2th, = plt.plot(speedup2[0], label="2 Processos")
	linha4th, = plt.plot(speedup2[1], label="4 Processos")
	linha8th, = plt.plot(speedup2[2], label="8 Processos")
	plt.legend(handles=[linha2th, linha4th, linha8th])
	
	plt.savefig("graficos/speedupMPI.png")
	#plt.show()

	#GRÁFICO DE EFICIÊNCIA COMPARANDO 2, 4 E 8 THREADS
	gs = gridspec.GridSpec(2, 1, height_ratios=[10, 1]) 
	plt.subplot(gs[0])
	plt.title("Gráfico de Eficiência")
	plt.grid(True)
	plt.ylabel("Eficiência (%)")
	plt.xlabel("Ordem da Matriz")
	plt.xticks(ticks, ordemMatriz, fontsize=8, rotation=90)
	
	linha2th, = plt.plot(eficiencia2[0], label="2 Processos")
	linha4th, = plt.plot(eficiencia2[1], label="4 Processos")
	linha8th, = plt.plot(eficiencia2[2], label="8 Processos")
	plt.legend(handles=[linha2th, linha4th, linha8th])
	
	plt.savefig("graficos/eficienciaMPI.png")
	#plt.show()

	#*****************************************************************************************************************************
	#*****************************************************************************************************************************
	#********************************************* COMPARAÇÃO ENTRE MPI E PARALELO ***********************************************
	#*****************************************************************************************************************************
	#*****************************************************************************************************************************

	#GRÁFICO DE EFICIÊNCIA COMPARANDO MPI X OMP
	gs = gridspec.GridSpec(2, 1, height_ratios=[10, 1]) 
	plt.subplot(gs[0])
	plt.title("Gráfico de Eficiência MPI X OMP")
	plt.grid(True)
	plt.ylabel("Eficiência (%)")
	plt.xlabel("Ordem da Matriz")
	plt.xticks(ticks, ordemMatriz, fontsize=8, rotation=90)
	
	linha2th, = plt.plot(eficiencia1[0], label="2 Threads")
	linha4th, = plt.plot(eficiencia1[1], label="4 Threads")
	linha8th, = plt.plot(eficiencia1[2], label="8 Threads")
	linha2mpi, = plt.plot(eficiencia2[0], label="2 Processos")
	linha4mpi, = plt.plot(eficiencia2[1], label="4 Processos")
	linha8mpi, = plt.plot(eficiencia2[2], label="8 Processos")
	plt.legend(handles=[linha2th, linha4th, linha8th, linha2mpi, linha4mpi, linha8mpi])
	
	plt.savefig("graficos/eficienciaMPI_x_OMP.png")
	#plt.show()

	#GRÁFICO DE SPEEDUP COMPARANDO MPI X OMP
	gs = gridspec.GridSpec(2, 1, height_ratios=[10, 1]) 
	plt.subplot(gs[0])
	plt.title("Gráfico de SpeedUP MPI X OMP")
	plt.grid(True)
	plt.ylabel("SpeedUP")
	plt.xlabel("Ordem da Matriz")
	plt.xticks(ticks, ordemMatriz, fontsize=8, rotation=90)
	
	linha2th, = plt.plot(speedup1[0], label="2 Threads")
	linha4th, = plt.plot(speedup1[1], label="4 Threads")
	linha8th, = plt.plot(speedup1[2], label="8 Threads")
	linha2mpi, = plt.plot(speedup2[0], label="2 Processos")
	linha4mpi, = plt.plot(speedup2[1], label="4 Processos")
	linha8mpi, = plt.plot(speedup2[2], label="8 Processos")
	plt.legend(handles=[linha2th, linha4th, linha8th, linha2mpi, linha4mpi, linha8mpi])
	
	plt.savefig("graficos/speedupMPI_x_OMP.png")
	#plt.show()

def gerarParalelo():
	ordemMatriz = []

	tempoSequencial = []
	tempoParalelo = []
	desvioPadraoSequencial = []
	desvioPadraoParalelo = []
	speedup = []
	eficiencia = []

	speedup.append([])
	eficiencia.append([])

	flag4thread = 0
	cont = 1
	i = 0
	ticks = [0]

	file = open("resultadoFinal.out", "r")
	# \n
	a = file.readline()
	while(a != ''):
		#Inicio de novos resultados
		if(a == "*----------*\n"):
			speedup.append([])
			eficiencia.append([])
			flag4thread += 1

			#Incrementando contador da matriz
			i += 1

		elif(a != '\n'):
			ticks.append(cont)
			cont += 1

			#Memória da matriz
			file.readline()

			#Ordem da matriz
			a = file.readline()
			if(cont < 45):
				if(cont % 2 == 0):
					ordemMatriz.append( a.split('*')[1] )
				else:
					ordemMatriz.append( '' )

			#Obtendo tempo sequencial
			a = file.readline()
			#Pegando somente os resultados com 4 threads
			if(flag4thread == 1):
				tempoSequencial.append( float(a.split('*')[1]) )

			#Obtendo tempo paralelo
			a = file.readline()
			#Pegando somente os resultados com 4 threads
			if(flag4thread == 1):
				tempoParalelo.append( float(a.split('*')[1]) )

			#Obtendo desvio padrão sequencial
			a = file.readline()
			#Pegando somente os resultados com 4 threads
			if(flag4thread == 1):
				desvioPadraoSequencial.append( float(a.split('*')[1]) )

			#Obtendo desvio padrão paralelo
			a = file.readline()
			#Pegando somente os resultados com 4 threads
			if(flag4thread == 1):
				desvioPadraoParalelo.append( float(a.split('*')[1]) )

			# \n
			a = file.readline()
			#Diferença de valor máxima
			a = file.readline()
			# \n
			a = file.readline()

			a = file.readline()
			speedup[i].append( float(a.split('*')[1]) )

			a = file.readline()

			eficiencia[i].append( float(a.split('*')[1]) )
			# \n
			a = file.readline()
			# \n
			a = file.readline()
			# Tempo total
			a = file.readline()
			# \n
			a = file.readline()
			# ***
			a = file.readline()

		a = file.readline()

	
	#print(ordemMatriz)
	#print(speedup)
	#print(eficiencia)

	file.close()

	return ordemMatriz, ticks, tempoSequencial, tempoParalelo, desvioPadraoSequencial, desvioPadraoParalelo, speedup, eficiencia

def gerarMPI():
	ordemMatriz = []

	tempoSequencial = []
	tempoParalelo = []
	desvioPadraoSequencial = []
	desvioPadraoParalelo = []
	speedup = []
	eficiencia = []

	speedup.append([])
	eficiencia.append([])

	flag4thread = 0
	cont = 1
	i = 0
	ticks = [0]

	file = open("mpi248proc.out", "r")
	
	# title
	a = file.readline()
	#\n
	a = file.readline()
	
	while(a != ''):

		#Inicio de novos resultados
		if(a == "--------------------\n"):
			if(file.readline() != ''):
				speedup.append([])
				eficiencia.append([])
				flag4thread += 1			

			#Incrementando contador da matriz
			i += 1

		elif("[" in a):
			pass

		elif(a != '\n'):
			ticks.append(cont)
			cont += 1

			#Memória da matriz
			file.readline()
			#nro
			file.readline()

			#Ordem da matriz
			a = file.readline()
			
			if(cont < 45):
				if(cont % 2 == 0):
					ordemMatriz.append( a.split('*')[1] )
				else:
					ordemMatriz.append( '' )

			#Obtendo tempo sequencial
			a = file.readline()
			#Pegando somente os resultados com 4 threads
			if(flag4thread == 1):
				tempoSequencial.append( float(a.split('*')[1]) )

			#Obtendo tempo paralelo
			a = file.readline()
			#Pegando somente os resultados com 4 threads
			if(flag4thread == 1):
				tempoParalelo.append( float(a.split('*')[1]) )

			#Obtendo desvio padrão sequencial
			a = file.readline()
			#Pegando somente os resultados com 4 threads
			if(flag4thread == 1):
				desvioPadraoSequencial.append( float(a.split('*')[1]) )

			#Obtendo desvio padrão paralelo
			a = file.readline()
			#Pegando somente os resultados com 4 threads
			if(flag4thread == 1):
				desvioPadraoParalelo.append( float(a.split('*')[1]) )

			# \n
			a = file.readline()
			#Diferença de valor máxima
			a = file.readline()
			# \n
			a = file.readline()

			#Speedup
			a = file.readline()
			speedup[i].append( float(a.split('*')[1]) )

			#Eficiencia
			a = file.readline()
			eficiencia[i].append( float(a.split('*')[1]) )

			# \n
			a = file.readline()
			# \n
			#a = file.readline()

		a = file.readline()

	
	'''
	print(ordemMatriz)
	print("\n\n")
	print(speedup)
	print("\n\n")
	print(eficiencia)
	'''

	file.close()

	return ordemMatriz, ticks, tempoSequencial, tempoParalelo, desvioPadraoSequencial, desvioPadraoParalelo, speedup, eficiencia

main()