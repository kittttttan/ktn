/**
 * @fileOverview Decimal in JavaScript.
 * @example
 *    var a = ktn.Decimal.num(7, -3);
 *    var b = ktn.Decimal.str('100.1');
 *    var c = a.mul(b);
 *    a.toString();  // === '0.007'
 *    b.toString();  // === '100.1'
 *    c.toString();  // === '0.7007'
 * @author kittttttan
 */
(function(ktn){
  "use strict";

  // require
  var Integer;
  if (typeof require !== 'undefined') {
    Integer = require('../lib/integer.js').Integer;
  } else {
    Integer = ktn.Integer;
  }
  if (typeof Integer === 'undefined') {
    throw new Error('Decimal requires Integer');
  }

  /**
   * Decimal
   * @class ktn.Decimal
   * @param {ktn.Integer} l
   * @param {number} e
   */
  function Decimal(l, e) {
    /**
     * @private
     * @property {ktn.Integer} ktn.Decimal#_l
     */
    this._l = l;

    /**
     * @private
     * @property {number} ktn.Decimal#_e
     */
    this._e = e;
  }

  // static method
  /**
   * Convert String to Decimal.
   * @static
   * @method ktn.Decimal.str
   * @param {string} n
   * @return {ktn.Decimal}
   */
  Decimal.str = function(str) {
    var index = str.indexOf('.');
    if (index < 0) {
      // '.' is not found
      return new Decimal(Integer.str(str), 0);
    }
    var trim = str.substring(0, index) + str.substring(index + 1);
    var i = 0;
    while (trim.charAt(i) === '0') { ++i; }
    if (i) { trim = trim.substring(i); }
    return new Decimal(Integer.str(trim), index - str.length + 1);
  };
  var decStr = Decimal.str;

  /**
   * Convert Number to Decimal.
   * @static
   * @method ktn.Decimal.num
   * @param {number} a
   * @param {number} b
   * @return {ktn.Decimal}
   */
  Decimal.num = function(a, b) {
    return new Decimal(Integer.num(a), b);
  };
  var decNum = Decimal.num;

  /**
   * Convert anything to Decimal.
   * @static
   * @method ktn.Decimal.dec
   * @param {object} l
   * @param {object} e
   * @return {ktn.Decimal}
   */
  Decimal.dec = function(l, e) {
    if (!arguments.length) {
      return new Decimal(new Integer(), 0);
    }
    if (arguments.length === 1) {
      if (l instanceof Decimal) { return l.clone(); }
      if (typeof l === "string") { return decStr(l); }
      return new Decimal(Integer.any(l), 0);
    }
    return new Decimal(Integer.any(l), e | 0);
  };
  var decimal = Decimal.dec;

  /**
   * Convert Fraction to Decimal.
   * @static
   * @method ktn.Decimal.rat
   * @param {ktn.Rational} a
   * @param {number} b
   * @return {ktn.Decimal}
   */
  Decimal.rat = function(a, b) {
    return new Decimal(a._n, 0).div(new Decimal(a._d, 0), b);
  };

  /**
   * 1
   * @static
   * @method ktn.Decimal.one
   * @return {ktn.Decimal} 1
   */
  Decimal.one = function(){ return decNum(1, 0); };

  /**
   * 0
   * @static
   * @method ktn.Decimal.zero
   * @return {ktn.Decimal} 0
   */
  Decimal.zero = function(){ return decNum(0, 0); };

  Decimal.prototype = {
    /**
     * @const
     * @property ktn.Decimal#constructor
     * @type ktn.Decimal
     */
    constructor: Decimal,

    /**
     * Copy Decimal.
     * @method ktn.Decimal#clone
     * @return {ktn.Decimal}
     */
    clone: function() {
      return new Decimal(this._l, this._e);
    },

    /**
     * @method ktn.Decimal#toString
     * @return {string}
     */
    toString: function() {
      if (this._e < 0) {
        var str = this._l.toString(),
            n = -this._e - str.length;
        if (n < 0) {
          return str.slice(0, this._e) + '.' + str.slice(this._e);
        }
        var zeros = '';
        for (var z = '0'; n > 0; n >>>= 1, z += z) {
          if (n & 1) { zeros += z; }
        }
        return '0.'+ zeros + str;
      }
      return this._l.addzero(this._e).toString();
    },

    /**
     * @method ktn.Decimal#html
     * @return {string}
     */
    html: function() {
      return this._l.toString() + '&times;10<sup>' + this._e + '</sup>';
    },

    /**
     * @method ktn.Decimal#tex
     * @return {string}
     */
    tex: function() {
      return this._l.toString() + '\times 10^' + this._e;
    },

    /**
     * @method ktn.Decimal#valueOf
     * @return {number}
     */
    valueOf: function() {
      return this._l.valueOf() * Math.pow(10, this._e);
    },

    /**
     * @private
     * @method ktn.Decimal#_co_
     * @return {number}
     */
    _co_: function() { return 3; },

    /**
     * @method ktn.Decimal#dot
     * @return {number}
     */
    dot: function() {
      var e = this._e < 0 ? -this._e : 0;
      return this._l.toString().length - e;
    },

    /**
     * @method ktn.Decimal#floor
     * @return {ktn.Integer}
     */
    floor: function() {
      return Integer.str(this._l.toString().substring(0, this.dot()));
    },

    /**
     * @method ktn.Decimal#setLen
     * @return {ktn.Decimal}
     */
    setLen: function(n) {
      var a = this.clone(),
          str = a._l.toString(),
          diff = n - str.length;
      if (diff < 0) {
        a._l = Integer.str(str.substring(0, n));
        a._e -= diff;
      } else {
        var zeros = '';
        for (var z = '0'; diff > 0; diff >>>= 1, z += z) {
          if (diff & 1) { zeros += z; }
        }
        a._l = Integer.str(str + zeros);
      }
      return a;
    },

    /**
     * @method ktn.Decimal#trim
     * @return {ktn.Decimal}
     */
    trim: function() {
      var a = this.clone(),
          str = a._l.toString(),
          i = str.length - 1;
      while (i >= 0 && str.charAt(i) === '0') {
        ++a._e;
        --i;
      }
      str = str.substring(0, i + 1);
      a._l = Integer.str(str);
      return a;
    },

    /**
     * @method ktn.Decimal#add
     * @param {ktn.Decimal} b
     * @return {ktn.Decimal}
     */
    add: function(b) {
      var diff = this._e - b._e;
      if (diff > 0) {
        return new Decimal(this._l.addzero(diff).add(b._l), b._e);
      }
      if (diff < 0) {
        return new Decimal(this._l.add(b._l.addzero(-diff)), this._e);
      }
      return new Decimal(this._l.add(b.l), this._e);
    },

    /**
     * @method ktn.Decimal#sub
     * @param {ktn.Decimal} b
     * @return {ktn.Decimal}
     */
    sub: function(b) {
      var diff = this._e - b._e;
      if (diff > 0) {
        return new Decimal(this._l.addzero(diff).sub(b._l), b._e);
      }
      if (diff < 0) {
        return new Decimal(this._l.sub(b._l.addzero(-diff)), this._e);
      }
      return new Decimal(this._l.sub(b._l), this._e);
    },

    /**
     * @method ktn.Decimal#mul
     * @param {ktn.Decimal} b
     * @return {ktn.Decimal}
     */
    mul: function(b) {
      var diff = this._e + b._e;
      return new Decimal(this._l.mul(b._l), diff);
    },

    /**
     * @method ktn.Decimal#div
     * @param {ktn.Decimal} b
     * @param {number} c
     * @return {ktn.Decimal}
     */
    div: function(b, c) {
      c = c || 20;
      var diff = this._l.toString().length - b._l.toString().length,
          e = this._e - b._e,
          f = b._e,
          arr = [];
      if (diff < 0) {
        return new Decimal(this._l.addzero(c - diff + 1 - f).div(b._l),
                                   -c + diff - 1 + e + f).trim();
      }
      return new Decimal(this._l.addzero(c - f).div(b._l), -c + e + f).trim();
    },

    /**
     * @private
     * @method ktn.Decimal#_add_
     * @param {object} a
     * @return {ktn.Decimal}
     */
    _add_: function(a) { return this.add(decimal(a)); },

    /**
     * @private
     * @method ktn.Decimal#_sub_
     * @param {object} a
     * @return {ktn.Decimal}
     */
    _sub_: function(a) { return this.sub(decimal(a)); },

    /**
     * @private
     * @method ktn.Decimal#_mul_
     * @param {object} a
     * @return {ktn.Decimal}
     */
    _mul_: function(a) { return this.mul(decimal(a)); },

    /**
     * @private
     * @method ktn.Decimal#_div_
     * @param {object} a
     * @return {ktn.Decimal}
     */
    _div_: function(a, b) { return this.div(decimal(a), b); }
  };

  // export
  if (typeof module !== 'undefined' && typeof module.exports !== 'undefined') {
    module.exports.Decimal = Decimal;
  } else {
    ktn.Decimal = Decimal;
  }
}(typeof ktn !== 'undefined' ? ktn : {}));