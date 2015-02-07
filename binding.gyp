{
  "targets": [
    {
      "target_name": "zstd",
      "cflags": [ "-O3" ],
      "sources": [ "src/zstd.cc", "deps/zstd/lib/zstd.h", "deps/zstd/lib/zstd.c" ],
      "include_dirs": ["<!(node -e \"require('nan')\")"]
    }
  ]
}
