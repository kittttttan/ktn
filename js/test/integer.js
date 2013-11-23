describe("Integer", function() {

  it("init", function() {
    expect(Integer.zero().toString()).toEqual(Integer.str("0").toString());
    expect(Integer.num(0).toString()).toEqual(Integer.str("0").toString());
    expect(Integer.str("0").eq(Integer.zero())).toBe(true);
    
    expect(Integer.num(1).toString()).toEqual(Integer.str("1").toString());
    expect(Integer.one().toString()).toEqual(Integer.str("1").toString());

    expect(Integer.num(123456789).toString()).toEqual(Integer.str("123456789").toString());
    expect(Integer.num(2147483647).toString()).toEqual(Integer.str("2147483647").toString());
    expect(Integer.num(1234567890).toString()).toEqual(Integer.str("1234567890").toString());
    expect(Integer.any(12345678900).toString()).toEqual(Integer.str("12345678900").toString());
    expect(Integer.num(0x7fffffff).toString()).toEqual(Integer.str("7fffffff", 16).toString());
    expect(Integer.any(-0x7fffffff).toString()).toEqual(Integer.str("-7fffffff", 16).toString());
    
    expect(Integer.str("111", 2).toString()).toEqual("7");
    expect(Integer.str("1234567", 8).toString()).toEqual("342391");
    expect(Integer.str("ffffffff", 16).toString()).toEqual("4294967295");

    expect(Integer.any(1234567).toString()).toEqual("1234567");
    expect(Integer.any(3.14).toString()).toEqual("3");
    expect(Integer.any("7654321").toString()).toEqual("7654321");
    expect(Integer.any(1e7).toString()).toEqual("10000000");
    expect(Integer.any(1.23e5).toString()).toEqual("123000");
    expect(Integer.any(1.23456e2).toString()).toEqual("123");
  });

  it("basic", function() {
    var a = Integer.str("1234567890");
    var b = Integer.num(10).pow(7);

    expect(a.toString()).toEqual("1234567890");
    expect(a.clone().toString()).toEqual("1234567890");
    expect(a.neg().toString()).toEqual("-1234567890");

    expect(b.toString()).toEqual("10000000");

    expect(a.add(b).toString()).toEqual("1244567890");
    expect(a.sub(b).toString()).toEqual("1224567890");
    expect(a.mul(b).toString()).toEqual("12345678900000000");
    expect(a.div(b).toString()).toEqual("123");
    expect(a.mod(b).toString()).toEqual("4567890");
    expect(a.square().toString()).toEqual("1524157875019052100");
    
    expect(a.sqrt().toString()).toEqual("35136");
    expect(a.gcd(b).toString()).toEqual("10");
  });

  it("add", function() {
    var a = Integer.num(1e7);
    
    expect(a.add(Integer.zero()).toString()).toEqual("10000000");
    expect(a.add(Integer.one()).toString()).toEqual("10000001");
    expect(a.add(a).toString()).toEqual("20000000");
  });

  it("sub", function() {
    var a = Integer.num(1e7);
    
    expect(a.sub(Integer.zero()).toString()).toEqual("10000000");
    expect(a.sub(Integer.one()).toString()).toEqual("9999999");
    expect(a.sub(a).toString()).toEqual("0"); 
    expect(Integer.one().sub(a).toString()).toEqual("-9999999");
    expect(Integer.zero().sub(a).toString()).toEqual("-10000000");
  });

  it("mul", function() {
    var a = Integer.num(1e7);
    
    expect(a.mul(Integer.zero()).toString()).toEqual("0");
    expect(a.mul(Integer.one()).toString()).toEqual("10000000");
    expect(a.mul(a).toString()).toEqual("100000000000000"); 
    expect(Integer.one().neg().mul(a).toString()).toEqual("-10000000");
  });

  it("div", function() {
    var a = Integer.num(1e7);
    
    expect(function(){ a.div(Integer.zero()); }).toThrow();
    expect(a.div(Integer.one()).toString()).toEqual("10000000");
    expect(a.div(a).toString()).toEqual("1"); 
    expect(Integer.one().neg().div(a).toString()).toEqual("0");
  });

  it("mod", function() {
    var a = Integer.num(1e7);
    
    expect(function(){ a.mod(Integer.zero()); }).toThrow();
    expect(a.mod(Integer.one()).toString()).toEqual("0");
    expect(a.mod(a).toString()).toEqual("0"); 
    expect(Integer.one().neg().mod(a).toString()).toEqual("-1");
  });

  it("shift", function() {
    var one = Integer.one();
    var two = Integer.num(2);
    var ls7 = one.leftShift(7);
    var ls17 = one.leftShift(17);
    var ls27 = one.leftShift(27);
    var ls37 = one.leftShift(37);
    var ls47 = one.leftShift(47);

    expect(ls7.toString()).toEqual("128");
    expect(ls17.toString()).toEqual("131072");
    expect(ls27.toString()).toEqual("134217728");
    expect(ls37.toString()).toEqual("137438953472");
    expect(ls47.toString()).toEqual("140737488355328");

    expect(two.pow(7).toString()).toEqual("128");
    expect(two.pow(17).toString()).toEqual("131072");
    expect(two.pow(27).toString()).toEqual("134217728");
    expect(two.pow(37).toString()).toEqual("137438953472");
    expect(two.pow(47).toString()).toEqual("140737488355328");

    expect(ls7.rightShift(7).toString()).toEqual("1");
    expect(ls17.rightShift(17).toString()).toEqual("1");
    expect(ls27.rightShift(27).toString()).toEqual("1");
    expect(ls37.rightShift(37).toString()).toEqual("1");
    expect(ls47.rightShift(47).toString()).toEqual("1");
  });

  it("factorial", function () {
    expect(Integer.factorial(0).toString()).toEqual("1");
    expect(Integer.factorial(1).toString()).toEqual("1");
    expect(Integer.factorial(2).toString()).toEqual("2");
    expect(Integer.factorial(3).toString()).toEqual("6");
    expect(Integer.factorial(10).toString()).toEqual("3628800");
    expect(Integer.factorial(17).toString()).toEqual("355687428096000");
  });

  var fib = function(a) {
    var b = Integer.zero();
    for (var i = 0, c = Integer.one(), d; i < a; ++i) {
      d = b.clone();
      b = b.add(c);
      c = d;
    }
    return b;
  };

  it("fib", function(){
    expect(fib(0).toString()).toEqual("0");
    expect(fib(1).toString()).toEqual("1");
    expect(fib(2).toString()).toEqual("1");
    expect(fib(3).toString()).toEqual("2");
    expect(fib(4).toString()).toEqual("3");
    expect(fib(38).toString()).toEqual("39088169");
  });

  var pi = function(a) {
    if (!a) { a = 1; }
    var n = Integer.num(10).pow(a);

    function arccot(m) {
      var c = n, a = c.div(m), b = a.clone(), m2 = m.square(),
          k = Integer.num(1), s = true, l2 = Integer.num(2);
      while (c.isNonZero()) {
        b = b.div(m2);
        k = k.add(l2);
        c = b.div(k);
        s = !s;
        if (s) {
          a = a.add(c);
        } else {
          a = a.sub(c);
        }
      }
      return a;
    }

    var a5 = arccot(Integer.num(5));
    var a239 = arccot(Integer.num(239));
    return a5.leftShift(2).sub(a239).leftShift(2);
  };

  it("pi", function(){
    var p = "314159265358979323846264338327950288419716939937510582097494459230781640628620948";
    expect(pi(80).toString()).toEqual(p);
  });
});

/*
(function() {
  var jasmineEnv = jasmine.getEnv();
  jasmineEnv.updateInterval = 250;

  var htmlReporter = new jasmine.HtmlReporter();
  jasmineEnv.addReporter(htmlReporter);

  jasmineEnv.specFilter = function(spec) {
    return htmlReporter.specFilter(spec);
  };
  var currentWindowOnload = window.onload;
  window.onload = function() {
    if (currentWindowOnload) {
      currentWindowOnload();
    }

    document.querySelector('.version').innerHTML = jasmineEnv.versionString();
    execJasmine();
  };

  function execJasmine() {
    jasmineEnv.execute();
  }
})();
*/