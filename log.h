#ifndef _log_h
#define _log_h

#define log_msg(msg) fprintf(stdout, "LOG: %s\n", msg)
#define log_msg_1(msg, x1) fprintf(stdout, "LOG: " msg "\n", x1)
#define log_msg_2(msg, x1, x2) fprintf(stdout, "LOG: " msg "\n", x1, x2)
#define log_msg_3(msg, x1, x2, x3) fprintf(stdout, "LOG: " msg "\n", x1, x2, x3)

#endif
