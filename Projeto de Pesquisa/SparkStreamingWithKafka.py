from pyspark.sql import SparkSession
from pyspark.sql.functions import explode
from pyspark.sql.functions import split

if __name__ == "__main__":
  spark = SparkSession.builder.appName("StructuredKafkaWordCount").getOrCreate()
  spark.sparkContext.setLogLevel("WARN")

  lines = spark.readStream.format("kafka").option("kafka.bootstrap.servers", "localhost:9092").option("subscribe", "topic1").load()

  words = lines.select(explode(split(lines.value, ' ')).alias('word'))

  wordCounts = words.groupBy('word').count()

  query = wordCounts.writeStream.outputMode("complete").format("console").start()
                
  query.awaitTermination()