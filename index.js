var addon = require('./build/Release/zstd');

module.exports.compress = addon.compress;
module.exports.decompress = addon.decompress;
