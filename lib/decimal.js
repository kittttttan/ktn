/**
 * @fileOverview Decimal in JavaScript.
 * @version 2012-02-12
 * @author kittttttan
 * @url http://kittttttan.web.fc2.com/math/mathjs.html
 * @example
 *    var a = Decimal.num(7, -3);
 *    var b = Decimal.str('100.1');
 *    var c = a.mul(b);
 *    a.toString();  // === '0.007'
 *    b.toString();  // === '100.1'
 *    c.toString();  // === '0.7007'
 */

//"use strict";
/** @requires Long */
if (typeof require !== 'undefined') {
  var Long = require('../lib/long.js').Long;
}
if (typeof Long === 'undefined') {
  throw 'Decimal requires Long';
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

/**
 * Convert String to Decimal.
 * @param {string} n
 * @returns {Decimal}
 */
function decStr(str) {
  var index = str.indexOf('.');
  if (index < 0) {
    // '.' is not found
    return new Decimal(Long.str(str), 0);
  }
  var trim = str.substring(0, index) + str.substring(index + 1);
  var i = 0;
  while (trim.charAt(i) === '0') { i++; }
  if (i) { trim = trim.substring(i); }
  return new Decimal(Long.str(trim), index - str.length + 1);
}

/**
 * Convert Number to Decimal.
 * @param {number} a
 * @param {number} b
 * @returns {Decimal}
 */
function decNum(a, b) {
  return new Decimal(Long.num(a), b);
}

/**
 * Convert anything to Decimal.
 * @param {object} l
 * @param {object} e
 * @returns {Decimal}
 */
function decimal(l, e) {
  if (!arguments.length) {
    return new Decimal(new Long(), 0);
  }
  if (arguments.length === 1) {
    if (l instanceof Decimal) { return l.clone(); }
    if (typeof l === "string") { return decStr(l); }
    return new Decimal(Long.longint(l), 0);
  }
  return new Decimal(Long.longint(l), e | 0);
}

/**
 * 1
 * @constant
 * @type {Decimal}
 */
var DEC_ONE = decNum(1, 0);

/**
 * 0
 * @constant
 * @type {Decimal}
 */
var DEC_ZERO = decNum(0, 0);

/**
 * Copy Decimal.
 * @param {Decimal} a
 * @returns {Decimal}
 */
function decClone(a) {
  return new Decimal(a._l, a._e);
}

/**
 * @param {Decimal} a
 * @returns {string}
 */
function decToString(a) {
  if (a._e < 0) {
    var str = a._l.toString(),
        n = -a._e - str.length;
    if (n < 0) {
      return str.slice(0, a._e) + '.' + str.slice(a._e);
    }
    var zeros = '';
    for (var z = '0'; n > 0; n >>>= 1, z += z) {
      if (n & 1) { zeros += z; }
    }
    return '0.'+ zeros + str;
  }
  return a._l.addzero(a._e).toString();
}

/**
 * @param {Decimal} a
 * @returns {string}
 */
function decHtml(a) {
  return a._l.toString() + '&times;10<sup>' + a._e + '</sup>';
}

/**
 * @param {Decimal} a
 * @returns {string}
 */
function decTex(a) {
  return a._l.toString() + '\times 10^' + a._e;
}

/**
 * @param {Decimal} a
 * @returns {number}
 */
function decValue(a) {
  return a._l.valueOf() * Math.pow(10, a._e);
}

/**
 * @param {Decimal} a
 * @param {Decimal} b
 * @returns {Decimal}
 */
function decAdd(a, b) {
  var diff = a._e - b._e;
  if (diff > 0) {
    return new Decimal(a._l.addzero(diff).add(b._l), b._e);
  }
  if (diff < 0) {
    return new Decimal(a._l.add(b._l.addzero(-diff)), a._e);
  }
  return new Decimal(a._l.add(b.l), a._e);
}

/**
 * @param {Decimal} a
 * @param {Decimal} b
 * @returns {Decimal}
 */
function decSub(a, b) {
  var diff = a._e - b._e;
  if (diff > 0) {
    return new Decimal(a._l.addzero(diff).sub(b._l), b._e);
  }
  if (diff < 0) {
    return new Decimal(a._l.sub(b._l.addzero(-diff)), a._e);
  }
  return new Decimal(a._l.sub(b._l), a._e);
}

/**
 * @param {Decimal} a
 * @param {Decimal} b
 * @returns {Decimal}
 */
function decMul(a, b) {
  var diff = a._e + b._e;
  return new Decimal(a._l.mul(b._l), diff);
}

/**
 * @param {Decimal} a
 * @param {Decimal} b
 * @param {number} c
 * @returns {Decimal}
 */
function decDiv(a, b, c) {
  c = c || 20;
  var diff = a._l.toString().length - b._l.toString().length,
      e = a._e - b._e,
      f = b._e,
      arr = [];
  if (diff < 0) {
    return new Decimal(a._l.addzero(c - diff + 1 - f).div(b._l),
                               -c + diff - 1 + e + f).trim();
  }
  return new Decimal(a._l.addzero(c - f).div(b._l), -c + e + f).trim();
}

/**
 * Convert Fraction to Decimal.
 * @param {Fraction} a
 * @param {number} b
 * @returns {Decimal}
 */
function decFrac(a, b) {
  return decDiv(new Decimal(a._n, 0), new Decimal(a._d, 0), b);
}

/**
 * @param {Decimal} a
 * @returns {number}
 */
function decDot(a) {
  var e = a._e < 0 ? -a._e : 0;
  return a._l.toString().length - e;
}

/**
 * @param {Decimal} a
 * @returns {Long}
 */
function decFloor(a) {
  return Long.str(a._l.toString().substring(0, decDot(a)));
}

/**
 * @param {Decimal} a
 * @returns {Decimal}
 */
function decSetLength(a, n) {
  var str = a._l.toString(),
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
}

/**
 * @param {Decimal} a
 * @returns {Decimal}
 */
function decTrim(a) {
  var str = a._l.toString(),
      i = str.length - 1;
  while (i >= 0 && str.charAt(i) === '0') {
    a._e += 1;
    i -= 1;
  }
  str = str.substring(0, i + 1);
  a._l = Long.str(str);
  return a;
}

Decimal.prototype = {
  constructor: Decimal,

  /** @returns {Decimal} */
  clone: function() { return decClone(this); },

  /** @returns {string} */
  toString: function() { return decToString(this); },
  /** @returns {string} */
  _html_: function() { return decHtml(this); },
  /** @returns {string} */
  _tex_: function() { return decTex(this); },

  /** @returns {number} */
  valueOf: function() { return decValue(this); },
  /** @returns {number} */
  _co_: function() { return 3; },

  /** @returns {number} */
  dot : function() { return decDot(this); },
  /** @returns {Long} */
  floor : function() { return decFloor(this); },
  /** @returns {Decimal} */
  setLen: function(a) { return decSetLength(this, a); },
  /** @returns {Decimal} */
  trim: function() { return decTrim(this); },

  /**
   * @param {Decimal} a
   * @returns {Decimal}
   */
  add: function(a) { return decAdd(this, a); },

  /**
   * @param {Decimal} a
   * @returns {Decimal}
   */
  sub: function(a) { return decSub(this, a); },

  /**
   * @param {Decimal} a
   * @returns {Decimal}
   */
  mul: function(a) { return decMul(this, a); },

  /**
   * @param {Decimal} a
   * @returns {Decimal}
   */
  div: function(a, b) { return decDiv(this, a, b); },

  /**
   * @param {object} a
   * @returns {Decimal}
   */
  _add_: function(a) { return decAdd(this, decimal(a)); },

  /**
   * @param {object} a
   * @returns {Decimal}
   */
  _sub_: function(a) { return decSub(this, decimal(a)); },

  /**
   * @param {object} a
   * @returns {Decimal}
   */
  _mul_: function(a) { return decMul(this, decimal(a)); },

  /**
   * @param {object} a
   * @returns {Decimal}
   */
  _div_: function(a, b) { return decDiv(this, decimal(a), b); }
};

// static method
Decimal.dec = decimal;
Decimal.num = decNum;
Decimal.str = decStr;
Decimal.frac = decFrac;

// exports for node
if (typeof module !== 'undefined' && typeof module.exports !== 'undefined') {
  module.exports.Decimal = Decimal;
}
