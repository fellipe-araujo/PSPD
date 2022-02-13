## Execução

1) Estando na pasta `/version 2` (em duas máquinas diferentes), rode o seguinte comando:

```bash
make
```

2) Abra 2 terminais em uma máquina e 1 terminal em outra máquina;
3) **Máquina 1**: rode o comando:

```bash
./app_server
```

4) **Máquina 2**: no primeiro terminal, rode o comando:

```bash
./app_server
```

5) **Máquina 2**: No segundo terminal, rode o comando:

```bash
./app_client <ip1> <ip2> <vector_length>
```
**Obs:** o <*ip1*> é o *ip* da máquina 1, e o <*ip2*> é o *ip* da máquina 2 onde os servers estão rodando, e <*vector_length*> é o tamanho do vector que se quer utilizar.