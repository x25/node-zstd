node-zstd [![](https://travis-ci.org/x25/node-zstd.svg?branch=master)](https://travis-ci.org/x25/node-zstd)
=====

ZSTD lossless compression algorithm bindings for Node.js.

## Installation

```bash
$ npm install zstd
```

In case of npm install error:

```
gyp: Call to 'node -e "require('nan')"' returned exit status 127. while trying to load binding.gyp
```

Try to `sudo apt-get install nodejs-legacy` or `sudo ln -s "$(which nodejs)" /usr/bin/node`.

## Usage

```js
var zstd = require('zstd');

var data = new Buffer(1000);
data.fill('A');

var compressed = zstd.compress(data);
var decompressed = zstd.decompress(compressed);

console.log('Compressed:', compressed.length);
console.log('Decompressed:', decompressed.length);

//Compressed: 25
//Decompressed: 1000
```

## Tests

```sh
$ npm test
```

## License
MIT
