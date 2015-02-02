{
  "targets": [
    {
      "target_name": "zstd",
      "cflags": [ "-O3" ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": ["-fno-exceptions"],
      "sources": [ "src/zstd.cc", "deps/zstd/lib/zstd.h", "deps/zstd/lib/zstd.c" ],
      "include_dirs": ["<!(node -e \"require('nan')\")"]
    }
  ]
}
