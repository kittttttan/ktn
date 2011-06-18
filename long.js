/**
 * @fileOverview Big Integer in JavaScript.
 * @version 2011-06-18
 * @author kittttttan
 * @url http://kittttttan.web.fc2.com/math/mathjs.html
 * @example
 *   var a = longStr("12345678909876543210"); // from String
 *   var b = longNum(7777777);  // from Number
 *   longMul(a, b);  // => 96021937474622850618244170
 * @license
 * Based on:
 *   BigInt.js - Arbitrary size integer math package for JavaScript
 *   Copyright (C) 2000 Masanao Izumo <iz@onicos.co.jp>
 *   Version: 1.0.1
 *   Licence: GPL
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 */

//"use strict";

/**
 * Big integer.
 * @class Big integer
 * @property {Array.<number>} _ds Digits
 * @property {boolean} _sn Sign +, -
 */
function Long() {
  this._ds = [0];
  this._sn = true;
}

/**
 * Set length.
 * @private
 * @param {number} a Length
 * @param {boolean} b Sign
 */
function longAlloc(a, b) {
  var c = new Long;
  if (!b) {
    c._sn = false;
  }
  c._ds.length = a;
  return c;
}

/**
 * Assign zero to initialize.
 * @private
 * @param {Long} a
 * @param {number} b
 * @returns {Long}
 */
function longFillZero(a, b) {
  var ds = a._ds;
  while (b--) { ds[b] = 0; }
  return a;
}

/**
 * Convert integer to Long.
 * @param {number} a
 * @returns {Long}
 */
function longNum(a) {
  var b = new Long;
  if (a < 0) {
    a = -a;
    b._sn = false;
  }
  a &= 0x7fffffff;
  b._ds[0] = a & 0xffff;
  var c = a >>> 16;
  if (c) {
    b._ds[1] = c & 0xffff;
  }
  return b;
}

/**
 * 1
 * @constant
 * @type {Long}
 */
var LONG_ONE = longNum(1);

/**
 * 0
 * @constant
 * @type {Long}
 */
var LONG_ZERO = new Long;

/**
 * Delete zeros.
 * @param {Long} a
 * @returns {Long}
 */
function longNorm(a) {
  var ds = a._ds;
  var len = ds.length;
  do { len -= 1; } while (len && !ds[len]);
  ds.length = len + 1;
  // -0 -> +0
  if (!ds[0] && !len) { a._sn = true; }
  return a;
}

/**
 * Converts string to Long.
 * @param {string} a For example '-9' or 'FF' etc.
 * @param {number} b Base 2, 8, 10 or 16
 * @returns {Long}
 */
function longStr(a, /** @default 10 */b) {
  b = b || 10;
  if (b === 16) { return longByte(a.slice(0,-1)); }
  var a_i = 0;
  var sign = true;
  a += '@';	// Terminator;
  if (a.charAt(a_i) === '+') {
    a_i += 1;
  } else if (a.charAt(a_i) === '-') {
    a_i += 1;
    sign = false;
  }
  // Ignore following zeros. '00102' is regarded as '102'.
  while (a.charAt(a_i) === '0') { a_i += 1; }
  if (a.charAt(a_i) === '@') { return new Long; }
  if (b === 8) {
    var len = 3 * (a.length - a_i);
  } else {
    if (a.charAt(a_i) === '@') { a_i -= 1; }
    len = (a.length - a_i) << 2;
  }
  len = (len >>> 4) + 1;
  var z = longAlloc(len, sign);
  longFillZero(z, len);
  var zds = z._ds;
  var i;
  var blen = 1;
  while (true) {
    c = a.charAt(a_i);
    a_i += 1;
    if (c === '@') { break; }
    c = parseInt(c, 10);
    i = 0;
    while (true) {
      while (i < blen) {
        c += zds[i] * b;
        zds[i] = c & 0xffff;
        i += 1;
        c >>>= 16;
      }
      if (c) {
        blen += 1;
      } else {
        break;
      }
    }
  }
  return longNorm(z);
}

