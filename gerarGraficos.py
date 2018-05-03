import matplotlib.pyplot as plt

def main():
	ordemMatriz = []
	tempoSequencial = []
	tempoParalelo = []
	desvioPadraoSequencial = []
	desvioPadraoParalelo = []
	speedup = []
	eficiencia = []

	file = open("teste.txt", "r")
	# \n
	a = file.readline()
	while(a != ''):
		if(a != '\n'):
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

	plt.title("Tempo Sequencial X Tempo Paralelo")
	plt.grid(True)
	plt.ylabel("Tempo (seg)")
	plt.xlabel("Ordem da Matriz")
	plt.xticks([0,1,2,3],ordemMatriz)
	plt.plot(tempoSequencial)
	plt.plot(tempoParalelo)
	plt.show()

main()