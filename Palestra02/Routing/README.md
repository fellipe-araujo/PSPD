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
node receive_logs_direct.js info warning error
# => [*] Waiting for logs. To exit press CTRL+C
```

3. No terminal 3, rode o seguinte comando:

```bash
node emit_log_direct.js error "Run. Run. Or it will explode."
# => [x] Sent 'error':'Run. Run. Or it will explode.'
```