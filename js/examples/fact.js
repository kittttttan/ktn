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
 * Factorial
 * @param {number} a
 * @return {Integer} a!
 */
function fact(a) {
  var f = Integer.one();
  for (var i = 2; i < a + 1; ++i) {
    f = f.mul(Integer.num(i));
  }
  return f;
}

function pad(value, n) {
  return ('        '+ value).slice(-n);
}

var t0, t1, t2;
var s1, s2;
console.log('*** simple fact vs Integer.factorial ***');
console.log('     n  simple Integer');

var arr = [999, 1999, 2999, 3999, 6999, 9999];
for (var i = 0, l = arr.length; i < l; ++i) {
  t0 = Date.now();
  s1 = fact(arr[i]);
  t1 = Date.now();
  s2 = Integer.factorial(arr[i]);
  t2 = Date.now();
  
  if (!s1.equal(s2)) {
    throw new Error('error: values are different\n'+
        s1.toString() +'\n'+
        s2.toString());
  }
  
  console.log(pad(arr[i], 6) + pad(t1 - t0, 8) + pad(t2 - t1, 8));
}

}(this));
