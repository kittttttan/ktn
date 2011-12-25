/**
 * Execute this file with v8 or Rhino,
 * or open fraction.htm with the web browser. 
 */
"use strict";
if (typeof window !== 'undefined') {
  var print = function(a) {
    document.writeln(a);
  };
} else if (typeof load !== 'undefined') {
  load('../long.js');
  load('../fraction.js');
}

function basic() {
  var a = fracNum(1024, 78);
  var b = fracStr('-123/777');
  return ['a = ',a, '\nb = ',b,
         '\na + b = ',fracAdd(a, b), '\na - b = ',fracSub(a, b),
         '\na * b = ',fracMul(a, b), '\na / b = ',fracDiv(a, b)
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
  var i = 2, e = fracNum(2), b = fracNum(1);
  for (; i < a; i++) {
       b = fracMul(b, fracNum(1, i, true));
       e = fracAdd(e, b);
  }
  return e;
}

function main() {
  var d = +new Date;

  print(basic());
  var e = exp(20);
  print('e ~= ' + e.toString());
  print('  ~= ' + e.valueOf());

  print('\nTime: '+ (new Date - d) + 'ms');
}

main();
