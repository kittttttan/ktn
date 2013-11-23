/**
 * @file Samples for Integer
 */
(function(G) {
"use strict";

if (typeof require !== 'undefined') {
  G.Integer = require('../src/integer.js').Integer;
}
var Integer = G.Integer;

function pad(value, n) {
  return ('        '+ value).slice(-n);
}

function gcdTest(a) {
  console.log('*** gcd vs gcdBin ***');
  console.log('length    gcd gcdBin');

  var r = 20;
  var m = [], n = [];
  for (var i = 1; i <= a; ++i) {
    m[i] = Integer.random(i * r);
    n[i] = Integer.random(i * r);
  }

  var t0;
  var t1;
  var t2;
  var j, k;
  for (i = 1; i <= a; ++i) {
    t0 = Date.now();
    j = m[i].gcd(n[i]);
    t1 = Date.now();
    k = m[i].gcdBin(n[i]);
    t2 = Date.now();
    
    if (!j.eq(k)) {
      throw new Error('error: values are different\n'+
          j.toString() +'\n'+
          k.toString());
    }
    console.log(pad(i * r, 6) +' '+ pad(t1 - t0, 6) +' '+ pad(t2 - t1, 6));
  }
}

gcdTest(10);

}(this));
