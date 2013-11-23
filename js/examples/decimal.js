/**
 * @file Samples for Decimal
 */
(function(G) {
"use strict";

if (typeof require !== 'undefined') {
  G.Rational = require('../src/rational.js').Rational;
  G.Decimal = require('../src/decimal.js').Decimal;
}
var Rational = G.Rational;
var Decimal = G.Decimal;

function basic() {
  var a = Decimal.dec(1, 2);
  var b = Decimal.str('0.0777');
  return ['a = ',a, '\nb = ',b,
          '\na + b = ',a.add(b), '\na - b = ',a.sub(b),
          '\na * b = ',a.mul(b), '\na / b = ',a.div(b)
          ].join('');
}

/**
 * Calculate Napier's constant
 * e = 2.71828...
 * @param {number} a Loop time
 * @returns {number}
 */
function exp(a) {
  a = a || 10;
  var i = 2;
  var e = Rational.num(2);
  var b = Rational.num(1);
  for (; i < a; ++i) {
       b = b.mul(Rational.num(1, i, true));
       e = e.add(b);
  }
  return e;
}

// Test
var d = Date.now();

console.log(basic());

var e = exp(20);
console.log('e ~= ' + e.toString());
console.log('  ~= ' + Decimal.rat(e, 30).toString());

console.log('\nTime: '+ (Date.now() - d) + 'ms');

}(this));
