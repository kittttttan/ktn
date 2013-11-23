/**
 * @file Samples for Integer
 */
(function(G) {
"use strict";

if (typeof require !== 'undefined') {
  G.Integer = require('../src/integer.js').Integer;
}
var Integer = G.Integer;

/**
 * Basic operations
 */
function basic() {
  var r = (Math.random() * 4 | 0) + 3,
      a = Integer.random(r),
      b = Integer.num(100000);
  console.log('    a = '+ a.toString());
  console.log('    b = '+ b.toString());
  console.log('a + b = '+ a.add(b).toString());
  console.log('a - b = '+ a.sub(b).toString());
  console.log('a * b = '+ a.mul(b).toString());
  console.log('a / b = '+ a.div(b).toString());
  console.log('a % b = '+ a.mod(b).toString());
  console.log('a >> 2 = '+ a.rightShift(2).toString());
  console.log('a << 2 = '+ a.leftShift(2).toString());
  console.log('a >> 17 = '+ a.rightShift(17).toString());
  console.log('a << 17 = '+ a.leftShift(17).toString());
  console.log('a ^ 2 = '+ a.pow(2).toString());
  console.log('a ^ 0.5 = '+ a.pow(0.5).toString());
  console.log('        ~ '+ a.sqrt().toString());
  console.log('gcd(a, b) = '+ a.gcd(b).toString());
  console.log('          = '+ a.gcdBin(b).toString());
}

/**
 * Calculates fibonacchi number simply
 * @param {number} a
 * @returns {Integer} <var>a</var>th fibonacchi number
 */
function fib(a) {
  var b = Integer.zero();
  for (var i = 0, c = Integer.one(), d; i < a; ++i) {
    d = b.clone();
    b = b.add(c);
    c = d;
  }
  return b;
}

/**
 * test for square
 * @param {number} a
 */
function square(a) {
  for (var i = 1, t; i < a; ++i) {
    t = Integer.random(i);
    console.log(t.mul(t).equal(t.square()));
  }
}

/**
 * Compares performance mul vs square
 * @param {number} a
 */
function sqvsmul(a) {
  var nums = [];
  for (var i = 0; i < a; ++i) {
    nums[i] = Integer.random(i + 1);
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
  console.log('mul: '+ (t1 - t0) +'ms\nsq:  '+ (t2 - t1) +'ms\n     '
      + (100*(t2 - t1)/(t1 - t0)).toFixed(2) +'%');
}

/**
 * test for Integer#gcdBin
 * @param {number} a
 */
function gcd(a) {
  for (var i = 0, b, c; i < a; ++i) {
    b = Integer.random(i + 20);
    c = Integer.random(i + 20);
    console.log(b.gcd(c).equal(b.gcdBin(c)));
  }
}

/**
 * Compares performance Integer#gcd vs Integer#gcdBin
 * @param {number} a
 */
function gcdvsbin(a) {
  var m = [], n = [];
  for (var i = 0; i < a; ++i) {
  for (i = 0; i < a; ++i) {
    m[i].gcd(n[i]);
  }
  var t1 = Date.now();
  for (i = 0; i < a; ++i) {
    m[i].gcdBin(n[i]);
  }
  var t2 = Date.now();
  console.log('gcd: '+ (t1 - t0) +'ms\nbin: '+ (t2 - t1) +'ms\n      '
      + (100*(t2 - t1)/(t1 - t0)).toFixed(2) +'%');
    m[i] = Integer.random(i + 20);
    n[i] = Integer.random(i + 20);
  }
  var t0 = Date.now();
}

/**
 * Computes pi
 * @param {number}
 * @returns {Integer}
 */
function pi(a) {
  if (!a) { a = 1; }
  var n = Integer.num(10).pow(a);

  function arccot(m) {
    var c = n, a = c.div(m), b = a.clone(), m2 = m.square(),
        k = Integer.num(1), s = true, l2 = Integer.num(2);
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

  var a5 = arccot(Integer.num(5)), a239 = arccot(Integer.num(239));
  //print(a5);
  //print(a239);
  return a5.leftShift(2).sub(a239).leftShift(2);
}

// Test
var d = Date.now();
console.log('-- basic operations --');
basic();

console.log('\n-- fibonacchi --');
console.log('fib(77) = ');
console.log(fib(77).toString());

console.log('\n-- pi --');
console.log('pi(77) = ');
console.log(pi(77).toString());
console.log('\nTime: '+ (Date.now() - d) + 'ms');

}(this));
