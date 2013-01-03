/**
 * @fileOverview Fraction in JavaScript.
 * @version 2012-03-20
 * @url https://github.com/kittttttan/ktn
 * @example
 *    var a = Fraction.num(2, 3);
 *    var b = Fraction.str('-3/12');
 *    var c = a.add(b);
 *    a.toString();  // === '(2/3)'
 *    b.toString();  // === '(-1/4)'
 *    c.toString();  // === '(5/12)'
 */

/** @requires Long */
if (typeof require !== 'undefined') {
  var Long = require('../lib/long.js').Long;
}
if (typeof Long === 'undefined') {
  throw new Error('Fraction requires Long');
}

/**
 * @param {Long} a
 * @param {Long} b
 * @returns {Array.<Long>}
 */
function fracCancel(a, b) {
  var g = a.gcd(b);
  a = a.div(g);
  b = b.div(g);
  if (!b._s) {
    a._s = !a._s;
    b._s = true;
  }
  return [a, b];
}

/**
 * Long Fraction
 * @class Long Fraction
 * @property {Long} _n Numerator
 * @property {Long} _d Denominator
 * @param {Long} n
 * @param {Long} d
 * @param {boolean} f If f is true then skip fracCancel().
 */
function Fraction(n, d, f) {
  if (f) {
    this._n = n;
    this._d = d;
  } else {
    var t = fracCancel(n, d);
    this._n = t[0];
    this._d = t[1];
  }
}

(function(){
  "use strict";
  // static method
  /**
   * 1/1
   * @constant
   * @type {Fraction}
   */
  Fraction.ONE = new Fraction(Long.num(1), Long.num(1), true);

  /**
   * 0/1
   * @constant
   * @type {Fraction}
   */
  Fraction.ZERO = new Fraction(new Long(), Long.num(1), true);

  /**
   * Convert Number to Fraction.
   * @param {number} a Numerator
   * @param {number} b Denominator
   * @param {boolean} c
   * @returns {Fraction}
   */
  Fraction.num = function(a, b, c) {
    if (!b) {
      return new Fraction(Long.num(a), Long.num(1), true);
    }
    return new Fraction(Long.num(a), Long.num(b), c);
  };
  var fracNum = Fraction.num;

  /**
   * Convert String to Fraction.
   * @param {string} a ex.'-1/2'
   * @returns {Fraction}
   */
  Fraction.str = function(a) {
    a = a.split('/');
    return new Fraction(Long.str(a[0]), Long.str(a[1] || '1'));
  };
  var fracStr = Fraction.str;

  /**
   * Convert anything to Fraction.
   * @param a
   * @param b
   * @throws {Error} ZeroDivisionError
   * @returns {Fraction}
   */
  Fraction.frac = function frac(a, b) {
    if (!arguments.length) {
      return new Fraction(new Long(), Long.num(1), true);
    }
    if (arguments.length === 1) {
      if (a instanceof Fraction) { return a.clone(); }
      if (typeof a === "string") { return fracStr(a); }
      return new Fraction(Long.longint(a), Long.num(1), true);
    }
    if (!b) {
      if (!a) { return NaN; }
      if (a < 0) { return -Infinity; }
      return Infinity;
    }
    if (!a) { return new Fraction(new Long(), Long.num(1), true); }
    return new Fraction(Long.longint(a), Long.longint(b));
  };
  var frac = Fraction.frac;

  Fraction.prototype = {
    constructor: Fraction,

    /**
     * @returns {Fraction}
     */
    clone: function() {
      return new Fraction(this._n, this._d, true);
    },

    /**
     * @returns {number}
     */
    valueOf: function() {
      return 1.0 * this._n.valueOf() / this._d.valueOf();
    },

    /**
     * @returns {string}
     */
    toString: function() {
      //if (this._d == 1) {return this._n.toString();}
      return '(' + [this._n, this._d].join('/') + ')';
    },
    
    html: function() { return this.toString(); },

    /**
     * @returns {string}
     */
    tex: function() {
      //if (this._d == 1) {return this._n.toString();}
      return ['\\frac{', this._n, '}{', this._d, '}'].join('');
    },

    /**
     * @returns {Fraction} |this|
     */
    abs: function() {
      return new Fraction(this._n.abs(), this._d, true);
    },

    /**
     * @returns {Fraction} -this
     */
    neg: function() {
      return new Fraction(this._n.neg(), this._d, true);
    },

    /**
     * @param {Fraction} b
     * @returns {boolean} this == b
     */
    eq: function(b) {
      b = frac(b);
      if (this._n.eq(b._n) && this._d.eq(b._d)) { return true; }
      return false;
    },

    /**
     * @param {Fraction} b
     * @returns {boolean} this === b
     */
    equal: function(b) {
      if (!(b instanceof Fraction)) { return false; }
      if (this._n.equal(b._n) && this._d.equal(b._d)) { return true; }
      return false;
    },

    /**
     * @param {Fraction} b
     * @returns {number} <br>
     *    1 (this > b)<br>
     *    0 (this = b)<br>
     *   -1 (this < b)
     */
    cmp: function(b) {
      return this._n.mul(b._d).cmp(this._d.mul(b._n));
    },

    /**
     * Multiplicative inverse (or reciprocal)
     * @returns {Fraction}
     */
    inv: function() {
      if (!this._n.isNonZero()) {
        if (!this._d.isNonZero()) { return NaN; }
        if (this._d._s) { return Infinity; }
        return -Infinity;
      }
      return new Fraction(this._d, this._n, true);
    },

    /** @returns {number} */
    _co_: function() { return 2; },

    /**
     * @param {Fraction} b
     * @returns {Fraction} this + b
     */
    add: function(b) {
      return new Fraction(this._n.mul(b._d).add(this._d.mul(b._n)),
                          this._d.mul(b._d));
    },

    /**
     * @param {Fraction} b
     * @returns {Fraction} this - b
     */
    sub: function(b) {
      return new Fraction(this._n.mul(b._d).sub(this._d.mul(b._n)),
                          this._d.mul(b._d));
    },

    /**
     * @param {Fraction} b
     * @returns {Fraction} this * b
     */
    mul: function(b) {
      return new Fraction(this._n.mul(b._n), this._d.mul(b._d));
    },

    /**
     * @param {Fraction} b
     * @returns {Fraction} this / b
     */
    div: function(b) {
      return new Fraction(this._n.mul(b._d), this._d.mul(b._n));
    },

    /**
     * @param {number} b
     * @returns {Fraction} this^b
     */
    pow: function(b) {
      return new Fraction(this._n.pow(b), this._d.pow(b), true);
    },

    /**
     * @param {object} a
     * @returns {Fraction}
     */
    _add_: function(a) { return this.add(frac(a)); },

    /**
     * @param {object} a
     * @returns {Fraction}
     */
    _sub_: function(a) { return this.sub(frac(a)); },

    /**
     * @param {object} a
     * @returns {Fraction}
     */
    _mul_: function(a) { return this.mul(frac(a)); },

    /**
     * @param {object} a
     * @returns {Fraction}
     */
    _div_: function(a) { return this.div(frac(a)); }
  };
}());

// exports for node
if (typeof module !== 'undefined' && typeof module.exports !== 'undefined') {
  module.exports.Fraction = Fraction;
}
