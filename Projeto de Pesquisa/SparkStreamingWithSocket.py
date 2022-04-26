import os
import shutil
import sys

from pyspark import SparkContext
from pyspark.streaming import StreamingContext
from tabulate import tabulate

# Obter ou registrar a variável brodcast
def getWordExcludeList(sparkContext):
	if ('wordExcludeList' not in globals()):
		globals()['wordExcludeList'] = sparkContext.broadcast(["a", "b", "c"])

	return globals()['wordExcludeList']

# Obter ou registrar um acumulador
def getDroppedWordsCounter(sparkContext):
	if ('droppedWordsCounter' not in globals()):
		globals()['droppedWordsCounter'] = sparkContext.accumulator(0)

	return globals()['droppedWordsCounter']

def createContext(host, port, outputPath):
	# Se o print abaixo não for impresso, isso significa que o StreamingContext
	# foi carregado do novo ponto de verificação
	print("Creating new context")

	if os.path.exists(outputPath):
		os.remove(outputPath)

	sc = SparkContext(appName="PythonStreamingRecoverableNetworkWordCount")
	sc.setLogLevel("WARN")
	ssc = StreamingContext(sc, 1)

	# Cria um fluxo de socket stream para o host e port de destino e
	# conta as palavras no fluxo de entrada a partir do delimitador
	lines = ssc.socketTextStream(host, port)
	words = lines.flatMap(lambda line: line.split(" "))
	wordCounts = words.map(lambda x: (x, 1)).reduceByKey(lambda x, y: x + y)

	def echo(time, rdd):
		# Obter ou registrar uma lista de exclusão -> broadcast
		excludeList = getWordExcludeList(rdd.context)

		# Obter ou registrar um acumulador de contador de palavras soltas
		droppedWordsCounter = getDroppedWordsCounter(rdd.context)

		# Usa o "excludeList" para descartar palavras repetidas e
		# usa o "droppedWordsCounter" para contá-las
		def filterFunc(wordCount):
			if wordCount[0] in excludeList.value:
				droppedWordsCounter.add(wordCount[1])
				return False
			else:
				return True

		data = rdd.filter(filterFunc).collect()
		qtd_words = 0
		dataTable = []
		broken = '\n=========================================\n'
		columnNames = ["Palavras", "Quantidade"]

		if len(data) > 0:
			print(broken)
			counts = "DADOS RECEBIDOS EM %s" % (time)
			print(counts)

			for item in data:
				qtd_words += item[1]
				dataTable.append([item[0], item[1]])

			print(tabulate(dataTable, headers=columnNames, tablefmt="fancy_grid"))
			
			print("\nQUANTIDADE DE PALAVRAS RECEBIDAS: {}".format(qtd_words))
			print("LOG SALVO EM " + os.path.abspath(outputPath))
			print(broken)

			header_out = counts + "\nQUANTIDADE DE PALAVRAS RECEBIDAS: {}".format(qtd_words)
			
			with open(outputPath, 'a') as f:
				f.write(header_out + "\n\n" + "%s" % (tabulate(dataTable, headers=columnNames, tablefmt="fancy_grid")) + '\n' + broken + '\n')

	wordCounts.foreachRDD(echo)
	return ssc

if __name__ == "__main__":
	if len(sys.argv) != 5:
		print("Usage: recoverable_network_wordcount.py <hostname> <port> <checkpoint-directory> <output-file>", file=sys.stderr)
		sys.exit(-1)

	host, port, checkpoint, output = sys.argv[1:]

	if os.path.exists("checkpoint"):
		shutil.rmtree("checkpoint")

	ssc = StreamingContext.getOrCreate(checkpoint, lambda: createContext(host, int(port), output))
	ssc.start()
	ssc.awaitTermination()