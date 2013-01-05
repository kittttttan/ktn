/**
 * @fileOverview Tiny HTTP Server for node
 * @author kittttttan
 * @example
 *     node server.js localhost 1337
 */

var http = require('http'),
    fs = require('fs'),
    qs = require('querystring'),
    url = require('url'),

    ROOT = __dirname,
    MIME = {
      '.bmp': 'image/bmp',
      '.css': 'text/css',
      '.gif': 'image/gif',
      '.gz': 'application/gzip',
      '.htm': 'text/html',
      '.html': 'text/html',
      '.ico': 'image/vnd.microsoft.icon',
      '.jpg': 'image/jpeg',
      '.js': 'application/javascript',
      '.json': 'application/json',
      '.mid': 'audio/midi',
      '.mp3': 'audio/mp3',
      '.mpg': 'video/mpeg',
      '.pdf': 'application/pdf',
      '.png': 'image/png',
      '.txt': 'text/plain',
      '.wav': 'audio/wav',
      '.xml': 'application/xml',
      '.zip': 'application/zip'
    },
    DEFAULT_PATH = 'index.html',
    DEFAULT_EXT = '.html';

/**
 * @param {string} s
 * @param {number} zeros
 * @return {string}
 */
function padZero(s, zeros) {
  var z = '00';
  switch (zeros) {
  case 3: z += '0'; break;
  case 4: z += '00'; break;
  case 5: z += '000'; break;
  default: zeros = 2; break;
  }
  
  return (z + (s || '')).slice(-zeros);
}

/**
 * @param {Date} d
 * @return {string} yyyy-MM-dd
 */
function formatDate(d) {
  var year = d.getFullYear();
  var month = d.getMonth() + 1;
  var date = d.getDate();
  var hour = d.getHours();
  var minute = d.getMinutes();
  var sec = d.getSeconds();
  var msec = d.getMilliseconds();
  
  return year +'-'+ padZero(month, 2) +'-'+ padZero(date, 2) +
      ' '+ padZero(hour, 2) +':'+ padZero(minute, 2) +':'+ padZero(sec, 2) +
      '.'+ padZero(msec, 3);
}

/**
 * @param {string} str
 * @throws
 */
function logging(str) {
  str = formatDate(new Date()) + ' ' + str;
  console.log(str);
  fs.appendFile('node.log', str + '\n', function(err) {
    if (err) { throw err; }
  });
}

/**
 * @param {string} a
 * @return {string}
 */
function escapeHTML(a) {
  return a.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;')
          .replace(/"/g, '&quot;').replace(/'/g, '&apos;');//"
}

/**
 * generate index
 * @param {string} path
 * @return {string} file list in HTML
 */
function genIndex(path) {
  var files = fs.readdirSync(path);
  var fileLength = files.length;
  var stat;
  var fname;
  var body = '<!DOCTYPE html><html><head><meta charset="UTF-8">'+
         '<title>'+ path +'</title>'+
         '</head><body><h1>Index</h1><ul>';
  for (var i = 0; i < fileLength; ++i) {
    fname = files[i];
    stat = fs.statSync(path + fname);
    if (stat.isDirectory()) { fname += '/'; }
    body += '<li><a href=\"'+ fname +'\">'+ fname +'</a>';
  }
  body += '</ul></body></html>';
  
  return body;
}

/**
 * @param req
 * @param res
 * @param query
 */
function proc(req, res, query) {
  var statusCode = 500,
      body = '',
      reqUrl = url.parse(req.url),
      pathname = reqUrl.pathname;
  
  try {
    if (!query) { query = qs.parse(reqUrl.query); }
    
    if (!pathname || pathname.charAt(pathname.length - 1) === '/') {
      // generate index
      if (!fs.existsSync(ROOT + DEFAULT_PATH)) {
        body = genIndex(ROOT + pathname);
        statusCode = 200;
        res.writeHead(statusCode, {
          'Content-type': 'text/html',
          'Content-length': body.length
        });
        res.write(body);
        res.end();
        return;
      } else {
        pathname += DEFAULT_PATH;
      }
    }

    var ext = '',
        dot = pathname.lastIndexOf('.');
    if (dot < 0) {
      ext = DEFAULT_EXT;
      pathname += ext;
    } else {
      ext = pathname.substring(dot);
    }

    if (fs.existsSync(ROOT + pathname)) {
      body = fs.readFileSync(ROOT + pathname).toString();

      statusCode = 200;
      res.writeHead(statusCode, {
        'Content-type': (ext in MIME) ? MIME[ext] : 'application/octet-stream',
        'Content-length': body.length
      });
      res.write(body);
      res.end();
    } else {
      statusCode = 404;
      body = '404 not found\n';
      res.writeHead(statusCode, {
        'Content-type': 'text/plain',
        'Content-length': body.length
      });
      res.write(body);
      res.end();
    }
  } catch(e) {
    logging(e.message);
    statusCode = 500;
    body = '500 server error\n';
    res.writeHead(statusCode, {
      'Content-type': 'text/plain',
      'Content-length': body.length
    });
    res.write(body);
    res.end();
  }
}

function main() {
  var host = '127.0.0.1';
  var port = 1337;
  var argv = process.argv;
  var args = argv.length;

  // option
  if (args > 2) {
    var argv2 = argv[2];
    if (argv2 === '-h' || argv2 === '--help') {
      console.log('  node server.js [host [port]]');
      return;
    }
    host = argv2;
    if (args > 3) {
      port = argv[3];
    }
  }

  http.createServer(function(req, res) {
    logging(req.method +' '+ req.url);
    
    if (req.method === 'POST') {
      var data = '';
      req.on('data', function(chunk) {
        data += chunk;
      });
      req.on('end', function() {
        proc(req, res, qs.parse(data));
      });
    } else {
      proc(req, res, null);
    }
  }).listen(port, host);
  console.log('Server running at http://' + host + ':' + port);
}

main();
