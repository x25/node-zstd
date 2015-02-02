#include <node.h>
#include <nan.h>

#include "../deps/zstd/lib/zstd.h"

using namespace v8;

NAN_METHOD(Decompress) {
  NanScope();

  if (args.Length() < 1 || !node::Buffer::HasInstance(args[0])) {

    return NanThrowError("First argument needs to be a buffer");
  }

  Local<Object> inputBuffer = args[0]->ToObject();

  size_t srcSize = node::Buffer::Length(inputBuffer);
  size_t decompressedMaxSize = srcSize*8000;// ^^

  char *srcData = node::Buffer::Data(inputBuffer);
  char *decompressedBuffer;

  try {

    decompressedBuffer = new char[decompressedMaxSize];

  } catch (const std::bad_alloc&) {

    return NanThrowError("Not enough memory!");
  }

  size_t decompressedSize = ZSTD_decompress(decompressedBuffer, decompressedMaxSize, srcData, srcSize);

  if (ZSTD_isError(decompressedSize)) {

    delete[] decompressedBuffer;
    return NanThrowError(ZSTD_getErrorName(decompressedSize));
  }

  Local<Object> result = NanNewBufferHandle(decompressedBuffer, decompressedSize);

  delete[] decompressedBuffer;

  NanReturnValue(result);
}

NAN_METHOD(Compress) {
  NanScope();

  if (args.Length() < 1 || !node::Buffer::HasInstance(args[0])) {

    return NanThrowError("First argument needs to be a buffer");
  }

  Local<Object> inputBuffer = args[0]->ToObject();

  size_t srcSize = node::Buffer::Length(inputBuffer);

  char *srcData = node::Buffer::Data(inputBuffer);

  size_t maxCompressedSize = ZSTD_compressBound(srcSize);

  char* compressedBuffer;

  try {

    compressedBuffer = new char[maxCompressedSize];

  } catch (const std::bad_alloc&) {

    return NanThrowError("Not enough memory!");
  }

  size_t compressedSize = ZSTD_compress(compressedBuffer, maxCompressedSize, srcData, srcSize);

  Local<Object> result = NanNewBufferHandle(compressedBuffer, compressedSize);
  delete[] compressedBuffer;

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
