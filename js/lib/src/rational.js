/**
 * @fileOverview Rational in JavaScript.
 * @example
 *    var a = ktn.Rational.num(2, 3);
 *    var b = ktn.Rational.str('-3/12');
 *    var c = a.add(b);
 *    a.toString();  // === '(2/3)'
 *    b.toString();  // === '(-1/4)'
 *    c.toString();  // === '(5/12)'
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
   * @private
   * @param {ktn.Integer} a
   * @param {ktn.Integer} b
   * @return {ktn.Integer[]}
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
   * @class ktn.Rational
   * @param {ktn.Integer} n
   * @param {ktn.Integer} d
   * @param {boolean} f If f is true then skip cancel().
   */
  function Rational(n, d, f) {
    if (f) {
      /**
       * Numerator
       * @private
       * @property {ktn.Integer} ktn.Rational#_n
       */
      this._n = n;
      
      /**
       * Denominator
       * @private
       * @property {ktn.Integer} ktn.Rational#_d
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
   * @method ktn.Rational.one
   * @return {ktn.Rational} 1/1
   */
  Rational.one = function() {
    return new Rational(Integer.num(1), Integer.num(1), true);
  };

  /**
   * 0/1
   * @static
   * @method ktn.Rational.zero
   * @return {ktn.Rational} 0/1
   */
  Rational.zero = function() {
    return new Rational(new Integer(), Integer.num(1), true);
  };

  /**
   * Convert Number to Rational.
   * @static
   * @method ktn.Rational.num
   * @param {number} a Numerator
   * @param {number} b Denominator
   * @param {boolean} c
   * @return {ktn.Rational}
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
   * @method ktn.Rational.str
   * @param {string} a ex.'-1/2'
   * @return {ktn.Rational}
   */
  Rational.str = function(a) {
    a = a.split('/');
    return new Rational(Integer.str(a[0]), Integer.str(a[1] || '1'));
  };
  var ratStr = Rational.str;

  /**
   * Convert anything to Rational.
   * @static
   * @method ktn.Rational.any
   * @param a
   * @param b
   * @throws {Error} ZeroDivisionError
   * @return {ktn.Rational}
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
      if (!a) { return NaN; }
      if (a < 0) { return -Infinity; }
      return Infinity;
    }
    if (!a) { return new Rational(new Integer(), Integer.num(1), true); }
    return new Rational(Integer.any(a), Integer.any(b));
  };
  var rat = Rational.any;

  Rational.prototype = {
    /**
     * @const
     * @property ktn.Rational#constructor
     * @type ktn.Rational
     */
    constructor: Rational,

    /**
     * @method ktn.Rational#clone
     * @return {ktn.Rational}
     */
    clone: function() {
      return new Rational(this._n, this._d, true);
    },

    /**
     * @method ktn.Rational#valueOf
     * @return {number}
     */
    valueOf: function() {
      return 1.0 * this._n.valueOf() / this._d.valueOf();
    },

    /**
     * @method ktn.Rational#toString
     * @return {string}
     */
    toString: function() {
      //if (this._d == 1) {return this._n.toString();}
      return '(' + [this._n, this._d].join('/') + ')';
    },
    
    /**
     * @method ktn.Rational#html
     * @return {string}
     */
    html: function() { return this.toString(); },

    /**
     * @method ktn.Rational#tex
     * @return {string}
     */
    tex: function() {
      //if (this._d == 1) {return this._n.toString();}
      return ['\\frac{', this._n, '}{', this._d, '}'].join('');
    },

    /**
     * @method ktn.Rational#abs
     * @return {ktn.Rational} |this|
     */
    abs: function() {
      return new Rational(this._n.abs(), this._d, true);
    },

    /**
     * @method ktn.Rational#neg
     * @return {ktn.Rational} -this
     */
    neg: function() {
      return new Rational(this._n.neg(), this._d, true);
    },

    /**
     * @method ktn.Rational#eq
     * @param {ktn.Rational} b
     * @return {boolean} this == b
     */
    eq: function(b) {
      b = rat(b);
      if (this._n.eq(b._n) && this._d.eq(b._d)) { return true; }
      return false;
    },

    /**
     * @method ktn.Rational#equal
     * @param {ktn.Rational} b
     * @return {boolean} this === b
     */
    equal: function(b) {
      if (!(b instanceof Rational)) { return false; }
      if (this._n.equal(b._n) && this._d.equal(b._d)) { return true; }
      return false;
    },

    /**
     * @method ktn.Rational#cmp
     * @param {ktn.Rational} b
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
     * @method ktn.Rational#inv
     * @return {ktn.Rational}
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
     * @method ktn.Rational#_co_
     * @return {number}
     */
    _co_: function() { return 2; },

    /**
     * @method ktn.Rational#add
     * @param {ktn.Rational} b
     * @return {ktn.Rational} this + b
     */
    add: function(b) {
      return new Rational(this._n.mul(b._d).add(this._d.mul(b._n)),
                          this._d.mul(b._d));
    },

    /**
     * @method ktn.Rational#sub
     * @param {ktn.Rational} b
     * @return {ktn.Rational} this - b
     */
    sub: function(b) {
      return new Rational(this._n.mul(b._d).sub(this._d.mul(b._n)),
                          this._d.mul(b._d));
    },

    /**
     * @method ktn.Rational#mul
     * @param {ktn.Rational} b
     * @return {ktn.Rational} this * b
     */
    mul: function(b) {
      return new Rational(this._n.mul(b._n), this._d.mul(b._d));
    },

    /**
     * @method ktn.Rational#div
     * @param {ktn.Rational} b
     * @return {ktn.Rational} this / b
     */
    div: function(b) {
      return new Rational(this._n.mul(b._d), this._d.mul(b._n));
    },

    /**
     * @method ktn.Rational#pow
     * @param {number} b
     * @return {ktn.Rational} this^b
     */
    pow: function(b) {
      return new Rational(this._n.pow(b), this._d.pow(b), true);
    },

    /**
     * @private
     * @method ktn.Rational#_add_
     * @param {object} a
     * @return {ktn.Rational}
     */
    _add_: function(a) { return this.add(rat(a)); },

    /**
     * @private
     * @method ktn.Rational#_sub_
     * @param {object} a
     * @return {ktn.Rational}
     */
    _sub_: function(a) { return this.sub(rat(a)); },

    /**
     * @private
     * @method ktn.Rational#_mul_
     * @param {object} a
     * @return {ktn.Rational}
     */
    _mul_: function(a) { return this.mul(rat(a)); },

    /**
     * @private
     * @method ktn.Rational#_div_
     * @param {object} a
     * @return {ktn.Rational}
     */
    _div_: function(a) { return this.div(rat(a)); }
  };

  // export
  if (typeof module !== 'undefined' && typeof module.exports !== 'undefined') {
    module.exports.Rational = Rational;
  } else {
    ktn.Rational = Rational;
  }
}(typeof ktn !== 'undefined' ? ktn : {}));