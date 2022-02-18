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

2. No terminal 2, rode o seguinte comando:

```bash
node rpc_server.js
# => [x] Awaiting RPC requests
```

3. No terminal 3, rode o seguinte comando:

```bash
node rpc_client.js 30
# => [x] Requesting fib(30)
```