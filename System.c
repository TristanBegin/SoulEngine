#include "SoulEngine.h"
#pragma comment (lib, "Alpha_Engine.lib")

void SystemInitialize(HINSTANCE instanceH, int show)
{
  // Initialize the system

  // Create new AESysInitInfo
  AESysInitInfo sysInitInfo;

  //Set all the values of it.
  sysInitInfo.mCreateWindow = 1;
  sysInitInfo.mAppInstance = instanceH;
  sysInitInfo.mShow = show;
  sysInitInfo.mWinWidth = 1080;
  sysInitInfo.mWinHeight = 720;
  sysInitInfo.mCreateConsole = 1; //NOT the same as console we know. stdout will not print to it.
  sysInitInfo.mMaxFrameRate = 60;
  sysInitInfo.mpWinCallBack = NULL; //MyWinCallBack;
  sysInitInfo.mClassStyle = CS_HREDRAW | CS_VREDRAW;
  sysInitInfo.mWindowStyle = WS_OVERLAPPEDWINDOW; //WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;;
  sysInitInfo.mWindowHandle = NULL;
  sysInitInfo.mHandleWindowMessages = 1;

  //Set AESysInit to the adress of our new sysInitInfo.
  AESysInit(&sysInitInfo);

  // Reset the system modules.
  AESysReset();

}