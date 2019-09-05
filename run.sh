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

try "3 * 4" 12
try "2 + 3 * 4" 14
try "2 * 3 + 4" 10

try "6 / 2" 3
try "5 / 2" 2
try "4 / 2 + 3" 5
try "4 + 9 / 3" 7

try "(2)" 2
try "(((30)))" 30
try "(2 * 30)" 60
try "2 + (3 * 4)" 14
try "2 * (5 + 4)" 18
try "(2 + 3) * (4 + 5)" 45

try "+3" 3
try "-(-100)" 100
try "50 - (-20)" 70
try "-(-3 * 3)" 9

echo OK