/**
 * Converts hex to Long.
 * @param {string} a For example 'ff' or '-123456789abcdef' etc.
 * @throws {Error} ValueError
 * @returns {Long}
 */
function longByte(a) {
  if (!a) { return new Long; }
  if (a.charAt(0) === '-') {
    if (!a.charAt(1)) { return new Long; }
    var s = false;
    a = a.substring(1);
  } else {
    s = true;
  }
  var b = [];
  var t = a.slice(-4);
  var n;
  while (t) {
    n = parseInt(t, 16);
    if (isNaN(n)) {
      throw new Error('longByte('+arguments[0]+'):ValueError');
    }
    b.push(n);
    a = a.slice(0, -4);
    t = a.slice(-4);
  }
  var z = new Long;
  z._sn = s;
  z._ds = b;
  return longNorm(z);
}

/**
 * Copy Long.
 * @param {Long} a
 * @returns {Long}
 */
function longClone(a) {
  var b = new Long;
  b._sn = a._sn;
  b._ds = Array.prototype.concat.call(a._ds);
  return b;
}

/**
 * Converts anything to Long.
 * @param {Object} a
 * @returns {Long}
 */
function longint(a) {
  if (typeof a === 'object') {
    return (a instanceof Long) ? longClone(a) : new Long;
  }
  if (typeof a === 'string') {
    return longStr(a);
  }
  if (typeof a === 'number') {
    if (-2147483647 <= a && a <= 2147483647) {
      return longNum(a);
    }
    a += '';
    var i = a.indexOf('e', 0);
    if (i < 0) {
      // 'e' is no found
      return longStr(a);
    }
    var a1 = a.substr(0, i);
    var a2 = a.substr(i + 2, a.length - (i + 2));
    var fpt = a1.indeaOf('.', 0);
    if (fpt >= 0) {
      // '.' is found
      var np = a1.length - (fpt + 1);
      a1 = a1.substr(0, fpt) + a1.substr(fpt + 1, np);
      a2 -= np;
    } else {
      // '.' is not found
      a2 = parseInt(a2, 10);
    }
    while (a2-- > 0) { a1 += '0'; }
    return longStr(a1);
  }
  return new Long;
}

/**
 * Negate Long.
 * @param {Long} a
 * @returns {Long} -a
 */
function longNeg(a) {
  var z = longClone(a);
  z._sn = z._sn ? false : true;
  return z;
}

/**
 * Absolute Long.
 * @param {Long} a
 * @returns {Long} |a|
 */
function longAbs(a){
  var z = longClone(a);
  z._sn = true;
  return z;
}

/**
 * ==
 * @param {Long} a
 * @param {Long} b
 * @returns {boolean}
 */
function longEq(a, b) {
  if (a === b) { return true; }
  a = longint(a);
  b = longint(b);
  var ads = a._ds;
  var bds = b._ds;
  if (a._sn !== b._sn) { return false; }
  if (l !== bds.length) { return false; }
  var i = 0;
  var l = ads.length;
  for (; i < l; i++) {
    if (ads[i] !== bds[i]) { return false; }
  }
  return true;
}

/**
 * ===
 * @param {Long} a
 * @param {Long} b
 * @returns {boolean}
 */
function longEqual(a, b) {
  if (a === b) { return true; }
  if (!(a instanceof Long) || !(b instanceof Long)) { return false; }
  var ads = a._ds;
  var bds = b._ds;
  if (a._sn !== b._sn) { return false; }
  if (l !== bds.length) { return false; }
  var i = 0;
  var l = ads.length;
  for (; i < l; i++) {
    if (ads[i] !== bds[i]) { return false; }
  }
  return true;
}

/**
 * Compare between two absolute values of Long objects.
 * @param {Long} a
 * @param {Long} b
 * @returns {number} <br>
 *    -1 (|a| < |b|)<br>
 *     0 (|a| == |b|)<br>
 *     1 (|a| > |b|)
 */
