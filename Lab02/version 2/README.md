## Execução

1) Estando na pasta `version 2`, abra 3 terminais e rode os próximos comandos;
2) No terminal 1:

```bash
# Compilar o código
gcc server.c -o server

# Rodar o servidor 1
./server <IP_ADDRESS_SERVER> 5200
```

3) No terminal 2:

```bash
# Compilar o código
gcc server.c -o server

# Rodar o servidor 2
./server <IP_ADDRESS_SERVER> 5300
```

4) No terminal 3:

```bash
# Compilar o código
gcc client.c -o client -lpthread -lm

# Rodar o cliente
./client <IP_ADDRESS_CLIENT> 5100
```