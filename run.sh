#!/bin/bash

try() {
    expected="$1"
    input="$2"

    ./main "$input" > tmp.s
    gcc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ $actual = $expected ]; then
        echo "${input} => ${actual}"
    else
        echo "{$expected} expected, but got {$actual}"
        exit 1
    fi
}

try 1 1
try 100 100
try 255 255
