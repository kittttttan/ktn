/**
 * @fileOverview Fraction in JavaScript.
 * @version 2012-02-12
 * @url https://github.com/kittttttan/ktn
 * @example
 *    var a = Fraction.num(2, 3);
 *    var b = Fraction.str('-3/12');
 *    var c = a.add(b);
 *    a.toString();  // === '(2/3)'
 *    b.toString();  // === '(-1/4)'
 *    c.toString();  // === '(5/12)'
 */

//"use strict";
/** @requires Long */
if (typeof require !== 'undefined') {
  var Long = require('../lib/long.js').Long;
}
if (typeof Long === 'undefined') {
  throw 'Fraction requires Long';
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
 * @property {Long} _n
 * @property {Long} _d
 * @param {Long} n Numerator
 * @param {Long} d Denominator
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

/**
 * 1/1
 * @constant
 * @type {Fraction}
 */
var FRAC_ONE = new Fraction(Long.num(1), Long.num(1), true);

/**
 * 0/1
 * @constant
 * @type {Fraction}
 */
var FRAC_ZERO = new Fraction(new Long(), Long.num(1), true);

/**
 * Convert Number to Fraction.
 * @param {number} a Numerator
 * @param {number} b Denominator
 * @param {boolean} c
 * @returns {Fraction}
 */
function fracNum(a, b, c) {
  if (!b) {
    return new Fraction(Long.num(a), Long.num(1), true);
  }
  return new Fraction(Long.num(a), Long.num(b), c);
}

/**
 * Convert String to Fraction.
 * @param {string} a ex.'-1/2'
 * @returns {Fraction}
 */
function fracStr(a) {
  a = a.split('/');
  return new Fraction(Long.str(a[0]), Long.str(a[1] || '1'));
}

/**
 * Convert anything to Fraction.
 * @param a
 * @param b
 * @throws {Error} ZeroDivisionError
 * @returns {Fraction}
 */
function frac(a, b) {
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
}

/**
 * @param {Fraction} a
 * @param {Fraction} b
 * @returns {Fraction} a + b
 */
function fracAdd(a, b) {
  return new Fraction(a._n.mul(b._d).add(a._d.mul(b._n)),
                      a._d.mul(b._d));
}

/**
 * @param {Fraction} a
 * @param {Fraction} b
 * @returns {Fraction} a - b
 */
function fracSub(a, b) {
  return new Fraction(a._n.mul(b._d).sub(a._d.mul(b._n)),
                      a._d.mul(b._d));
}

/**
 * @param {Fraction} a
 * @param {Fraction} b
 * @returns {Fraction} a * b
 */
function fracMul(a, b) {
  return new Fraction(a._n.mul(b._n), a._d.mul(b._d));
}

/**
 * @param {Fraction} a
 * @param {Fraction} b
 * @returns {Fraction} a / b
 */
function fracDiv(a, b) {
  return new Fraction(a._n.mul(b._d), a._d.mul(b._n));
}

/**
 * @param {Fraction} a
 * @param {number} b
 * @returns {Fraction} a^b
 */
function fracPow(a, b) {
  return new Fraction(a._n.pow(b), a._d.pow(b), true);
}

/**
 * @param {Fraction} a
 * @returns {Fraction} |a|
 */
function fracAbs(a) {
  return new Fraction(a._n.abs(), a._d, true);
}

/**
 * @param {Fraction} a
 * @returns {Fraction} -a
 */
function fracNeg(a) {
  return new Fraction(a._n.neg(), a._d, true);
}

/**
 * @param {Fraction} a
 * @param {Fraction} b
 * @returns {boolean} a == b
 */
function fracEq(a, b) {
  a = frac(a);
  b = frac(b);
  if (a._n.eq(b._n) && a._d.eq(b._d)) { return true; }
  return false;
}

/**
 * @param {Fraction} a
 * @param {Fraction} b
 * @returns {boolean} a === b
 */
function fracEqual(a, b) {
  if (!(a instanceof Fraction) || !(b instanceof Fraction)) { return false; }
  if (a._n.equal(b._n) && a._d.equal(b._d)) { return true; }
  return false;
}

/**
 * @param {Fraction} a
 * @param {Fraction} b
 * @returns {number} <br>
 *    1 (a > b)<br>
 *    0 (a = b)<br>
 *   -1 (a < b)
 */
function fracCmp(a, b) {
  return a._n.mul(b._d).cmp(a._d.mul(b._n));
}

/**
 * @param {Fraction} a
 * @returns {Fraction}
 */
function fracClone(a) {
  return new Fraction(a._n, a._d, true);
}

/**
 * @param {Fraction} a
 * @returns {string}
 */
function fracToString(a) {
  //if (a._d == 1) {return a._n.toString();}
  return '(' + [a._n, a._d].join('/') + ')';
}

/**
 * @param {Fraction} a
 * @returns {string}
 */
function fracTex(a) {
  //if (a._d == 1) {return a._n.toString();}
  return ['\\frac{', a._n, '}{', a._d, '}'].join('');
}

/**
 * @param {Fraction} a
 * @returns {number}
 */
function fracValue(a) {
  return a._n.valueOf() / a._d.valueOf();
}

Fraction.prototype = {
  constructor: Fraction,

  /**
   * @returns {Fraction}
   * @see fracClone
   */
  clone: function() { return fracClone(this); },

  /**
   * @returns {number}
   * @see fracValue
   */
  valueOf: function() { return fracValue(this); },

  /**
   * @returns {string}
   * @see fracToString
   */
  toString: function() { return fracToString(this); },
  html: function() { return fracToString(this); },

  /**
   * @returns {string}
   * @see fracTex
   */
  tex: function(){ return fracTex(this); },

  /**
   * @returns {Fraction}
   * @see fracAbs
   */
  abs: function() { return fracAbs(this); },

  /**
   * @returns {Fraction}
   * @see fracNeg
   */
  neg: function() { return fracNeg(this); },

  /**
   * @param {object} a
   * @returns {boolean}
   * @see fracEq
   */
  eq: function(a) { return fracEq(a); },

  /**
   * @param {Fraction} a
   * @returns {boolean}
   * @see fracEqual
   */
  equal: function(a) { return fracEqual(a); },

  /**
   * @param {Fraction} a
   * @returns {number}
   * @see fracCmp
   */
  cmp: function(a) { return fracCmp(frac(a)); },

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
   * @param {Fraction} a
   * @returns {Fraction}
   * @see fracAdd
   */
  add: function(a) { return fracAdd(this, a); },

  /**
   * @param {Fraction} a
   * @returns {Fraction}
   * @see fracSub
   */
  sub: function(a) { return fracSub(this, a); },

  /**
   * @param {Fraction} a
   * @returns {Fraction}
   * @see fracMul
   */
  mul: function(a) { return fracMul(this, a); },

  /**
   * @param {Fraction} a
   * @returns {Fraction}
   * @see fracDiv
   */
  div: function(a) { return fracDiv(this, a); },

  /**
   * @param {number} a
   * @returns {Fraction}
   * @see fracPow
   */
  pow: function(a) { return fracPow(this, a); },

  /**
   * @param {object} a
   * @returns {Fraction}
   * @see fracAdd
   */
  _add_: function(a) { return fracAdd(this, frac(a)); },

  /**
   * @param {object} a
   * @returns {Fraction}
   * @see fracSub
   */
  _sub_: function(a) { return fracSub(this, frac(a)); },

  /**
   * @param {object} a
   * @returns {Fraction}
   * @see fracMul
   */
  _mul_: function(a) { return fracMul(this, frac(a)); },

  /**
   * @param {object} a
   * @returns {Fraction}
   * @see fracDiv
   */
  _div_: function(a) { return fracDiv(this, frac(a)); }
};

// static method
Fraction.frac = frac;
Fraction.num = fracNum;
Fraction.str = fracStr;

// exports for node
if (typeof module !== 'undefined' && typeof module.exports !== 'undefined') {
  module.exports.Fraction = Fraction;
}
