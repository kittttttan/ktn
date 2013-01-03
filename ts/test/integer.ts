/// <reference path="../lib/integer.ts"/>
var Integer = ktn.Integer;

console.log('  *** overview ***');
console.log(Integer);

console.log('\n  *** constant ***');
console.log('zero = '+ Integer.zero().toString());
console.log('one = '+ Integer.one().toString());

console.log('\n  *** init ***');
var a = Integer.any(100000000000000000000000);
console.log(a.toString());

a = Integer.any(100000020000000000000000);
console.log(a.toString());

console.log('\n  *** basic operations ***');
var n = Integer.num(1234567890);
var m = Integer.str('1000000');

console.log(n.toString() +' + '+ m.toString() +' = '+ n.add(m).toString());
console.log(n.toString() +' - '+ m.toString() +' = '+ n.sub(m).toString());
console.log(n.toString() +' * '+ m.toString() +' = '+ n.mul(m).toString());
console.log(n.toString() +' / '+ m.toString() +' = '+ n.div(m).toString());
console.log(n.toString() +' % '+ m.toString() +' = '+ n.mod(m).toString());
