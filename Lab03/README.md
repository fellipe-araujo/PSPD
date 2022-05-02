## Execução

1) Na pasta `/Codigos`, instale as dependências:

```bash
yarn install
```
2) Abra 3 terminais
3) No primeiro terminal, rode o comando:

```bash
docker run -it --rm --name rabbitmq -p 5672:5672 -p 15672:15672 rabbitmq:3.9-management
```

4) No segundo terminal, rode o comando:

```bash
node receive.js
```

4) No terceiro terminal, rode o comando:

```bash
node emit.js
```