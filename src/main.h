#include <stdio.h>
#include <Windows.h>
#include <string.h>

#include "../include/sfloader/awe_dll.h"
#include "printv.h"

void help();
BOOL checkDRam();
BOOL initAWEMAN();
BOOL closeAWEMAN();
BOOL loadSoundFont(char*, char*, int);
char *getSetting(char*);
void listSoundFonts();
BOOL updateRegister(char*, char*);
char removeCitation(char*);
