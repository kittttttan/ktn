module ktn {
  'use strict';

  export class Integer {
    private static SHIFT: number = 15;
    private static BASE: number = 1 << Integer.SHIFT;
    private static MASK: number = Integer.BASE - 1;

    private _d: number[];
    private _s: bool;
    
    constructor() {
      this._d = [0];
      this._s = true;
    }
    
    public digits(): number[] {
      return this._d;
    }
    public sign(): bool;
    public sign(s: bool): void;
    public sign(s?: bool) {
      if (!arguments.length) { return this._s; }
      this._s = s;
    }
    // for ES5
    /*
    public get digits(): number[] {
      return this._d;
    }
    
    public get sign(): bool {
      return this._s;
    }
    
    public set sign(s: bool): void {
      this._s = s;
    }
    */
    
    public static zero(): Integer {
      return new Integer();
    }
    
    public static one(): Integer {
      return Integer.num(1);
    }
    
    public static num(n: number): Integer {
      var a: Integer = new Integer();
      if (n < 0) {
        n = -n;
        a._s = false;
      }

      n &= 0x7fffffff;
      a._d[0] = n & Integer.MASK;
      n >>>= Integer.SHIFT;
      if (n) {
        a._d[1] = n & Integer.MASK;
      }
      n >>>= Integer.SHIFT;
      if (n) {
        a._d[2] = n & Integer.MASK;
      }
      return a;
    }
    
    public static str(str: string, base?: number = 10): Integer {
      var index: number = 0,
          sign: bool = true,
          len: number;
      if (str.charAt(index) === '+') {
        ++index;
      } else if (str.charAt(index) === '-') {
        sign = false;
        ++index;
      }
      // Ignore following zeros. '00102' is regarded as '102'.
      while (str.charAt(index) === '0') { ++index; }
      if (!str.charAt(index)) { return new Integer(); }
      if (base === 8) {
        len = 3 * (str.length + 1 - index);
      } else {
        if (!str.charAt(index)) { --index; }
        len = (str.length + 1 - index) << 2;
      }
      len = (len >>> 4) + 1;

      var z: Integer = alloc(len, sign);
      Integer.fillZero(z, len);

      for (var c: string, n: number, zd: number[] = z._d, bl: number = 1;;) {
        c = str.charAt(index);
        ++index;
        if (!c) { break; }
        n = parseInt(c, base);
        for (var i: number = 0;;) {
          for (; i < bl; ++i) {
            n += zd[i] * base;
            zd[i] = n & MASK;
            n >>>= Integer.SHIFT;
          }
          if (n) {
            ++bl;
          } else {
            break;
          }
        }
      }
      return norm(z);
    }
    
    public static any(a: any): Integer {
      if (typeof a === 'object') {
        if (a instanceof Integer) { return a.clone(); }
        return new Integer();
      }
      if (typeof a === 'string') {
        return str(a);
      }
      if (typeof a === 'number') {
        if (-0x7fffffff <= a && a <= 0x7fffffff) {
          return num(a);
        }
        a += '';

        var i: number = a.indexOf('e', 0);
        if (i < 0) {
          // 'e' is not found
          return str(a);
        }

        var a1: string = a.substr(0, i),
            a2: number = parseInt(a.substr(i + 2, a.length - (i + 2)), 10),
            fpt: number = a1.indexOf('.', 0);
        if (fpt >= 0) {
          // '.' is not found
          var np: number = a1.length - (fpt + 1);
          a1 = a1.substr(0, fpt) + a1.substr(fpt + 1, np);
          a2 -= np;
        }
        for (; a2 > 0; --a2) { a1 += '0'; }
        return str(a1);
      }
      return new Integer();
    }
    
    public static random(a: number): Integer {
      var r: Integer = alloc(a, true),
          rd: number[] = r._d;
      for (var i: number = 0; i < a; ++i) {
        rd[i] = Math.random() * BASE | 0;
      }
      return norm(r);
    }
    
    private static alloc(length: number, sign: bool): Integer {
      var a: Integer = new Integer();
      a._s = sign ? true : false;
      a._d.length = length;
      return a;
    }
    
    private static fillZero(a: Integer, b: number): Integer {
      var d: number[] = a._d;
      while (b--) { d[b] = 0; }
      return a;
    }
    
    private static norm(a: Integer): Integer {
      var d: number[] = a._d,
          l:number = d.length;
      do { --l; } while (l && !d[l]);
      d.length = l + 1;
      // -0 -> +0
      if (!l && !d[l]) { a._s = true; }
      return a;
    }
        
    private static half(a: Integer): Integer {
      var d: number[] = a._d,
          l: number = d.length - 1;
      for (var i: number = 0; i < l; ++i) {
        d[i] = (((d[i + 1] & 1) << Integer.SHIFT) + d[i]) >>> 1;
      }
      d[l] >>>= 1;
      return norm(a);
    }
    
    private static dbl(a: Integer): Integer {
      var d: number[] = a._d,
          l: number = d.length,
          c: number = 0;
      for (var i: number = 0, t: number = 0; i < l; ++i) {
        t = (d[i] << 1) + c;
        d[i] = t & MASK;
        c = t >>> Integer.SHIFT;
      }
      if (c) { d[l] = c; }
      return norm(a);
    }
    
    private static IntegerBitLength(a: Integer): number {
      var ad: number[] = a._d;
      return ad[ad.length - 1].toString(2).length + ((ad.length - 1) << 4);
    }
    
    private static IntegerK(x: Integer, y: Integer): Integer {
      var N: number = IntegerBitLength(x),
          l: number = IntegerBitLength(y);
      if (N < l) { N = l; }
      if (N < 2001) { return x.mul(y); }

      // number of bits divided by 2, rounded up
      N = (N >>> 1) + (N & 1);

      // x = a + b 2^N, y = c + d 2^N
      var b: Integer = x.rightShift(N),
          a: Integer = x.sub(b.leftShift(N)),
          d: Integer = y.rightShift(N),
          c: Integer = y.sub(d.leftShift(N)),
          ac: Integer = IntegerK(a, c),
          bd: Integer = IntegerK(b, d),
          abcd: Integer = IntegerK(a.add(b), c.add(d));
      // xy
      // = (a + 2^N b) (c + 2^N d)
      // = ac + 2^N ((a + b) (c + d) - ac - bd) + 2^(N + 1) bd
      return ac.add(abcd.sub(ac).sub(bd).leftShift(N)).add(bd.leftShift(N << 1));
    }
    
    private static IntegerToByte(a: Integer, b?: number = 16): string {
      var d: number[] = a._d,
          i: number = d.length - 1,
          s: string = a._s ? '' : '-',
          z: string,
          l: number = 0;
      if (b === 2) {
        z = '0000000000000000';
        l = -16;
      } else {
        b = 16;
        z = '0000';
        l = -4;
      }
      s += d[i].toString(b);
      while (i--) {
        s += (z + d[i].toString(b)).slice(l);
      }
      return s;
    }

    public clone(): Integer {
      var b: Integer = new Integer();
      b._s = this._s;
      b._d = Array.prototype.concat.call(this._d);
      return b;
    }
    
    public toString(b?: number = 10): string {
      var i: number = this._d.length;
      if (i < 2 && !this._d[0]) { return '0'; }

      var j: number,
          hbase: number;
      switch (b) {
      case 16:
        j = (i << 3) + 2;
        hbase = 0x10000;
        break;
      case 8:
        j = (i << 4) + 2;
        hbase = 0x1000;
        break;
      case 2:
        j = (i << 4) + 2;
        hbase = 0x10;
        break;
      case 10: default:
        j = (i * 241 / 50 | 0) + 2;
        hbase = 10000;
        break;
      }

      var t: Integer = this.clone(),
          d: number[] = t._d,
          k: number = 0,
          n: number = 0,
          digits: string = '0123456789abcdef',
          s: string = '';
      while (i && j) {
        k = i;
        n = 0;
        while (k--) {
          n = (n << Integer.SHIFT) | d[k];
          d[k] = n / hbase | 0;
          n %= hbase;
        }
        if (!d[i - 1]) { --i; }
        k = 4;
        while (k--) {
          s = digits.charAt(n % b) + s;
          --j;
          n = n / b | 0;
          if (!i && !n) { break; }
        }
      }
      s = s.replace(/^0+/, '');
      if (!this._s) { s = '-' + s; }
      return s;      
    }
    
    public valueOf(): number {
      var f: number = 0.0,
          d: number[] = this._d,
          i: number = d.length;
      while (i--) { f = d[i] + Integer.BASE * f; }
      if (!this._s) { f = -f; }
      return f;
    }
    
    public addzero(b: number): Integer {
      var zeros: string = '',
          z: string = '0';
      for (; b > 0; b >>>= 1, z += z) {
        if (b & 1) { zeros += z; }
      }
      return Integer.str(this.toString() + zeros);
    }

    public leftShift(b: number): Integer {
      var a: Integer = this.clone(),
          ad: number[] = a._d,
          l: number = ad.length,
          d: number = b >> 4,
          cl: number = l + d + 1,
          bb: number = b & 0xf,
          c: Integer = Integer.alloc(cl, a._s),
          cd: number[] = c._d,
          i: number = 0,
          carry: number = 0;
      for (; i < d; ++i) { cd[i] = 0; }
      i = 0;
      for (var t: number = 0; i < l; ++i) {
        t = (ad[i] << bb) + carry;
        cd[i + d] = t & Integer.MASK;
        carry = t >> Integer.SHIFT;
      }
      cd[i + d] = carry;
      return Integer.norm(c);
    }

    public rightShift(b: number): Integer {
      var a: Integer = this.clone(),
          ad: number[] = a._d,
          l: number = ad.length,
          d: number = b >> 4,
          bb: number = b & 0xf,
          mask: number = (1 << bb) - 1;
      if (l <= d) { return new Integer(); }

      var cl: number = l - d,
          c: Integer = Integer.alloc(cl, a._s),
          cd: number[] = c._d,
          i: number = 0;
      for (; i < cl - 1; ++i) {
        cd[i] = ((ad[i + d + 1] & mask) << (Integer.SHIFT - bb)) + (ad[i + d] >> bb);
      }
      cd[i] = ad[i + d] >> bb;
      return Integer.norm(c);
    }

    public isOdd(): bool {
      return !!(this._d[0] & 1);
    }

    public isEven(): bool {
      return !(this._d[0] & 1);
    }

    public isNonZero(): bool {
      return (this._d.length > 1 || this._d[0]);
    }

    public square(): Integer {
      var a: Integer = this.clone(),
          x: number[] = a._d,
          t: number = x.length,
          s: Integer = Integer.alloc(t << 1, true),
          w: number[] = s._d;
      Integer.fillZero(s, w.length);

      for (var i: number = 0, j: number = 1,
              uv: number = 0, u: number = 0,
              v: number = 0, c: number = 0; i < t; ++i) {
        uv = w[i << 1] + x[i] * x[i];
        u = uv >>> Integer.SHIFT;
        v = uv & Integer.MASK;
        w[i << 1] = v;
        c = u;
        for (j = i + 1; j < t; ++j) {
          // uv = w[i + j] + (x[j] * x[i] << 1) + c
          // can overflow.
          uv = x[j] * x[i];
          u = (uv >>> Integer.SHIFT) << 1;
          v = (uv & Integer.MASK) << 1;
          v += w[i + j] + c;
          u += v >>> Integer.SHIFT;
          v &= Integer.MASK;
          w[i + j] = v;
          c = u;
        }
        w[i + t] = u;
      }

      return Integer.norm(s);
    }
    
    public sqrt(): Integer {
      var b: Integer = this.clone(),
          c: Integer = Integer.num(1);
      while (b.cmp(c) > 0) {
        Integer.half(b);
        Integer.dbl(c);
      }
      do {
        b = c.clone();
        c = this.divmod(c, false).add(c);
        Integer.half(c);
      } while (b.cmp(c) > 0);
      return b;
    }
    
    public pow(b: number): any {
      if (!b) { return Integer.num(1); }
      if (b > 0 && b === (b | 0)) {
        var p: Integer = Integer.num(1),
            a: Integer = this.clone();
        for (; b > 0; b >>= 1, a = a.square()) {
          if (b & 1) { p = p.mul(a); }
        }
        return p;
      }
      return Math.pow(this.valueOf(), b);
    }
    
    public gcd(b: Integer): Integer {
      if (this.cmpAbs(b) < 0) { return b.gcd(this); }

      var g: Integer = Integer.num(1),
          a: Integer = this.abs();
      b = b.abs();
      while (!(a._d[0] & 1) && !(b._d[0] & 1)) {
        Integer.half(a);
        Integer.half(b);
        Integer.dbl(g);
      }
      
      while (a.isNonZero()) {
        while (!(a._d[0] & 1)) {
          Integer.half(a);
        }
        while (!(b._d[0] & 1)) {
          Integer.half(b);
        }
        if (a.cmpAbs(b) < 0) {
          b = b.sub(a);
          Integer.half(b);
        } else {
          a = a.sub(b);
          Integer.half(a);
        }
      }
      
      return g.mul(b);
    }
    
    public addAbs(b: Integer, sign: bool): Integer {
      if (this._d.length < b._d.length) {
        return b.addAbs(this, sign);
      }
      var ad: number[] = this._d,
          bd: number[] = b._d,
          al: number = ad.length,
          bl: number = bd.length,
          z: Integer = Integer.alloc(bl + 1, sign),
          zd: number[] = z._d,
          i: number = 0,
          num: number = 0;
      for (; i < bl; ++i) {
        num += ad[i] + bd[i];
        zd[i] = num & Integer.MASK;
        num >>>= Integer.SHIFT;
      }
      for (; num && i < al; ++i) {
        num += ad[i];
        zd[i] = num & Integer.MASK;
        num >>>= Integer.SHIFT;
      }
      for (; i < al; ++i) {
        zd[i] = ad[i];
      }
      zd[i] = num & Integer.MASK;
      return Integer.norm(z);
    }
    
    public subAbs(b: Integer, sign: bool): Integer {
      var ad: number[] = this._d,
          bd: number[] = b._d,
          al: number = ad.length,
          bl: number = bd.length,
          z: Integer = Integer.alloc(al, sign),
          zd: number[] = z._d,
          i: number = 0,
          c: number = 0;
      for (; i < bl; ++i) {
        c = ad[i] - bd[i] - c;
        if (c < 0) {
          zd[i] = c & Integer.MASK;
          c = 1;
        } else {
          zd[i] = c;
          c = 0;
        }
      }
      for (; i < al; ++i) { 
        c = ad[i] - c;
        if (c < 0) {
          zd[i] = c & Integer.MASK;
          c = 1;
        } else {
          zd[i] = c;
          c = 0;
        }
      }
      return Integer.norm(z);
    }
    
    public add(b: Integer): Integer {
      if (this._s !== b._s) {
        if (this.cmpAbs(b) < 0) {
          return b.subAbs(this, b._s);
        }
        return this.subAbs(b, this._s);
      }
      return this.addAbs(b, this._s);
    }
    
    public sub(b: Integer): Integer {
      if (this._s === b._s) {
        if (this.cmpAbs(b) < 0) {
            return b.subAbs(this, !b._s);
        }
        return this.subAbs(b, this._s);
      }
      return this.addAbs(b, this._s);
    }

    public mul(b: Integer): Integer {
      // if (this.equal(b)) { return this.square(); }
      var ad: number[] = this._d,
          bd: number[] = b._d,
          al: number = ad.length,
          bl: number = bd.length;
      // if (al > 125 && bl > 125) { return IntegerK(this, b); }
      var j: number = al + bl,
          z: Integer = Integer.alloc(j, this._s === b._s);
      Integer.fillZero(z, j);
      for (var i: number = 0, n: number, d: number, e: number,
              zd: number[] = z._d; i < al; ++i) {
        d = ad[i]; 
        if (!d) { continue; }
        n = 0;
        for (j = 0; j < bl; ++j) {
          e = n + d * bd[j];
          n = zd[i + j] + e;
          if (e) { zd[i + j] = n & Integer.MASK; }
          n >>>= Integer.SHIFT;
        }
        if (n) { zd[i + j] = n; }
      }
      return Integer.norm(z);
    }

    public divmod(b: Integer, modulus: bool): Integer {
      var a: Integer = this.clone(),
          ad: number[] = a._d,
          bd: number[] = b._d,
          na: number = ad.length,
          nb: number = bd.length;
      if (nb < 2 && !bd[0]) {
        throw new Error('zero division');
        //if (modulus || na < 2 && !ad[0]) { return NaN; }
        //if (a._s === b._s) { return Infinity; }
        //return -Infinity;
      }

      var albl: bool = na === nb;
      if (na < nb || (albl && ad[na - 1] < bd[nb - 1])) {
        if (modulus) { return a; }
        return new Integer();
      }

      var dd: number = 0,
          z: Integer,
          zd: number[],
          t: number = 0,
          i: number = 0;
      if (nb === 1) {
        dd = bd[0];
        z = a.clone();
        zd = z._d;
        i = na;
        while (i--) {
          t = (t << Integer.SHIFT) | zd[i];
          zd[i] = (t / dd) & Integer.MASK;
          t %= dd;
        }
        if (modulus) {
          if (!a._s) { return Integer.num(-t); }
          return Integer.num(t);
        }
        z._s = a._s === b._s;
        return Integer.norm(z);
      }

      z = Integer.alloc(albl ? na + 2 : na + 1, a._s === b._s);
      zd = z._d;
      Integer.fillZero(z, zd.length);
      dd = Integer.BASE / (bd[nb - 1] + 1) & Integer.MASK;

      var j: number = 0,
          num: number = 0;
      if (dd === 1) {
        j = na;
        while (j--) { zd[j] = ad[j]; }
      } else {
        var bb: Integer = b.clone(),
            td: number[] = bb._d;

        for (; j < nb; ++j) {
          num += bd[j] * dd;
          td[j] = num & Integer.MASK;
          num >>>= Integer.SHIFT;
        }

        bd = td;
        j = num = 0;

        for (; j < na; ++j) {
          num += ad[j] * dd;
          zd[j] = num & Integer.MASK;
          num >>>= Integer.SHIFT;
        }

        zd[j] = num & Integer.MASK;
      }

      var q: number = 0,
          ee: number = 0;
      j = albl ? na + 1 : na;
      do {
        if (zd[j] === bd[nb - 1]) {
          q = Integer.MASK;
        } else {
          q = ((zd[j] << Integer.SHIFT) | zd[j - 1]) / bd[nb - 1] & Integer.MASK;
        }

        if (q) {
          i = num = t = 0;
          do {
            t += bd[i] * q;
            ee = (t & Integer.MASK) - num;
            num = zd[j - nb + i] - ee;
            if (ee) { zd[j - nb + i] = num & Integer.MASK; }
            num >>= Integer.SHIFT;
            t >>>= Integer.SHIFT;
          } while (++i < nb);

          num += zd[j - nb + i] - t;
          while (num) {
            i = num = 0;
            --q;

            do {
              ee = num + bd[i];
              num = zd[j - nb + i] + ee;
              if (ee) { zd[j - nb + i] = num & Integer.MASK; }
              num >>= Integer.SHIFT;
            } while (++i < nb);

            --num;
          }
        }
        zd[j] = q;
      } while (--j >= nb);

      var div: Integer = z.clone();
      zd = div._d;
      if (modulus) {
        if (dd) {
          t = 0;
          i = nb;
          while (i--) {
            t = (t << Integer.SHIFT) | zd[i];
            zd[i] = (t / dd) & Integer.MASK;
            t %= dd;
          }
        }
        zd.length = nb;
        div._s = a._s;
        return Integer.norm(div);
      }

      j = (albl ? na + 2 : na + 1) - nb;
      for (i = 0; i < j; ++i) { zd[i] = zd[i + nb]; }
      zd.length = j;
      return Integer.norm(div);
    }

    public div(b: Integer): Integer {
      return this.divmod(b, false);
    }

    public mod(b: Integer): Integer {
      return this.divmod(b, true);
    }

    public cmpAbs(b: Integer): number {
      if (this === b) { return 0; }
      
      var ad: number[] = this._d,
          bd: number[] = b._d,
          al: number = ad.length;
      if (al < bd.length) { return -1; }
      if (al > bd.length) { return 1; }
      
      do { --al; } while (al && ad[al] === bd[al]);
      if (!al && ad[0] === bd[0]) { return 0; }
      
      return ad[al] > bd[al] ? 1 : -1;
    }

    public cmp(b: Integer): number {
      if (this === b) { return 0; }
      
      var ad: number[] = this._d,
          bd: number[] = b._d,
          al: number = ad.length;
      if (this._s !== b._s) { return this._s ? 1 : -1; }
      if (al < bd.length) { return this._s ? -1 : 1; }
      if (al > bd.length) { return this._s ? 1 : -1; }
      
      do { --al; } while (al && ad[al] === bd[al]);
      if (!al && ad[0] === bd[0]) {
        return (this._s ? 1 : 0) - (b._s ? 1 : 0);
      }
      
      if (ad[al] > bd[al]) { return this._s ? 1 : -1; }
      return this._s ? -1 : 1;
    }

    public eq(b: Integer): bool {
      if (this === b) { return true; }
      
      b = Integer.any(b);
      if (this._s !== b._s) { return false; }
      
      var ad: number[] = this._d,
          bd: number[] = b._d,
          l: number = ad.length;
      if (l !== bd.length) { return false; }
      
      for (var i: number = 0; i < l; ++i) {
        if (ad[i] !== bd[i]) { return false; }
      }
      
      return true;
    }

    public equal(b: Integer): bool {
      if (this === b) { return true; }
      if (!(b instanceof Integer)) { return false; }
      if (this._s !== b._s) { return false; }
      
      var ad: number[] = this._d,
          bd: number[] = b._d,
          l: number = ad.length;
      if (l !== bd.length) { return false; }
      
      for (var i: number = 0; i < l; ++i) {
        if (ad[i] !== bd[i]) { return false; }
      }
      
      return true;
    }

    public abs(): Integer {
      var z: Integer = this.clone();
      z._s = true;
      return z;
    }

    public neg(): Integer {
      var z: Integer = this.clone();
      if (z.isNonZero()) { z._s = !z._s; }
      return z;
    }
  }
}