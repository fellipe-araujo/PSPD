# Parte 1: Spark Streaming + Socket (Netcat)

Para a configuração de ambiente, será necessário instalar algumas ferramentas e bibliotecas.

## Versões dos *framework's* e bibliotecas

* spark: **v3.1.3**
* hadoop: **v3.2**
* python: **v3.8**

## Sistema Operacional utilizado

* Ubuntu 20.04.4 LTS

## Instalação do Spark

1) Para a instalação do **spark**, use o comando abaixo para realizar o download:

```bash
wget https://downloads.apache.org/spark/spark-3.1.3/spark-3.1.3-bin-hadoop3.2.tgz
```

2) Extraia o arquivo baixado com o comando abaixo:

```bash
tar xvf spark-*
```

## Instalação do Netcat

1) Para instalar o **netcat**, use o comando abaixo:

```bash
sudo apt-get install netcat
```

## Instalação das Dependências

1) Para instalar as **dependências** desse projeto, execute o comando abaixo estando na pasta `/Projeto de Pesquisa`:

```bash
pip3 install -r requirements.txt
```

## Execução do Programa

### Iniciando um cluster manualmente

1) Para iniciar um servidor *master standalone*, rode o seguinte comando:

```bash
<diretorio-descompactado-spark>/spark-3.1.3-bin-hadoop3.2/sbin/start-master.sh
```

*Obs1: para acessar a interface do usuário do Spark Web, acesse `localhost:8080`*

2) Para iniciar um processo de trabalho (*slave*), rode o seguinte comando:

```bash
<diretorio-descompactado-spark>/spark-3.1.3-bin-hadoop3.2/sbin/start-slave.sh spark://<master>:<port>
```

*Obs2: \<master> se refere ao IP da máquina em execução ou ao nome do host*

*Obs3: \<port> se refere ao IP da máquina em execução ou ao nome do host*

### Abrir um Socket (terminal 1)

Para o **terminal 1**, iremos abrir um *socket* na porta *9999* com o **netcat**:

```bash
nc -lk 9999
```

### Conectando um aplicativo ao Cluster (terminal 2)

Para o **terminal 2**, esteja na pasta `/Projeto de Pesquisa/part1`. Aqui, iremos executar o **Spark Streaming** para ouvir e receber dados na porta *9999*:

```bash
<diretorio-descompactado-spark>/bin/spark-submit --master spark://<master>:7077 ./SparkStreamingWithSocker.py <master> 9999
```

*Obs1: \<master> se refere ao IP da máquina em execução ou ao nome do host*

* Agora, é só digitar um texto no **terminal 1** (onde o *netcat* está em execução) e ver a saída resultante no **terminal 2**.

*Obs2: será gerado um arquivo chamado **logs** com os resultados da contagem de palavras*.