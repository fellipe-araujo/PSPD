## Execução

1) Estando na pasta `version 1`, abra 2 terminais e rode os próximos comandos;
2) No terminal 1:

```bash
# Compilar o código
gcc server.c -o server

# Rodar o servidor
./server <IP_ADDRESS_SERVER> 5200
```

3) No terminal 2:

```bash
# Compilar o código
gcc client.c -o client -lpthread -lm

# Rodar o cliente
./client <IP_ADDRESS_CLIENT> 5100
```