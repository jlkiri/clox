#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char *argv[])
{
  initVm();

  Chunk chunk;
  init_chunk(&chunk);

  write_constant(&chunk, 1.2, 123);
  write_constant(&chunk, 3.4, 123);
  write_chunk(&chunk, OP_ADD, 123);
  write_constant(&chunk, 5.6, 123);
  write_chunk(&chunk, OP_DIVIDE, 123);
  write_chunk(&chunk, OP_NEGATE, 123);
  write_chunk(&chunk, OP_RETURN, 123);

  disassemble_chunk(&chunk, "test chunk");
  interpret(&chunk);
  freeVm();
  free_chunk(&chunk);
  return 0;
}