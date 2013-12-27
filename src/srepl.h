#ifndef SPHIA_REPL_H
#define SPHIA_REPL_H 1

#include "linenoise/linenoise.h"

// Defines a database instance and the path it points to.
typedef struct {
  char path[256];
  void *db;
  void *env;
} sphia_t;

// Start a repl session for the database, returns -1 on error.
int
srepl_init (sphia_t *);

#endif

