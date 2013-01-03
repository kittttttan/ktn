/**
 * @fileOverview Decimal in JavaScript.
 * @version 2012-03-20
 * @url https://github.com/kittttttan/ktn
 * @example
 *    var a = Decimal.num(7, -3);
 *    var b = Decimal.str('100.1');
 *    var c = a.mul(b);
 *    a.toString();  // === '0.007'
 *    b.toString();  // === '100.1'
 *    c.toString();  // === '0.7007'
 */

/** @requires Long */
if (typeof require !== 'undefined') {
  var Long = require('../lib/long.js').Long;
}
if (typeof Long === 'undefined') {
  throw new Error('Decimal requires Long');
}

/**
 * Decimal
 * @class Decimal
 * @property {Long} _l
 * @property {number} _e
 * @param {Long} l
 * @param {number} e
 */
function Decimal(l, e) {
  this._l = l;
  this._e = e;
}

(function(){
  "use strict";

  // static method
  /**
   * Convert String to Decimal.
   * @param {string} n
   * @returns {Decimal}
   */
  Decimal.str = function(str) {
    var index = str.indexOf('.');
    if (index < 0) {
      // '.' is not found
      return new Decimal(Long.str(str), 0);
    }
    var trim = str.substring(0, index) + str.substring(index + 1);
    var i = 0;
    while (trim.charAt(i) === '0') { ++i; }
    if (i) { trim = trim.substring(i); }
    return new Decimal(Long.str(trim), index - str.length + 1);
  };
  var decStr = Decimal.str;

  /**
   * Convert Number to Decimal.
   * @param {number} a
   * @param {number} b
   * @returns {Decimal}
   */
  Decimal.num = function(a, b) {
    return new Decimal(Long.num(a), b);
  };
  var decNum = Decimal.num;

  /**
   * Convert anything to Decimal.
   * @param {object} l
   * @param {object} e
   * @returns {Decimal}
   */
  Decimal.dec = function(l, e) {
    if (!arguments.length) {
      return new Decimal(new Long(), 0);
    }
    if (arguments.length === 1) {
      if (l instanceof Decimal) { return l.clone(); }
      if (typeof l === "string") { return decStr(l); }
      return new Decimal(Long.longint(l), 0);
    }
    return new Decimal(Long.longint(l), e | 0);
  };
  var decimal = Decimal.dec;

  /**
   * Convert Fraction to Decimal.
   * @param {Fraction} a
   * @param {number} b
   * @returns {Decimal}
   */
  Decimal.frac = function(a, b) {
    return new Decimal(a._n, 0).div(new Decimal(a._d, 0), b);
  };
  var decFrac = Decimal.frac;

  /**
   * 1
   * @constant
   * @type {Decimal}
   */
  Decimal.ONE = decNum(1, 0);

  /**
   * 0
   * @constant
   * @type {Decimal}
   */
  Decimal.ZERO = decNum(0, 0);

  Decimal.prototype = {
    constructor: Decimal,

    /**
     * Copy Decimal.
     * @returns {Decimal}
     */
    clone: function() {
      return new Decimal(this._l, this._e);
    },

    /**
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
     * @returns {string}
     */
    _html_: function() {
      return this._l.toString() + '&times;10<sup>' + this._e + '</sup>';
    },

    /**
     * @returns {string}
     */
    _tex_: function() {
      return this._l.toString() + '\times 10^' + this._e;
    },

    /**
     * @returns {number}
     */
    valueOf: function() {
      return this._l.valueOf() * Math.pow(10, this._e);
    },

    /** @returns {number} */
    _co_: function() { return 3; },

    /**
     * @returns {number}
     */
    dot: function() {
      var e = this._e < 0 ? -this._e : 0;
      return this._l.toString().length - e;
    },

    /**
     * @returns {Long}
     */
    floor: function() {
      return Long.str(this._l.toString().substring(0, this.dot()));
    },

    /**
     * @returns {Decimal}
     */
    setLen: function(n) {
      var a = this.clone(),
          str = a._l.toString(),
          diff = n - str.length;
      if (diff < 0) {
        a._l = Long.str(str.substring(0, n));
        a._e -= diff;
      } else {
        var zeros = '';
        for (var z = '0'; diff > 0; diff >>>= 1, z += z) {
          if (diff & 1) { zeros += z; }
        }
        a._l = Long.str(str + zeros);
      }
      return a;
    },

    /**
     * @returns {Decimal}
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
      a._l = Long.str(str);
      return a;
    },

    /**
     * @param {Decimal} b
     * @returns {Decimal}
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
     * @param {Decimal} b
     * @returns {Decimal}
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
     * @param {Decimal} b
     * @returns {Decimal}
     */
    mul: function(b) {
      var diff = this._e + b._e;
      return new Decimal(this._l.mul(b._l), diff);
    },

    /**
     * @param {Decimal} b
     * @param {number} c
     * @returns {Decimal}
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
     * @returns {Decimal}
     */
    _add_: function(a) { return this.add(decimal(a)); },

    /**
     * @param {object} a
     * @returns {Decimal}
     */
    _sub_: function(a) { return this.sub(decimal(a)); },

    /**
     * @param {object} a
     * @returns {Decimal}
     */
    _mul_: function(a) { return this.mul(decimal(a)); },

    /**
     * @param {object} a
     * @returns {Decimal}
     */
    _div_: function(a, b) { return this.div(decimal(a), b); }
  };
}());

// exports for node
if (typeof module !== 'undefined' && typeof module.exports !== 'undefined') {
  module.exports.Decimal = Decimal;
}
