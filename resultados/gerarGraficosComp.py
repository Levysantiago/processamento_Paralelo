import matplotlib.pyplot as plt
import numpy as np
from matplotlib import gridspec

def main():
	ordemMatriz = []
	speedup = []
	eficiencia = []

	speedup.append([])
	eficiencia.append([])

	cont = 1
	i = 0
	ticks = [0]

	file = open("resultadoFinalComp.out", "r")
	# \n
	a = file.readline()
	while(a != ''):
		#Inicio de novos resultados
		if(a == "*----------*\n"):
			speedup.append([])
			eficiencia.append([])

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
			#tempoSequencial.append( float(a.split('*')[1]) )

			#Obtendo tempo paralelo
			a = file.readline()
			#tempoParalelo.append( float(a.split('*')[1]) )

			#Obtendo desvio padrão sequencial
			a = file.readline()
			#desvioPadraoSequencial.append( float(a.split('*')[1]) )

			#Obtendo desvio padrão paralelo
			a = file.readline()
			#desvioPadraoParalelo.append( float(a.split('*')[1]) )

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

	#GRÁFICO DE SPEEDUP
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

	#GRÁFICO DE EFICIÊNCIA
	gs = gridspec.GridSpec(2, 1, height_ratios=[10, 1]) 
	plt.subplot(gs[0])
	plt.title("Gráfico de SpeedUP")
	plt.grid(True)
	plt.ylabel("Eficiência")
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