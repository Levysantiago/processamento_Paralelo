import matplotlib.pyplot as plt
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

	file = open("teste.txt", "r")
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
			ordemMatriz.append( a.split('*')[1] )

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

	print(ordemMatriz)
	print(tempoSequencial)
	print(tempoParalelo)
	print(desvioPadraoSequencial)
	print(desvioPadraoParalelo)
	print(speedup)
	print(eficiencia)

	file.close()

	gs = gridspec.GridSpec(2, 1, height_ratios=[10, 1]) 
	plt.subplot(gs[0])
	#plt.subplot2grid((2, 1), (0, 0))
	plt.title("Tempo Sequencial X Tempo Paralelo")
	plt.grid(True)
	plt.ylabel("Tempo (seg)")
	plt.xlabel("Ordem da Matriz")
	plt.xticks(ticks, ordemMatriz, fontsize=8, rotation=90)
	linha_sequencial, = plt.plot(tempoSequencial, label="Sequencial")
	linha_paralelo, = plt.plot(tempoParalelo, label="Paralelo")
	plt.legend(handles=[linha_sequencial, linha_paralelo])
	plt.show()

main()