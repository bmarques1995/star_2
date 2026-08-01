#!/bin/sh

set -eu

if [ "$#" -ne 1 ]; then
    echo "Uso: $0 <diretorio-do-preset>"
    echo
    echo "Exemplo:"
    echo "  $0 build/debug"
    exit 1
fi

PRESET_DIR=$1

TEST_EXECUTABLE="$PRESET_DIR/bin/star_tests"
LIBRARY="$PRESET_DIR/lib/libstar.so"

PROFILE_DATA="$PRESET_DIR/coverage.profdata"
HTML_DIR="$PRESET_DIR/coverage"

if [ ! -x "$TEST_EXECUTABLE" ]; then
    echo "Erro: executável não encontrado:"
    echo "  $TEST_EXECUTABLE"
    exit 1
fi

if [ ! -f "$LIBRARY" ]; then
    echo "Erro: biblioteca não encontrada:"
    echo "  $LIBRARY"
    exit 1
fi

rm -f "$PRESET_DIR"/*.profraw "$PROFILE_DATA"

export LLVM_PROFILE_FILE="$PRESET_DIR/%p.profraw"

echo "==> Executando testes..."
"$TEST_EXECUTABLE"

set -- "$PRESET_DIR"/*.profraw

if [ ! -f "$1" ]; then
    echo "Erro: nenhum arquivo .profraw foi gerado."
    echo "Verifique se o binário foi compilado com:"
    echo "  -fprofile-instr-generate"
    echo "  -fcoverage-mapping"
    exit 1
fi

echo "==> Gerando perfil..."
llvm-profdata merge \
    -sparse \
    "$PRESET_DIR"/*.profraw \
    -o "$PROFILE_DATA"

echo "==> Relatório..."
llvm-cov report \
    "$TEST_EXECUTABLE" \
    -object "$LIBRARY" \
    -instr-profile="$PROFILE_DATA"

echo "==> HTML..."
llvm-cov show \
    "$TEST_EXECUTABLE" \
    -object "$LIBRARY" \
    -instr-profile="$PROFILE_DATA" \
    -format=html \
    -output-dir="$HTML_DIR"

echo
echo "Relatório gerado em:"
echo "  $HTML_DIR/index.html"