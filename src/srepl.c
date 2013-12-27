#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "srepl.h"

//not completely perfect parser
void parse_args(char *line, opt_t *opts) {
  char *temp = strtok(line, " ");
  while(temp != NULL) {
    if(0 == strcmp(temp, "--key")) {
      temp = strtok(NULL, " ");
      opts->key = temp;
    }
    if(0 == strcmp(temp, "--value")) {
      temp = strtok(NULL, " ");
      opts->value = temp;
    }
  }
}

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
srepl_init (sphia_t *sphia, cmd_t *cmds, opt_t *opts) {
  int rc = 0;
  char *line;

  linenoiseSetCompletionCallback(completion);
  
  linenoiseHistoryLoad("history.txt");

  while((line = linenoise("sphia> ")) != NULL) {
    for(size_t i = 0; i < sizeof(*cmds) / sizeof(cmds[0]); i++) {
      if(0 == strcmp(cmds[i].name, line) || 
        (NULL != cmds[i].alt && 0 == strcmp(cmds[i].alt, line))) {
       if(0 == strcmp(cmds[i].name, "set")) {
           parse_args(line, opts);
       }

       if(NULL != cmds[i].func) {
         if(0 == (rc = cmds[i].func(sphia, opts))) {
           linenoiseHistoryAdd(line);
           linenoiseHistorySave("History.txt");
         }else {
        
         }
       }
     }
   }
      
    free(line);
  }
  
  return rc;
}
