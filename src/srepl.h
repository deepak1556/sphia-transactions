#ifndef SPHIA_REPL_H
#define SPHIA_REPL_H 1

#include <sophia.h>
#include "linenoise/linenoise.h"

// Defines a database instance and the path it points to.
typedef struct {
  char path[256];
  void *db;
  void *env;
} sphia_t;

typedef struct {
  char *key;
  char *value;
} opt_t;

typedef int (*cmd_func_t)(sphia_t *, opt_t *);

typedef struct {
  char *name;
  char *alt;
  cmd_func_t func;
} cmd_t;


// Start a repl session
int
srepl_init (sphia_t *, cmd_t *, opt_t *);

#endif

