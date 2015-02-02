var assert = require('assert');
var zstd = require('./..');

for (var i=0;i<100;i++) {

  var bsize = 1024*1024;
  var data = new Buffer(bsize);
  data.fill('A');

  var compressed = zstd.compress(data);
  var decompressed = zstd.decompress(compressed);

  assert(decompressed.length == bsize);
}

