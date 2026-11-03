# TP2 - Rotas na Mina

Esta pasta contém o enunciado, o arquivos iniciais da implementação e 31 casos
de teste com suas respectivas saídas esperadas.

## Compilação

```sh
make
```

## Avaliação dos exemplos

```sh
make test
```

O comando executa o programa em todos os arquivos `tests/caso*.in` e
compara os resultados com os arquivos `.out` correspondentes.

Para executar testes separadamente com cada algoritmo, também é possível usar 

```sh
make test-guloso

make test-divconq
```

## Limpeza

```sh
make clean
```
