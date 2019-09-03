#!/bin/bash

try() {
    input="$1"
    expected="$2"

    ./main "$input" > tmp.s
    gcc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ $actual = $expected ]; then
        echo "${input} => ${actual}"
    else
        echo "${expected} expected, but got ${actual}"
        exit 1
    fi
}

try 1 1
try 100 100
try 255 255

try "3+4" 7
try "100-10" 90
try "1+20+3-4+30-1" 49
try "30  - 1+3      -20 + 17" 29

echo OK