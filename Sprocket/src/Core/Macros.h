#ifndef MACROS_H
#define MACROS_H

#ifdef SPROCKET_BUILD_DLL
  #define SPROCKET_API __declspec(dllexport)
#else
  #define SPROCKET_API __declspec(dllimport)
#endif

#endif