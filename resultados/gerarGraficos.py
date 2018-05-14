import matplotlib.pyplot as plt
import numpy as np
from matplotlib import gridspec

def main():
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
	plt.show()

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
	plt.show()

	#GRÁFICO DE SPEEDUP COMPARANDO 2, 4 E 8 THREADS
	gs = gridspec.GridSpec(2, 1, height_ratios=[10, 1]) 
	plt.subplot(gs[0])
	plt.title("Gráfico de SpeedUP")
	plt.grid(True)
	plt.ylabel("SpeedUP")
	plt.xlabel("Ordem da Matriz")
	plt.xticks(ticks, ordemMatriz, fontsize=8, rotation=90)
	
	linha2th, = plt.plot(speedup[0], label="2 Threads")
	linha4th, = plt.plot(speedup[1], label="4 Threads")
	linha8th, = plt.plot(speedup[2], label="8 Threads")
	plt.legend(handles=[linha2th, linha4th, linha8th])
	
	plt.savefig("graficos/speedup.png")
	plt.show()

	#GRÁFICO DE EFICIÊNCIA COMPARANDO 2, 4 E 8 THREADS
	gs = gridspec.GridSpec(2, 1, height_ratios=[10, 1]) 
	plt.subplot(gs[0])
	plt.title("Gráfico de Eficiência")
	plt.grid(True)
	plt.ylabel("Eficiência (%)")
	plt.xlabel("Ordem da Matriz")
	plt.xticks(ticks, ordemMatriz, fontsize=8, rotation=90)
	
	linha2th, = plt.plot(eficiencia[0], label="2 Threads")
	linha4th, = plt.plot(eficiencia[1], label="4 Threads")
	linha8th, = plt.plot(eficiencia[2], label="8 Threads")
	plt.legend(handles=[linha2th, linha4th, linha8th])
	
	plt.savefig("graficos/eficiencia.png")
	plt.show()

	file.close()

main()