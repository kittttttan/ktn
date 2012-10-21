(function(G) {
"use strict";

// Init: define print & load module
var print, _T, _S = '',
    lng;
if (typeof window !== 'undefined') {
  // browser
  print = function(a) { _S += a + '\n'; };
  _T = G.document.getElementById('log');
  lng = Long;
} else if (typeof load !== 'undefined') {
  // V8
  print = G.print;
  load('../lib/lng.js');
  lng = Long;
} else if (typeof require !== 'undefined') {
  // node
  print = console.log;
  lng = require('../lib/lng.js').Long;
}

/**
 * Basic operations
 * @returns {string}
 */
function basic() {
  var r = (Math.random() * 4 | 0) + 3,
      a = lng.random(r),
      b = lng.num(100000);
  return [
      '    a = ',a, '\n    b = ',b.toString(), '\na + b = ',a.add(b),
      '\na - b = ',a.sub(b), '\na * b = ',a.mul(b),
      '\na / b = ',a.div(b), '\na % b = ',a.mod(b),
      '\na ^ 2 = ',a.pow(2), '\na^0.5 = ',a.pow(0.5), ' ~ ',a.sqrt(),
      '\na >> 2 = ',a.rightShift(2),'\na << 2 = ',a.leftShift(2),
      '\ngcd(a, b) = ',a.gcd(b), ' = ',a.gcdBin(b)
      ].join('');
}

/**
 * Factorial
 * @param {number} a
 * @returns {Long} a!
 */
function fact(a) {
  var f = lng.num(1);
  for (var i = 2; i < a + 1; ++i) {
    f = f.mul(lng.num(i));
  }
  return f;
}

/**
 * Calculates fibonacchi number simply
 * @param {number} a
 * @returns {Long} <var>a</var>th fibonacchi number
 */
function fib(a) {
  var b = lng.num(0);
  for (var i = 0, c = lng.num(1), d; i < a; ++i) {
    d = b.clone();
    b = b.add(c);
    c = d;
  }
  return b;
}

/**
 * test for lngSquare
 * @param {number} a
 */
function square(a) {
  for (var i = 1, t; i < a; ++i) {
    t = lng.random(i);
    print(t.mul(t).equal(t.square()));
  }
}

/**
 * Compares performance lngMul vs lngSquare
 * @param {number} a
 */
function sqvsmul(a) {
  var nums = [];
  for (var i = 0; i < a; ++i) {
    nums[i] = lng.random(i + 1);
  }
  var t0 = Date.now();
  for (i = 0; i < a; ++i) {
    nums[i].mul(nums[i]);
  }
  var t1 = Date.now();
  for (i = 0; i < a; ++i) {
    nums[i].square();
  }
  var t2 = Date.now();
  print('mul: '+ (t1 - t0) +'ms\nsq:  '+ (t2 - t1) +'ms\n     '
      + (100*(t2 - t1)/(t1 - t0)).toFixed(2) +'%');
}

/**
 * test for lngGcdBin
 * @param {number} a
 */
function gcd(a) {
  for (var i = 0, b, c; i < a; ++i) {
    b = lng.random(i + 20);
    c = lng.random(i + 20);
    print(b.gcd(c).equal(b._gcd(c)));
  }
}

/**
 * Compares performance lngGcd vs lngGcdBin
 * @param {number} a
 */
function gcdvsbin(a) {
  var m = [], n = [];
  for (var i = 0; i < a; ++i) {
    m[i] = lng.random(i + 20);
    n[i] = lng.random(i + 20);
  }
  var t0 = Date.now();
  for (i = 0; i < a; ++i) {
    m[i].gcd(n[i]);
  }
  var t1 = Date.now();
  for (i = 0; i < a; ++i) {
    m[i]._gcd(n[i]);
  }
  var t2 = Date.now();
  print('gcd: '+ (t1 - t0) +'ms\nbin: '+ (t2 - t1) +'ms\n      '
      + (100*(t2 - t1)/(t1 - t0)).toFixed(2) +'%');
}

/**
 * Computes pi
 * @param {number}
 * @returns {Long}
 */
function pi(a) {
  if (!a) { a = 1; }
  var n = lng.num(10).pow(a);

  function arccot(m) {
    var c = n, a = c.div(m), b = a.clone(), m2 = m.square(),
        k = lng.num(1), s = true, l2 = lng.num(2);
    while (c.isNonZero()) {
      b = b.div(m2);
      k = k.add(l2);
      c = b.div(k);
      s = !s;
      if (s) {
        a = a.add(c);
      } else {
        a = a.sub(c);
      }
    }
    return a;
  }

  var a5 = arccot(lng.num(5)), a239 = arccot(lng.num(239));
  //print(a5);
  //print(a239);
  return a5.leftShift(2).sub(a239).leftShift(2);
}

// Test
var d = Date.now();

print('-- basic operations --');
print(basic());

print('\n-- fibonacchi --');
print('fib(77) = ');
print(fib(77).toString());

print('\n-- factorial --');
print('77! = ');
print(fact(77).toString());

print('\n-- pi --');
print('pi(77) = ');
print(pi(77).toString());

print('\nTime: '+ (Date.now() - d) + 'ms');

if (_T) { _T.value = _S; }
}(this));
