# Parte 1: Spark Streaming + Socket (Netcat)

Para a configuração de ambiente, será necessário instalar algumas ferramentas e bibliotecas.

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

1) Para instalar as **dependências** desse projeto, execute o comando abaixo:

```bash
pip3 install -r requirements.txt
```

## Execução do Programa

Para a execução nessa **parte 1**, abra 2 terminais.

1) Para o **terminal 1**, iremos abrir um *socket* na porta *9999* com o **netcat**:

```bash
nc -lk 9999
```

2) Para o **terminal 2**, esteja na pasta `/Projeto de Pesquisa`. Aqui, iremos executar o **Spark Streaming** para ouvir e receber dados na porta *9999*:

*Obs1: para `<diretorio-descompactado-spark>`, coloque o caminho onde você fez o download e descompactou o spark.*

*Obs2: para `<host>`, coloque o **ip** do seu computador.*

```bash
<diretorio-descompactado-spark>/spark-3.1.3-bin-hadoop3.2/bin/spark-submit ./SparkStreamingWithSocket.py <host> 9999 ./checkpoint ./logs
```

3) Agora, é só digitar um texto no **terminal 1** (onde o *netcat* está em execução) e ver a saída resultante no **terminal 2**.

*Obs: será gerado um arquivo chamado **logs** com os resultados da contagem de palavras*.