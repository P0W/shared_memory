/* 

Shared Memory Interface
# Author : _P0W!

Disclaimer : Used C++ Boost Libraries, WinAPIs are not reliable on all environment
and locking mutexes to simulteneously read and write will require more code.

FWIW Boost is a set of libraries for the C++ programming language 
Most of the Boost libraries are licensed under the Boost Software License, 
designed to allow Boost to be used with both free and proprietary software projects. 
Many of Boost's founders are on the C++ standards committee

Source: http://en.wikipedia.org/wiki/Boost_%28C++_libraries%29

Ref: http://www.boost.org/

*/

#ifdef __cplusplus
#include<iostream>
#include<cstring>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
extern "C" {
#endif

#ifndef _SHMEM_H_
#define _SHMEM_H_
#ifndef USE_DLL
#ifndef __cplusplus
#error Must use C++ to compile this DLL
#else
#define DLL_EXPORT_TYPE __declspec( dllexport )
#endif
#else
#define DLL_EXPORT_TYPE __declspec( dllimport )
#endif

   // static void* SharedMemoryPtr;             // Shared Memory Address
   // DLL_EXPORT_TYPE extern  char SharedMemoryName[12];     //  Shared Memory Name
   // DLL_EXPORT_TYPE extern  long long SharedMemorySize;   //   Shared Memory Size
   // DLL_EXPORT_TYPE  void SetSharedMemory    ( const char*  );
   DLL_EXPORT_TYPE  int CreateSharedMemory ( const char*, long long );
   DLL_EXPORT_TYPE  int RemoveSharedMemory ( const char*  );
   DLL_EXPORT_TYPE  int WriteSharedMemory  ( const char*, const char*  ) ;
   DLL_EXPORT_TYPE  int ReadSharedMemory   ( const char*, char* );
   DLL_EXPORT_TYPE  void* GetSharedMemoryRegion ( const char*  );
#endif

#ifdef __cplusplus
}
#endif