#include <stdio.h>
#include <Windows.h>
#include <string.h>

#include "printv.h"
#include "awe_dll.h"

void help();
BOOL checkDRam();
BOOL initAWEMAN();
BOOL closeAWEMAN();
BOOL loadSoundFont(char*, char*, int);
char *getSetting(char*);
void listSoundFonts();
BOOL updateRegister(char*, char*);
char removeCitation(char*);