function longAbsCmp(a, b) {
  if (a === b) { return 0; }
  var ads = a._ds;
  var bds = b._ds;
  var al = ads.length;
  if (al < bds.length) { return -1; }
  if (al > bds.length) { return 1; }
  do { al -= 1; } while (al && ads[al] === bds[al]);
  if (!al && ads[0] === bds[0]) { return 0; }
  return ads[al] > bds[al] ? 1 : -1;
}

/**
 * Compare between two Long.
 * @param {Long} a
 * @param {Long} b
 * @returns {number} <br>
 *    -1 (a < b)<br>
 *     0 (a == b)<br>
 *     1 (a > b)
 */
function longCmp(a, b) {
  if (a === b) { return 0; }
  var ads = a._ds;
  var bds = b._ds;
  var al = ads.length;
  if (a._sn !== b._sn) { return a._sn ? 1 : -1; }
  if (al < bds.length) { return a._sn ? -1 : 1; }
  if (al > bds.length) { return a._sn ? 1 : -1; }
  do { al -= 1; } while (al && ads[al] === bds[al]);
  if (!al && ads[0] === bds[0]) { return a._sn - b._sn; }
  if (ads[al] > bds[al]) { return a._sn ? 1 : -1; }
  return a._sn ? -1 : 1;
}

/**
 * Convert Long to number.
 * @param {Long} a
 * @returns {number}
 */
function longValue(a) {
  var d = 0.0;
  var ds = a._ds;
  var i = ds.length;
  while (i--) { d = ds[i] + 65536.0 * d; }
  if (!a._sn) { d = -d; }
  return d;
}

/**
 * Add absolute values of Long.
 * @param {Long} a
 * @param {Long} b
 * @param {boolean} sign
 * @returns {Long} <br>
 *    |a| + |b| (sign == true)<br>
 *  -(|a| + |b|) (else)
 */
function longAddAbs(a, b, sign) {
  if (a._ds.length > b._ds.length) {
    //swap a,b
    var z = a; a = b; b = z;
  }
  var ads = a._ds;
  var bds = b._ds;
  z = longAlloc(bds.length + 1, sign);
  var zds = z._ds;
  var i = 0;
  var len = ads.length;
  var num = 0;
  for (; i < len; i++) {
    num += ads[i] + bds[i];
    zds[i] = num & 0xffff;
    num >>>= 16;
  }
  len = bds.length;
  while (num && i < len) {
    num += bds[i];
    zds[i] = num & 0xffff;
    i += 1;
    num >>>= 16;
  }
  while (i < len) {
    zds[i] = bds[i];
    i += 1;
  }
  zds[i] = num & 0xffff;
  return longNorm(z);
}

/**
 * Subtract absolute values of Long.
 * @param {Long} a
 * @param {Long} b
 * @param {boolean} sign
 * @returns {Long} <br>
 *     ||a| - |b|| (sign == true)<br>
 *    -||a| - |b|| (else)
 */
function longSubAbs(a, b, sign) {
  var ads = a._ds;
  var bds = b._ds;
  var al = ads.length;
  var bl = bds.length;
  var z = longAlloc(al, sign);
  var zds = z._ds;
  var i = 0;
  var c = 0;
  for (; i < bl; i++) {
    c = ads[i] - bds[i] - c;
    if (c < 0) {
      zds[i] = c & 0xffff;
      c = 1;
    } else {
      zds[i] = c;
      c = 0;
    }
  } 
  for (; i < al; i++) { 
    c = ads[i] - c;
    if (c < 0) {
      zds[i] = c & 0xffff;
      c = 1;
    } else {
      zds[i] = c;
      c = 0;
    }
  }
  return longNorm(z);
}

/**
 * Addition.
 * @param {Long} a
 * @param {Long} b
 * @returns {Long} a + b
 */
