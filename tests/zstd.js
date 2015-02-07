var assert = require('assert');
var zstd = require('./..');

function isSame(a, b) {
	if (a.length !== b.length) return false;
	for (var i = 0; i < a.length; i++) {
		if (a[i] !== b[i]) return false;
	}
	return true;
};

for (var i = 0; i<100; i++) {

	var bsize = 1024*1024;
	var data = new Buffer(bsize);
	data.fill('A');

	var compressed = zstd.compress(data);

	var decompressed = zstd.decompress(compressed);

	assert(isSame(decompressed, data));
}

