/// <reference path="../lib/long.ts"/>
import long_ = module('../lib/long');
var long = long_.Long;

console.log('  *** overview ***');
console.log(long);

console.log('\n  *** constant ***');
console.log('ZERO = '+ long.ZERO.toString());
console.log('ONE = '+ long.ONE.toString());

console.log('\n  *** init ***');
var a: long_.Long = long.longint(100000000000000000000000);
console.log(a.toString());

a = long.longint(100000020000000000000000);
console.log(a.toString());

console.log('\n  *** basic operations ***');
var n: long_.Long = long.num(1234567890);
var m: long_.Long = long.str('1000000');

console.log(n.toString() +' + '+ m.toString() +' = '+ n.add(m).toString());
console.log(n.toString() +' - '+ m.toString() +' = '+ n.sub(m).toString());
console.log(n.toString() +' * '+ m.toString() +' = '+ n.mul(m).toString());
console.log(n.toString() +' / '+ m.toString() +' = '+ n.div(m).toString());
console.log(n.toString() +' % '+ m.toString() +' = '+ n.mod(m).toString());
