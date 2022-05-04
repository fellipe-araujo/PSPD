from pyspark.sql import SparkSession
from pyspark.sql.functions import explode, split, regexp_replace, col, lower
from tabulate import tabulate
from datetime import datetime

initial_time = datetime.now().strftime('%d/%m/%y %H:%M:%S')
dict_response = {'s': 0, 'p': 0, 'r': 0, '6': 0, '8': 0, '11': 0, 'total': 0}

def count_words(array_words, array_table):
  dict_response = {'s': 0, 'p': 0, 'r': 0, '6': 0, '8': 0, '11': 0, 'total': 0}

  for item in array_words:
    if item[0].startswith('s'):
      dict_response["s"] += item[1]
    elif item[0].startswith('p'):
      dict_response["p"] += item[1]
    elif item[0].lower().startswith('r'):
      dict_response["r"] += item[1]

    if len(item[0]) == 6:
      dict_response["6"] += item[1]
    elif len(item[0]) == 8:
      dict_response["8"] += item[1]
    elif len(item[0]) == 11:
      dict_response["11"] += item[1]

    array_table.append([item[0], item[1]])
    dict_response["total"] += item[1]

  return [dict_response, array_table]

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

    response_count_words = count_words(data, dataTable)

    print(tabulate(response_count_words[1], headers=columnNames, tablefmt="fancy_grid"))

    period = 'INTERVALO DE TEMPO MONITORADO: {}  -  {}'.format(initial_time, now)
    metrics = '\nQUANTIDADE TOTAL DE PALAVRAS RECEBIDAS: {}\
               \nQUANTIDADE DE PALAVRAS RECEBIDAS QUE COMECAM COM \'S\': {}\
               \nQUANTIDADE DE PALAVRAS RECEBIDAS QUE COMECAM COM \'P\': {}\
               \nQUANTIDADE DE PALAVRAS RECEBIDAS QUE COMECAM COM \'R\': {}\
               \nQUANTIDADE DE PALAVRAS RECEBIDAS QUE POSSUEM 6 LETRAS: {}\
               \nQUANTIDADE DE PALAVRAS RECEBIDAS QUE POSSUEM 8 LETRAS: {}\
               \nQUANTIDADE DE PALAVRAS RECEBIDAS QUE POSSUEM 11 LETRAS: {}'\
               .format(
                response_count_words[0]["total"],
                response_count_words[0]["s"],
                response_count_words[0]["p"],
                response_count_words[0]["r"],
                response_count_words[0]["6"],
                response_count_words[0]["8"],
                response_count_words[0]["11"],
               )
    
    print(metrics)
    print(broken)

    with open("./logs",'w') as f:
      pass

    with open('./logs', 'a') as f:
      f.write(period + "\n" + metrics + "\n\n" + "%s" % (tabulate(response_count_words[1], headers=columnNames, tablefmt="fancy_grid")) + '\n' + broken + '\n')

  print('\\n## BATCH {} FINALIZADO. AGUARDANDO NOVAS REQUISICOES ##\n\n'.format(epoch_id))

if __name__ == "__main__":
  spark = SparkSession.builder.appName("StructuredKafkaWordCount").getOrCreate()
  spark.sparkContext.setLogLevel("WARN")

  # Cria um DataFrame representando o fluxo de linhas de entrada da conexão para <host>:<port> e para o topic especificado
  lines = spark.readStream.format("kafka").option("kafka.bootstrap.servers", "localhost:9092").option("subscribe", "pspd-topic").load()

  # Divide as linhas em palavras
  words = lines.select(explode(split(lines.value, ' ')).alias('word'))

  # Remover vírgulas e pontos; e deixar tudo em lowercase
  words = words.withColumn('word', lower(regexp_replace(col('word'), r'(\.|\,|\:|\"|\'|\(|\)|\;)', '')))

  # Gera a contagem de palavras em execução
  wordCounts = words.groupBy('word').count()

  # Começa a executar a consulta e, para cada requisição, é executada a função personalizada 'generate_metrics'
  query = wordCounts.writeStream.foreachBatch(generate_metrics).outputMode("complete").start()
                
  query.awaitTermination()