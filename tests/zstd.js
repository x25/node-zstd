var assert = require('assert');
var zstd = require('./..');
var bsize = 10000000;
var data = new Buffer(bsize);
data.fill("A");

for (var i=0;i<100;i++) {

  var compressed = zstd.compress(data);
  var decompressed = zstd.decompress(compressed);

  assert(decompressed.length == bsize);
}

