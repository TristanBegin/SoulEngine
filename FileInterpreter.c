#include "SoulEngine.h"
#include "Behaviors.h"
#include <stdio.h>

void InterpretArchetypeFiles()
{
  FILE * ArchList = NULL;
  char * nextArch = "DONE";
  errno_t err = fopen_s(&ArchList, "ArchetypeList.txt", "r");

  //if (err == 0) 
  //  fscanf_s(ArchList, "<%s>", &nextArch);

  fclose(ArchList);
  OutputDebugString(nextArch);
}