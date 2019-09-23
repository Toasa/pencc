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

funcCall() {
    input="$1"
    ./main "$input" > tmp.s
    cc -c -w extern_funcs.c
    gcc -o tmp tmp.s extern_funcs.o
    ./tmp
}

try "main() { 1; }" 1
try "main() { 100; }" 100
try "main() { 255; }" 255

try "main() { 3+4; }" 7
try "main() { 100-10; }" 90
try "main() { 1+20+3-4+30-1; }" 49
try "main() { 30  - 1+3      -20 + 17; }" 29

try "main() { 3 * 4; }" 12
try "main() { 2 + 3 * 4; }" 14
try "main() { 2 * 3 + 4; }" 10

try "main() { 6 / 2; }" 3
try "main() { 5 / 2; }" 2
try "main() { 4 / 2 + 3; }" 5
try "main() { 4 + 9 / 3; }" 7

try "main() { (2); }" 2
try "main() { (((30))); }" 30
try "main() { (2 * 30); }" 60
try "main() { 2 + (3 * 4); }" 14
try "main() { 2 * (5 + 4); }" 18
try "main() { (2 + 3) * (4 + 5); }" 45

try "main() { +3; }" 3
try "main() { -10 + 20; }" 10
try "main() { -(-100); }" 100
try "main() { 50 - (-20); }" 70
try "main() { -(-3 * 3); }" 9

try "main() { 1==1; }" 1
try "main() { 130==130; }" 1
try "main() { 1!=1; }" 0
try "main() { 30!=1; }" 1

try "main() { 5 < 5; }" 0
try "main() { 5 <= 5; }" 1
try "main() { 55 > 55; }" 0
try "main() { 55 >= 55; }" 1
try "main() { (1==1) + (1!=10) + (5 < 5) + (78 >= 79); }" 2

try "main() { a=10; a; }" 10
try "main() { i = 1; i; }" 1
try "main() { a = 10; b = 10; a * b == 100; }" 1
try "main() { a = 1; f  =12;     k = 10; a + f * k; }" 121
try "main() { a = b = 10; a * b == 100; }" 1

try "main() { abc = 2; def=30; def / abc; }" 15
try "main() {foo = bar = 12; foo * bar; }" 144;

try "main() { return 10; }" 10;
try "main() { a = 14; return a; }" 14
try "main() { return 100; return 200; }" 100
try "main() { a = 5; b = 30; return a * b; }" 150

try """
main() {
    a = 5;
    return a + 10;
}
""" 15

try """
main() {
    i = 1;
    i = i + 1;
    i = i + 1;
    i = i + 1;
    i = i + 1;
    i = i + 1;
    i = i + 1;
    i = i + 1;
    return i;
}
""" 8

try """
main() {
    if (1) {
        return 46;
    }
}
""" 46

try """
main() {
    a = 40;
    if (a / 2 == 20) return 5; return 6;
}
""" 5

try """
main() {
    a = 40;
    if (a / 2 == 21) return 5; return 6;
}
""" 6

try """
main() {
    a = 10;
    if (a == 21) {
        return 5;
    } else {
        return 6;
        x = 7;
        return x;
    }
}
""" 6

try """
main() {
    i = 2;
    while (i < 100) {
        i = 2 * i;
    }
    return i;
}
""" 128

try """
main() {
    i = 2;
    while (i < 100) {
        i = 2 * i;
        return i;
    }
}
""" 4

try """
main() {
    a = 4;
    for (i = 33; i < 100; i = i + 33) {
        a = a + i;
    }
    return a;
}
""" 202

try """
main() {
    x = 5;
    for (i = x; i < 10; i = i + 1) {
        x = x + i;
    }
    return x;
}
""" 40

try """
main() {
    {
        a = 10;
        b = 20;
        return a * b;
    }
}
""" 200

try """
main() {
    i = 1;
    j = 1;
    while (i * j < 100) {
        i = i * 2;
        j = j * 3;
    }
    return i * j;
}
""" 216

try """
main(x, y) {
    x = 30;
    y = 20;
    return x + y;
}""" 50

try """
add(x, y) {
    return x + y;
}

main() {
    return add(20, 10);
}
""" 30

try """
sum(x, y) {
    s = 0;
    for (i = x; i <= y; i = i + 1) {
        s = s + i;
    }
    return s;
}

main() {
    return sum(1, 10);
}
""" 55

try """
mul3(x, y, z) {
    return x * y * z;
}

main() {
    return mul3(3, 4, 5);
}
""" 60

# try """
# fibo(n) {
#     if (n <= 1) {
#         return 1;
#     }
#     return fibo(n - 1) + fibo(n - 2);
# }

# main() {
#     return fibo(3);
# }
# """ 3

funcCall "main() { howdy(); }"

funcCall "main() { add2(10, 11); }"
funcCall "main() { add3(10, 11, 12); }"
funcCall "main() { add4(10, 11, 12, 13); }"
funcCall "main() { add5(10, 11, 12, 13, 14); }"

echo OK