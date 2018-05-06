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

	cont = 1
	ticks = [0]

	file = open("resultadoFinal.out", "r")
	# \n
	a = file.readline()
	while(a != ''):
		if(a != '\n'):
			ticks.append(cont)
			cont += 1

			#Memória da matriz
			file.readline()

			#Ordem da matriz
			a = file.readline()
			if(cont % 2 == 0):
				ordemMatriz.append( a.split('*')[1] )
			else:
				ordemMatriz.append( '' )

			#Obtendo tempo sequencial
			a = file.readline()
			tempoSequencial.append( float(a.split('*')[1]) )

			#Obtendo tempo paralelo
			a = file.readline()
			tempoParalelo.append( float(a.split('*')[1]) )

			#Obtendo desvio padrão sequencial
			a = file.readline()
			desvioPadraoSequencial.append( float(a.split('*')[1]) )

			#Obtendo desvio padrão paralelo
			a = file.readline()
			desvioPadraoParalelo.append( float(a.split('*')[1]) )

			# \n
			a = file.readline()
			#Diferença de valor máxima
			a = file.readline()
			# \n
			a = file.readline()

			a = file.readline()
			speedup.append( float(a.split('*')[1]) )

			a = file.readline()
			eficiencia.append( float(a.split('*')[1]) )
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

	'''
	print(ordemMatriz)
	print(tempoSequencial)
	print(tempoParalelo)
	print(desvioPadraoSequencial)
	print(desvioPadraoParalelo)
	print(speedup)
	print(eficiencia)
	'''

	file.close()

	#GRÁFICO DE TEMPO SEQUENCIAL X TEMPO PARALELO
	gs = gridspec.GridSpec(2, 1, height_ratios=[10, 1]) 
	plt.subplot(gs[0])
	plt.title("Tempo Sequencial X Tempo Paralelo")
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
	plt.title("Desvio Padrão Sequencial X Desvio Padrão Paralelo")
	plt.grid(True)
	plt.ylabel("Desvio Padrão")
	plt.xlabel("Ordem da Matriz")
	plt.xticks(ticks, ordemMatriz, fontsize=8, rotation=90)
	linha_sequencial, = plt.plot(desvioPadraoSequencial, label="Sequencial")
	linha_paralelo, = plt.plot(desvioPadraoParalelo, label="Paralelo")
	plt.legend(handles=[linha_sequencial, linha_paralelo])
	plt.savefig("graficos/desvioPadrao.png")
	plt.show()

	#GRÁFICO DE SPEEDUP
	gs = gridspec.GridSpec(2, 1) 
	plt.subplot(gs[0])
	plt.title("Gráfico de SpeedUP")
	plt.grid(True)
	plt.ylabel("SpeedUP")
	plt.xlabel("Ordem da Matriz")
	plt.xticks(ticks, ordemMatriz, fontsize=8, rotation=90)
	plt.plot(speedup)
	plt.show()

	#GRÁFICO DE EFICIÊNCIA
	gs = gridspec.GridSpec(2, 1) 
	plt.subplot(gs[0])
	plt.title("Gráfico de Eficiência")
	plt.grid(True)
	plt.ylabel("Eficiência")
	plt.xlabel("Ordem da Matriz")
	plt.xticks(ticks, ordemMatriz, fontsize=8, rotation=90)
	plt.plot(eficiencia)
	plt.show()

main()