#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"
#include "chunk.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_FUNCTION(value) is_obj_type(value, OBJ_FUNCTION)
#define IS_NATIVE(value) is_obj_type(value, OBJ_NATIVE)
#define IS_STRING(value) is_obj_type(value, OBJ_STRING)

#define AS_FUNCTION(value) ((ObjFunction *)AS_OBJ(value))
#define AS_NATIVE(value) \
  (((ObjNative *)AS_OBJ(value))->function)
#define AS_STRING(value) ((ObjString *)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString *)AS_OBJ(value))->chars)

typedef enum
{
  OBJ_FUNCTION,
  OBJ_STRING,
  OBJ_NATIVE
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

typedef Value (*NativeFn)(int arg_count, Value *args);

typedef struct
{
  Obj obj;
  NativeFn function;
} ObjNative;

struct sObjString
{
  Obj obj;
  int length;
  uint32_t hash;
  char *chars;
};

ObjFunction *new_function();
ObjNative *new_native(NativeFn function);
ObjString *take_string(char *chars, int length);
ObjString *copy_string(const char *chars, int length);
void print_object(Value value);

static inline bool is_obj_type(Value value, ObjType type)
{
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
