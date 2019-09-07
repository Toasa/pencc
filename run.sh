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

try "1;" 1
try "100;" 100
try "255;" 255

try "3+4;" 7
try "100-10;" 90
try "1+20+3-4+30-1;" 49
try "30  - 1+3      -20 + 17;" 29

try "3 * 4;" 12
try "2 + 3 * 4;" 14
try "2 * 3 + 4;" 10

try "6 / 2;" 3
try "5 / 2;" 2
try "4 / 2 + 3;" 5
try "4 + 9 / 3;" 7

try "(2);" 2
try "(((30)));" 30
try "(2 * 30);" 60
try "2 + (3 * 4);" 14
try "2 * (5 + 4);" 18
try "(2 + 3) * (4 + 5);" 45

try "+3;" 3
try "-10 + 20;" 10
try "-(-100);" 100
try "50 - (-20);" 70
try "-(-3 * 3);" 9

try "1==1;" 1
try "130==130;" 1
try "1!=1;" 0
try "30!=1;" 1

try "5 < 5;" 0
try "5 <= 5;" 1
try "55 > 55;" 0
try "55 >= 55;" 1
try "(1==1) + (1!=10) + (5 < 5) + (78 >= 79);" 2

try "a=10; a;" 10
try "a = 10; b = 10; a * b == 100;" 1
try "a = 1; f  =12;     k = 10; a + f * k;" 121
try "a = b = 10; a * b == 100;" 1

try "abc = 2; def=30; def / abc;" 15
try "foo = bar = 12; foo * bar;" 144;

try "return 10;" 10;
try "a = 14; return a;" 14
try "return 100; return 200;" 100
try "a = 5; b = 30; return a * b;" 150

try """
a = 5;
return a + 10;
""" 15

# try """
# if (1) return 46;
# """ 46


echo OK