function longAdd(a, b) {
  if (a._sn !== b._sn) {
    if (longAbsCmp(a, b) < 0) {
      return longSubAbs(b, a, b._sn);
    }
    return longSubAbs(a, b, a._sn);
  }
  return longAddAbs(a, b, a._sn);
}

/**
 * Subtraction.
 * @param {Long} a
 * @param {Long} b
 * @returns {Long} a - b
 */
function longSub(a, b) {
  if (a._sn === b._sn) {
    if (longAbsCmp(a, b) < 0) {
        return longSubAbs(b, a, b._sn ? false : true);
    }
    return longSubAbs(a, b, a._sn);
  }
  return longAddAbs(a, b, a._sn);
}

/**
 * Multiply with Karatsuba Method.
 * @param {Long} a a._ds.length === 2
 * @param {Long} b b._ds.length === 2
 * @returns {Long} a * b
 */
function karatsuba(a, b) {
  var z = longAlloc(4, a._sn === b._sn);
  var zds = z._ds;
  var ads = a._ds;
  var bds = b._ds;
  zds[0] = ads[0] * bds[0];
  zds[2] = ads[1] * bds[1];
  zds[1] = zds[0] + zds[2] - (ads[1] - ads[0]) * (bds[1] - bds[0]);
  zds[1] += zds[0] >>> 16;
  zds[2] += zds[1] >>> 16;
  zds[3] = zds[2] >>> 16;
  //zds[4] = zds[3] >>> 16;
  zds[0] &= 0xffff;
  zds[1] &= 0xffff;
  zds[2] &= 0xffff;
  //zds[3] &= 0xffff;
  return longNorm(z);
}

/**
 * Multiplication for big.
 * @param {Long} a
 * @param {Long} b
 * @returns {Long} a * b
 */
function longTc(a, b) {
  var ads = a._ds;
  var bds = b._ds;
  var al = ads.length;
  var bl = bds.length; // al >= bl
  var l = (al << 1) - 1;
  var z = longAlloc(l, a._sn === b._sn);
  longFillZero(z, l);
  var zds = z._ds;
  var w = [];
  var i = al;
  while (i-- > bl) {
    bds[i] = 0;
  }
  i = al;
  while (i--) {
    w[i] = ads[i] * bds[i];
  }

  var bt = function(sub, sup) {
    var i, sum = 0;
    for (i = sub; i <= sup; i++) {
      sum += w[i];
    }
    while (sub < sup) {
      sum -= (ads[sup] - ads[sub]) * (bds[sup] - bds[sub]);
      sup -= 1;
      sub += 1;
    }
    return sum;
  };

  for (i = 0; i < al; i++) {
    zds[i] = bt(0, i);
  }
  for (i = 1; i < al; i++) {
    zds[i + al - 1] = bt(i, al - 1);
  }
  var j = 0;
  for (i = 0; i < l; i++) {
    zds[i] += j;
    j = zds[i] >>> 16;
    zds[i] &= 0xffff;
  }
  zds[i] = j;
  return longNorm(z);
}

/**
 * Multiplication.
 * @param {Long} a
 * @param {Long} b
 * @returns {Long} a * b
 */
function longMul(a, b) {
  // if (longEqual(a, b)) {return longSquare(a);}
  var ads = a._ds;
  var bds = b._ds;
  var al = ads.length;
  var bl = bds.length;
  if (al === 2 && bl === 2) { return karatsuba(a, b); }
  if (al > 29 && bl > 29) { return al > bl ? longTc(a, b) : longTc(b, a); }
  var j = al + bl + 1;
  var z = longAlloc(j, a._sn === b._sn);
  longFillZero(z, j);
  var zds = z._ds;
  var i, n, dd, ee;
  for (i = 0; i < al; i++) {
    dd = ads[i]; 
    if (!dd) { continue; }
    n = 0;
    for (j = 0; j < bl; j++) {
      ee = n + dd * bds[j];
      n = zds[i + j] + ee;
      if (ee) { zds[i + j] = n & 0xffff; }
      n >>>= 16;
    }
    if (n) { zds[i + j] = n; }
  }
  return longNorm(z);
}

