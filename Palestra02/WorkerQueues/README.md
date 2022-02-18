## Como rodar

Antes de mais nada, na pasta `/Palestra02`, rode o seguinte comando:

```bash
npm install amqplib
```

1. No terminal 1, rode o seguinte comando para não precisar instalar o rabbitmq:

```bash
# for RabbitMQ 3.9, the latest series
docker run -it --rm --name rabbitmq -p 5672:5672 -p 15672:15672 rabbitmq:3.9-management
```

2. No terminal 2, rode o seguinte comando (worker 1):

```bash
node worker.js
```

3. No terminal 3, rode o seguinte comando (worker 2):

```bash
node worker.js
```

4. No terminal 4, rode o seguinte comando:

```bash
node new_task.js <qualquer-mensagem>
```