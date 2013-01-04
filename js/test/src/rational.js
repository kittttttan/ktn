/**
 * @file Samples for Rational
 */
(function(G) {
"use strict";
var print, _T, _S = '',
    Rational;
if (typeof window !== 'undefined') {
  // browser
  print = function(a) { _S += a + '\n'; };
  _T = G.document.getElementById('log');
  Rational = G.ktn.Rational;
} else if (typeof require !== 'undefined') {
  // node
  print = console.log;
  Rational = require('../lib/rational.js').Rational;
}

function basic() {
  var a = Rational.num(1024, 78);
  var b = Rational.str('-123/777');
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
  var i = 2, e = Rational.num(2), b = Rational.num(1);
  for (; i < a; i++) {
       b = b.mul(Rational.num(1, i, true));
       e = e.add(b);
  }
  return e;
}

// Test
var d = Date.now();

print(basic());

var e = exp(20);
print('e ~= ' + e.toString());
print('  ~= ' + e.valueOf());

print('\nTime: '+ (Date.now() - d) + 'ms');

if (_T) { _T.value = _S; }
}(this));
