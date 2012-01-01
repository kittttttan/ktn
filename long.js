/**
 * @fileOverview Big Integer in JavaScript.
 * @version 2012-01-01
 * @author kittttttan
 * @url http://kittttttan.web.fc2.com/math/mathjs.html
 * @example
 *   var a = longStr("12345678909876543210");
 *   var b = longNum(7777777);
 *   var c = longMul(a, b);
 *   // c.toString() == '96021937474622850618244170'
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
 * @property {Array.<number>} _d Digits [d0, d1, ..., dn]
 * @property {boolean} _s Sign +, -. true means +.
 */
function Long() {
  this._d = [0];
  this._s = true;
}

/**
 * Set length.
 * @private
 * @param {number} length
 * @param {boolean} sign
 */
function longAlloc(length, sign) {
  var a = new Long();
  if (!sign) {
    a._s = false;
  }
  a._d.length = length;
  return a;
}

/**
 * Assign zero to initialize.
 * @private
 * @param {Long} a
 * @param {number} b Length
 * @returns {Long}
 */
function longFillZero(a, b) {
  var d = a._d;
  while (b--) { d[b] = 0; }
  return a;
}

/**
 * Convert integer to Long.
 * @param {number} n
 * @returns {Long}
 */
function longNum(n) {
  var a = new Long();
  if (n < 0) {
    n = -n;
    a._s = false;
  }
  n &= 0x7fffffff;
  a._d[0] = n & 0xffff;
  n >>>= 16;
  if (n) {
    a._d[1] = n & 0xffff;
  }
  return a;
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
var LONG_ZERO = new Long();

/**
 * Delete following zeros. [2, 0, 1, 0, 0] -> [2, 0, 1]
 * @param {Long} a
 * @returns {Long}
 */
function longNorm(a) {
  var d = a._d,
      l = d.length;
  do { l--; } while (l && !d[l]);
  d.length = l + 1;
  // -0 -> +0
  if (!l) { a._s = true; }
  return a;
}

/**
 * Converts hex to Long.
 * @param {string} str For example 'ff' or '-123456789abcdef' etc.
 * @returns {Long}
 */
function longByte(str) {
  if (!str) { return new Long(); }

  var s = true;
  if (str.charAt(0) === '-') {
    if (!str.charAt(1)) { return new Long(); }
    s = false;
    str = str.substring(1);
  }

  var d = [], t, n;
  for (;;) {
    t = str.slice(-4);
    if (!t) { break; }
    n = parseInt(t, 16);
    if (isNaN(n)) { break; }
    d.push(n);
    str = str.slice(0, -4);
  }

  var z = new Long();
  z._s = s;
  z._d = d;
  return longNorm(z);
}

/**
 * Converts string to Long.
 * @param {string} str For example '-9' or 'FF' etc.
 * @param {number} base 2, 8, 10 or 16
 * @returns {Long}
 */
function longStr(str, /** @default 10 */base) {
  if (base === 16) { return longByte(str); }
  if (!base) { base = 10; }

  var index = 0, sign, len;
  if (str.charAt(index) === '+') {
    sign = true;
    index++;
  } else if (str.charAt(index) === '-') {
    sign = false;
    index++;
  }
  // Ignore following zeros. '00102' is regarded as '102'.
  while (str.charAt(index) === '0') { index++; }
  if (!str.charAt(index)) { return new Long(); }
  if (base === 8) {
    len = 3 * (str.length + 1 - index);
  } else {
    if (!str.charAt(index)) { index--; }
    len = (str.length + 1 - index) << 2;
  }
  len = (len >>> 4) + 1;

  var z = longAlloc(len, sign);
  longFillZero(z, len);

  for (var c, n, zd = z._d, bl = 1;;) {
    c = str.charAt(index);
    index++;
    if (!c) { break; }
    n = parseInt(c, 10);
    for (var i = 0;;) {
      for (; i < bl; i++) {
        n += zd[i] * base;
        zd[i] = n & 0xffff;
        n >>>= 16;
      }
      if (n) {
        bl++;
      } else {
        break;
      }
    }
  }
  return longNorm(z);
}

/**
 * Copy Long.
 * @param {Long} a
 * @returns {Long}
 */
function longClone(a) {
  var b = new Long();
  b._s = a._s;
  b._d = Array.prototype.concat.call(a._d);
  return b;
}

/**
 * Converts anything to Long.
 * @param {Object} a
 * @returns {Long}
 */
function longint(a) {
  if (typeof a === 'object') {
    if (a instanceof Long) { return longClone(a); }
    return new Long();
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

    var a1 = a.substr(0, i),
        a2 = a.substr(i + 2, a.length - (i + 2)),
        fpt = a1.indeaOf('.', 0);
    if (fpt < 0) {
      // '.' is not found
      a2 = parseInt(a2, 10);
    } else {
      var np = a1.length - (fpt + 1);
      a1 = a1.substr(0, fpt) + a1.substr(fpt + 1, np);
      a2 -= np;
    }
    for (; a2 > 0; a2--) { a1 += '0'; }
    return longStr(a1);
  }
  return new Long();
}

/**
 * Negate Long.
 * @param {Long} a
 * @returns {Long} -a
 */
function longNeg(a) {
  var z = longClone(a);
  if (z.isNonZero()) { z._s = !z._s; }
  return z;
}

/**
 * Absolute Long.
 * @param {Long} a
 * @returns {Long} |a|
 */
function longAbs(a){
  var z = longClone(a);
  z._s = true;
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
  if (a._s !== b._s) { return false; }
  var ad = a._d,
      bd = b._d,
      l = ad.length;
  if (l !== bd.length) { return false; }
  for (var i = 0; i < l; i++) {
    if (ad[i] !== bd[i]) { return false; }
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
  if (a._s !== b._s) { return false; }
  var ad = a._d,
      bd = b._d,
      l = ad.length;
  if (l !== bd.length) { return false; }
  for (var i = 0; i < l; i++) {
    if (ad[i] !== bd[i]) { return false; }
  }
  return true;
}

/**
 * Compare between two absolute values of Long objects.
 * @param {Long} a
 * @param {Long} b
 * @returns {number} <br>
 *    -1 (|a| < |b|)<br>
 *     0 (|a| = |b|)<br>
 *     1 (|a| > |b|)
 */
function longAbsCmp(a, b) {
  if (a === b) { return 0; }
  var ad = a._d,
      bd = b._d,
      al = ad.length;
  if (al < bd.length) { return -1; }
  if (al > bd.length) { return 1; }
  do { al--; } while (al && ad[al] === bd[al]);
  if (!al && ad[0] === bd[0]) { return 0; }
  return ad[al] > bd[al] ? 1 : -1;
}

/**
 * Compare between two Long.
 * @param {Long} a
 * @param {Long} b
 * @returns {number} <br>
 *    -1 (a < b)<br>
 *     0 (a = b)<br>
 *     1 (a > b)
 */
function longCmp(a, b) {
  if (a === b) { return 0; }
  var ad = a._d,
      bd = b._d,
      al = ad.length;
  if (a._s !== b._s) { return a._s ? 1 : -1; }
  if (al < bd.length) { return a._s ? -1 : 1; }
  if (al > bd.length) { return a._s ? 1 : -1; }
  do { al--; } while (al && ad[al] === bd[al]);
  if (!al && ad[0] === bd[0]) { return a._s - b._s; }
  if (ad[al] > bd[al]) { return a._s ? 1 : -1; }
  return a._s ? -1 : 1;
}

/**
 * Convert Long to number.
 * @param {Long} a
 * @returns {number}
 */
function longValue(a) {
  var f = 0.0,
      d = a._d,
      i = d.length;
  while (i--) { f = d[i] + 65536.0 * f; }
  if (!a._s) { f = -f; }
  return f;
}

/**
 * Right shift by 1.
 * @param {Long} a
 * @returns {Long} a >> 1
 */
function longHalf(a) {
  var d = a._d,
      l = d.length - 1;
  for (var i = 0; i < l; i++) {
    d[i] = (((d[i + 1] & 1) << 16) + d[i]) >>> 1;
  }
  d[l] >>>= 1;
  return longNorm(a);
}

/**
 * Left shift by 1.
 * @param {Long} a
 * @returns {Long} a << 1
 */
function longDouble(a) {
  var d = a._d,
      l = d.length,
      c = 0;
  for (var i = 0, t = 0; i < l; i++) {
    t = (d[i] << 1) + c;
    d[i] = t & 0xffff;
    c = t >>> 16;
  }
  if (c) { d[l] = c; }
  return longNorm(a);
}

/**
 * <<
 * @param {Long} a
 * @param {number} b
 * @returns {Long}
 */
function longL(a, b) {
  var ad = a._d,
      l = ad.length,
      d = b >> 4,
      cl = l + d + 1,
      bb = b & 0xf,
      c = longAlloc(cl, a._s),
      cd = c._d,
      i = 0,
      carry = 0;
  for (; i < d; i++) { cd[i] = 0; }
  i = 0;
  for (var t = 0; i < l; i++) {
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
  var ad = a._d,
      l = ad.length,
      d = b >> 4,
      bb = b & 0xf,
      mask = (1 << bb) - 1;
  if (l <= d) { return new Long(); }

  var cl = l - d,
      c = longAlloc(cl, a._s),
      cd = c._d,
      i = 0;
  for (; i < cl - 1; i++) {
    cd[i] = ((ad[i + d + 1] & mask) << (16 - bb)) + (ad[i + d] >> bb);
  }
  cd[i] = ad[i + d] >> bb;
  return longNorm(c);
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
  if (a._d.length < b._d.length) {
    return longAddAbs(b, a, sign);
  }
  var ad = a._d,
      bd = b._d,
      al = ad.length,
      bl = bd.length,
      z = longAlloc(bl + 1, sign),
      zd = z._d,
      i = 0,
      num = 0;
  for (; i < bl; i++) {
    num += ad[i] + bd[i];
    zd[i] = num & 0xffff;
    num >>>= 16;
  }
  for (; num && i < al; i++) {
    num += ad[i];
    zd[i] = num & 0xffff;
    num >>>= 16;
  }
  for (; i < al; i++) {
    zd[i] = ad[i];
  }
  zd[i] = num & 0xffff;
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
  var ad = a._d,
      bd = b._d,
      al = ad.length,
      bl = bd.length,
      z = longAlloc(al, sign),
      zd = z._d,
      i = 0,
      c = 0;
  for (; i < bl; i++) {
    c = ad[i] - bd[i] - c;
    if (c < 0) {
      zd[i] = c & 0xffff;
      c = 1;
    } else {
      zd[i] = c;
      c = 0;
    }
  }
  for (; i < al; i++) { 
    c = ad[i] - c;
    if (c < 0) {
      zd[i] = c & 0xffff;
      c = 1;
    } else {
      zd[i] = c;
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
  if (a._s !== b._s) {
    if (longAbsCmp(a, b) < 0) {
      return longSubAbs(b, a, b._s);
    }
    return longSubAbs(a, b, a._s);
  }
  return longAddAbs(a, b, a._s);
}

/**
 * Subtraction.
 * @param {Long} a
 * @param {Long} b
 * @returns {Long} a - b
 */
function longSub(a, b) {
  if (a._s === b._s) {
    if (longAbsCmp(a, b) < 0) {
        return longSubAbs(b, a, !b._s);
    }
    return longSubAbs(a, b, a._s);
  }
  return longAddAbs(a, b, a._s);
}

/**
 * Multiply with Karatsuba Method.
 * @param {Long} a a._d.length === 2
 * @param {Long} b b._d.length === 2
 * @returns {Long} a * b
 */
function karatsuba(a, b) {
  var z = longAlloc(4, a._s === b._s),
      zd = z._d,
      ad = a._d,
      bd = b._d;
  zd[0] = ad[0] * bd[0];
  zd[2] = ad[1] * bd[1];
  zd[1] = zd[0] + zd[2] - (ad[1] - ad[0]) * (bd[1] - bd[0]);
  zd[1] += zd[0] >>> 16;
  zd[2] += zd[1] >>> 16;
  zd[3] = zd[2] >>> 16;
  zd[0] &= 0xffff;
  zd[1] &= 0xffff;
  zd[2] &= 0xffff;
  return longNorm(z);
}

/**
 * Multiplication for big. (buggy)
 * @ignore
 * @param {Long} a
 * @param {Long} b
 * @returns {Long} a * b
 */
function longTc(a, b) {
  if (a._d.length < b._d.length) {
    return longTc(b, a);
  }
  var ad = a._d,
      bd = b._d,
      al = ad.length,
      bl = bd.length, // al >= bl
      l = (al << 1) - 1,
      z = longAlloc(l, a._s === b._s);
  longFillZero(z, l);
  var zd = z._d,
      w = [],
      i = al;
  while (i-- > bl) {
    bd[i] = 0;
  }
  i = al;
  while (i--) {
    w[i] = ad[i] * bd[i];
  }

  var bt = function(sub, sup) {
    var s0 = 0, i = sub, j = sup;
    for (; i < j; i++, j--) {
      s0 += w[i] + w[j] - (ad[j] - ad[i]) * (bd[j] - bd[i]);
    }
    if (i === j) {
      s0 += w[i];
    }
    return s0;
  };

  for (i = 0; i < al; i++) {
    zd[i] = bt(0, i);
  }
  for (i = 1; i < al; i++) {
    zd[i + al - 1] = bt(i, al - 1);
  }
  var j = 0;
  for (i = 0; i < l; i++) {
    zd[i] += j;
    j = zd[i] >>> 16;
    zd[i] &= 0xffff;
  }
  zd[i] = j;
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
  var ad = a._d,
      bd = b._d,
      al = ad.length,
      bl = bd.length;
  if (al === 2 && bl === 2) { return karatsuba(a, b); }
  // if (al > 29 && bl > 29) { return longTc(a, b); }
  var j = al + bl + 1,
      z = longAlloc(j, a._s === b._s);
  longFillZero(z, j);
  for (var i = 0, n, d, e, zd = z._d; i < al; i++) {
    d = ad[i]; 
    if (!d) { continue; }
    n = 0;
    for (j = 0; j < bl; j++) {
      e = n + d * bd[j];
      n = zd[i + j] + e;
      if (e) { zd[i + j] = n & 0xffff; }
      n >>>= 16;
    }
    if (n) { zd[i + j] = n; }
  }
  return longNorm(z);
}

/**
 * Division or Mod.
 * @param {Long} a
 * @param {Long} b
 * @param {boolean} modulus If true then mod, else div.
 * @returns {Long} <br>
 *    a % b (modulus == true)<br>
 *    a / b (else)
 */
function longDivmod(a, b, modulus) {
  var ad = a._d,
      bd = b._d,
      na = ad.length,
      nb = bd.length;
  if (nb < 2 && !bd[0]) {
    // zero division
    if (modulus || na < 2 && !ad[0]) { return NaN; }
    if (a._s === b._s) { return Infinity; }
    return -Infinity;
  }

  var albl = na === nb;
  if (na < nb || (albl && ad[na - 1] < bd[nb - 1])) {
    if (modulus) { return longNorm(a); }
    return new Long();
  }

  var dd = 0, z, zd, t = 0, i = 0;
  if (nb === 1) {
    dd = bd[0];
    z = longClone(a);
    zd = z._d;
    i = na;
    while (i--) {
      t = t * 0x10000 + zd[i];
      zd[i] = (t / dd) & 0xffff;
      t %= dd;
    }
    if (modulus) {
      if (!a._s) { return longNum(-t); }
      return longNum(t);
    }
    z._s = a._s === b._s;
    return longNorm(z);
  }

  z = longAlloc(albl ? na + 2 : na + 1, a._s === b._s);
  zd = z._d;
  longFillZero(z, zd.length);
  dd = 0x10000 / (bd[nb - 1] + 1) & 0xffff;

  var j = 0, num = 0;
  if (dd === 1) {
    j = na;
    while (j--) { zd[j] = ad[j]; }
  } else {
    var bb = longClone(b), td = bb._d;

    for (; j < nb; j++) {
      num += bd[j] * dd;
      td[j] = num & 0xffff;
      num >>>= 16;
    }

    bd = td;
    j = num = 0;

    for (; j < na; j++) {
      num += ad[j] * dd;
      zd[j] = num & 0xffff;
      num >>>= 16;
    }

    zd[j] = num & 0xffff;
  }

  var q = 0, ee = 0;
  j = albl ? na + 1 : na;
  do {
    if (zd[j] === bd[nb - 1]) {
      q = 0xffff;
    } else {
      q = (zd[j] * 0x10000 + zd[j - 1]) / bd[nb - 1] & 0xffff;
    }

    if (q) {
      i = num = t = 0;
      do {
        t += bd[i] * q;
        ee = (t & 0xffff) - num;
        num = zd[j - nb + i] - ee;
        if (ee) { zd[j - nb + i] = num & 0xffff; }
        num >>= 16;
        t >>>= 16;
      } while (++i < nb);

      num += zd[j - nb + i] - t;
      while (num) {
        i = num = 0;
        q--;

        do {
          ee = num + bd[i];
          num = zd[j - nb + i] + ee;
          if (ee) { zd[j - nb + i] = num & 0xffff; }
          num >>= 16;
        } while (++i < nb);

        num--;
      }
    }
    zd[j] = q;
  } while (--j >= nb);

  var div = longClone(z);
  zd = div._d;
  if (modulus) {
    if (dd) {
      t = 0;
      i = nb;
      while (i--) {
        t = t * 0x10000 + zd[i];
        zd[i] = (t / dd) & 0xffff;
        t %= dd;
      }
    }
    zd.length = nb;
    div._s = a._s;
    return longNorm(div);
  }

  j = (albl ? na + 2 : na + 1) - nb;
  for (i = 0; i < j; i++) { zd[i] = zd[i + nb]; }
  zd.length = j;
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
 * Fast squaring.
 * @param {Long} a
 * @returns {Long} a * a
 */
function longSquare(a) {
  var x = a._d,
      t = x.length,
      s = longAlloc(t << 1, true),
      w = s._d;
  longFillZero(s, w.length);

  for (var i = 0, j = 1, uv = 0, u = 0, v = 0, c = 0; i < t; i++) {
    uv = w[i << 1] + x[i] * x[i];
    u = uv >>> 16;
    v = uv & 0xffff;
    w[i << 1] = v;
    c = u;
    for (j = i + 1; j < t; j++) {
      // uv = w[i + j] + (x[j] * x[i] << 1) + c
      // can overflow.
      uv = x[j] * x[i];
      u = (uv >>> 16) << 1;
      v = (uv & 0xffff) << 1;
      v += w[i + j] + c;
      u += v >>> 16;
      v &= 0xffff;
      w[i + j] = v;
      c = u;
    }
    w[i + t] = u;
  }

  return longNorm(s);
}

/**
 * Square root.
 * @param {Long} a
 * @returns {Long} <code>&radic;</code>a
 */
function longSqrt(a){
  var b = longClone(a),
      c = longNum(1);
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
 * @returns {Long|number} a<sup>b</sup>
 */
function longPow(a, b) {
  if (!b) { return longNum(1); }
  if (b > 0 && b === (b|0)) {
    var result = longNum(1);
    for (; b > 0; b >>= 1, a = longSquare(a)) {
      if (b & 1) { result = longMul(result, a); }
    }
    return result;
  }
  return Math.pow(longValue(a), b);
}

/**
 * Random.
 * @param {number} a Length
 * @returns {Long}
 */
function longRandom(a) {
  var r = longAlloc(a, true),
      rd = r._d;
  for (var i = 0; i < a; i++) {
    rd[i] = Math.random() * 0x10000 | 0;
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
 * @param {number} b Base 2 or 16
 * @returns {string}
 */
function longToByte(a, /** @default 16 */b) {
  var d = a._d,
      i = d.length - 1,
      s = a._s ? '' : '-',
      z, l = 0;
  if (b === 2) {
    z = '0000000000000000';
    l = -16;
  } else {
    b = 16;
    z = '0000';
    l = -4;
  }
  s += d[i].toString(b);
  while (i--) {
    s += (z + d[i].toString(b)).slice(l);
  }
  return s;
}

/**
 * Convert Long to String.
 * @param {Long} a
 * @param {number} b Base 2, 8, 10 or 16
 * @returns {string}
 */
function longToString(a, /** @default 10 */b) {
  if (!b) { b = 10; }
  if (b === 2 || b === 16) { return longToByte(a, b); }

  var i = a._d.length;
  if (i < 2 && !a._d[0]) { return '0'; }

  var j = 0, hbase = 10000;
  if (b === 8) {
    j = (i << 4) + 2;
    hbase = 0x1000;
  } else {
    j = (i * 241 / 50 | 0) + 2;
  }

  var t = longClone(a),
      d = t._d,
      k = 0,
      n = 0,
      digits = '0123456789',
      s = '';
  while (i && j) {
    k = i;
    n = 0;
    while (k--) {
      n = (n << 16) + d[k];
      d[k] = n / hbase | 0;
      n %= hbase;
    }
    if (!d[i - 1]) { i--; }
    k = 4;
    while (k--) {
      s = digits.charAt(n % b) + s;
      j--;
      n = n / b | 0;
      if (!i && !n) { break; }
    }
  }
  s = s.replace(/^0+/, '');
  if (!a._s) { s = '-' + s; }
  return s;
}

/**
 * Add zeros and shift decimal.
 * @param {Long} a
 * @param {number} b Number of zeros.
 * @returns {Long} a * 10<sup>n</sup>
 */
function longAddZero(a, b) {
  var zeros = '',
      z = '0';
  for (; b > 0; b >>>= 1, z += z) {
    if (b & 1) { zeros += z; }
  }
  return longStr(longToString(a) + zeros);
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
  getDigits: function() { return this._d; },
  /** @returns {boolean} */
  getSign: function() { return this._s; },

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
  isOdd: function() { return !!(this._d[0] & 1); },
  /** @returns {boolean} */
  isEven: function() { return !(this._d[0] & 1); },
  /** @returns {boolean} */
  isNonZero: function() { return (this._d.length > 1 || this._d[0]); },

  /**
   * @returns {Long}
   * @see longSquare
   */
  square: function() { return longSquare(this); },

  /**
   * @returns {Long}
   * @see longSqrt
   */
  sqrt: function() { return longSqrt(this); },

  /**
   * @param {number} a
   * @returns {Long|number}
   * @see longPow
   */
  pow: function(a) { return longPow(this, a); },

  /**
   * @param {number} a
   * @returns {Long}
   * @see longGcd
   */
  gcd: function(a) { return longGcd(this, a); },

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
