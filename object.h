#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"
#include "chunk.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_FUNCTION(value) is_obj_type(value, OBJ_FUNCTION)
#define IS_STRING(value) is_obj_type(value, OBJ_STRING)

#define AS_FUNCTION(value) ((ObjFunction *)AS_OBJ(value))
#define AS_STRING(value) ((ObjString *)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString *)AS_OBJ(value))->chars)

typedef enum
{
  OBJ_FUNCTION,
  OBJ_STRING,
} ObjType;

struct sObj
{
  ObjType type;
  struct sObj *next;
};

typedef struct
{
  Obj obj;
  int arity;
  Chunk chunk;
  ObjString *name;
} ObjFunction;

struct sObjString
{
  Obj obj;
  int length;
  uint32_t hash;
  char chars[];
};

ObjFunction *new_function();
ObjString *take_string(char *chars, int length);
ObjString *copy_string(const char *chars, int length);
void print_object(Value value);

static inline bool is_obj_type(Value value, ObjType type)
{
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
