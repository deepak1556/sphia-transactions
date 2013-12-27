#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "srepl.h"

void completion(const char *buf, linenoiseCompletions *lc) {
  switch (buf[0]) {
  case 'b':
    linenoiseAddCompletion(lc, "begin");
    break;
  case 'c':
    linenoiseAddCompletion(lc, "commit");
    break;
  case 's':
    linenoiseAddCompletion(lc, "status");
    linenoiseAddCompletion(lc, "set");
    break;
  case 'r':
    linenoiseAddCompletion(lc, "rollback");
  }
}

int
srepl_init (sphia_t *sphia) {
  int rc = 0;
  char *line;

  linenoiseSetCompletionCallback(completion);
  
  linenoiseHistoryLoad("history.txt");

  while((line = linenoise("sphia> ")) != NULL) {
    if(line[0] != '\0' && line[0] != '/') {
      printf("echo: '%s%s'\n", line, sphia->path);
      linenoiseHistoryAdd(line);
      linenoiseHistorySave("history.txt");
    }else if(line[0] == '/'){
  printf("Unrecognised command: %s\n", line);
    }
      free(line);
  }
  
  return rc;
}
