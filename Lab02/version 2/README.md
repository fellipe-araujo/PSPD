## Execução

1) No arquivo `/version 2/client.c`, na **linha 13** altere o valor do **IP** para a máquina desejada; 
2) Estando na pasta `version 2`, abra 3 terminais e rode os próximos comandos;
3) No terminal 1:

```bash
# Compilar o código
gcc server.c -o server

# Rodar o servidor 1
./server <IP_ADDRESS_SERVER> 5200
```

4) No terminal 2:

```bash
# Compilar o código
gcc server.c -o server

# Rodar o servidor 2
./server <IP_ADDRESS_SERVER> 5300
```

5) No terminal 3:

```bash
# Compilar o código
gcc client.c -o client -lpthread -lm

# Rodar o cliente
./client <IP_ADDRESS_CLIENT> 5100
```