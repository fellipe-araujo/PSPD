from pyspark.sql import SparkSession
from pyspark.sql.functions import explode
from pyspark.sql.functions import split
from tabulate import tabulate
from datetime import datetime

dict_response = {'s': 0, 'p': 0, 'r': 0, '6': 0, '8': 0, '11': 0, 'total': 0}

def count_words(array_words):

  for item in array_words:
    if item[0].startswith('s'):
      dict_response["s"] += 1
    elif item[0].startswith('p'):
      dict_response["p"] += 1
    elif item[0].startswith('r'):
      dict_response["r"] += 1
    
    if len(item[0]) == 6:
      dict_response["6"] += 1
    elif len(item[0]) == 8:
      dict_response["8"] += 1
    elif len(item[0]) == 11:
      dict_response["11"] += 1

  return dict_response

def generate_metrics(df, epoch_id):
  data = df.collect()
  dataTable = []
  broken = '\n=========================================\n'
  columnNames = ["Palavras", "Quantidade"]

  if data:
    now = datetime.now().strftime('%d/%m/%y %H:%M:%S')
    print(broken)
    counts = "DADOS RECEBIDOS EM "
    print(counts + now)

    results = {}
    results = count_words(data)
    dict_response["total"] = 0

    for item in data:
      dataTable.append([item[0], item[1]])
      dict_response["total"] += item[1]

    print(tabulate(dataTable, headers=columnNames, tablefmt="fancy_grid"))
    
    print("\nQUANTIDADE TOTAL DE PALAVRAS RECEBIDAS: {}".format(results["total"]))
    print("\nQUANTIDADE DE PALAVRAS RECEBIDAS QUE COMECAM COM 'S': {}".format(results["s"]))
    print("\nQUANTIDADE DE PALAVRAS RECEBIDAS QUE COMECAM COM 'P': {}".format(results["p"]))
    print("\nQUANTIDADE DE PALAVRAS RECEBIDAS QUE COMECAM COM 'R': {}".format(results["r"]))
    print("\nQUANTIDADE DE PALAVRAS RECEBIDAS QUE POSSUEM 6 LETRAS: {}".format(results["6"]))
    print("\nQUANTIDADE DE PALAVRAS RECEBIDAS QUE POSSUEM 8 LETRAS: {}".format(results["8"]))
    print("\nQUANTIDADE DE PALAVRAS RECEBIDAS QUE POSSUEM 11 LETRAS: {}".format(results["11"]))
    print(broken)

if __name__ == "__main__":
  spark = SparkSession.builder.appName("StructuredKafkaWordCount").getOrCreate()
  spark.sparkContext.setLogLevel("WARN")

  lines = spark.readStream.format("kafka").option("kafka.bootstrap.servers", "localhost:9092").option("subscribe", "topic1").load()

  words = lines.select(explode(split(lines.value, ' ')).alias('word'))

  wordCounts = words.groupBy('word').count()

  query = wordCounts.writeStream.foreachBatch(generate_metrics).outputMode("complete").start()
                
  query.awaitTermination()