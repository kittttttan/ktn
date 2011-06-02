/**
 * @fileOverview Big Integer in JavaScript.
 * @version 2011-06-02
 * @author kittttttan
 * @url http://kittttttan.web.fc2.com/work/mathjs.html
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

/**
 * Big integer.
 * @class Big integer
 * @property {Array.<number>} _ds Digits
 * @property {boolean} _sn Sign +, -
 * @param {number|string} len
 * @param {boolean} sign
 */
function Long(len, sign) {
  if (arguments.length === 1) {
    len = longint(len);
    this._sn = len._sn;
    this._ds = len._ds;
  } else {
    len |= 0;
    if (len < 1) {
      this._sn = true;
      this._ds = [0];
    } else {
      this._sn = sign ? true : false;
      this._ds = [];
      this._ds.length = len;
    }
  }
}

/**
 * Assign zero to initialize.
 * @private
 * @param {Long} a
 * @returns {void}
 */
function longFillZero(a) {
  var ds = a._ds;
  var l = ds.length;
  while (l--) { ds[l] = 0; }
}

/**
 * Convert integer to Long.
 * @param {number} a
 * @returns {Long}
 */
function longNum(a) {
  var sign;
  if (a < 0) {
    a = -a;
    sign = false;
  } else {
    sign = true;
  }
  a &= 0x7fffffff;
  if (a <= 0xffff) {
    var b = new Long(1, sign);
    b._ds[0] = a;
  } else {
    b = new Long(2, sign);
    b._ds[0] = a & 0xffff;
    b._ds[1] = (a >>> 16) & 0xffff;
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
var LONG_ZERO = new Long();

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
 * @param {string} a For example '-9' or '0xFF' etc.
 * @param {number} b Base 2, 8, 10 or 16
 * @returns {Long}
 */
function longStr(a, b) {
  var a_i = 0;
  var sign = true;
  a += '@';	// Terminator;
  if (a.charAt(a_i) === '+') {
    a_i += 1;
  } else if (a.charAt(a_i) === '-') {
    a_i += 1;
    sign = false;
  }
  if (a.charAt(a_i) === '@') { return new Long(); }
  if (!b) {
    if (a.charAt(a_i) === '0') {
      var c = a.charAt(a_i + 1);
      switch (c) {
        case 'x': case 'X': b = 16; break;
        case 'b': case 'B': b = 2; break;
        default: b = 8; break;
      }
    } else {
      b = 10;
    }
  }
  if (b === 8) {
    while (a.charAt(a_i) === '0') { a_i += 1; }
    var len = 3 * (a.length - a_i);
  } else {
    // b === 10, 2 or 16
    if (b === 16 && a.charAt(a_i) === '0' &&
        (a.charAt(a_i + 1) === 'x' || a.charAt(a_i + 1) === 'X')) {
      a_i += 2;
    }
    if (b === 2 && a.charAt(a_i) === '0' &&
        (a.charAt(a_i + 1) === 'b' || a.charAt(a_i + 1) === 'B')) {
      a_i += 2;
    }
    while (a.charAt(a_i) === '0') { a_i += 1; }
    if (a.charAt(a_i) === '@') { a_i -= 1; }
    len = (a.length - a_i) << 2;
  }
  len = (len >>> 4) + 1;
  var z = new Long(len, sign);
  longFillZero(z);
  var zds = z._ds;
  var num;
  var i;
  var blen = 1;
  while (true) {
    c = a.charAt(a_i);
    a_i += 1;
    if (c === '@') { break; }
    switch (c) {
      case '0': c = 0; break;
      case '1': c = 1; break;
      case '2': c = 2; break;
      case '3': c = 3; break;
      case '4': c = 4; break;
      case '5': c = 5; break;
      case '6': c = 6; break;
      case '7': c = 7; break;
      case '8': c = 8; break;
      case '9': c = 9; break;
      case 'a': case 'A': c = 10; break;
      case 'b': case 'B': c = 11; break;
      case 'c': case 'C': c = 12; break;
      case 'd': case 'D': c = 13; break;
      case 'e': case 'E': c = 14; break;
      case 'f': case 'F': c = 15; break;
      default: c = b; break;
    }
    if (c >= b) { break; }
    i = 0;
    num = c;
    while (true) {
      while (i < blen) {
        num += zds[i] * b;
        zds[i] = num & 0xffff;
        i += 1;
        num >>>= 16;
      }
      if (num) {
        blen += 1;
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
  var b = new Long(a._ds.length, a._sn);
  b._ds = Array.prototype.concat.call(a._ds);
  return b;
}

/**
 * Converts anything to Long.
 * @param {Object} a
 * @returns {Long}
 */
function longint(a) {
  if (typeof(a) === 'object') {
    return (a instanceof Long) ? longClone(a) : new Long();
  }
  if (typeof(a) === 'string') {
    return longStr(a);
  }
  if (typeof(a) === 'number') {
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
      a2 |= 0;
    }
    while (a2-- > 0) { a1 += '0'; }
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
  z = new Long(bds.length + 1, sign);
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
  var z = new Long(al, sign);
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
  var z = new Long(4, a._sn === b._sn);
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
  var z = new Long(l, a._sn === b._sn);
  longFillZero(z);
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
  var z = new Long(j, a._sn === b._sn);
  longFillZero(z);
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
    return new Long();
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

  z = new Long(albl ? na + 2 : na + 1, a._sn === b._sn);
  longFillZero(z);
  zds = z._ds;
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
  var s = new Long((al << 1) + 1, true);
  longFillZero(s);
  var sds = s._ds;
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
  a |= 0;
  var r = new Long(a, true);
  var rds = r._ds;
  while (a--) {
    rds[a] = Math.random() * 0xffff | 0;
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
  var i = a._ds.length;
  if (i < 2 && !a._ds[0]) { return '0'; }
  var j;
  var hbase;
  switch (b) {
    case 16:
      j = (i << 2) + 2;
      hbase = 0x10000;
      break;
    case 8:
      j = (i << 4) + 2;
      hbase = 0x1000;
      break;
    case 2:
      j = (i << 4) + 2;
      hbase = 0x10;
      break;
    case 10: default:
      j = (i * 241 / 50 | 0) + 2;
      hbase = 10000;
      break;
  }
  var t = longClone(a);
  var ds = t._ds;
  var digits = '0123456789abcdef';
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
      num /= b;
      num |= 0;
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
  var bb = b & 0xf;
  var c = new Long(l + d + 1, a._sn);
  var cd = c._ds;
  var cl = cd.length;
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
  if (l <= d) { return new Long(); }
  var c = new Long(l - d, a._sn);
  var cd = c._ds;
  var cl = cd.length;
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
  /** @see longToString */
  toString: function(base) { return longToString(this, base); },
  /** @see longValue */
  valueOf: function() { return longValue(this); },
  getDigits: function() { return this._ds; },
  getSign: function() { return this._sn; },
  /** @see longClone */
  clone: function() { return longClone(this); },
  /** @see longAddZero */
  addzero: function(a) { return longAddZero(this, a); },
  rshift: function(a) { return longR(this, a); },
  lshift: function(a) { return longL(this, a); },
  /** @see longHalf */
  half: function() { return longHalf(this); },
  /** @see longDouble */
  dbl: function() { return longDouble(this); },
  isOdd: function() { return (this._ds[0] & 1); },
  isEven: function() { return !(this._ds[0] & 1); },
  isNonZero: function() { return (this._ds.length > 1 || this._ds[0]); },
  /** @see longSqrt */
  sqrt: function() { return longSqrt(this); },
  /** @see longPow */
  pow: function(a) { return longPow(this, a); },
  /** @see longGcd */
  gcd: function(a) { return longGcd(this, a); },
  _len_: function() { return this._ds.length; },
  /** @see longAdd */
  add: function(a) { return longAdd(this, a); },
  /** @see longSub */
  sub: function(a) { return longSub(this, a); },
  /** @see longMul */
  mul: function(a) { return longMul(this, a); },
  /** @see longDivmod */
  div: function(a) { return longDivmod(this, a, false); },
  /** @see longDivmod */
  mod: function(a) { return longDivmod(this, a, true); },
  _add_: function(a) { return longAdd(this, longint(a)); },
  _sub_: function(a) { return longSub(this, longint(a)); },
  _mul_: function(a) { return longMul(this, longint(a)); },
  _div_: function(a) { return longDivmod(this, longint(a), false); },
  _mod_: function(a) { return longDivmod(this, longint(a), true); },
  /** @see longCmp */
  _cmp_: function(a) { return longCmp(this, longint(a)); },
  /** @see longEq */
  _eq_: function(a) { return longEq(this, a); },
  /** @see longEqual */
  _equal_: function(a) { return longEqual(this, a); },
  _co_: function() { return 0.5; },
  /** @see longAbs */
  _abs_: function() { return longAbs(this); },
  /** @see longNeg */
  _neg_: function() { return longNeg(this); }
};
