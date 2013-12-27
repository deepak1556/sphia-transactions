#include <sophia.h>
#include <sp.h>
#include "sphia-macro.h"
#include "srepl.h" 

void *db;
void *env;

char *err = NULL;

GEN_CMD(begin) {
  int rc = sp_begin(sphia->db);
  if(rc == 0) {
    printf("Transaction initialised");
  }else if(rc == 1) {
    printf("Transaction under process");
  }else {
    SPHIA_FERROR(sp_error(sphia->env));
    return 1;
  }

  return 0;
}

GEN_CMD(set) {
  if(opts->key != NULL && opts->value != NULL) {
    int rc = sp_set(sphia->db, &opts->key, strlen(opts->key) + 1, &opts->value, strlen(opts->value) + 1);
    if(rc == 0) {
      printf("Currently in queue");
    }else {
      SPHIA_FERROR(sp_error(sphia->env));
      return 1;
    }
  }else {
    printf("Invalid or too few arguements");
    return 1;
  }

  return 0;
}

GEN_CMD(commit) {
  int rc = sp_commit(sphia->db);
  if(rc != 0) {
    SPHIA_FERROR(sp_error(sphia->env));
    return 1;
  }else {
    printf("Batch operations completed");
  }

  return 0;
}

static cmd_t cmds[] = {
  {"begin", NULL, cmd_begin}, 
  {"set", NULL, cmd_set},
  {"commit", NULL, cmd_commit}
};


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
  opt_t *opts = malloc(sizeof(opt_t));

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
  free(sphia);
  free(opts);
  
  return 0;
}
