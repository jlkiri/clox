#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

static void repl()
{
  char line[1024];
  for (;;)
  {
    printf("> ");

    if (!fgets(line, sizeof(line), stdin))
    {
      printf("\n");
      break;
    }

    interpret(line);
  }
}

static char *read_file(const char *path)
{
  FILE *file = fopen(path, "rb");
  //> no-file
  if (file == NULL)
  {
    fprintf(stderr, "Could not open file \"%s\".\n", path);
    exit(74);
  }
  //< no-file

  fseek(file, 0L, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);

  char *buffer = (char *)malloc(fileSize + 1);
  //> no-buffer
  if (buffer == NULL)
  {
    fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
    exit(74);
  }

  //< no-buffer
  size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
  //> no-read
  if (bytesRead < fileSize)
  {
    fprintf(stderr, "Could not read file \"%s\".\n", path);
    exit(74);
  }

  //< no-read
  buffer[bytesRead] = '\0';

  fclose(file);
  return buffer;
}

static void run_file(const char *path)
{
  char *source = read_file(path);
  InterpretResult result = interpret(source);
  free(source);

  if (result == INTERPRET_COMPILE_ERROR)
    exit(65);
  if (result == INTERPRET_RUNTIME_ERROR)
    exit(70);
}

int main(int argc, const char *argv[])
{
  init_vm();

  if (argc == 1)
  {
    repl();
  }
  else if (argc == 2)
  {
    run_file(argv[1]);
  }
  else
  {
    fprintf(stderr, "Usage: clox [path]\n");
    exit(64);
  }

  free_vm();
  return 0;
}