node-zstd [![](https://travis-ci.org/x25/node-zstd.svg?branch=master)](https://travis-ci.org/x25/node-zstd)
=====

ZSTD lossless compression algorithm bindings for node.js.

```bash
$ npm install zstd
```

## Usage

```js
var zstd = require('zstd');

var data = new Buffer(1000);
data.fill("A");

var compressed = zstd.compress(data);
var decompressed = zstd.decompress(compressed);
```

## Tests

```sh
$ npm test
```

## License
MIT
