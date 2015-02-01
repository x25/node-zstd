{
  "targets": [
    {
      "target_name": "zstd",
      "cflags": [ '-O2' ],
      "sources": [ "src/zstd.cc", "deps/zstd/lib/zstd.c" ],
      "include_dirs": ["<!(node -e \"require('nan')\")"]
    }
  ]
}