/**
 * Division or Modulo.
 * @param {Long} a
 * @param {Long} b
 * @param {boolean} modulo If true then mod, else div.
 * @throws {Error} ZeroDivisionError
 * @returns {Long} <br>
 *    a % b (modulo == true)<br>
 *    a / b (else)
 */
function longDivmod(a, b, modulo) {
  var ads = a._ds;
  var bds = b._ds;
  var na = ads.length;
  var nb = bds.length;
  if (nb < 2 && !bds[0]) {
    //return a;
    throw new Error(
        ['ZeroDivisionError:longDivmod(',a,',',b,',',modulo,')'].join(''));
  }

  var albl = na === nb;
  if (na < nb || (albl && ads[na - 1] < bds[nb - 1])) {
    if (modulo) { return longNorm(a); }
    return new Long;
  }

  var dd, z, zds, t, i;
  if (nb === 1) {
    dd = bds[0];
    z = longClone(a);
    zds = z._ds;
    t = 0;
    i = na;
    while (i--) {
      t *= 0x10000;
      t += zds[i];
      zds[i] = (t / dd) & 0xffff;
      t %= dd;
    }
    z._sn = a._sn === b._sn;
    if (modulo) {
      if (!a._sn) { return longNum(-t); }
      return longNum(t);
    }
    return longNorm(z);
  }

  z = longAlloc(albl ? na + 2 : na + 1, a._sn === b._sn);
  zds = z._ds;
  longFillZero(z, zds.length);
  dd = 0x10000 / (bds[nb - 1] + 1) & 0xffff;

  var j, bb, tds, num;
  if (dd !== 1) {
    bb = longClone(b);
    tds = bb._ds;

    for (j = 0, num = 0; j < nb; j++) {
      num += bds[j] * dd;
      tds[j] = num & 0xffff;
      num >>>= 16;
    }

    bds = tds;

    for (j = 0, num = 0; j < na; j++) {
      num += ads[j] * dd;
      zds[j] = num & 0xffff;
      num >>>= 16;
    }

    zds[j] = num & 0xffff;

  } else {
    //zds[na] = 0;
    j = na;
    while (j--) { zds[j] = ads[j]; }
  }

  var q, ee;
  j = albl ? na + 1 : na;
  do {
    if (zds[j] === bds[nb - 1]) {
      q = 0xffff;
    } else {
      q = (zds[j] * 0x10000 + zds[j - 1]) / bds[nb - 1] & 0xffff;
    }

    if (q) {
      i = num = t = 0;
      do {
        t += bds[i] * q;
        ee = (t & 0xffff) - num;
        num = zds[j - nb + i] - ee;
        if (ee) { zds[j - nb + i] = num & 0xffff; }
        num >>= 16;
        t >>>= 16;
      } while (++i < nb);

      num += zds[j - nb + i] - t;
      while (num) {
        i = num = 0;
        q -= 1;

        do {
          ee = num + bds[i];
          num = zds[j - nb + i] + ee;
          if (ee) { zds[j - nb + i] = num & 0xffff; }
          num >>= 16;
        } while (++i < nb);

        num -= 1;
      }
    }
    zds[j] = q;
  } while (--j >= nb);

  if (modulo) {
    var mod = longClone(z);
    if (dd) {
      zds = mod._ds;
      t = 0;
      i = nb;
      while (i--) {
        t = t * 0x10000 + zds[i];
        zds[i] = (t / dd) & 0xffff;
        t %= dd;
      }
    }
    zds.length = nb;
    mod._sn = a._sn;
    return longNorm(mod);
  }

  var div = longClone(z);
  zds = div._ds;
  j = (albl ? na + 2 : na + 1) - nb;
  for (i = 0; i < j; i++) { zds[i] = zds[i + nb]; }
  zds.length = i;
  return longNorm(div);
}

