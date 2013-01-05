/**
 * @fileOverview Rational in JavaScript.
 * @example
 *    var Rational = require('/path/to/decimal.js').Rational;
 *    var a = Rational.num(2, 3);
 *    var b = Rational.str('-3/12');
 *    var c = a.add(b);
 *    a.toString();  // === '(2/3)'
 *    b.toString();  // === '(-1/4)'
 *    c.toString();  // === '(5/12)'
 * @author kittttttan
 */
(function(){
  "use strict";

  // require
  if (typeof exports === 'undefined') {
    throw new Error('no exports');
  }
  var Integer = require('../lib/integer.js').Integer;
  if (typeof Integer === 'undefined') {
    throw new Error('Rational requires Integer');
  }

  /**
   * @private
   * @param {Integer} a
   * @param {Integer} b
   * @return {Integer[]}
   */
  function cancel(a, b) {
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
   * Rational
   * @class Rational
   * @param {Integer} n
   * @param {Integer} d
   * @param {boolean} f If f is true then skip cancel().
   */
  function Rational(n, d, f) {
    if (f) {
      /**
       * Numerator
       * @private
       * @property {Integer} Rational#_n
       */
      this._n = n;
      
      /**
       * Denominator
       * @private
       * @property {Integer} Rational#_d
       */
      this._d = d;
    } else {
      var t = cancel(n, d);
      this._n = t[0];
      this._d = t[1];
    }
  }

  // static method
  /**
   * 1/1
   * @static
   * @method Rational.one
   * @return {Rational} 1/1
   */
  Rational.one = function() {
    return new Rational(Integer.num(1), Integer.num(1), true);
  };

  /**
   * 0/1
   * @static
   * @method Rational.zero
   * @return {Rational} 0/1
   */
  Rational.zero = function() {
    return new Rational(new Integer(), Integer.num(1), true);
  };

  /**
   * Convert Number to Rational.
   * @static
   * @method Rational.num
   * @param {number} a Numerator
   * @param {number} b Denominator
   * @param {boolean} c
   * @return {Rational}
   */
  Rational.num = function(a, b, c) {
    if (!b) {
      return new Rational(Integer.num(a), Integer.num(1), true);
    }
    return new Rational(Integer.num(a), Integer.num(b), c);
  };
  var ratNum = Rational.num;

  /**
   * Convert String to Rational.
   * @static
   * @method Rational.str
   * @param {string} a ex.'-1/2'
   * @return {Rational}
   */
  Rational.str = function(a) {
    a = a.split('/');
    return new Rational(Integer.str(a[0]), Integer.str(a[1] || '1'));
  };
  var ratStr = Rational.str;

  /**
   * Convert anything to Rational.
   * @static
   * @method Rational.any
   * @param a
   * @param b
   * @throws {Error} ZeroDivisionError
   * @return {Rational}
   */
  Rational.any = function(a, b) {
    if (!arguments.length) {
      return new Rational(new Integer(), Integer.num(1), true);
    }
    if (arguments.length === 1) {
      if (a instanceof Rational) { return a.clone(); }
      if (typeof a === "string") { return ratStr(a); }
      return new Rational(Integer.any(a), Integer.num(1), true);
    }
    if (!b) {
      throw new Error('zero division');
      //if (!a) { return NaN; }
      //if (a < 0) { return -Infinity; }
      //return Infinity;
    }
    if (!a) { return new Rational(new Integer(), Integer.num(1), true); }
    return new Rational(Integer.any(a), Integer.any(b));
  };
  var rat = Rational.any;

  Rational.prototype = {
    /**
     * @const
     * @property Rational#constructor
     * @type Rational
     */
    constructor: Rational,

    /**
     * @method Rational#clone
     * @return {Rational}
     */
    clone: function() {
      return new Rational(this._n, this._d, true);
    },

    /**
     * @method Rational#valueOf
     * @return {number}
     */
    valueOf: function() {
      return 1.0 * this._n.valueOf() / this._d.valueOf();
    },

    /**
     * @method Rational#toString
     * @return {string}
     */
    toString: function() {
      //if (this._d == 1) {return this._n.toString();}
      return '(' + [this._n, this._d].join('/') + ')';
    },
    
    /**
     * @method Rational#html
     * @return {string}
     */
    html: function() { return this.toString(); },

    /**
     * @method Rational#tex
     * @return {string}
     */
    tex: function() {
      //if (this._d == 1) {return this._n.toString();}
      return ['\\frac{', this._n, '}{', this._d, '}'].join('');
    },

    /**
     * @method Rational#abs
     * @return {Rational} |this|
     */
    abs: function() {
      return new Rational(this._n.abs(), this._d, true);
    },

    /**
     * @method Rational#neg
     * @return {Rational} -this
     */
    neg: function() {
      return new Rational(this._n.neg(), this._d, true);
    },

    /**
     * @method Rational#eq
     * @param {Rational} b
     * @return {boolean} this == b
     */
    eq: function(b) {
      b = rat(b);
      if (this._n.eq(b._n) && this._d.eq(b._d)) { return true; }
      return false;
    },

    /**
     * @method Rational#equal
     * @param {Rational} b
     * @return {boolean} this === b
     */
    equal: function(b) {
      if (!(b instanceof Rational)) { return false; }
      if (this._n.equal(b._n) && this._d.equal(b._d)) { return true; }
      return false;
    },

    /**
     * @method Rational#cmp
     * @param {Rational} b
     * @return {number}
     *      1 (this > b)
     *      0 (this = b)
     *     -1 (this < b)
     */
    cmp: function(b) {
      return this._n.mul(b._d).cmp(this._d.mul(b._n));
    },

    /**
     * Multiplicative inverse (or reciprocal)
     * @method Rational#inv
     * @return {Rational}
     */
    inv: function() {
      if (!this._n.isNonZero()) {
        if (!this._d.isNonZero()) { return NaN; }
        if (this._d._s) { return Infinity; }
        return -Infinity;
      }
      return new Rational(this._d, this._n, true);
    },

    /**
     * @private
     * @method Rational#_co_
     * @return {number}
     */
    _co_: function() { return 2; },

    /**
     * @method Rational#add
     * @param {Rational} b
     * @return {Rational} this + b
     */
    add: function(b) {
      return new Rational(this._n.mul(b._d).add(this._d.mul(b._n)),
                          this._d.mul(b._d));
    },

    /**
     * @method Rational#sub
     * @param {Rational} b
     * @return {Rational} this - b
     */
    sub: function(b) {
      return new Rational(this._n.mul(b._d).sub(this._d.mul(b._n)),
                          this._d.mul(b._d));
    },

    /**
     * @method Rational#mul
     * @param {Rational} b
     * @return {Rational} this * b
     */
    mul: function(b) {
      return new Rational(this._n.mul(b._n), this._d.mul(b._d));
    },

    /**
     * @method Rational#div
     * @param {Rational} b
     * @return {Rational} this / b
     */
    div: function(b) {
      return new Rational(this._n.mul(b._d), this._d.mul(b._n));
    },

    /**
     * @method Rational#pow
     * @param {number} b
     * @return {Rational} this^b
     */
    pow: function(b) {
      return new Rational(this._n.pow(b), this._d.pow(b), true);
    },

    /**
     * @private
     * @method Rational#_add_
     * @param {object} a
     * @return {Rational}
     */
    _add_: function(a) { return this.add(rat(a)); },

    /**
     * @private
     * @method Rational#_sub_
     * @param {object} a
     * @return {Rational}
     */
    _sub_: function(a) { return this.sub(rat(a)); },

    /**
     * @private
     * @method Rational#_mul_
     * @param {object} a
     * @return {Rational}
     */
    _mul_: function(a) { return this.mul(rat(a)); },

    /**
     * @private
     * @method Rational#_div_
     * @param {object} a
     * @return {Rational}
     */
    _div_: function(a) { return this.div(rat(a)); }
  };

  exports.Rational = Rational;
}());