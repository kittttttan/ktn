/**
 * @fileOverview Big Integer in JavaScript.
 * @example
 *   var Integer = require('/path/to/integer.js').Integer;
 *   var a = Integer.str('12345678909876543210');
 *   var b = Integer.num(7777777);
 *   var c = a.mul(b);
 *   c.toString(); // === '96021937474622850618244170'
 * @author kittttttan
 */
(function(exports) {
  "use strict";
  
  /**
   * Integer
   * @class Integer
   */
  function Integer() {
    /**
     * Digits [d0, d1, ..., dn]
     * @private
     * @property {number[]} Integer#_d
     */
    this._d = [0];
    
    /**
     * Sign +, -. `false` means -.
     * @private
     * @property {boolean} Integer#_s
     */
    this._s = true;
  }

  /**
   * @private
   * @const
   * @type number
   */
  var SHIFT = 15;
  
  /**
   * @private
   * @const
   * @type number
   */
  var BASE = 1 << SHIFT;
  
  /**
   * @private
   * @const
   * @type number
   */
  var MASK = BASE - 1;

  // static
  /**
   * 1
   * @static
   * @method Integer.one
   * @return {Integer} 1
   */
  Integer.one = function() { return longNum(1); };

  /**
   * 0
   * @static
   * @method Integer.zero
   * @return {Integer} 0
   */
  Integer.zero = function() { return new Integer(); };
  
  /**
   * Converts integer to Integer.
   * @static
   * @method Integer.num
   * @param {number} n
   * @return {Integer}
   */
  Integer.num = function(n) {
    var a = new Integer();
    
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
  var longNum = Integer.num;

  /**
   * Converts string to Integer.
   * @static
   * @method Integer.str
   * @param {string} str For example '-9' or 'FF' etc.
   * @param {number} [base=10] 2, 8, 10 or 16
   * @return {Integer}
   */
  Integer.str = function(str, base) {
    if (!base) { base = 10; }

    var index = 0;
    var sign = true;
    if (str.charAt(index) === '+') {
      ++index;
    } else if (str.charAt(index) === '-') {
      sign = false;
      ++index;
    }
    
    // Ignore following zeros. '00102' is regarded as '102'.
    while (str.charAt(index) === '0') { ++index; }
    if (!str.charAt(index)) { return new Integer(); }
    
    var len;
    if (base === 8) {
      len = 3 * (str.length + 1 - index);
    } else {
      if (!str.charAt(index)) { --index; }
      len = (str.length + 1 - index) << 2;
    }
    len = (len >>> 4) + 1;

    var z = longAlloc(len, sign);
    longFillZero(z, len);

    var zd = z._d, bl = 1;
    var c, n, i;
    for (;;) {
      c = str.charAt(index);
      ++index;
      if (!c) { break; }
      
      n = parseInt(c, base);
      for (i = 0;;) {
        for (; i < bl; ++i) {
          n += zd[i] * base;
          zd[i] = n & MASK;
          n >>>= SHIFT;
        }
        
        if (!n) { break; }

        ++bl;
      }
    }
    
    return norm(z);
  };
  var longStr = Integer.str;

  /**
   * Converts anything to Integer.
   * @static
   * @method Integer.any
   * @param {Object} a
   * @return {Integer}
   */
  Integer.any = function(a) {
    if (typeof a === 'object') {
      if (a instanceof Integer) { return a.clone(); }
      return new Integer();
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

      var a1 = a.substr(0, i);
      var a2 = parseInt(a.substr(i + 2, a.length - (i + 2)), 10);
      var fpt = a1.indexOf('.', 0);

      if (fpt >= 0) {
        // '.' is found
        var np = a1.length - (fpt + 1);
        a1 = a1.substr(0, fpt) + a1.substr(fpt + 1, np);
        a2 -= np;
      }
      for (; a2 > 0; --a2) { a1 += '0'; }
      
      return longStr(a1);
    }
    
    return new Integer();
  };
  var any = Integer.any;

  /**
   * Random.
   * @static
   * @method Integer.random
   * @param {number} a Length
   * @return {Integer}
   */
  Integer.random = function(a) {
    var r = longAlloc(a, true);
    var rd = r._d;

    for (var i = 0; i < a; ++i) {
      rd[i] = Math.random() * BASE | 0;
    }
    
    return norm(r);
  };
  
  /**
   * @private
   * @param {number} n
   * @return {Integer}
   */
  function fact_odd(n) {
    n = n|0;
    
    var m = Integer.one();
    var mi = 0;
    var mj = 0;
    var i = 0;
    var j = 0;
    var l = 0;
    var limit = 1 << (SHIFT << 1);
    
    for (;; i = i + 1|0) {
      l = (n / (1 << i)) | 0;
      
      if (l < 3) break;
      
      mi = 1;
      mj = 1;
      for (j = 3; (j|0) <= (l|0); j = j + 2|0) {
        mi = mi * j;
        if (mi > limit) {
          m = m.mul(Integer.num(mj));
          mi = mj = j;
        } else {
          mj = mi;
        }
      }
      
      if ((mj|0) > 1) { m = m.mul(Integer.num(mj)); }
    }
    
    return m;
  }
  
  /**
   * @private
   * @param {number} n
   * @return {Integer}
   */
  function fact_even(n) {
    n = n|0;
    
    var s = 0;
    while (n) {
      n = n >>> 1;
      s = s + n;
    }
    
    return Integer.one().leftShift(s);
  }
  
  /**
   * @static
   * @method Integer.factorial
   * @param {number} n
   * @return {Integer}
   */
  Integer.factorial = function(n) {
    n = n|0;
    if (n < 1) { return Integer.one(); }
    return fact_odd(n).mul(fact_even(n));
  }

  /**
   * Set length.
   * @private
   * @param {number} length
   * @param {boolean} sign
   */
  function longAlloc(length, sign) {
    var a = new Integer();
    a._s = sign ? true : false;
    a._d.length = length;
    
    return a;
  }

  /**
   * Assign zero to initialize.
   * @private
   * @param {Integer} a
   * @param {number} b Length
   * @return {Integer}
   */
  function longFillZero(a, b) {
    var d = a._d;
    while (b--) { d[b] = 0; }
    
    return a;
  }

  /**
   * Delete following zeros. [2, 0, 1, 0, 0] -> [2, 0, 1]
   * @private
   * @param {Integer} a
   * @return {Integer}
   */
  function norm(a) {
    var d = a._d;
    var l = d.length|0;

    do { l=l-1|0; } while (l && !d[l]);
    d.length = l + 1|0;
    
    // -0 -> +0
    if (!l && !d[l]) { a._s = true; }
    
    return a;
  }

  /**
   * Right shift by 1.
   * @private
   * @param {Integer} a
   * @return {Integer} a >> 1
   */
  function longHalf(a) {
    var d = a._d;
    var l = d.length - 1;

    for (var i = 0; i < l; ++i) {
      d[i] = (((d[i + 1] & 1) << SHIFT) + d[i]) >>> 1;
    }
    d[l] >>>= 1;
    
    return norm(a);
  }

  /**
   * Left shift by 1.
   * @private
   * @param {Integer} a
   * @return {Integer} a << 1
   */
  function longDouble(a) {
    var d = a._d;
    var l = d.length;
    var c = 0;

    for (var i = 0, t = 0; i < l; ++i) {
      t = (d[i] << 1) + c;
      d[i] = t & MASK;
      c = t >>> SHIFT;
    }
    if (c) { d[l] = c; }
    
    return norm(a);
  }

  /**
   * Get length of bit
   * @private
   * @param {Integer} a
   * @return {number}
   */
  function longBitLength(a) {
    var ad = a._d;
    return ad[ad.length - 1].toString(2).length + ((ad.length - 1) << 4);
  }

  /**
   * Multiply with Karatsuba Method.
   * @private
   * @param {Integer} x
   * @param {Integer} y
   * @return {Integer} x * y
   */
  function longK(x, y) {
    var N = longBitLength(x);
    var l = longBitLength(y);

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

  Integer.prototype = {
    /**
     * @const
     * @property Integer#constructor
     * @type Integer
     */
    constructor: Integer,

    /**
     * Convert Integer to String.
     * @method Integer#toString
     * @param {number} [b=10] Base 2, 8, 10 or 16
     * @return {string}
     */
    toString: function(b) {
      if (!b) { b = 10; }

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

      var t = this.clone();
      var d = t._d;
      var k = 0;
      var n = 0;
      var digits = '0123456789abcdef';
      var s = '';

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
     * Convert Integer to number.
     * @method Integer#valueOf
     * @return {number}
     */
    valueOf: function() {
      var f = 0.0;
      var d = this._d;
      var i = d.length;

      while (i--) { f = d[i] + BASE * f; }
      if (!this._s) { f = -f; }
      
      return f;
    },

    /**
     * @method Integer#getDigits
     * @return {number[]}
     */
    getDigits: function() { return this._d; },
    
    /**
     * @method Integer#getSign
     * @return {boolean}
     */
    getSign: function() { return this._s; },

    /**
     * Copy Integer.
     * @method Integer#clone
     * @return {Integer}
     */
    clone: function() {
      var b = new Integer();
      b._s = this._s;
      b._d = Array.prototype.concat.call(this._d);
      
      return b;
    },

    /**
     * Add zeros and shift decimal.
     * @method Integer#addzero
     * @param {number} b Number of zeros.
     * @return {Integer} this * 10<sup>n</sup>
     */
    addzero: function(b) {
      var zeros = '';
      var z = '0';

      for (; b > 0; b >>>= 1, z += z) {
        if (b & 1) { zeros += z; }
      }
      
      return longStr(this.toString() + zeros);
    },

    /**
     * <<
     * @method Integer#leftShift
     * @param {number} b
     * @return {Integer}
     */
    leftShift: function(b) {
      var a = this;
      var ad = a._d;
      var l = ad.length|0;
      var d = (b / SHIFT) | 0;
      var cl = l + d + 1;
      var bb = b % SHIFT;
      var c = longAlloc(cl, a._s);
      var cd = c._d;
      var i = 0;
      var carry = 0;

      for (; (i|0) < (d|0); i=i+1|0) { cd[i] = 0; }
      
      var t = 0;
      for (i = 0; (i|0) < (l|0); i=i+1|0) {
        t = (ad[i] << bb) + carry;
        cd[i + d] = t & MASK;
        carry = t >> SHIFT;
      }
      cd[i + d] = carry;
      
      return norm(c);
    },

    /**
     * >>
     * @method Integer#rightShift
     * @param {number} b
     * @return {Integer}
     */
    rightShift: function(b) {
      var a = this;
      var ad = a._d;
      var l = ad.length;
      var d = (b / SHIFT) | 0;

      if (l <= d) { return new Integer(); }

      var bb = b % SHIFT;
      var mask = (1 << bb) - 1;
      var cl = l - d;
      var c = longAlloc(cl, a._s);
      var cd = c._d;
      var i = 0;

      for (; i < cl - 1; ++i) {
        cd[i] = ((ad[i + d + 1] & mask) << (SHIFT - bb)) + (ad[i + d] >> bb);
      }
      cd[i] = ad[i + d] >> bb;
      
      return norm(c);
    },

    /**
     * @method Integer#isOdd
     * @return {boolean}
     */
    isOdd: function() { return !!(this._d[0] & 1); },
    
    /**
     * @method Integer#isEven
     * @return {boolean}
     */
    isEven: function() { return !(this._d[0] & 1); },
    
    /**
     * @method Integer#isNonZero
     * @return {boolean}
     */
    isNonZero: function() { return (this._d.length > 1 || this._d[0]); },

    /**
     * Fast squaring.
     * @method Integer#square
     * @return {Integer} this * this
     */
    square: function() {
      //var a = this.clone(),
      //    x = a._d,
      var x = this._d;
      var t = x.length;
      var s = longAlloc(t << 1, true);
      var w = s._d;
      longFillZero(s, w.length);

      var i, j, c, uv, u, v;
      for (var i = 0; i < t; ++i) {
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

      return norm(s);
    },

    /**
     * Square root.
     * @method Integer#sqrt
     * @return {Integer} <code>&radic;</code>this
     */
    sqrt: function() {
      var b = this.clone();
      var c = Integer.one();

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
     * @method Integer#pow
     * @param {number} b
     * @return {Integer|number} this<sup>b</sup>
     */
    pow: function(b) {
      if (!b) { return Integer.one(); }
      
      if (b > 0 && b === (b | 0)) {
        var p = Integer.one();
        var a = this.clone();
        
        for (; b > 0; b >>= 1, a = a.square()) {
          if (b & 1) { p = p.mul(a); }
        }
        
        return p;
      }
      
      return Math.pow(this.valueOf(), b);
    },

    /**
     * Greatest Common Divisor.
     * @method Integer#gcd
     * @param {Integer} b
     * @return {Integer}
     */
    gcd: function(b) {
      var c;
      var a = this.abs();
      
      while ((c = a.divmod(b, true)).isNonZero()) {
        a = b;
        b = c;
      }
      
      return b;
    },

    /**
     * Greatest Common Divisor.
     * @method Integer#gcdBin
     * @param {Integer} b
     * @return {Integer}
     */
    gcdBin: function(b) {
      if (this.cmpAbs(b) < 0) { return b.gcdBin(this); }

      var g = Integer.one();
      var a = this.abs();
      b = b.abs();
      while (!(a._d[0] & 1) && !(b._d[0] & 1)) {
        longHalf(a);
        longHalf(b);
        longDouble(g);
      }
      
      while (a.isNonZero()) {
        while (!(a._d[0] & 1)) {
          longHalf(a);
        }
        
        while (!(b._d[0] & 1)) {
          longHalf(b);
        }
        
        if (a.cmpAbs(b) < 0) {
          b = b.sub(a);
          longHalf(b);
        } else {
          a = a.sub(b);
          longHalf(a);
        }
      }
      
      return g.mul(b);
    },

    /**
     * Add absolute values of Integer.
     * @method Integer#addAbs
     * @param {Integer} b
     * @param {boolean} sign
     * @return {Integer}
     *        |this| + |b| (sign == true)  
     *      -(|this| + |b|) (else)
     */
    addAbs: function(b, sign) {
      if (this._d.length < b._d.length) {
        return b.addAbs(this, sign);
      }
      
      var ad = this._d;
      var bd = b._d;
      var al = ad.length;
      var bl = bd.length;
      var z = longAlloc(al + 1, sign);
      var zd = z._d;
      var i = 0;
      var num = 0;

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
      
      return norm(z);
    },

    /**
     * Subtract absolute values of Integer.
     * @method Integer#subAbs
     * @param {Integer} b
     * @param {boolean} sign
     * @return {Integer}
     *      ||this| - |b|| (sign == true)
     *     -||this| - |b|| (else)
     */
    subAbs: function(b, sign) {
      var ad = this._d;
      var bd = b._d;
      var al = ad.length;
      var bl = bd.length;
      var z = longAlloc(al, sign);
      var zd = z._d;
      var i = 0;
      var c = 0;

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
      
      return norm(z);
    },

    /**
     * Addition.
     * @method Integer#add
     * @param {Integer} b
     * @return {Integer} this + b
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
     * @method Integer#sub
     * @param {Integer} b
     * @return {Integer} this - b
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
     * @method Integer#mul
     * @param {Integer} b
     * @return {Integer} this * b
     */
    mul: function(b) {
      // if (this.equal(b)) { return this.square(); }
      
      var ad = this._d;
      var bd = b._d;
      var al = ad.length|0;
      var bl = bd.length|0;
      // if (al > 125 && bl > 125) { return longK(this, b); }
      
      var j = al + bl|0;
      var z = longAlloc(j, this._s === b._s);

      longFillZero(z, j);
      for (var i = 0, n = 0, d = 0, e = 0, zd = z._d;
          (i|0) < (al|0); i = i + 1|0) {
        d = ad[i]; 
        if (!d) { continue; }
        
        n = 0;
        for (j = 0; (j|0) < (bl|0); j=j+1|0) {
          e = n + d * bd[j];
          n = zd[i + j] + e;
          if (e) { zd[i + j] = n & MASK; }
          n >>>= SHIFT;
        }
        
        if (n) { zd[i + j] = n|0; }
      }
      
      return norm(z);
    },

    /**
     * Division or Mod.
     * @method Integer#divmod
     * @param {Integer} b
     * @param {boolean} modulus If true then mod, else div.
     * @throws {Error} zero division
     * @return {Integer}
     *     this % b (modulus == true)  
     *     this / b (else)
     */
    divmod: function(b, modulus) {
      var a = this.clone();
      var ad = a._d;
      var bd = b._d;
      var na = ad.length;
      var nb = bd.length;

      if (nb < 2 && !bd[0]) {
        // zero division
        throw new Error('zero division');
        //if (modulus || na < 2 && !ad[0]) { return NaN; }
        //if (a._s === b._s) { return Infinity; }
        //return -Infinity;
      }

      var albl = na === nb;
      if (na < nb || (albl && ad[na - 1] < bd[nb - 1])) {
        if (modulus) { return a; }
        return new Integer();
      }

      var dd = 0;
      var t = 0;
      var i = 0;
      var z;
      var zd;
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
        return norm(z);
      }

      z = longAlloc(albl ? na + 2 : na + 1, a._s === b._s);
      zd = z._d;
      longFillZero(z, zd.length);
      dd = BASE / (bd[nb - 1] + 1) & MASK;

      var j = 0;
      var num = 0;
      if (dd === 1) {
        j = na;
        while (j--) { zd[j] = ad[j]; }
      } else {
        var bb = b.clone();
        var td = bb._d;

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

      var q = 0;
      var ee = 0;
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
        
        return norm(div);
      }

      j = (albl ? na + 2 : na + 1) - nb;
      for (i = 0; i < j; ++i) { zd[i] = zd[i + nb]; }
      zd.length = j;
      
      return norm(div);
    },

    /**
     * Division.
     * @method Integer#div
     * @param {Integer} b
     * @return {Integer} this / b
     */
    div: function(b) {
      return this.divmod(b, false);
    },

    /**
     * Modulo.
     * @method Integer#mod
     * @param {Integer} b
     * @return {Integer} this % b
     */
    mod: function(b) {
      return this.divmod(b, true);
    },

    /**
     * @private
     * @method Integer#_add_
     * @param {object} a
     * @return {Integer}
     */
    _add_: function(a) { return this.add(any(a)); },

    /**
     * @private
     * @method Integer#_sub_
     * @param {object} a
     * @return {Integer}
     */
    _sub_: function(a) { return this.sub(any(a)); },

    /**
     * @private
     * @method Integer#_mul_
     * @param {object} a
     * @return {Integer}
     */
    _mul_: function(a) { return this.mul(any(a)); },

    /**
     * @private
     * @method Integer#_div_
     * @param {object} a
     * @return {Integer}
     */
    _div_: function(a) { return this.divmod(any(a), false); },

    /**
     * @private
     * @method Integer#_mod_
     * @param {object} a
     * @return {Integer}
     */
    _mod_: function(a) { return this.divmod(any(a), true); },

    /**
     * Compare between two absolute values of Integer objects.
     * @method Integer#cmpAbs
     * @param {Integer} b
     * @return {number}
     *      -1 (|this| < |b|)
     *       0 (|this| = |b|)
     *       1 (|this| > |b|)
     */
    cmpAbs: function(b) {
      if (this === b) { return 0; }

      var ad = this._d;
      var bd = b._d;
      var al = ad.length;
      var bl = bd.length;

      if (al < bl) { return -1; }
      if (al > bl) { return 1; }
      
      do { --al; } while (al && ad[al] === bd[al]);
      if (!al && ad[0] === bd[0]) { return 0; }
      
      return ad[al] > bd[al] ? 1 : -1;
    },

    /**
     * Compare between two Integer.
     * @method Integer#cmp
     * @param {Integer} b
     * @return {number}
     *     -1 (this < b)
     *      0 (this = b)
     *      1 (this > b)
     */
    cmp: function(b) {
      if (this === b) { return 0; }
      if (this._s !== b._s) { return this._s ? 1 : -1; }

      var ad = this._d;
      var bd = b._d;
      var al = ad.length;
      var bl = bd.length;

      if (al < bl) { return this._s ? -1 : 1; }
      if (al > bl) { return this._s ? 1 : -1; }
      
      do { --al; } while (al && ad[al] === bd[al]);
      if (!al && ad[0] === bd[0]) {
        return (this._s ? 1 : 0) - (b._s ? 1 : 0);
      }
      
      if (ad[al] > bd[al]) { return this._s ? 1 : -1; }
      return this._s ? -1 : 1;
    },

    /**
     * ==
     * @method Integer#eq
     * @param {Integer} b
     * @return {boolean}
     */
    eq: function(b) {
      if (this === b) { return true; }
      
      b = any(b);
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
     * @method Integer#equal
     * @param {Integer} b
     * @return {boolean}
     */
    equal: function(b) {
      if (this === b) { return true; }
      if (!(b instanceof Integer)) { return false; }
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
     * @private
     * @method Integer#_co_
     * @return {number}
     */
    _co_: function() { return 1; },

    /**
     * Absolute Integer.
     * @method Integer#abs
     * @return {Integer} |this|
     */
    abs: function(){
      var z = this.clone();
      z._s = true;
      return z;
    },

    /**
     * Negate Integer.
     * @method Integer#neg
     * @return {Integer} -this
     */
    neg: function() {
      var z = this.clone();
      if (z.isNonZero()) { z._s = !z._s; }
      return z;
    }
  };

  exports.Integer = Integer;
}(typeof exports !== 'undefined' ? exports : this));