node-zstd [![](https://travis-ci.org/x25/node-zstd.svg?branch=master)](https://travis-ci.org/x25/node-zstd)
=====

ZSTD lossless compression algorithm bindings for Node.js.

```bash
$ npm install zstd
```

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
