/**
 * @fileOverview Fraction in JavaScript.
 * @version 2011-03-20
 * @requires Long
 * @author kittttttan
 * @url http://kittttttan.web.fc2.com/math/mathjs.html
 * @example
 *    var a = fracNum(2, 3);  // => (2/3)
 *    var b = fracStr('-3/12'); // => (-1/4)
 *    fracAdd(a, b);  // => (5/12)
 */

//"use strict";

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
    n = fracCancel(n, d);
    this._n = n[0];
    this._d = n[1];
  }
}

/**
 * 1/1
 * @constant
 * @type {Fraction}
 */
var FRAC_ONE = new Fraction(longNum(1), longNum(1), true);

/**
 * 0/1
 * @constant
 * @type {Fraction}
 */
var FRAC_ZERO = new Fraction(new Long, longNum(1), true);

/**
 * Convert Number to Fraction.
 * @param {number} a Numerator
 * @param {number} b Denominator
 * @param {boolean} c
 * @returns {Fraction}
 */
function fracNum(a, b, c) {
  if (!b) {
    return new Fraction(longNum(a), longNum(1), true);
  }
  return new Fraction(longNum(a), longNum(b), c);
}

/**
 * Convert String to Fraction.
 * @param {string} a ex.'-1/2'
 * @returns {Fraction}
 */
function fracStr(a) {
  a = a.split('/');
  return new Fraction(longStr(a[0]), longStr(a[1] || '1'));
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
    return new Fraction(new Long, longNum(1), true);
  }
  if (arguments.length === 1) {
    if (a instanceof Fraction) { return a.clone(); }
    if (typeof a === "string") { return fracStr(a); }
    return new Fraction(longint(a), longNum(1), true);
  }
  if (!b) {
    throw new Error(['ZeroDivisionError:frac(',a,',',b,')'].join(''));
  }
  if (!a) { return new Fraction(new Long, longNum(1), true); }
  return new Fraction(longint(a), longint(b));
}

/**
 * @param {Long} a
 * @param {Long} b
 * @returns {Array.<Long>}
 */
function fracCancel(a, b) {
  var g = longGcd(a, b);
  a = longDivmod(a, g, false);
  b = longDivmod(b, g, false);
  if (!b._sn) {
    a._sn = !a._sn;
    b._sn = true;
  }
  return [a, b];
}

/**
 * @param {Fraction} a
 * @param {Fraction} b
 * @returns {Fraction} a + b
 */
function fracAdd(a, b) {
  return new Fraction(longAdd(longMul(a._n, b._d), longMul(a._d, b._n)),
                      longMul(a._d, b._d));
}

/**
 * @param {Fraction} a
 * @param {Fraction} b
 * @returns {Fraction} a - b
 */
function fracSub(a, b) {
  return new Fraction(longSub(longMul(a._n, b._d), longMul(a._d, b._n)),
                      longMul(a._d, b._d));
}

/**
 * @param {Fraction} a
 * @param {Fraction} b
 * @returns {Fraction} a * b
 */
function fracMul(a, b) {
  return new Fraction(longMul(a._n, b._n), longMul(a._d, b._d));
}

/**
 * @param {Fraction} a
 * @param {Fraction} b
 * @returns {Fraction} a / b
 */
function fracDiv(a, b) {
  return new Fraction(longMul(a._n, b._d), longMul(a._d, b._n));
}

/**
 * @param {Fraction} a
 * @param {number} b
 * @returns {Fraction} a^b
 */
function fracPow(a, b) {
  return new Fraction(longPow(a._n, b), longPow(a._d, b), true);
}

/**
 * @param {Fraction} a
 * @returns {Fraction} |a|
 */
function fracAbs(a) {
  return new Fraction(longAbs(a._n), a._d, true);
}

/**
 * @param {Fraction} a
 * @returns {Fraction} -a
 */
function fracNeg(a) {
  return new Fraction(longNeg(a._n), a._d, true);
}

/**
 * @param {Fraction} a
 * @param {Fraction} b
 * @returns {boolean} a == b
 */
function fracEq(a, b) {
  f = frac(a);
  g = frac(b);
  if (longEq(a._n, b._n) && longEq(a._d, b._d)) { return true; }
  return false;
}

/**
 * @param {Fraction} a
 * @param {Fraction} b
 * @returns {boolean} a === b
 */
function fracEqual(a, b) {
  if (!(a instanceof Fraction) || !(b instanceof Fraction)) { return false; }
  if (longEqual(a._n, b._n) && longEqual(a._d, b._d)) { return true; }
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
  return cmp(longMul(a._n, b._d), longMul(a._d, b._n));
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

/**
 * @param {Fraction} a
 * @returns {number}
 */
function fracLength(a) {
  return a.toString().length;
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
  _html_: function() { return fracToString(this); },

  /**
   * @returns {string}
   * @see fracTex
   */
  _tex_: function(){ return fracTex(this); },

  /**
   * @returns {number}
   * @see fracLength
   */
  _len_: function() { return fracLength(this); },

  /**
   * @returns {Fraction}
   * @see fracAbs
   */
  _abs_: function() { return fracAbs(this); },

  /**
   * @returns {Fraction}
   * @see fracNeg
   */
  _neg_: function() { return fracNeg(this); },

  /**
   * @param {object} a
   * @returns {boolean}
   * @see fracEq
   */
  _eq_: function(a) { return fracEq(a); },

  /**
   * @param {Fraction} a
   * @returns {boolean}
   * @see fracEqual
   */
  _equal_: function(a) { return fracEqual(a); },

  /**
   * @param {Fraction} a
   * @returns {number}
   * @see fracCmp
   */
  _cmp_: function(a) { return fracCmp(frac(a)); },

  /**
   * Multiplicative inverse (or reciprocal)
   * @returns {Fraction}
   */
  _inv_: function() {
    if (!this._n.isNonZero()) {
      throw new Error(
          ['ZeroDivisionError:Fraction(',this._d,',',this._n,')'].join(''));
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
