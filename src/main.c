#include <sophia.h>
#include <sp.h>
#include "sphia-macro.h"
#include "srepl.h" 

void *db;
void *env;

char *err = NULL;

static int
transaction_init(sphia_t *sphia) {

  SPHIA_DB_OPEN(env, db, sphia->path, SPO_CREAT|SPO_RDWR) {

    if (NULL == db) {
      SPHIA_FERROR(sp_error(env));
      SPHIA_DESTROY(env) {

        return 1;

      } SPHIA_CATCH(err, env) {

        SPHIA_FERROR(err);
        return 1;
      }
      return 1;
    } else {
      sphia->db = db;
      sphia->env = env;
      free(db);
      free(env);
      if (-1 == sp_begin(db)) {
        SPHIA_FERROR(sp_error(env));
        return 1;
      }
    }

  } SPHIA_CATCH(err, env) {

    SPHIA_FERROR(err);
    return 1;

  }

  return 0;
}

int
main(int argc, char *argv[]) {
  int rc;
  char *prgname = argv[0];
  sphia_t *sphia = malloc(sizeof(sphia_t));

  while(argc > 2) {
    argc--;
    argv++;
    if(!strcmp(*argv, "--path")) {
      argv++;
      strcpy(sphia->path, *argv);
    }else {
      fprintf(stderr, "Usage: %s [--path]\n", prgname);
      exit(1);
    }
  }

  if (0 == strlen(sphia->path)) {
    char *default_path = getenv("SPHIA_PATH");
    if (NULL != default_path) {
      strcpy(sphia->path, default_path);
    } else {
      strcpy(sphia->path, ".");
    }
  }

  if (0 != (rc = transaction_init(sphia))) return rc;

  if(0 != (rc = srepl_init(sphia))) return rc;

  sp_destroy(sphia->db);
  sp_destroy(sphia->env);
  return 0;
}
