/**
 * @fileOverview Big Integer in JavaScript.
 * @version 2012-03-25
 * @url https://github.com/kittttttan/ktn
 * @example
 *   var a = Long.str("12345678909876543210");
 *   var b = Long.num(7777777);
 *   var c = a.mul(b);
 *   c.toString() // === '96021937474622850618244170'
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
 * @property {Array.<number>} _d Digits [d0, d1, ..., dn]
 * @property {boolean} _s Sign +, -. false means -.
 */
function Long() {
  this._d = [0];
  this._s = true;
}

(function(){
  "use strict";
  
  /**
   * @private
   * @const
   */
  var SHIFT = 15;
  
  /**
   * @private
   * @const
   */
  var BASE = 1 << SHIFT;
  
  /**
   * @private
   * @const
   */
  var MASK = BASE - 1;

  // static
  /**
   * Convert integer to Long.
   * @param {number} n
   * @returns {Long}
   */
  Long.num = function(n) {
    var a = new Long();
    if (n < 0) {
      n = -n;
      a._s = false;
    }
    n &= 0x7fffffff;
    a._d[0] = n & MASK;
    n >>>= SHIFT;
    if (n) {
      a._d[1] = n & MASK;
    }
    n >>>= SHIFT;
    if (n) {
      a._d[2] = n & MASK;
    }
    return a;
  };
  var longNum = Long.num;

  /**
   * Converts string to Long.
   * @param {string} str For example '-9' or 'FF' etc.
   * @param {number} base 2, 8, 10 or 16
   * @returns {Long}
   */
  Long.str = function(str, /** @default 10 */base) {
    //if (base === 16) { return longByte(str); }
    if (!base) { base = 10; }

    var index = 0, sign = true, len;
    if (str.charAt(index) === '+') {
      ++index;
    } else if (str.charAt(index) === '-') {
      sign = false;
      ++index;
    }
    // Ignore following zeros. '00102' is regarded as '102'.
    while (str.charAt(index) === '0') { ++index; }
    if (!str.charAt(index)) { return new Long(); }
    if (base === 8) {
      len = 3 * (str.length + 1 - index);
    } else {
      if (!str.charAt(index)) { --index; }
      len = (str.length + 1 - index) << 2;
    }
    len = (len >>> 4) + 1;

    var z = longAlloc(len, sign);
    longFillZero(z, len);

    for (var c, n, zd = z._d, bl = 1;;) {
      c = str.charAt(index);
      ++index;
      if (!c) { break; }
      n = parseInt(c, base);
      for (var i = 0;;) {
        for (; i < bl; ++i) {
          n += zd[i] * base;
          zd[i] = n & MASK;
          n >>>= SHIFT;
        }
        if (n) {
          ++bl;
        } else {
          break;
        }
      }
    }
    return longNorm(z);
  };
  var longStr = Long.str;

  /**
   * Converts anything to Long.
   * @param {Object} a
   * @returns {Long}
   */
  Long.longint = function(a) {
    if (typeof a === 'object') {
      if (a instanceof Long) { return a.clone(); }
      return new Long();
    }
    if (typeof a === 'string') {
      return longStr(a);
    }
    if (typeof a === 'number') {
      if (-0x7fffffff <= a && a <= 0x7fffffff) {
        return longNum(a);
      }
      a += '';

      var i = a.indexOf('e', 0);
      if (i < 0) {
        // 'e' is not found
        return longStr(a);
      }

      var a1 = a.substr(0, i),
          a2 = parseInt(a.substr(i + 2, a.length - (i + 2)), 10),
          fpt = a1.indexOf('.', 0);
      if (fpt >= 0) {
        // '.' is found
        var np = a1.length - (fpt + 1);
        a1 = a1.substr(0, fpt) + a1.substr(fpt + 1, np);
        a2 -= np;
      }
      for (; a2 > 0; --a2) { a1 += '0'; }
      return longStr(a1);
    }
    return new Long();
  };
  var longint = Long.longint;

  /**
   * Random.
   * @param {number} a Length
   * @returns {Long}
   */
  Long.random = function(a) {
    var r = longAlloc(a, true),
        rd = r._d;
    for (var i = 0; i < a; ++i) {
      rd[i] = Math.random() * BASE | 0;
    }
    return longNorm(r);
  };

  /**
   * Set length.
   * @private
   * @param {number} length
   * @param {boolean} sign
   */
  function longAlloc(length, sign) {
    var a = new Long();
    a._s = sign ? true : false;
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
   * Delete following zeros. [2, 0, 1, 0, 0] -> [2, 0, 1]
   * @param {Long} a
   * @returns {Long}
   */
  function longNorm(a) {
    var d = a._d,
        l = d.length;
    do { --l; } while (l && !d[l]);
    d.length = l + 1;
    // -0 -> +0
    if (!l && !d[l]) { a._s = true; }
    return a;
  }

  /**
   * Converts hex to Long.
   * @deprecated buggy
   * @param {string} str For example 'ff' or '-123456789abcdef' etc.
   * @returns {Long}
   */
  function longByte(str) {
    if (!str) { return new Long(); }

    var s = true;
    if (str.charAt(0) === '-') {
      if (str.length < 2) { return new Long(); }
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
   * Right shift by 1.
   * @param {Long} a
   * @returns {Long} a >> 1
   */
  function longHalf(a) {
    var d = a._d,
        l = d.length - 1;
    for (var i = 0; i < l; ++i) {
      d[i] = (((d[i + 1] & 1) << SHIFT) + d[i]) >>> 1;
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
    for (var i = 0, t = 0; i < l; ++i) {
      t = (d[i] << 1) + c;
      d[i] = t & MASK;
      c = t >>> SHIFT;
    }
    if (c) { d[l] = c; }
    return longNorm(a);
  }

  /**
   * Get length of bit
   * @param {Long} a
   * @returns {number}
   */
  function longBitLength(a) {
    var ad = a._d;
    return ad[ad.length - 1].toString(2).length + ((ad.length - 1) << 4);
  }

  /**
   * Multiply with Karatsuba Method.
   * @param {Long} x
   * @param {Long} y
   * @returns {Long} x * y
   */
  function longK(x, y) {
    var N = longBitLength(x),
        l = longBitLength(y);
    if (N < l) { N = l; }
    if (N < 2001) { return x.mul(y); }

    // number of bits divided by 2, rounded up
    N = (N >>> 1) + (N & 1);

    // x = a + b 2^N, y = c + d 2^N
    var b = x.rightShift(N),
        a = x.sub(b.leftShift(N)),
        d = y.rightShift(N),
        c = y.sub(d.leftShift(N)),
        ac = longK(a, c),
        bd = longK(b, d),
        abcd = longK(a.add(b), c.add(d));
    // xy
    // = (a + 2^N b) (c + 2^N d)
    // = ac + 2^N ((a + b) (c + d) - ac - bd) + 2^(N + 1) bd
    return ac.add(abcd.sub(ac).sub(bd).leftShift(N)).add(bd.leftShift(N << 1));
  }

  /**
   * Convert Long to String.
   * @param {Long} a
   * @param {number} b Base 2 or 16
   * @returns {string}
   * @deprecated
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

  Long.prototype = {
    constructor: Long,

    /**
     * Convert Long to String.
     * @param {number} b Base 2, 8, 10 or 16
     * @returns {string}
     */
    toString: function(/** @default 10 */b) {
      if (!b) { b = 10; }
      //if (b === 2 || b === 16) { return longToByte(this, b); }

      var i = this._d.length;
      if (i < 2 && !this._d[0]) { return '0'; }

      var j, hbase;
      switch (b) {
      case 16:
        j = (i << 3) + 2;
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

      var t = this.clone(),
          d = t._d,
          k = 0,
          n = 0,
          digits = '0123456789abcdef',
          s = '';
      while (i && j) {
        k = i;
        n = 0;
        while (k--) {
          n = (n << SHIFT) | d[k];
          d[k] = n / hbase | 0;
          n %= hbase;
        }
        if (!d[i - 1]) { --i; }
        k = 4;
        while (k--) {
          s = digits.charAt(n % b) + s;
          --j;
          n = n / b | 0;
          if (!i && !n) { break; }
        }
      }
      s = s.replace(/^0+/, '');
      if (!this._s) { s = '-' + s; }
      return s;
    },

    /**
     * Convert Long to number.
     * @returns {number}
     */
    valueOf: function() {
      var f = 0.0,
          d = this._d,
          i = d.length;
      while (i--) { f = d[i] + BASE * f; }
      if (!this._s) { f = -f; }
      return f;
    },

    /** @returns {Array.<number>} */
    getDigits: function() { return this._d; },
    /** @returns {boolean} */
    getSign: function() { return this._s; },

    /**
     * Copy Long.
     * @returns {Long}
     */
    clone: function() {
      var b = new Long();
      b._s = this._s;
      b._d = Array.prototype.concat.call(this._d);
      return b;
    },

    /**
     * Add zeros and shift decimal.
     * @param {number} b Number of zeros.
     * @returns {Long} this * 10<sup>n</sup>
     */
    addzero: function(b) {
      var zeros = '',
          z = '0';
      for (; b > 0; b >>>= 1, z += z) {
        if (b & 1) { zeros += z; }
      }
      return longStr(this.toString() + zeros);
    },

    /**
     * <<
     * @param {number} b
     * @returns {Long}
     */
    leftShift: function(b) {
      var a = this.clone(),
          ad = a._d,
          l = ad.length,
          d = b >> 4,
          cl = l + d + 1,
          bb = b & 0xf,
          c = longAlloc(cl, a._s),
          cd = c._d,
          i = 0,
          carry = 0;
      for (; i < d; ++i) { cd[i] = 0; }
      i = 0;
      for (var t = 0; i < l; ++i) {
        t = (ad[i] << bb) + carry;
        cd[i + d] = t & MASK;
        carry = t >> SHIFT;
      }
      cd[i + d] = carry;
      return longNorm(c);
    },

    /**
     * >>
     * @param {number} b
     * @returns {Long}
     */
    rightShift: function(b) {
      var a = this.clone(),
          ad = a._d,
          l = ad.length,
          d = b >> 4,
          bb = b & 0xf,
          mask = (1 << bb) - 1;
      if (l <= d) { return new Long(); }

      var cl = l - d,
          c = longAlloc(cl, a._s),
          cd = c._d,
          i = 0;
      for (; i < cl - 1; ++i) {
        cd[i] = ((ad[i + d + 1] & mask) << (SHIFT - bb)) + (ad[i + d] >> bb);
      }
      cd[i] = ad[i + d] >> bb;
      return longNorm(c);
    },

    /** @returns {boolean} */
    isOdd: function() { return !!(this._d[0] & 1); },
    /** @returns {boolean} */
    isEven: function() { return !(this._d[0] & 1); },
    /** @returns {boolean} */
    isNonZero: function() { return (this._d.length > 1 || this._d[0]); },

    /**
     * Fast squaring.
     * @returns {Long} this * this
     */
    square: function() {
      var a = this.clone(),
          x = a._d,
          t = x.length,
          s = longAlloc(t << 1, true),
          w = s._d;
      longFillZero(s, w.length);

      for (var i = 0, j = 1, uv = 0, u = 0, v = 0, c = 0; i < t; ++i) {
        uv = w[i << 1] + x[i] * x[i];
        u = uv >>> SHIFT;
        v = uv & MASK;
        w[i << 1] = v;
        c = u;
        for (j = i + 1; j < t; ++j) {
          // uv = w[i + j] + (x[j] * x[i] << 1) + c
          // can overflow.
          uv = x[j] * x[i];
          u = (uv >>> SHIFT) << 1;
          v = (uv & MASK) << 1;
          v += w[i + j] + c;
          u += v >>> SHIFT;
          v &= MASK;
          w[i + j] = v;
          c = u;
        }
        w[i + t] = u;
      }

      return longNorm(s);
    },

    /**
     * Square root.
     * @returns {Long} <code>&radic;</code>this
     */
    sqrt: function() {
      var b = this.clone(),
          c = longNum(1);
      while (b.cmp(c) > 0) {
        longHalf(b);
        longDouble(c);
      }
      do {
        b = c.clone();
        c = this.divmod(c, false).add(c);
        longHalf(c);
      } while (b.cmp(c) > 0);
      return b;
    },

    /**
     * Pow.
     * @param {number} b
     * @returns {Long|number} this<sup>b</sup>
     */
    pow: function(b) {
      if (!b) { return longNum(1); }
      if (b > 0 && b === (b | 0)) {
        var p = longNum(1), a = this.clone();
        for (; b > 0; b >>= 1, a = a.square()) {
          if (b & 1) { p = p.mul(a); }
        }
        return p;
      }
      return Math.pow(this.valueOf(), b);
    },

    /**
     * Greatest Common Divisor.
     * @param {Long} b
     * @returns {Long}
     */
    gcd: function(b) {
      var c, a = this.clone();
      while ((c = a.divmod(b, true)).isNonZero()) {
        a = b;
        b = c;
      }
      return b;
    },

    /**
     * Greatest Common Divisor.
     * @param {Long} b
     * @returns {Long}
     */
    gcdBin: function(b) {
      if (this.cmpAbs(b) < 0) { return b._gcd(this); }

      var g = longNum(1), a = this.clone();
      while (!(a._d[0] & 1) && !(b._d[0] & 1)) {
        a = longHalf(a);
        b = longHalf(b);
        g = longDouble(g);
      }
      
      while (a.isNonZero()) {
        while (!(a._d[0] & 1)) {
          a = longHalf(a);
        }
        while (!(b._d[0] & 1)) {
          b = longHalf(b);
        }
        if (a.cmpAbs(b) < 0) {
          b = longHalf(b.sub(a));
        } else {
          a = longHalf(a.sub(b));
        }
      }
      
      return g.mul(b);
    },

    /**
     * Add absolute values of Long.
     * @param {Long} b
     * @param {boolean} sign
     * @returns {Long} <br>
     *    |this| + |b| (sign == true)<br>
     *  -(|this| + |b|) (else)
     */
    addAbs: function(b, sign) {
      if (this._d.length < b._d.length) {
        return b.addAbs(this, sign);
      }
      var ad = this._d,
          bd = b._d,
          al = ad.length,
          bl = bd.length,
          z = longAlloc(bl + 1, sign),
          zd = z._d,
          i = 0,
          num = 0;
      for (; i < bl; ++i) {
        num += ad[i] + bd[i];
        zd[i] = num & MASK;
        num >>>= SHIFT;
      }
      for (; num && i < al; ++i) {
        num += ad[i];
        zd[i] = num & MASK;
        num >>>= SHIFT;
      }
      for (; i < al; ++i) {
        zd[i] = ad[i];
      }
      zd[i] = num & MASK;
      return longNorm(z);
    },

    /**
     * Subtract absolute values of Long.
     * @param {Long} b
     * @param {boolean} sign
     * @returns {Long} <br>
     *     ||this| - |b|| (sign == true)<br>
     *    -||this| - |b|| (else)
     */
    subAbs: function(b, sign) {
      var ad = this._d,
          bd = b._d,
          al = ad.length,
          bl = bd.length,
          z = longAlloc(al, sign),
          zd = z._d,
          i = 0,
          c = 0;
      for (; i < bl; ++i) {
        c = ad[i] - bd[i] - c;
        if (c < 0) {
          zd[i] = c & MASK;
          c = 1;
        } else {
          zd[i] = c;
          c = 0;
        }
      }
      for (; i < al; ++i) { 
        c = ad[i] - c;
        if (c < 0) {
          zd[i] = c & MASK;
          c = 1;
        } else {
          zd[i] = c;
          c = 0;
        }
      }
      return longNorm(z);
    },

    /**
     * Addition.
     * @param {Long} b
     * @returns {Long} this + b
     */
    add: function(b) {
      if (this._s !== b._s) {
        if (this.cmpAbs(b) < 0) {
          return b.subAbs(this, b._s);
        }
        return this.subAbs(b, this._s);
      }
      return this.addAbs(b, this._s);
    },

    /**
     * Subtraction.
     * @param {Long} b
     * @returns {Long} this - b
     */
    sub: function(b) {
      if (this._s === b._s) {
        if (this.cmpAbs(b) < 0) {
            return b.subAbs(this, !b._s);
        }
        return this.subAbs(b, this._s);
      }
      return this.addAbs(b, this._s);
    },

    /**
     * Multiplication.
     * @param {Long} b
     * @returns {Long} this * b
     */
    mul: function(b) {
      // if (this.equal(b)) { return this.square(); }
      var ad = this._d,
          bd = b._d,
          al = ad.length,
          bl = bd.length;
      // if (al > 125 && bl > 125) { return longK(this, b); }
      var j = al + bl,
          z = longAlloc(j, this._s === b._s);
      longFillZero(z, j);
      for (var i = 0, n, d, e, zd = z._d; i < al; ++i) {
        d = ad[i]; 
        if (!d) { continue; }
        n = 0;
        for (j = 0; j < bl; ++j) {
          e = n + d * bd[j];
          n = zd[i + j] + e;
          if (e) { zd[i + j] = n & MASK; }
          n >>>= SHIFT;
        }
        if (n) { zd[i + j] = n; }
      }
      return longNorm(z);
    },

    
    /**
     * Division or Mod.
     * @param {Long} b
     * @param {boolean} modulus If true then mod, else div.
     * @returns {Long} <br>
     *    this % b (modulus == true)<br>
     *    this / b (else)
     */
    divmod: function(b, modulus) {
      var a = this.clone(),
          ad = a._d,
          bd = b._d,
          na = ad.length,
          nb = bd.length;
      if (nb < 2 && !bd[0]) {
        // zero division
        throw 'zero division';
        //if (modulus || na < 2 && !ad[0]) { return NaN; }
        //if (a._s === b._s) { return Infinity; }
        //return -Infinity;
      }

      var albl = na === nb;
      if (na < nb || (albl && ad[na - 1] < bd[nb - 1])) {
        if (modulus) { return a; }
        return new Long();
      }

      var dd = 0, z, zd, t = 0, i = 0;
      if (nb === 1) {
        dd = bd[0];
        z = a.clone();
        zd = z._d;
        i = na;
        while (i--) {
          t = (t << SHIFT) | zd[i];
          zd[i] = (t / dd) & MASK;
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
      dd = BASE / (bd[nb - 1] + 1) & MASK;

      var j = 0, num = 0;
      if (dd === 1) {
        j = na;
        while (j--) { zd[j] = ad[j]; }
      } else {
        var bb = b.clone(), td = bb._d;

        for (; j < nb; ++j) {
          num += bd[j] * dd;
          td[j] = num & MASK;
          num >>>= SHIFT;
        }

        bd = td;
        j = num = 0;

        for (; j < na; ++j) {
          num += ad[j] * dd;
          zd[j] = num & MASK;
          num >>>= SHIFT;
        }

        zd[j] = num & MASK;
      }

      var q = 0, ee = 0;
      j = albl ? na + 1 : na;
      do {
        if (zd[j] === bd[nb - 1]) {
          q = MASK;
        } else {
          q = ((zd[j] << SHIFT) | zd[j - 1]) / bd[nb - 1] & MASK;
        }

        if (q) {
          i = num = t = 0;
          do {
            t += bd[i] * q;
            ee = (t & MASK) - num;
            num = zd[j - nb + i] - ee;
            if (ee) { zd[j - nb + i] = num & MASK; }
            num >>= SHIFT;
            t >>>= SHIFT;
          } while (++i < nb);

          num += zd[j - nb + i] - t;
          while (num) {
            i = num = 0;
            --q;

            do {
              ee = num + bd[i];
              num = zd[j - nb + i] + ee;
              if (ee) { zd[j - nb + i] = num & MASK; }
              num >>= SHIFT;
            } while (++i < nb);

            --num;
          }
        }
        zd[j] = q;
      } while (--j >= nb);

      var div = z.clone();
      zd = div._d;
      if (modulus) {
        if (dd) {
          t = 0;
          i = nb;
          while (i--) {
            t = (t << SHIFT) | zd[i];
            zd[i] = (t / dd) & MASK;
            t %= dd;
          }
        }
        zd.length = nb;
        div._s = a._s;
        return longNorm(div);
      }

      j = (albl ? na + 2 : na + 1) - nb;
      for (i = 0; i < j; ++i) { zd[i] = zd[i + nb]; }
      zd.length = j;
      return longNorm(div);
    },

    /**
     * Division.
     * @param {Long} b
     * @returns {Long} this / b
     */
    div: function(b) {
      return this.divmod(b, false);
    },

    /**
     * Modulo.
     * @param {Long} b
     * @returns {Long} this % b
     */
    mod: function(b) {
      return this.divmod(b, true);
    },

    /**
     * @param {object} a
     * @returns {Long}
     */
    _add_: function(a) { return this.add(longint(a)); },

    /**
     * @param {object} a
     * @returns {Long}
     */
    _sub_: function(a) { return this.sub(longint(a)); },

    /**
     * @param {object} a
     * @returns {Long}
     */
    _mul_: function(a) { return this.mul(longint(a)); },

    /**
     * @param {object} a
     * @returns {Long}
     */
    _div_: function(a) { return this.divmod(longint(a), false); },

    /**
     * @param {object} a
     * @returns {Long}
     */
    _mod_: function(a) { return this.divmod(longint(a), true); },

    /**
     * Compare between two absolute values of Long objects.
     * @param {Long} b
     * @returns {number} <br>
     *    -1 (|this| < |b|)<br>
     *     0 (|this| = |b|)<br>
     *     1 (|this| > |b|)
     */
    cmpAbs: function(b) {
      if (this === b) { return 0; }
      var ad = this._d,
          bd = b._d,
          al = ad.length;
      if (al < bd.length) { return -1; }
      if (al > bd.length) { return 1; }
      do { --al; } while (al && ad[al] === bd[al]);
      if (!al && ad[0] === bd[0]) { return 0; }
      return ad[al] > bd[al] ? 1 : -1;
    },

    /**
     * Compare between two Long.
     * @param {Long} b
     * @returns {number} <br>
     *    -1 (this < b)<br>
     *     0 (this = b)<br>
     *     1 (this > b)
     */
    cmp: function(b) {
      if (this === b) { return 0; }
      var ad = this._d,
          bd = b._d,
          al = ad.length;
      if (this._s !== b._s) { return this._s ? 1 : -1; }
      if (al < bd.length) { return this._s ? -1 : 1; }
      if (al > bd.length) { return this._s ? 1 : -1; }
      do { --al; } while (al && ad[al] === bd[al]);
      if (!al && ad[0] === bd[0]) {
        return (this._s ? 1 : 0) - (b._s ? 1 : 0);
      }
      if (ad[al] > bd[al]) { return this._s ? 1 : -1; }
      return this._s ? -1 : 1;
    },

    /**
     * ==
     * @param {Long} b
     * @returns {boolean}
     */
    eq: function(b) {
      if (this === b) { return true; }
      b = longint(b);
      if (this._s !== b._s) { return false; }
      var ad = this._d,
          bd = b._d,
          l = ad.length;
      if (l !== bd.length) { return false; }
      for (var i = 0; i < l; ++i) {
        if (ad[i] !== bd[i]) { return false; }
      }
      return true;
    },

    /**
     * ===
     * @param {Long} b
     * @returns {boolean}
     */
    equal: function(b) {
      if (this === b) { return true; }
      if (!(b instanceof Long)) { return false; }
      if (this._s !== b._s) { return false; }
      var ad = this._d,
          bd = b._d,
          l = ad.length;
      if (l !== bd.length) { return false; }
      for (var i = 0; i < l; ++i) {
        if (ad[i] !== bd[i]) { return false; }
      }
      return true;
    },

    /** @returns {number} */
    _co_: function() { return 1; },

    /**
     * Absolute Long.
     * @returns {Long} |this|
     */
    abs: function(){
      var z = this.clone();
      z._s = true;
      return z;
    },

    /**
     * Negate Long.
     * @returns {Long} -this
     */
    neg: function() {
      var z = this.clone();
      if (z.isNonZero()) { z._s = !z._s; }
      return z;
    }
  };

  /**
   * 1
   * @constant
   * @type {Long}
   */
  Long.ONE = longNum(1);

  /**
   * 0
   * @constant
   * @type {Long}
   */
  Long.ZERO = new Long();
}());

// exports for node
if (typeof module !== 'undefined' && typeof module.exports !== 'undefined') {
  module.exports.Long = Long;
}
