#include "SoulEngine.h"
#include "Behaviors.h"
#include <stdio.h>

#define MAX_LENGTH 50

void AddNull(char * buffer)
{
  buffer[strlen(buffer) - 1] = '\0';
}
void InterpretArchetypeFiles()
{
  FILE * ArchList = NULL;
  FILE * curArch = NULL;
  char buffer[MAX_LENGTH];
  char nextArch[MAX_LENGTH];
  int myint = 0;
  char mychar = 'a';

  ArchList = fopen("ArchetypeList.txt", "r");

  if (fgets(buffer, MAX_LENGTH, ArchList))
  {
    AddNull(buffer);
    //sscanf(buffer, "%s", &nextArch); <-WORKS :D
    curArch = fopen(buffer, "r");

  }


  
  OutputDebugString(nextArch);

  fclose(ArchList);
  
}