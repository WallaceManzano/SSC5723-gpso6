Para a compulação do código, utilizar o comando 
`gcc -pthread product_consumer.c -o product_consumer`

No arquivo product_consumer.c há 3 constants
```
#define MAX_ITEMS 30
#define N_CONSUMERS 2
#define N_PRODUCERS 4
```
A constante `MAX_ITEMS` define a quantide máxima de posições na lista, `N_CONSUMERS` define a quantidade de consumidores e `N_PRODUCERS` define a quantidade de produtores.
Os elementos produzidos são numeros aleatórios.
