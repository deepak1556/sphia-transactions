
//
// sphia-macro.h
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//

#ifndef __SPHIA_MACRO_H__
#define __SPHIA_MACRO_H__ 1

#define SPHIA_MACRO_VERSION "0.0.1"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sophia.h>

#define SPHIA_FERROR(e)                                   \
  fprintf(stderr, "%s\n", e);

#define SPHIA_CATCH(err, o)                               \
  else if (1 && (err = sp_error(o)))

#define SPHIA_ENV(env, path, mode)                        \
  void *env = sp_env();                                   \
  if (NULL != env &&                                      \
      0 == sp_ctl(env, SPDIR, mode, path))                \

#define SPHIA_DB_OPEN(env, db, path, mode)                \
  void *env = sp_env();                                   \
  void *db = NULL;                                        \
  if (NULL != env &&                                      \
      0 == sp_ctl(env, SPDIR, mode, path) &&              \
      (db = sp_open(env)))

#define SPHIA_DESTROY(o)                                  \
  if (0 == sp_destroy(o))

#endif
