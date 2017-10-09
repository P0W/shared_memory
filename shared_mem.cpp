/* 
shared_mem.cpp
Defines the exported functions for the DLL application.

Author : _P0W!

Disclaimer : C++ Boost Libraries are used, WinAPIs are not reliable on all environment
and locking mutexes to simulteneously read and write will require more code.

FWIW..Boost is a set of libraries for the C++ programming language 
Most of the Boost libraries are licensed under the Boost Software License, 
designed to allow Boost to be used with both free and proprietary software projects. 
Many of Boost's founders are on the C++ standards committee

Source: http://en.wikipedia.org/wiki/Boost_%28C++_libraries%29

Ref: http://www.boost.org/

*/


/*
Dear maintainer,

There's nothing new here, all magic is done by boost::interprocess.

Just a side note: global data and satic memory shouldn't be shared between apllication and DLL without common data segment,
I'm mapping the memory several thousand times, I'll improve this, when this DLL is actually put to use.

Also, once you are done trying to 'optimize' this routine, with several other concepts
and have realized what a terrible mistake that was, please increment the following counter as a warning
to the next engineer.

total_hours_wasted_here = 8

Thanks,
_P0W!

*/

#include "shared_mem.h"

// #pragma data_seg (".myseg")

// char  SharedMemoryName[12];     
// long  long SharedMemorySize;
// void* SharedMemoryPtr ;
// #pragma data_seg()

// void* SharedMemory( const char* shmem_name )
// {
// try{
// boost::interprocess::shared_memory_object shm
// (boost::interprocess::open_only, //Open if already created
// shmem_name, 
// boost::interprocess::read_write);

// boost::interprocess::mapped_region region
// (shm, 
// boost::interprocess::read_write
// );
// size_t nbytes = strlen(shmem_name);      
// strncpy(SharedMemoryName, shmem_name, ( nbytes > 12u ) ? 12u : nbytes );
// SharedMemory = region.get_address();
// SharedMemorySize = region.get_size();
// return region.get_address() ;
// }
// catch(...){
// std::cerr << "Error Creating Shared Memory : " << shmem_name << std::endl;
// }
// return ;
// }


int CreateSharedMemory(const char* shmem_name, long long length  )
{
   try{
      // remove_shmem(shmem_name);
      boost::interprocess::shared_memory_object shm 
         (boost::interprocess::create_only,
         shmem_name,
         boost::interprocess::read_write);
      //Set size
      shm.truncate(length);
      return 1;
   }

   catch(boost::interprocess::interprocess_exception &ex){
      std::cerr << "Error Creating Shared Memory : '" 
         << shmem_name << "' -> "
         << ex.what() << std::endl;  
   }
   return 0;
}

int RemoveSharedMemory(const char* shmem_name )
{   
   /* Returns false on error. Never throws  
   fail returning false if the shared memory does not exist,
   the file is open or the file is still memory mapped by other processes: 
   */
   return   boost::interprocess::shared_memory_object::remove(shmem_name);     
}

int WriteSharedMemory( const char* shmem_name, const char* shmem )
{
   try{
      boost::interprocess::shared_memory_object shm
         (
         boost::interprocess::open_only, //Open if already created
         shmem_name, 
         boost::interprocess::read_write
         );

      //Map the whole shared memory in this process
      boost::interprocess::mapped_region region
         (
         shm, 
         boost::interprocess::read_write
         );

      std::memcpy(region.get_address(), shmem, region.get_size());
      return 1;
   }
   catch(boost::interprocess::interprocess_exception &ex){
#if 0
      std::cerr << "Error Writing to Shared Memory : '" 
         << shmem_name << "' -> "
         << ex.what() << std::endl;
#endif
   }
   return 0;
}


int ReadSharedMemory( const char* shmem_name, char *shmem )
{
   //Open already created shared memory object.
   // Same process might not be calling it, so create new object
   try{
      boost::interprocess::shared_memory_object shm 
         (
         boost::interprocess::open_only, 
         shmem_name, 
         boost::interprocess::read_only
         );

      //Map the whole shared memory in this process
      boost::interprocess::mapped_region region
         (
         shm, 
         boost::interprocess::read_only
         );

      //CAUTION : No check is performed
      std::memcpy( shmem, reinterpret_cast<char *>(region.get_address()), region.get_size() ); 
      return 1;

   }
   catch(boost::interprocess::interprocess_exception &ex){
#if 0
      std::cerr << "Error Reading to Shared Memory : '" 
         << shmem_name << "' -> "
         << ex.what() << std::endl;
#endif
   }
   return 0;
}


void* GetSharedMemoryRegion( const char* shmem_name )
{
   try{
      boost::interprocess::shared_memory_object shm 
         (
         boost::interprocess::open_only, 
         shmem_name, 
         boost::interprocess::read_only
         );

      //Map the whole shared memory in this process
      boost::interprocess::mapped_region region
         (
         shm, 
         boost::interprocess::read_only
         );

      //CAUTION : No check is performed
      return region.get_address() ;
   }

   catch(boost::interprocess::interprocess_exception &ex){
#if 0
      std::cerr << "Error Reading to Shared Memory : '" 
         << shmem_name << "' -> "
         << ex.what() << std::endl;
#endif
   }

   return NULL ;
}