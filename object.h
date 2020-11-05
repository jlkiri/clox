#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"
#include "chunk.h"
#include "table.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_CLASS(value) is_obj_type(value, OBJ_CLASS)
#define IS_CLOSURE(value) is_obj_type(value, OBJ_CLOSURE)
#define IS_FUNCTION(value) is_obj_type(value, OBJ_FUNCTION)
#define IS_INSTANCE(value) is_obj_type(value, OBJ_INSTANCE)
#define IS_NATIVE(value) is_obj_type(value, OBJ_NATIVE)
#define IS_STRING(value) is_obj_type(value, OBJ_STRING)

#define AS_CLASS(value) ((ObjClass *)AS_OBJ(value))
#define AS_CLOSURE(value) ((ObjClosure *)AS_OBJ(value))
#define AS_FUNCTION(value) ((ObjFunction *)AS_OBJ(value))
#define AS_INSTANCE(value) ((ObjInstance *)AS_OBJ(value))
#define AS_NATIVE(value) \
  (((ObjNative *)AS_OBJ(value))->function)
#define AS_STRING(value) ((ObjString *)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString *)AS_OBJ(value))->chars)

typedef enum
{
  OBJ_CLASS,
  OBJ_FUNCTION,
  OBJ_INSTANCE,
  OBJ_STRING,
  OBJ_UPVALUE,
  OBJ_NATIVE,
  OBJ_CLOSURE
} ObjType;

struct sObj
{
  ObjType type;
  bool is_marked;
  struct sObj *next;
};

typedef struct
{
  Obj obj;
  int arity;
  int upvalue_count;
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

typedef struct ObjUpvalue
{
  Obj obj;
  Value *location;
  Value closed;
  struct ObjUpvalue *next;
} ObjUpvalue;

typedef struct
{
  Obj obj;
  ObjFunction *function;
  ObjUpvalue **upvalues;
  int upvalue_count;
} ObjClosure;

typedef struct
{
  Obj obj;
  ObjString *name;
} ObjClass;

typedef struct
{
  Obj obj;
  ObjClass *klass;
  Table fields;
} ObjInstance;

ObjClass *new_class(ObjString *name);
ObjClosure *new_closure(ObjFunction *function);
ObjFunction *new_function();
ObjInstance *new_instance(ObjClass *klass);
ObjNative *new_native(NativeFn function);
ObjString *take_string(char *chars, int length);
ObjString *copy_string(const char *chars, int length);
ObjUpvalue *new_upvalue(Value *slot);
void print_object(Value value);

static inline bool is_obj_type(Value value, ObjType type)
{
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
