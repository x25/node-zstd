#include <node.h>
#include <nan.h>

#include "../deps/zstd/lib/zstd.h"

using namespace v8;

NAN_METHOD(Decompress) {
  NanScope();

  if (args.Length() < 1 || !node::Buffer::HasInstance(args[0])) {

    return NanThrowError("First argument needs to be a buffer");
  }

  size_t srcSize = node::Buffer::Length(args[0]);
  size_t decompressedMaxSize = srcSize*10000;// ^^

  char *srcBuffer = node::Buffer::Data(args[0]);

  void* decompressedBuffer = malloc(decompressedMaxSize);

  if (!decompressedBuffer) {

    return NanThrowError("Not enough memory!");
  }

  size_t decompressedSize = ZSTD_decompress(decompressedBuffer, decompressedMaxSize, srcBuffer, srcSize);

  if (ZSTD_isError(decompressedSize)) {

    free(decompressedBuffer);
    return NanThrowError(ZSTD_getErrorName(decompressedSize));
  }

  Local<Object> result = NanNewBufferHandle(decompressedSize); //slowBuffer in node <=0.10!

  char *outputData = node::Buffer::Data(result);

  memcpy(outputData, decompressedBuffer, decompressedSize);

  free(decompressedBuffer);

  NanReturnValue(result);
}

NAN_METHOD(Compress) {
  NanScope();

  if (args.Length() < 1 || !node::Buffer::HasInstance(args[0])) {

    return NanThrowError("First argument needs to be a buffer");
  }

  size_t srcSize = node::Buffer::Length(args[0]);

  char *srcBuffer = node::Buffer::Data(args[0]);

  size_t maxCompressedSize = ZSTD_compressBound(srcSize);
  void* compressedBuffer = malloc(maxCompressedSize);

  if (!compressedBuffer) {

    return NanThrowError("Not enough memory!");
  }

  size_t compressedSize = ZSTD_compress(compressedBuffer, maxCompressedSize, srcBuffer, srcSize);

  Local<Object> result = NanNewBufferHandle(compressedSize); //slowBuffer in node <=0.10!

  char *outputData = node::Buffer::Data(result);

  memcpy(outputData, compressedBuffer, compressedSize);

  free(compressedBuffer);

  NanReturnValue(result);
}

void Init(Handle<Object> exports) {
  exports->Set(
    NanNew<String>("compress"),
    NanNew<FunctionTemplate>(Compress)->GetFunction()
  );

  exports->Set(
    NanNew<String>("decompress"),
    NanNew<FunctionTemplate>(Decompress)->GetFunction()
  );
}

NODE_MODULE(zstd, Init)
