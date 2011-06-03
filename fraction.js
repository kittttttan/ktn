/**
 * @fileOverview Fraction Object in JavaScript.
 * @version 2011-03-20
 * @requires Long
 * @author kittttttan
 * @url http://kittttttan.web.fc2.com/work/mathjs.html
 */

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
var FRAC_ZERO = new Fraction(new Long(), longNum(1), true);

/**
 * Convert String to Fraction.
 * @param {string} str ex.'-1/2'
 * @returns {Fraction}
 */
function fracStr(str) {
  str = str.split('/');
  return new Fraction(longStr(str[0]), longStr(str[1] || '1'));
}

/**
 * Convert anything to Fraction.
 * @param n
 * @param d
 * @throws {Error} ZeroDivisionError
 * @returns {Fraction}
 */
function frac(n, d) {
  if (!arguments.length) {
    return new Fraction(new Long(), longNum(1), true);
  }
  if (arguments.length === 1) {
    if (n instanceof Fraction) { return n.clone(); }
    if (typeof(n) === "string") { return fracStr(n); }
    return new Fraction(longint(n), longNum(1), true);
  }
  if (!d) {
    throw new Error(['ZeroDivisionError:frac(',n,',',d,')'].join(''));
  }
  if (!n) { return new Fraction(new Long(), longNum(1), true); }
  return new Fraction(longint(n), longint(d));
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
    a._sn = a._sn ? false : true;
    b._sn = true;
  }
  return [a, b];
}

/**
 * @param {Fraction} f
 * @param {Fraction} g
 * @returns {Fraction} f + g
 */
function fracAdd(f, g) {
  return new Fraction(longAdd(longMul(f._n, g._d), longMul(f._d, g._n)),
                      longMul(f._d, g._d));
}

/**
 * @param {Fraction} f
 * @param {Fraction} g
 * @returns {Fraction} f - g
 */
function fracSub(f, g) {
  return new Fraction(longSub(longMul(f._n, g._d), longMul(f._d, g._n)),
                      longMul(f._d, g._d));
}

/**
 * @param {Fraction} f
 * @param {Fraction} g
 * @returns {Fraction} f * g
 */
function fracMul(f, g) {
  return new Fraction(longMul(f._n, g._n), longMul(f._d, g._d));
}

/**
 * @param {Fraction} f
 * @param {Fraction} g
 * @returns {Fraction} f / g
 */
function fracDiv(f, g) {
  return new Fraction(longMul(f._n, g._d), longMul(f._d, g._n));
}

/**
 * @param {Fraction} f
 * @param {number} n
 * @returns {Fraction} f^n
 */
function fracPow(f, n) {
  return new Fraction(longPow(f._n, n), longPow(f._d, n), true);
}

/**
 * @param {Fraction} f
 * @returns {Fraction} |f|
 */
function fracAbs(f) {
  return new Fraction(longAbs(f._n), f._d, true);
}

/**
 * @param {Fraction} f
 * @returns {Fraction} -f
 */
function fracNeg(f) {
  return new Fraction(longNeg(f._n), f._d, true);
}

/**
 * @param {Fraction} f
 * @param {Fraction} g
 * @returns {boolean} f == g
 */
function fracEq(f, g) {
  f = frac(f);
  g = frac(g);
  if (longEq(f._n, g._n) && longEq(f._d, g._d)) { return true; }
  return false;
}

/**
 * @param {Fraction} f
 * @param {Fraction} g
 * @returns {boolean} f === g
 */
function fracEqual(f, g) {
  if (!(f instanceof Fraction) || !(g instanceof Fraction)) { return false; }
  if (longEqual(f._n, g._n) && longEqual(f._d, g._d)) { return true; }
  return false;
}

/**
 * @param {Fraction} f
 * @param {Fraction} g
 * @returns {number} If f > g then 1, else if f == g then 0, else -1.
 */
function fracCmp(f, g) {
  var a = longMul(f._n, g._d);
  var b = longMul(f._d, g._n);
  return cmp(a, b);
}

/**
 * @param {Fraction} f
 * @returns {Fraction}
 */
function fracClone(f) {
  return new Fraction(f._n, f._d, true);
}

/**
 * @param {Fraction} f
 * @returns {string}
 */
function fracToString(f) {
  //if (f._d == 1) {return f._n.toString();}
  return '(' + [f._n, f._d].join('/') + ')';
}

/**
 * @param {Fraction} f
 * @returns {string}
 */
function fracTex(f) {
  //if (f._d == 1) {return f._n.toString();}
  return ['\\frac{', f._n, '}{', f._d, '}'].join('');
}

/**
 * @param {Fraction} f
 * @returns {number}
 */
function fracValue(f) {
  return f._n.valueOf() / f._d.valueOf();
}

/**
 * @param {Fraction} f
 * @returns {number}
 */
function fracLength(f) {
  return f.toString().length;
}

Fraction.prototype = {
  constructor: Fraction,
  /** Apply {@link fracClone} */
  clone: function() { return fracClone(this); },
  /** Apply {@link fracToString} */
  toString: function() { return fracToString(this); },
  /** Apply {@link fracValue} */
  valueOf: function() { return fracValue(this); },
  /** Apply {@link fracToString} */
  _html_: function() { return fracToString(this); },
  /** Apply {@link fracTex} */
  _tex_: function(){ return fracTex(this); },
  /** Apply {@link fracLength} */
  _len_: function() { return fracLength(this); },
  /** Apply {@link fracAbs} */
  _abs_: function() { return fracAbs(this); },
  /** Apply {@link fracNeg} */
  _neg_: function() { return fracNeg(this); },
  /** Apply {@link fracEq} */
  _eq_: function(x) { return fracEq(x); },
  /** Apply {@link fracEqual} */
  _equal_: function(x) { return fracEqual(x); },
  /** Apply {@link fracCmp} */
  _cmp_: function(x) { return fracCmp(frac(x)); },
  _inv_: function() { return new Fraction(this._d, this._n, true); },
  _co_: function() { return 1; },
  _asim_: function(x) { return isNum(x); },
  _msim_: function(x) { return isNum(x); },
  /** Apply {@link fracAdd} */
  add: function(f) { return fracAdd(this, f); },
  /** Apply {@link fracSub} */
  sub: function(f) { return fracSub(this, f); },
  /** Apply {@link fracMul} */
  mul: function(f) { return fracMul(this, f); },
  /** Apply {@link fracDiv} */
  div: function(f) { return fracDiv(this, f); },
  /** Apply {@link fracPow} */
  pow: function(n) { return fracPow(this, n); },
  _add_: function(x) { return fracAdd(this, frac(x)); },
  _sub_: function(x) { return fracSub(this, frac(x)); },
  _mul_: function(x) { return fracMul(this, frac(x)); },
  _div_: function(x) { return fracDiv(this, frac(x)); },
  _d_: function() { return FRAC_ZERO; }
};