/**
 * Division.
 * @param {Long} a
 * @param {Long} b
 * @returns {Long} a / b
 */
function longDiv(a, b) {
  return longDivmod(a, b, false);
}

/**
 * Modulo.
 * @param {Long} a
 * @param {Long} b
 * @returns {Long} a % b
 */
function longMod(a, b) {
  return longDivmod(a, b, true);
}

/**
 * Fast squaring. (buggy)
 * @ignore
 * @param {Long} a
 * @returns {Long} a * a
 */
function longSquare(a) {
  var ads = a._ds;
  var al = ads.length;
  var s = longAlloc((al << 1) + 1);
  var sds = s._ds;
  longFillZero(s, sds.length);
  var i = 0;
  var j;
  var carry;
  var wai;
  for (; i < al; i++) {
    carry = sds[i << 1] + ads[i] * ads[i];
    sds[i << 1] = carry & 0xffff;
    carry >>>= 16;
    wai = ads[i] << 1;
    for (j = i + 1; j < al; j++) {
      carry += sds[i + j] + ads[j] * wai;
      sds[i + j] = carry & 0xffff;
      carry >>>= 16;
    }
    if (carry) {
      carry += sds[i + al];
      sds[i + al] = carry & 0xffff;
      carry >>>= 16;
    }
    if (carry) { sds[i + al + 1] += carry & 0xffff; }
  }
  return longNorm(s);
}

/**
 * Square root.
 * @param {Long} a
 * @returns {Long} <code>&radic;</code>a
 */
function longSqrt(a){
  var b = longClone(a);
  var c = longNum(1);
  while (longCmp(b, c) > 0) {
    longHalf(b);
    longDouble(c);
  }
  do {
    b = longClone(c);
    c = longAdd(longDivmod(a, c, false), c);
    longHalf(c);
  } while (longCmp(b, c) > 0);
	return b;
}

/**
 * Pow.
 * @param {Long} a
 * @param {number} b
 * @returns {Long} a<sup>b</sup>
 */
function longPow(a, b) {
  if (!b) { return longNum(1); }
  if (b > 0) {
    var result = longNum(1);
    for (; b > 0; b >>= 1, a = longMul(a, a)) {
      if (b & 1) { result = longMul(result, a); }
    }
    return result;
  }
  return undefined;
}

/**
 * Random.
 * @param {number} a Length
 * @returns {Long}
 */
function longRandom(a) {
  var r = longAlloc(a, true);
  var rds = r._ds;
  for (var i = 0; i < a + 1; i++) {
    rds[i] = Math.random() * 0xffff | 0;
  }
  return longNorm(r);
}

/**
 * Greatest Common Divisor.
 * @param {Long} a
 * @param {Long} b
 * @returns {Long}
 */
function longGcd(a, b) {
  var c;
  while ((c = longDivmod(a, b, true)).isNonZero()) {
    a = b;
    b = c;
  }
  return b;
}

/**
 * Convert Long to String.
 * @param {Long} a
 * @param {number} b Base 2, 8, 10 or 16
 * @returns {string}
 */
function longToString(a, /** @default 10 */b) {
  b = b || 10;
  if (b === 2 || b === 16) { return longToByte(a, b); }
  var i = a._ds.length;
  if (i < 2 && !a._ds[0]) { return '0'; }
  var j;
  var hbase;
  switch (b) {
    case 8:
      j = (i << 4) + 2;
      hbase = 0x1000;
      break;
    case 10: default:
      j = (i * 241 / 50 | 0) + 2;
      hbase = 10000;
      break;
  }
  var t = longClone(a);
  var ds = t._ds;
  var digits = '0123456789';
  var k;
  var num;
  var c;
  var s = '';
  while (i && j) {
    k = i;
    num = 0;
    while (k--) {
      num = (num << 16) + ds[k];
      ds[k] = num / hbase | 0;
      num %= hbase;
    }
    if (!ds[i-1]) { i -= 1; }
    k = 4;
    while (k--) {
      c = num % b;
      s = digits.charAt(c) + s;
      j -= 1;
      num = num / b | 0;
      if (!i && !num) { break; }
    }
  }
  i = 0;
  while (s[i] === '0') { i += 1; }
  if (i) { s = s.substring(i, s.length); }
  if (!a._sn) { s = '-' + s; }
  return s;
}

