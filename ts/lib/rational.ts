/// <reference path="./integer.ts"/>
module ktn {
  'use strict';

  export class Rational {
    private _n: Integer;
    private _d: Integer;
    
    constructor(n: Integer, d: Integer, irreduc: bool) {
      if (irreduc) {
        this._n = n;
        this._d = d;
      } else {
        if (n.isNonZero()) {
          var t: Integer[] = Rational.cancel(n, d);
          this._n = t[0];
          this._d = t[1];
        } else {
          this._n = Integer.zero();
          this._d = Integer.one();
        }
      }
    }
    
    public static zero(): Rational {
      return new Rational(Integer.zero(), Integer.one(), true);
    }
    
    public static one(): Rational {
      return new Rational(Integer.one(), Integer.one(), true);
    }

    public
    //get
    numer(): Integer {
      return this._n;
    }
    
    public
    //get
    denom(): Integer {
      return this._d;
    }
    
    public static cancel(a: Integer, b: Integer): Integer[] {
      var g: Integer = a.gcd(b);
      var n: Integer = a.div(g);
      var d: Integer = b.div(g);
      if (!d.sign()) {
        n.sign(!n.sign());
        d.sign(true);
      }
      return [n, d];
    }
    
    public static num(a?: number = 0, b?: number = 1): Rational {
      return new Rational(Integer.num(a), Integer.num(b), false);
    }
    
    public static str(s: string): Rational {
      if (!s) { return Rational.zero(); }
      var ss: string[] = s.split('/');
      return new Rational(Integer.str(ss[0]), Integer.str(ss[1] || '1'), false);
    }
    
    public clone(): Rational {
      return new Rational(this._n, this._d, true);
    }
    
    public toString(): string {
      return '('+ this._n +'/'+ this._d +')';
    }
    
    public valueOf(): number {
      return 1.0 * this._n.valueOf() / this._d.valueOf();
    }
    
    public abs(): Rational {
      return new Rational(this._n.abs(), this._d, true);
    }
    
    public neg(): Rational {
      return new Rational(this._n.neg(), this._d, true);
    }
  }
}