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
  var r1 = (Math.random() * 4 | 0) + 3,
      r2 = (Math.random() * 4 | 0) + 3,
      a = longRandom(r1),
      b = longRandom(r2);
  return [
      '    a = ',a, '\n    b = ',b, '\na + b = ',longAdd(a,b),
      '\na - b = ',longSub(a,b), '\na * b = ',longMul(a,b),
      '\na / b = ',longDiv(a,b), '\na % b = ',longMod(a,b),
      '\na ^ 2 = ',longPow(a,2), '\na^0.5 = ',longPow(a, 0.5), ' ~ ',longSqrt(a),
      '\na >> 2 = ',longR(a,2), '\ngcd(a, b) = ',longGcd(a,b)
      ].join('');
}

// failed
function square(a) {
  for (var i = 1, t; i < a; i++) {
    t = longRandom(i);
    print(longEqual(longMul(t, t), longSquare(t)));
  }
}

// failed
function tc(a) {
  for (var i = 1, t; i < a; i++) {
    t = longRandom(i);
    print(longEqual(longMul(t, t), longTc(t, t)));
  }
}

// failed
function kara(a) {
  for (var i = 0, t; i < a; i++) {
    t = longRandom(2);
    print(longEqual(longMul(t, t), karatsuba(t, t)));
  }
}

function fact(a) {
  var f = longNum(1);
  for (var i = 2; i < a + 1; i++) {
    f = longMul(f, longNum(i));
  }
  return f;
}

function fib(a) {
  var b = longNum(0), c = longNum(1);
  for (var i = 0, d; i < a; i++) {
    d = longClone(b);
    b = longAdd(b, c);
    c = d;
  }
  return b;
}

function pi(a) {
  if (!a) { a = 1; }
  var n = longPow(longNum(10), a);

  function arctan(m) {
    var c = n, a = longDiv(c, m), b = longClone(a), m2 = longMul(m, m),
        k = longNum(1), s = true, l2 = longNum(2);
    while (c.isNonZero()) {
      b = longDiv(b, m2);
      k = longAdd(k, l2);
      c = longDiv(b, k);
      s = !s;
      if (s) {
        a = longAdd(a, c);
      } else {
        a = longSub(a, c);
      }
    }
    return a;
  }

  return longL(longSub(longL(arctan(longNum(5)), 2), arctan(longNum(239))), 2).toString();
}

function main() {
  var d = +new Date;

  print('-- basic operations --');
  print(basic());

  print('\n-- fibonacchi --');
  print('fib(77) = ');
  print(fib(77));

  print('\n-- factorial --');
  print('77! = ');
  print(fact(77));

  print('\n-- pi --');
  print('pi(77) = ');
  print(pi(77));

  print('\nTime: '+ (new Date - d) + 'ms');
}

main();
