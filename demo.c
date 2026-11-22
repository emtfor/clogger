#include "clogger.h"




int main(void){
int LVL = LOG_LEVEL_DEBUG;
const char * LOGFILE = "clogger.log";

    int log = log_init(LOGFILE, LVL);
        if (log == EXIT_FAILURE) {
        printf("unable to intialize log\n");
        return(EXIT_FAILURE);
        }
    log_info("\n\n**** Intialiized \n\n");
    log_error("This is an error");
    log_warning("This is a warning");
    log_debug("This is a debug call");

    return(0);
}
