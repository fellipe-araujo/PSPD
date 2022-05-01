import sys
from pyspark.sql import SparkSession
from pyspark.sql.functions import explode
from pyspark.sql.functions import split
from tabulate import tabulate
from datetime import datetime

initial_time = datetime.now().strftime('%d/%m/%y %H:%M:%S')

def generate_metrics(df, epoch_id):
  data = df.collect()
  dataTable = []
  broken = '\n=========================================\n'
  columnNames = ["Palavras", "Quantidade"]

  if data:
    now = datetime.now().strftime('%d/%m/%y %H:%M:%S')
    print(broken)
    counts = "TOTAL DE DADOS RECEBIDOS ATÉ "
    print(counts + now)

    total_words = 0

    for item in data:
      dataTable.append([item[0], item[1]])
      total_words += item[1]

    print(tabulate(dataTable, headers=columnNames, tablefmt="fancy_grid"))
    
    print("\nQUANTIDADE TOTAL DE PALAVRAS RECEBIDAS: {}".format(total_words))
    print(broken)

    period = 'INTERVALO DE TEMPO MONITORADO: {}  -  {}'.format(initial_time, now)
    total_words_result = '\nQUANTIDADE TOTAL DE PALAVRAS RECEBIDAS: {}'.format(total_words)

    with open("./logs",'w') as f:
      pass

    with open('./logs', 'a') as f:
      f.write(period + "\n" + total_words_result + "\n\n" + "%s" % (tabulate(dataTable, headers=columnNames, tablefmt="fancy_grid")) + '\n' + broken + '\n')

if __name__ == "__main__":
	if len(sys.argv) != 3:
		print('Usage: SparkStreamingWithSocket.py <hostname> <port>')

	spark = SparkSession.builder.appName("StructuredNetworkWordCount").getOrCreate()

  # Create DataFrame representing the stream of input lines from connection to localhost:9999
	lines = spark.readStream.format("socket").option("host", sys.argv[1]).option("port", int(sys.argv[2])).load()

  # Split the lines into words
	words = lines.select(explode(split(lines.value, " ")).alias("word"))

  # Generate running word count
	wordCounts = words.groupBy("word").count()

  # Start running the query that prints the running counts to the console
	query = wordCounts.writeStream.foreachBatch(generate_metrics).outputMode("update").start()

	query.awaitTermination()