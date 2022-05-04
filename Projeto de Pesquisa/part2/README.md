# Parte 2: Spark Streaming + Apache Kafka

Para a configuração de ambiente, será necessário instalar algumas ferramentas e bibliotecas. No decorrer deste README, será explicitado como baixar, instalar e configurar tudo o que for necessário.

## Versões dos *framework's* e bibliotecas

* kafka: **v3.1.0**
* spark: **v3.2.1**
* hadoop: **v3.2**
* scala: **v2.13**
* python: **v3.8**

## Sistema Operacional utilizado

* Ubuntu 20.04.4 LTS

## Instalação do Spark

Para a instalação do **spark**, caso ainda tenha feito durante a **parte 1** deste Projeto de Pesquisa, click nesse [link](https://github.com/fellipe-araujo/PSPD/tree/main/Projeto%20de%20Pesquisa/part1#instala%C3%A7%C3%A3o-do-spark) e veja como baixar e instalar o **spark**.

## Instalação do Kafka

1) Para a instalação do **kafka**, use o comando abaixo para realizar o download:

```bash
wget https://downloads.apache.org/kafka/3.1.0/kafka_2.13-3.1.0.tgz
```

2) Extraia o arquivo baixado com o comando abaixo:

```bash
tar -xzf kafka_2.13-3.1.0.tgz
```

## Instalação das Dependências

1) Para instalar as **dependências** desse projeto, execute o comando abaixo estando na pasta `/Projeto de Pesquisa`:

```bash
pip3 install -r requirements.txt
```

## Execução do Programa

### Iniciando ambiente Kafka

1) Para o **terminal 1**, inicie o serviço *ZooKeeper* com o comando abaixo:

```bash
<diretorio-descompactado-kafka>/bin/zookeeper-server-start.sh <diretorio-descompactado-kafka>/config/zookeeper.properties
```

2) Para o **terminal 2**, inicie o serviço *Kafka Broker* com o comando abaixo:

```bash
<diretorio-descompactado-kafka>/bin/kafka-server-start.sh <diretorio-descompactado-kafka>/config/server.properties
```

### Escrevendo um *evento* em um *tópico*

1) Em um **terminal 3**, envie um texto através de um ***Kafka Producer*** e veja a saída resultante no **terminal 4**. Use o comando abaixo:

```bash
python3 kafkaProducer.py localhost:9092 topic1
```

*Obs1: será gerado um arquivo chamado **logs** com os resultados da contagem de palavras em diferentes categorias*.

### Deploy de uma aplicação Spark + Kafka

1) Em um **terminal 4**, faça a implantação da aplicação com o comando abaixo:

```bash
<diretorio-descompactado-spark>/bin/spark-submit --packages org.apache.spark:spark-sql-kafka-0-10_2.13:3.2.1 ./SparkStreamingWithKafka.py
```
***Obs2: Quando aparecer a seguinte mensagem no terminal `## BATCH <ID> FINALIZADO. AGUARDANDO NOVAS REQUISICOES ##`, significa que aplicação está pronta para receber novos dados.***