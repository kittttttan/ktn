export class Long {
  private static SHIFT: number = 15;
  private static BASE: number = 1 << Long.SHIFT;
  private static MASK: number = Long.BASE - 1;

	private _d: number[];
	private _s: bool;
	
	constructor() {
    this._d = [0];
    this._s = true;
	}
	
	// for ES3
	public getDigits() {
	  return this._d;
	}
	
	public getSign() {
	  return this._s;
	}
	
	// for ES5
	/* 
	public static get digits() {
	  return _d;
	}
	
	public static get sign() {
	  return _s;
	}
	*/
	
	public static num(n: number): Long {
    var a: Long = new Long();
    if (n < 0) {
      n = -n;
      a._s = false;
    }

    n &= 0x7fffffff;
    a._d[0] = n & Long.MASK;
    n >>>= Long.SHIFT;
    if (n) {
      a._d[1] = n & Long.MASK;
    }
    n >>>= Long.SHIFT;
    if (n) {
      a._d[2] = n & Long.MASK;
    }
    return a;
	}
	
	public static str(str: string, base?: number = 10): Long {
    //if (base === 16) { return longByte(str); }

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
    if (!str.charAt(index)) { return new Long(); }
    if (base === 8) {
      len = 3 * (str.length + 1 - index);
    } else {
      if (!str.charAt(index)) { --index; }
      len = (str.length + 1 - index) << 2;
    }
    len = (len >>> 4) + 1;

    var z: Long = alloc(len, sign);
    Long.fillZero(z, len);

    for (var c: string, n: number, zd: number[] = z._d, bl: number = 1;;) {
      c = str.charAt(index);
      ++index;
      if (!c) { break; }
      n = parseInt(c, 10);
      for (var i: number = 0;;) {
        for (; i < bl; ++i) {
          n += zd[i] * base;
          zd[i] = n & MASK;
          n >>>= Long.SHIFT;
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
	
	public static longint(a: any): Long {
    if (typeof a === 'object') {
      if (a instanceof Long) { return a.clone(); }
      return new Long();
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
    return new Long();
	}
	
	public static random(a: number): Long {
    var r: Long = alloc(a, true),
        rd: number[] = r._d;
    for (var i: number = 0; i < a; ++i) {
      rd[i] = Math.random() * BASE | 0;
    }
    return norm(r);
	}

	
	public static ZERO: Long = new Long();
	public static ONE: Long = Long.num(1);
	
	private static alloc(length: number, sign: bool): Long {
    var a: Long = new Long();
    a._s = sign ? true : false;
    a._d.length = length;
    return a;
	}
	
	private static fillZero(a: Long, b: number): Long {
    var d: number[] = a._d;
    while (b--) { d[b] = 0; }
    return a;
	}
	
	private static norm(a: Long): Long {
    var d: number[] = a._d,
        l:number = d.length;
    do { --l; } while (l && !d[l]);
    d.length = l + 1;
    // -0 -> +0
    if (!l && !d[l]) { a._s = true; }
    return a;
	}
	  	
	private static half(a: Long): Long {
    var d: number[] = a._d,
        l: number = d.length - 1;
    for (var i: number = 0; i < l; ++i) {
      d[i] = (((d[i + 1] & 1) << Long.SHIFT) + d[i]) >>> 1;
    }
    d[l] >>>= 1;
    return norm(a);
	}
	
	private static dbl(a: Long): Long {
    var d: number[] = a._d,
        l: number = d.length,
        c: number = 0;
    for (var i: number = 0, t: number = 0; i < l; ++i) {
      t = (d[i] << 1) + c;
      d[i] = t & MASK;
      c = t >>> Long.SHIFT;
    }
    if (c) { d[l] = c; }
    return norm(a);
	}
	
	private static longBitLength(a: Long): number {
    var ad: number[] = a._d;
    return ad[ad.length - 1].toString(2).length + ((ad.length - 1) << 4);
	}
	
	private static longK(x: Long, y: Long): Long {
    var N: number = longBitLength(x),
        l: number = longBitLength(y);
    if (N < l) { N = l; }
    if (N < 2001) { return x.mul(y); }

    // number of bits divided by 2, rounded up
    N = (N >>> 1) + (N & 1);

    // x = a + b 2^N, y = c + d 2^N
    var b: Long = x.rightShift(N),
        a: Long = x.sub(b.leftShift(N)),
        d: Long = y.rightShift(N),
        c: Long = y.sub(d.leftShift(N)),
        ac: Long = longK(a, c),
        bd: Long = longK(b, d),
        abcd: Long = longK(a.add(b), c.add(d));
    // xy
    // = (a + 2^N b) (c + 2^N d)
    // = ac + 2^N ((a + b) (c + d) - ac - bd) + 2^(N + 1) bd
    return ac.add(abcd.sub(ac).sub(bd).leftShift(N)).add(bd.leftShift(N << 1));
	}
	
	private static longToByte(a: Long, b?: number = 16): string {
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

	public clone(): Long {
    var b: Long = new Long();
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

    var t: Long = this.clone(),
        d: number[] = t._d,
        k: number = 0,
        n: number = 0,
        digits: string = '0123456789abcdef',
        s: string = '';
    while (i && j) {
      k = i;
      n = 0;
      while (k--) {
        n = (n << Long.SHIFT) | d[k];
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
    while (i--) { f = d[i] + Long.BASE * f; }
    if (!this._s) { f = -f; }
    return f;
	}
	
	public addzero(b: number): Long {
    var zeros: string = '',
        z: string = '0';
    for (; b > 0; b >>>= 1, z += z) {
      if (b & 1) { zeros += z; }
    }
    return Long.str(this.toString() + zeros);
	}

	public leftShift(b: number): Long {
    var a: Long = this.clone(),
        ad: number[] = a._d,
        l: number = ad.length,
        d: number = b >> 4,
        cl: number = l + d + 1,
        bb: number = b & 0xf,
        c: Long = Long.alloc(cl, a._s),
        cd: number[] = c._d,
        i: number = 0,
        carry: number = 0;
    for (; i < d; ++i) { cd[i] = 0; }
    i = 0;
    for (var t: number = 0; i < l; ++i) {
      t = (ad[i] << bb) + carry;
      cd[i + d] = t & Long.MASK;
      carry = t >> Long.SHIFT;
    }
    cd[i + d] = carry;
    return Long.norm(c);
	}

	public rightShift(b: number): Long {
    var a: Long = this.clone(),
        ad: number[] = a._d,
        l: number = ad.length,
        d: number = b >> 4,
        bb: number = b & 0xf,
        mask: number = (1 << bb) - 1;
    if (l <= d) { return new Long(); }

    var cl: number = l - d,
        c: Long = Long.alloc(cl, a._s),
        cd: number[] = c._d,
        i: number = 0;
    for (; i < cl - 1; ++i) {
      cd[i] = ((ad[i + d + 1] & mask) << (Long.SHIFT - bb)) + (ad[i + d] >> bb);
    }
    cd[i] = ad[i + d] >> bb;
    return Long.norm(c);
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

	public square(): Long {
    var a: Long = this.clone(),
        x: number[] = a._d,
        t: number = x.length,
        s: Long = Long.alloc(t << 1, true),
        w: number[] = s._d;
    Long.fillZero(s, w.length);

    for (var i: number = 0, j: number = 1,
            uv: number = 0, u: number = 0,
            v: number = 0, c: number = 0; i < t; ++i) {
      uv = w[i << 1] + x[i] * x[i];
      u = uv >>> Long.SHIFT;
      v = uv & Long.MASK;
      w[i << 1] = v;
      c = u;
      for (j = i + 1; j < t; ++j) {
        // uv = w[i + j] + (x[j] * x[i] << 1) + c
        // can overflow.
        uv = x[j] * x[i];
        u = (uv >>> Long.SHIFT) << 1;
        v = (uv & Long.MASK) << 1;
        v += w[i + j] + c;
        u += v >>> Long.SHIFT;
        v &= Long.MASK;
        w[i + j] = v;
        c = u;
      }
      w[i + t] = u;
    }

    return Long.norm(s);
	}
	
	public sqrt(): Long {
    var b: Long = this.clone(),
        c: Long = Long.num(1);
    while (b.cmp(c) > 0) {
      Long.half(b);
      Long.dbl(c);
    }
    do {
      b = c.clone();
      c = this.divmod(c, false).add(c);
      Long.half(c);
    } while (b.cmp(c) > 0);
    return b;
	}
	
	public pow(b: number): any {
    if (!b) { return Long.num(1); }
    if (b > 0 && b === (b | 0)) {
      var p: Long = Long.num(1),
          a: Long = this.clone();
      for (; b > 0; b >>= 1, a = a.square()) {
        if (b & 1) { p = p.mul(a); }
      }
      return p;
    }
    return Math.pow(this.valueOf(), b);
	}
	
	public gcd(b: Long): Long {
    if (this.cmpAbs(b) < 0) { return b.gcd(this); }

    var g: Long = Long.num(1),
        a: Long = this.clone();
    while (!(a._d[0] & 1) && !(b._d[0] & 1)) {
      a = Long.half(a);
      b = Long.half(b);
      g = Long.dbl(g);
    }
    
    while (a.isNonZero()) {
      while (!(a._d[0] & 1)) {
        a = Long.half(a);
      }
      while (!(b._d[0] & 1)) {
        b = Long.half(b);
      }
      if (a.cmpAbs(b) < 0) {
        b = Long.half(b.sub(a));
      } else {
        a = Long.half(a.sub(b));
      }
    }
    
    return g.mul(b);
	}
	
	public addAbs(b: Long, sign: bool): Long {
    if (this._d.length < b._d.length) {
      return b.addAbs(this, sign);
    }
    var ad: number[] = this._d,
        bd: number[] = b._d,
        al: number = ad.length,
        bl: number = bd.length,
        z: Long = Long.alloc(bl + 1, sign),
        zd: number[] = z._d,
        i: number = 0,
        num: number = 0;
    for (; i < bl; ++i) {
      num += ad[i] + bd[i];
      zd[i] = num & Long.MASK;
      num >>>= Long.SHIFT;
    }
    for (; num && i < al; ++i) {
      num += ad[i];
      zd[i] = num & Long.MASK;
      num >>>= Long.SHIFT;
    }
    for (; i < al; ++i) {
      zd[i] = ad[i];
    }
    zd[i] = num & Long.MASK;
    return Long.norm(z);
	}
	
	public subAbs(b: Long, sign: bool): Long {
    var ad: number[] = this._d,
        bd: number[] = b._d,
        al: number = ad.length,
        bl: number = bd.length,
        z: Long = Long.alloc(al, sign),
        zd: number[] = z._d,
        i: number = 0,
        c: number = 0;
    for (; i < bl; ++i) {
      c = ad[i] - bd[i] - c;
      if (c < 0) {
        zd[i] = c & Long.MASK;
        c = 1;
      } else {
        zd[i] = c;
        c = 0;
      }
    }
    for (; i < al; ++i) { 
      c = ad[i] - c;
      if (c < 0) {
        zd[i] = c & Long.MASK;
        c = 1;
      } else {
        zd[i] = c;
        c = 0;
      }
    }
    return Long.norm(z);
	}
	
	public add(b: Long): Long {
    if (this._s !== b._s) {
      if (this.cmpAbs(b) < 0) {
        return b.subAbs(this, b._s);
      }
      return this.subAbs(b, this._s);
    }
    return this.addAbs(b, this._s);
	}
	
	public sub(b: Long): Long {
    if (this._s === b._s) {
      if (this.cmpAbs(b) < 0) {
          return b.subAbs(this, !b._s);
      }
      return this.subAbs(b, this._s);
    }
    return this.addAbs(b, this._s);
	}

	public mul(b: Long): Long {
    // if (this.equal(b)) { return this.square(); }
    var ad: number[] = this._d,
        bd: number[] = b._d,
        al: number = ad.length,
        bl: number = bd.length;
    // if (al > 125 && bl > 125) { return longK(this, b); }
    var j: number = al + bl,
        z: Long = Long.alloc(j, this._s === b._s);
    Long.fillZero(z, j);
    for (var i: number = 0, n: number, d: number, e: number,
            zd: number[] = z._d; i < al; ++i) {
      d = ad[i]; 
      if (!d) { continue; }
      n = 0;
      for (j = 0; j < bl; ++j) {
        e = n + d * bd[j];
        n = zd[i + j] + e;
        if (e) { zd[i + j] = n & Long.MASK; }
        n >>>= Long.SHIFT;
      }
      if (n) { zd[i + j] = n; }
    }
    return Long.norm(z);
	}

	public divmod(b: Long, modulus: bool): Long {
    var a: Long = this.clone(),
        ad: number[] = a._d,
        bd: number[] = b._d,
        na: number = ad.length,
        nb: number = bd.length;
    if (nb < 2 && !bd[0]) {
      // zero division
      throw 'zero division';
      return new Long();
      //if (modulus || na < 2 && !ad[0]) { return NaN; }
      //if (a._s === b._s) { return Infinity; }
      //return -Infinity;
    }

    var albl: bool = na === nb;
    if (na < nb || (albl && ad[na - 1] < bd[nb - 1])) {
      if (modulus) { return a; }
      return new Long();
    }

    var dd: number = 0,
        z: Long,
        zd: number[],
        t: number = 0,
        i: number = 0;
    if (nb === 1) {
      dd = bd[0];
      z = a.clone();
      zd = z._d;
      i = na;
      while (i--) {
        t = (t << Long.SHIFT) | zd[i];
        zd[i] = (t / dd) & Long.MASK;
        t %= dd;
      }
      if (modulus) {
        if (!a._s) { return Long.num(-t); }
        return Long.num(t);
      }
      z._s = a._s === b._s;
      return Long.norm(z);
    }

    z = Long.alloc(albl ? na + 2 : na + 1, a._s === b._s);
    zd = z._d;
    Long.fillZero(z, zd.length);
    dd = Long.BASE / (bd[nb - 1] + 1) & Long.MASK;

    var j: number = 0,
        num: number = 0;
    if (dd === 1) {
      j = na;
      while (j--) { zd[j] = ad[j]; }
    } else {
      var bb: Long = b.clone(),
          td: number[] = bb._d;

      for (; j < nb; ++j) {
        num += bd[j] * dd;
        td[j] = num & Long.MASK;
        num >>>= Long.SHIFT;
      }

      bd = td;
      j = num = 0;

      for (; j < na; ++j) {
        num += ad[j] * dd;
        zd[j] = num & Long.MASK;
        num >>>= Long.SHIFT;
      }

      zd[j] = num & Long.MASK;
    }

    var q: number = 0,
        ee: number = 0;
    j = albl ? na + 1 : na;
    do {
      if (zd[j] === bd[nb - 1]) {
        q = Long.MASK;
      } else {
        q = ((zd[j] << Long.SHIFT) | zd[j - 1]) / bd[nb - 1] & Long.MASK;
      }

      if (q) {
        i = num = t = 0;
        do {
          t += bd[i] * q;
          ee = (t & Long.MASK) - num;
          num = zd[j - nb + i] - ee;
          if (ee) { zd[j - nb + i] = num & Long.MASK; }
          num >>= Long.SHIFT;
          t >>>= Long.SHIFT;
        } while (++i < nb);

        num += zd[j - nb + i] - t;
        while (num) {
          i = num = 0;
          --q;

          do {
            ee = num + bd[i];
            num = zd[j - nb + i] + ee;
            if (ee) { zd[j - nb + i] = num & Long.MASK; }
            num >>= Long.SHIFT;
          } while (++i < nb);

          --num;
        }
      }
      zd[j] = q;
    } while (--j >= nb);

    var div: Long = z.clone();
    zd = div._d;
    if (modulus) {
      if (dd) {
        t = 0;
        i = nb;
        while (i--) {
          t = (t << Long.SHIFT) | zd[i];
          zd[i] = (t / dd) & Long.MASK;
          t %= dd;
        }
      }
      zd.length = nb;
      div._s = a._s;
      return Long.norm(div);
    }

    j = (albl ? na + 2 : na + 1) - nb;
    for (i = 0; i < j; ++i) { zd[i] = zd[i + nb]; }
    zd.length = j;
    return Long.norm(div);
	}

	public div(b: Long): Long {
    return this.divmod(b, false);
	}

	public mod(b: Long): Long {
    return this.divmod(b, true);
	}

	public cmpAbs(b: Long): number {
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

	public cmp(b: Long): number {
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

	public eq(b: Long): bool {
    if (this === b) { return true; }
    
    b = Long.longint(b);
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

	public equal(b: Long): bool {
    if (this === b) { return true; }
    if (!(b instanceof Long)) { return false; }
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

	public abs(): Long {
    var z: Long = this.clone();
    z._s = true;
    return z;
	}

	public neg(): Long {
    var z: Long = this.clone();
    if (z.isNonZero()) { z._s = !z._s; }
    return z;
	}
}
