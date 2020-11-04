#include <stdlib.h>

#include "chunk.h"
#include "memory.h"
#include "vm.h"

void init_chunk(Chunk *chunk)
{
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  init_value_array(&chunk->constants);
}

void write_chunk(Chunk *chunk, uint8_t byte, int line)
{
  if (chunk->capacity < chunk->count + 1)
  {
    int old_capacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(old_capacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code, old_capacity, chunk->capacity);
    chunk->lines = GROW_ARRAY(int, chunk->lines, old_capacity, chunk->capacity);
  }

  chunk->code[chunk->count] = byte;
  chunk->lines[chunk->count] = line;
  chunk->count++;
}

void write_constant(Chunk *chunk, Value value, int line)
{
  int constant = add_constant(chunk, value);
  if (constant > UINT8_MAX)
  {
    uint8_t hb = (constant >> 16) & 0xFF;
    uint8_t mb = (constant >> 8) & 0xFF;
    uint8_t lb = constant & 0xFF;
    write_chunk(chunk, OP_CONSTANT_LONG, line);
    write_chunk(chunk, hb, line);
    write_chunk(chunk, mb, line);
    write_chunk(chunk, lb, line);
  }
  else
  {
    write_chunk(chunk, OP_CONSTANT, line);
    write_chunk(chunk, constant, line);
  }
}

int add_constant(Chunk *chunk, Value value)
{
  push(value);
  write_value_array(&chunk->constants, value);
  pop();
  return chunk->constants.count - 1;
}

void free_chunk(Chunk *chunk)
{
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->capacity);
  init_chunk(chunk);
}