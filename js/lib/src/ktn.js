/**
 * @fileOverview Basic APIs for ktn.js
 * @author kittttttan
 */

(function() {
  if (typeof window === 'undefined') {
    // for browser only
    return;
  }

  if (typeof window.require !== 'undefined') {
  	throw new Error('RequireException: `require` already defined');
  }

  /**
   * @global
   * @method require
   * @param {string} module
   * @param {function} [callback]
   * @return {object} exports
   */
  var require = window.require = function(module, callback) {
  	var url = require.resolve(module);

  	if (require.cache[url]) {
  		callback && setTimeout(function(){ callback(require.cache[url]); }, 0);
  		return require.cache[url];
  	}
  	
  	var exports = {};
  	var request = new XMLHttpRequest();
  	request.onreadystatechange = function() {
  		if (this.readyState !== 4) {
  			return;
  		}
  		if (this.status !== 200) {
  			throw new Error('failed: GET '+url+' '+this.status+' ('+this.statusText+')');
  		}

  		if (require.cache[url]) {
  			exports = require.cache[url];
  		} else if (this.getResponseHeader('content-type')
  		               .indexOf('application/json') !== -1) { 
  			exports = JSON.parse(this.responseText);
  			require.cache[url] = exports;
  		} else {
  			require.cache[url] = exports;
  			var source = this.responseText.match(
  			    /^\s*(?:(['"]use strict['"])(?:;\r?\n?|\r?\n))?\s*((?:.*\r?\n?)*)/);//"
  			var evalCode = '(function(){'+ source[1] +
  			    ';var exports=window.require.cache[\''+ url +'\'];\n'+
  			    source[2] +'})();';
  			eval(evalCode);
  		}

  		callback && callback(require.cache[url]);
  	};
  	
  	request.open('GET', url, !!callback);
  	request.send();
  	
  	return exports;
  };

  require.resolve = function(module) {
  	var r = module.match(/^(\.{0,2}\/)?([^\.]*)(\..*)?$/);
  	return (r[1] || '/') + r[2] + (r[3] || '.js');
  };

  require.cache = {};
}());