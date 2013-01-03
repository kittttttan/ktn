/**
 * @file Decimal in JavaScript.
 * @example
 *    var a = ktn.Decimal.num(7, -3);
 *    var b = ktn.Decimal.str('100.1');
 *    var c = a.mul(b);
 *    a.toString();  // === '0.007'
 *    b.toString();  // === '100.1'
 *    c.toString();  // === '0.7007'
 * @author kittttttan
 */

var ktn = ktn || {};
ktn.Decimal = (function(){
  "use strict";

  var Integer;
  /** @requires ktn.Integer */
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
   * @memberof ktn
   * @constructor
   * @property {ktn.Integer} _l
   * @property {number} _e
   * @param {ktn.Integer} l
   * @param {number} e
   */
  function Decimal(l, e) {
    this._l = l;
    this._e = e;
  }

  // static method
  /**
   * Convert String to Decimal.
   * @memberof ktn.Decimal
   * @param {string} n
   * @returns {ktn.Decimal}
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
   * @memberof ktn.Decimal
   * @param {number} a
   * @param {number} b
   * @returns {ktn.Decimal}
   */
  Decimal.num = function(a, b) {
    return new Decimal(Integer.num(a), b);
  };
  var decNum = Decimal.num;

  /**
   * Convert anything to Decimal.
   * @memberof ktn.Decimal
   * @param {object} l
   * @param {object} e
   * @returns {ktn.Decimal}
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
   * @memberof ktn.Decimal
   * @param {ktn.Rational} a
   * @param {number} b
   * @returns {ktn.Decimal}
   */
  Decimal.rat = function(a, b) {
    return new Decimal(a._n, 0).div(new Decimal(a._d, 0), b);
  };

  /**
   * 1
   * @memberof ktn.Decimal
   * @returns {ktn.Decimal} 1
   */
  Decimal.one = function(){ return decNum(1, 0); };

  /**
   * 0
   * @memberof ktn.Decimal
   * @returns {ktn.Decimal} 0
   */
  Decimal.zero = function(){ return decNum(0, 0); };

  Decimal.prototype = {
    constructor: Decimal,

    /**
     * Copy Decimal.
     * @memberof ktn.Decimal#
     * @returns {ktn.Decimal}
     */
    clone: function() {
      return new Decimal(this._l, this._e);
    },

    /**
     * @memberof ktn.Decimal#
     * @returns {string}
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
     * @memberof ktn.Decimal#
     * @returns {string}
     */
    html: function() {
      return this._l.toString() + '&times;10<sup>' + this._e + '</sup>';
    },

    /**
     * @memberof ktn.Decimal#
     * @returns {string}
     */
    tex: function() {
      return this._l.toString() + '\times 10^' + this._e;
    },

    /**
     * @memberof ktn.Decimal#
     * @returns {number}
     */
    valueOf: function() {
      return this._l.valueOf() * Math.pow(10, this._e);
    },

    /** @returns {number} */
    _co_: function() { return 3; },

    /**
     * @memberof ktn.Decimal#
     * @returns {number}
     */
    dot: function() {
      var e = this._e < 0 ? -this._e : 0;
      return this._l.toString().length - e;
    },

    /**
     * @memberof ktn.Decimal#
     * @returns {ktn.Integer}
     */
    floor: function() {
      return Integer.str(this._l.toString().substring(0, this.dot()));
    },

    /**
     * @memberof ktn.Decimal#
     * @returns {ktn.Decimal}
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
     * @memberof ktn.Decimal#
     * @returns {ktn.Decimal}
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
     * @memberof ktn.Decimal#
     * @param {ktn.Decimal} b
     * @returns {ktn.Decimal}
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
     * @memberof ktn.Decimal#
     * @param {ktn.Decimal} b
     * @returns {ktn.Decimal}
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
     * @memberof ktn.Decimal#
     * @param {ktn.Decimal} b
     * @returns {ktn.Decimal}
     */
    mul: function(b) {
      var diff = this._e + b._e;
      return new Decimal(this._l.mul(b._l), diff);
    },

    /**
     * @memberof ktn.Decimal#
     * @param {ktn.Decimal} b
     * @param {number} c
     * @returns {ktn.Decimal}
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
     * @param {object} a
     * @returns {ktn.Decimal}
     */
    _add_: function(a) { return this.add(decimal(a)); },

    /**
     * @param {object} a
     * @returns {ktn.Decimal}
     */
    _sub_: function(a) { return this.sub(decimal(a)); },

    /**
     * @param {object} a
     * @returns {ktn.Decimal}
     */
    _mul_: function(a) { return this.mul(decimal(a)); },

    /**
     * @param {object} a
     * @returns {ktn.Decimal}
     */
    _div_: function(a, b) { return this.div(decimal(a), b); }
  };

  return Decimal;
}());

// exports for node
if (typeof module !== 'undefined' && typeof module.exports !== 'undefined') {
  module.exports.Decimal = ktn.Decimal;
}
