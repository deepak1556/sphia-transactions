#include "sphia-macro/sphia-macro.h"
#include "commander/commander.h"
#include "repl/repl.h"
#include <sp.h>

static void *env;
static void *db;
static char path[256];

char *err = NULL;

// Defines a database instance and the path it points to.
typedef struct {
  char path[256];
  void *db;
  void *env;
} sphia_t;

repl_session_opts opts;

static char *
eval (repl_session_t *, char *);

static void
print (repl_session_t *, char *);

static void
error (repl_session_t *, char *);

static void
set_path(command_t *self) {
  strcpy(path, self->arg);
}

static int
transaction_init(sphia_t *sphia) {

  SPHIA_DB_OPEN(env, db, path, SPO_CREAT|SPO_RDWR) {

    if (NULL == db) {
      SPHIA_FERROR(sp_error(env));
      SPHIA_DESTROY(env);
      return 1;
    } else {
      sphia->db = db;
      sphia->env = env;
      srtcpy(sphia->path, path);
      if (-1 == sp_begin(db)) {
        SPHIA_FERROR(sp_error(env));
        return 1;
      } else {
        return 0;
      }
    }

  } SPHIA_CATCH(err, env) {

    SPHIA_FERROR(err);
    return 1;
  }

}

static int
srepl_init (sphia_t *sphia) {
  int rc = 0;

  opts.prompt = "sphia>";
  opts.eval_cb = eval;
  opts.print_cb = print;
  opts.error_cb = error;

  // initiate
  repl_session_t *sess = repl_session_new(&opts);

  // start
  rc = repl_session_start(sess);

  // destroy
  repl_session_destroy(sess);
  return rc;
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

static void
print (repl_session_t *sess, char *buf) {
  fprintf(sess->stdout, "%s\n", buf);
  repl_loop(sess);
}

static void
error (repl_session_t *sess, char *err) {
  fprintf(sess->stderr, "error: '%s'\n", err);
  repl_loop(sess);
}

static char *
eval (repl_session_t *sess, char *buf) {
  if (feof(sess->stdin)) {
    sess->rc = 0;
    return NULL;
  }


  printf("%s\n", buf);

  return NULL;
}
