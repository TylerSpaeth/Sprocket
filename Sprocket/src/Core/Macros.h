#ifndef MACROS_H
#define MACROS_H

#ifdef SPROCKET_BUILD_DLL
  #define SPROCKET_API __declspec(dllexport)
#else
  #ifdef SPROCKET_UNIT_TEST
    // For unit testing to stop exports
    #define SPROCKET_API
  #else
     #define SPROCKET_API __declspec(dllimport)
  #endif
#endif




#endif