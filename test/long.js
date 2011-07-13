"use strict";
/**
 * Execute this file with v8 or Rhino,
 * or open long.htm with the web browser. 
 */
if (typeof window !== 'undefined') {
  var print = function(a) {
    document.writeln(a);
  };
} else if (typeof load !== 'undefined') {
  load('../long.js');
}

function basic() {
  var r1 = (Math.random() * 3 | 0) + 2;
  var r2 = (Math.random() * 3 | 0) + 2;
  var a = longRandom(r1);
  var b = longRandom(r2);
  return [
      '    a = ',a, '\n    b = ',b, '\na + b = ',longAdd(a,b),
      '\na - b = ',longSub(a,b), '\na * b = ',longMul(a,b),
      '\na / b = ',longDiv(a,b), '\na % b = ',longMod(a,b),
      '\na ^ 2 = ',longPow(a,2), '\na^0.5 = ',longSqrt(a),
      '\na >> 2 = ',longR(a,2), '\ngcd(a, b) = ',longGcd(a,b)
      ].join('');
}

function fact(a) {
  var f = longNum(1);
  for (var i = 2; i < a + 1; i++) {
    f = longMul(f, longNum(i));
  }
  return f;
}

function fib(a) {
  var b = longNum(0);
  var c = longNum(1);
  for (var i = 0, d; i < a; i++) {
    d = longClone(b);
    b = longAdd(b, c);
    c = d;
  }
  return b;
}

function square(a) {
  for (var i = 0, t; i < a; i++) {
    t = longRandom(i);
    print(longEqual(longMul(t, t), longSquare(t)));
  }
}

function tc(a) {
  for (var i = 0, t; i < a; i++) {
    t = longRandom(i);
    print(longEqual(longMul(t, t), longTc(t, t)));
  }
}

function kara(a) {
  for (var i = 0, t; i < a; i++) {
    t = longRandom(2);
    print(longEqual(longMul(t, t), karatsuba(t, t)));
  }
}

function main() {
  var d = new Date;

  print('-- basic operations --');
  print(basic());

  print('\n-- fibonacchi --');
  print('fib(100) = ');
  print(fib(100));

  print('\n-- factorial --');
  print('73! = ');
  print(fact(73));

  print('\nTime: '+ (new Date - d) + 'ms');
}

main();
