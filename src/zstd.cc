#include <node.h>
#include <nan.h>
#include <cstdlib>

#include "../deps/zstd/lib/zstd.h"
#include "../deps/zstd/lib/zstd_static.h"

using namespace v8;

#define ZSTDJS_BLOCK_SIZE 128 * (1U<<10) //128 KB;

NAN_METHOD(Decompress) {
  NanScope();

  if (args.Length() < 1 || !node::Buffer::HasInstance(args[0])) {

    return NanThrowError("First argument needs to be a buffer");
  }

  Local<Object> inputBuffer = args[0]->ToObject();

  size_t srcSize = node::Buffer::Length(inputBuffer);
  char *srcData = node::Buffer::Data(inputBuffer);
  char *srcDataEnd = srcData + srcSize;

  ZSTD_cctx_t ctx  = ZSTD_createDCtx();
  size_t headerSize = ZSTD_getNextcBlockSize(ctx);

  if (srcData + headerSize > srcDataEnd) {

      return NanThrowError("Error: invalid header");
  }

  unsigned long long outSize = 0;

  // Decode frame header
  size_t sizeCheck = ZSTD_decompressContinue(ctx, NULL, 0, srcData, headerSize);

  if (ZSTD_isError(sizeCheck)) {

    return NanThrowError("Error decoding header");
  }

  srcData += headerSize;

  char* outBuff = NULL;

  size_t toRead = ZSTD_getNextcBlockSize(ctx);

  while (toRead)
  {
    if (srcData + toRead > srcDataEnd) {

      std::free(outBuff);
      return NanThrowError("Invalid block data");
    }

    void *r  = std::realloc(outBuff, outSize + ZSTDJS_BLOCK_SIZE);

    if (!r) {

       std::free(outBuff);
       return NanThrowError("Allocation error : not enough memory");
    }

    outBuff = (char*)r;

    size_t decodedSize = ZSTD_decompressContinue(ctx, outBuff + outSize,
                                                 ZSTDJS_BLOCK_SIZE,
                                                 srcData, toRead);

    if (ZSTD_isError(decodedSize)) {

      std::free(outBuff);
      return NanThrowError("Error decoding block");
    }

    srcData += toRead;
    outSize += decodedSize;

    toRead = ZSTD_getNextcBlockSize(ctx);
  }

  Local<Object> result = NanNewBufferHandle(outBuff, outSize);

  std::free(outBuff);
  ZSTD_freeDCtx(ctx);

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

  char* compressedBuffer = (char*)std::malloc(maxCompressedSize);

  if (!compressedBuffer) {

      return NanThrowError("Not enough memory!");
  }

  size_t compressedSize = ZSTD_compress(compressedBuffer, maxCompressedSize,
                                        srcData, srcSize);

  Local<Object> result = NanNewBufferHandle(compressedBuffer, compressedSize);
  std::free(compressedBuffer);

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
