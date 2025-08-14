#include "printv.h"

BOOL verbose = FALSE;

void setVerbose(BOOL v){
    verbose = v;
}

void printv(char *f, ...){
    va_list args;
    
    if(!verbose){
        return;
    }
        
    va_start(args, f);
    vprintf(f, args);
    va_end(args);
}
