#!/usr/bin/env bash

set -u

total_casos=31

if [[ $# -lt 1 || $# -gt 2 ]]; then
  echo "Uso: $0 CAMINHO_DO_EXECUTAVEL [ULTIMO_CASO]" >&2
  exit 2
fi

executavel=$1
ultimo_caso=${2:-$total_casos}

if [[ ! -x "$executavel" ]]; then
  echo "Erro: '$executavel' não existe ou não possui permissão de execução." >&2
  exit 2
fi

if [[ ! "$ultimo_caso" =~ ^[0-9]+$ ]] || (( ultimo_caso < 1 || ultimo_caso > total_casos )); then
  echo "Erro: ULTIMO_CASO deve ser um inteiro entre 1 e $total_casos." >&2
  exit 2
fi

diretorio=$(cd "$(dirname "$0")" && pwd)
saida_temporaria=$(mktemp "${TMPDIR:-/tmp}/tp2-saida.XXXXXX")
trap 'rm -f "$saida_temporaria"' EXIT

for ((indice = 1; indice <= ultimo_caso; indice++)); do
  printf -v caso "caso%02d" "$indice"
  entrada="$diretorio/testes/entradas/$caso.in"
  esperada="$diretorio/testes/saidas/$caso.out"

  if ! "$executavel" < "$entrada" > "$saida_temporaria"; then
    echo "$caso: erro durante a execução"
    exit 1
  fi

  if diff -q "$esperada" "$saida_temporaria" > /dev/null; then
    echo "$caso: OK"
  else
    echo "$caso: resposta incorreta"
    diff -u "$esperada" "$saida_temporaria" || true
    exit 1
  fi
done

echo "Todos os $ultimo_caso casos executados foram aprovados."