/**
 * Convert Long to String.
 * @param {Long} a
 * @param {number} b Base 2 or 16
 * @returns {string}
 */
function longToByte(a, /** @default 16 */b) {
  //b = b || 16;
  if (b !== 2) { b = 16; }
  var ds = a._ds;
  var i = ds.length - 1;
  var s = a._sn ? [] : ['-'];
  s.push(ds[i].toString(b));
  if (b === 2) {
    var z = '0000000000000000';
    var l = -16;
  } else {
    z = '0000';
    l = -4;
  }
  while (i) {
    i -= 1;
    s.push((z + ds[i].toString(b)).slice(l));
  }
  return s.join('');
}

/**
 * Right shift by 1.
 * @param {Long} a
 * @returns {Long} a >> 1
 */
function longHalf(a) {
  var ds = a._ds;
  var i = 0;
  var l = ds.length - 1;
  for (; i < l; i++) {
    ds[i] = (((ds[i + 1] & 1) << 16) + ds[i]) >>> 1;
  }
  ds[l] >>>= 1;
  return longNorm(a);
}

/**
 * Left shift by 1.
 * @param {Long} a
 * @returns {Long} a << 1
 */
function longDouble(a) {
  var ds = a._ds;
  var l = ds.length;
  var i = 0;
  var d = 0;
  var t;
  for (; i < l; i++) {
    t = (ds[i] << 1) + d;
    ds[i] = t & 0xffff;
    d = t >>> 16;
  }
  if (d) { ds[l] = d; }
  return longNorm(a);
}

/**
 * <<
 * @param {Long} a
 * @param {number} b
 * @returns {Long}
 */
function longL(a, b) {
  var ad = a._ds;
  var l = ad.length;
  var d = b >> 4;
  var cl = l + d + 1;
  var bb = b & 0xf;
  var c = longAlloc(cl, a._sn);
  var cd = c._ds;
  var i = 0;
  for (;i < d; i++) { cd[i] = 0; }
  var t;
  var carry = 0;
  for (i = 0; i < l; i++) {
    t = (ad[i] << bb) + carry;
    cd[i + d] = t & 0xffff;
    carry = t >> 16;
  }
  cd[i + d] = carry;
  return longNorm(c);
}

/**
 * >>
 * @param {Long} a
 * @param {number} b
 * @returns {Long}
 */
function longR(a, b) {
  var ad = a._ds;
  var l = ad.length;
  var d = b >> 4;
  var bb = b & 0xf;
  var mask = (1 << bb) - 1;
  if (l <= d) { return new Long; }
  var cl = l - d;
  var c = longAlloc(cl, a._sn);
  var cd = c._ds;
  var i = 0;
  for (; i < cl - 1; i++) {
    cd[i] = ((ad[i + d + 1] & mask) << (16 - bb)) + (ad[i + d] >> bb);
  }
  cd[i] = ad[i + d] >> bb;
  return longNorm(c);
}

/**
 * Add zeros and shift decimal.
 * @param {Long} a
 * @param {number} b Number of zeros.
 * @returns {Long} a * 10<sup>n</sup>
 */
function longAddZero(a, b) {
  var zeros = '';
  var z = '0';
  for (; b > 0; b >>>= 1, z += z) {
    if (b & 1) { zeros += z; }
  }
  return longStr(a.toString() + zeros);
}

