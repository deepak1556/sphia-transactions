#include "sphia-macro.h"
#include "commander/commander.h"
#include "srepl.h"

static void *env;
static void *db;
static char path[256];

char *err = NULL;

static int
transaction_init(sphia_t *sphia) {

  SPHIA_DB_OPEN(env, db, path, SPO_CREAT|SPO_RDWR) {

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
      srtcpy(sphia->path, path);
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
  command_t program;
  command_init(&program
    , "sphia-begin"
    , "0.0.1");
  command_option(&program
    , "-p"
    , "--path <path>"
    , "path to database"
    , set_path);
  command_parse(&program, argc, argv);

  if (0 == strlen(path)) {
    char *default_path = getenv("SPHIA_PATH");
    if (NULL != default_path) {
      strcpy(path, default_path);
    } else {
      strcpy(path, ".");
    }
  }

  int rc;
  sphia_t *sphia = malloc(sizeof(sphia_t));
  if (0 != (rc = transaction_init(sphia))) return rc;

  if(0 != (rc = srepl_init(sphia))) return rc;

  sp_destroy(db);
  sp_destroy(env);
  return 0;
}
