#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <zstd.h>

static void compress(char* fname, char* outName, int compress_level)
{
  FILE* source_file  = fopen(fname, "rb");
  FILE* destination_file = fopen(outName, "wb");
  size_t input_buffer_size = ZSTD_CStreamInSize();
  void* input_buffer  = malloc(input_buffer_size);
  size_t output_buffer_size = ZSTD_CStreamOutSize();
  void* output_buffer = malloc(output_buffer_size);
  ZSTD_CStream* stream = ZSTD_createCStream();
  ZSTD_initCStream(stream, compress_level);
  size_t read, to_read = input_buffer_size;

  while (read = fread(input_buffer, 1, to_read, source_file)) {
    ZSTD_inBuffer input = { input_buffer, read, 0 };

    while (input.pos < input.size) {
      ZSTD_outBuffer output = { output_buffer, output_buffer_size, 0 };
      to_read = ZSTD_compressStream(stream, &output, &input);

      if (to_read > input_buffer_size)
        to_read = input_buffer_size;

      fwrite(output_buffer, 1, output.pos, destination_file);
    }
  }

  ZSTD_outBuffer output = { output_buffer, output_buffer_size, 0 };
  ZSTD_endStream(stream, &output);

  fwrite(output_buffer, 1, output.pos, destination_file);

  ZSTD_freeCStream(stream);
  fclose(destination_file);
  fclose(source_file);
  free(input_buffer);
  free(output_buffer);
}

static char* destination_filename_from(char* filename)
{
  size_t length = strlen(filename) + 5;
  void* buffer = malloc(length);
  memset(buffer, 0, length);
  strcat(buffer, filename);
  strcat(buffer, ".zst");
  return (char*)buffer;
}

int main(int argc, char** argv)
{
  char* source = argv[1];
  char* destination = destination_filename_from(source);
  int compression_level = 1;

  printf("Compressing %s to %s with compression level %d…\n", source, destination, compression_level);
  compress(source, destination, compression_level);
  return 0;
}
