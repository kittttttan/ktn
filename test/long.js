/**
 * Execute this file with v8 or Rhino,
 * or open long.htm with the web browser. 
 */
"use strict";
if (typeof window !== 'undefined') {
  var print = function(a) {
    document.writeln(a);
  };
} else if (typeof load !== 'undefined') {
  load('../long.js');
}

/**
 * Basic operations
 * @returns {string}
 */
function basic() {
  var r1 = (Math.random() * 4 | 0) + 3,
      r2 = (Math.random() * 4 | 0) + 1,
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

/**
 * Factorial
 * @param {number} a
 * @returns {Long} a!
 */
function fact(a) {
  var f = longNum(1);
  for (var i = 2; i < a + 1; i++) {
    f = longMul(f, longNum(i));
  }
  return f;
}

/**
 * Calculates fibonacchi number simply
 * @param {number} a
 * @returns {Long} <var>a</var>th fibonacchi number
 */
function fib(a) {
  var b = longNum(0);
  for (var i = 0, c = longNum(1), d; i < a; i++) {
    d = longClone(b);
    b = longAdd(b, c);
    c = d;
  }
  return b;
}

/**
 * test for longSquare
 */
function square(a) {
  for (var i = 1, t; i < a; i++) {
    t = longRandom(i);
    print(longEqual(longMul(t, t), longSquare(t)));
  }
}

/**
 * Compares performance longMul vs longSquare
 */
function sqvsmul(a) {
  var nums = [];
  for (var i = 0; i < a; i++) {
    nums[i] = longRandom(i + 1);
  }
  var t0 = +new Date;
  for (i = 0; i < a; i++) {
    longMul(nums[i], nums[i]);
  }
  var t1 = +new Date;
  for (i = 0; i < a; i++) {
    longSquare(nums[i], nums[i]);
  }
  var t2 = +new Date;
  print('mul: '+ (t1 - t0) +'ms\nsq:  '+ (t2 - t1) +'ms\n     '
      + (100*(t2 - t1)/(t1 - t0)).toFixed(2) +'%');
}

/**
 * test for longTc
 */
function tc(a) {
  for (var i = 1, b, c; i < a; i++) {
    b = longRandom(i);
    c = longRandom(i);
    print(longEqual(longMul(b, c), longTc(b, c)));
  }
}

/**
 * test for karatsuba
 */
function kara(a) {
  for (var i = 0, b, c; i < a; i++) {
    b = longRandom(2);
    c = longRandom(2);
    print(longEqual(longMul(b, c), karatsuba(b, c)));
  }
}

/**
 * Computes pi
 * @param {number}
 * @returns {Long}
 */
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

  var a5 = arctan(longNum(5)), a239 = arctan(longNum(239));
  //print(a5);
  //print(a239);
  return longL(longSub(longL(a5, 2), a239), 2).toString();
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