Long.prototype = {
  constructor: Long,

  /**
   * @param {number} a
   * @returns {string}
   * @see longToString
   */
  toString: function(a) { return longToString(this, a); },

  /**
   * @see longValue
   * @returns {number}
   */
  valueOf: function() { return longValue(this); },

  /** @returns {Array.<number>} */
  getDigits: function() { return this._ds; },
  /** @returns {boolean} */
  getSign: function() { return this._sn; },

  /**
   * @returns {Long}
   * @see longClone
   */
  clone: function() { return longClone(this); },

  /**
   * @param {number} a
   * @returns {Long}
   * @see longAddZero
   */
  addzero: function(a) { return longAddZero(this, a); },

  /**
   * @param {number} a
   * @returns {Long}
   * @see longR
   */
  rshift: function(a) { return longR(this, a); },

  /**
   * @param {number} a
   * @returns {Long}
   * @see longL
   */
  lshift: function(a) { return longL(this, a); },

  /**
   * @returns {Long}
   * @see longHalf
   */
  half: function() { return longHalf(this); },

  /**
   * @returns {Long}
   * @see longDouble
   */
  dbl: function() { return longDouble(this); },

  /** @returns {boolean} */
  isOdd: function() { return !!(this._ds[0] & 1); },
  /** @returns {boolean} */
  isEven: function() { return !(this._ds[0] & 1); },
  /** @returns {boolean} */
  isNonZero: function() { return (this._ds.length > 1 || this._ds[0]); },

  /**
   * @returns {Long}
   * @see longSqrt
   */
  sqrt: function() { return longSqrt(this); },

  /**
   * @param {number} a
   * @returns {Long}
   * @see longPow
   */
  pow: function(a) { return longPow(this, a); },

  /**
   * @param {number} a
   * @returns {Long}
   * @see longGcd
   */
  gcd: function(a) { return longGcd(this, a); },

  /** @returns {number} */
  _len_: function() { return this._ds.length; },

  /**
   * @param {Long} a
   * @returns {Long}
   * @see longAdd
   */
  add: function(a) { return longAdd(this, a); },

  /**
   * @param {Long} a
   * @returns {Long}
   * @see longSub
   */
  sub: function(a) { return longSub(this, a); },

  /**
   * @param {Long} a
   * @returns {Long}
   * @see longMul
   */
  mul: function(a) { return longMul(this, a); },

  /**
   * @param {Long} a
   * @returns {Long}
   * @see longDivmod
   */
  div: function(a) { return longDivmod(this, a, false); },

  /**
   * @param {Long} a
   * @returns {Long}
   * @see longDivmod
   */
  mod: function(a) { return longDivmod(this, a, true); },

  /**
   * @param {object} a
   * @returns {Long}
   */
  _add_: function(a) { return longAdd(this, longint(a)); },

  /**
   * @param {object} a
   * @returns {Long}
   */
  _sub_: function(a) { return longSub(this, longint(a)); },

  /**
   * @param {object} a
   * @returns {Long}
   */
  _mul_: function(a) { return longMul(this, longint(a)); },

  /**
   * @param {object} a
   * @returns {Long}
   */
  _div_: function(a) { return longDivmod(this, longint(a), false); },

  /**
   * @param {object} a
   * @returns {Long}
   */
  _mod_: function(a) { return longDivmod(this, longint(a), true); },

  /**
   * @param {object} a
   * @returns {number}
   * @see longCmp
   */
  _cmp_: function(a) { return longCmp(this, longint(a)); },

  /**
   * @param {object} a
   * @returns {Long}
   * @see longEq
   */
  _eq_: function(a) { return longEq(this, a); },

  /**
   * @param {Long} a
   * @returns {Long}
   * @see longEqual
   */
  _equal_: function(a) { return longEqual(this, a); },

  /** @returns {number} */
  _co_: function() { return 1; },

  /**
   * @param {Long} a
   * @returns {Long}
   * @see longAbs
   */
  _abs_: function() { return longAbs(this); },

  /**
   * @param {Long} a
   * @returns {Long}
   * @see longNeg
   */
  _neg_: function() { return longNeg(this); }
};
