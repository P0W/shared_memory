
#!/usr/bin/python

# Tested with Python 3.3.0
# Author : _P0W!

## --  Shared Object Details --
##    /*
##    MinGW 4.6.1, Boost : 1.54.0 ( Boost Interprocess Shared Memory Object )
##        g++ -c  shared_mem.cpp -o shared_mem.o -IC:\MinGW\boost_1_45_0 -Wall -Wextra    
##        g++ -shared -Wl -o shared_mem.so shared_mem.o -lole32 -loleaut32
##    */


import ctypes

class Shared_Memory_Wrapper:
    ## Creates a "named" Shared Memory Block
    import sys
    sys.path.extend(['.'])
    __sharedObj = ctypes.cdll.LoadLibrary(r'shared_mem.dll') 
    
    # void CreateSharedMemory(const char*, long long );
    __create_shmem = __sharedObj.CreateSharedMemory 
##    __create_shmem.restype = ctypes.c_int

    # void RemoveSharedMemory(const char*  );
    __remove_shmem = __sharedObj.RemoveSharedMemory
##    __remove_shmem.restype = ctypes.c_int

    # void WriteSharedMemory ( const char* , const char* );
    __write_shmem = __sharedObj.WriteSharedMemory
##    __write_shmem.restype = ctypes.c_int

    # void ReadSharedMemory( const char* , char* );
    __read_shmem = __sharedObj.ReadSharedMemory
##    __read_shmem.restype = ctypes.c_int

    __Shared_Memory_Name_pointer =b''
    Buffer = ''
    ret_value = ctypes.c_int()
    
    def __init__(self,**args):
        ## Size, Name, create = True/False
        max_size = 65535
        create=True
        Shared_Memory_Name = b'P0WSharedMem'
        
        for _ in args:
            if _.lower() in 'size':
                max_size = args[_]
            elif _.lower() in 'create':
                create=args[_]
            elif _.lower() in 'name':
                Shared_Memory_Name =args[_]
                
        if type(Shared_Memory_Name) == str:
            Shared_Memory_Name = Shared_Memory_Name.encode('UTF-8')
            
        self.__Shared_Memory_Name_pointer = ctypes.c_char_p(Shared_Memory_Name)
        self.Buffer = ctypes.create_string_buffer(max_size) # create a max_size byte buffer, initialized to NULL bytes
        if create:
            self.ret_value = self.__create_shmem( self.__Shared_Memory_Name_pointer,
                                         ctypes.c_longlong(max_size) )
            print ('Created %s Shared Memory with %d bytes' %
                   (self.__Shared_Memory_Name_pointer.value, max_size ) )
    
    def __del__( self ):
        ## Destructor
        ## !!! Important !!!
        ## This might cause excessive memory/resource leaks, if its didn't invoke properly
        
        print ('Deallocating Memory %s ...' %
               self.__Shared_Memory_Name_pointer.value)
        self.ret_value = self.__remove_shmem( self.__Shared_Memory_Name_pointer             # const char* 
                            )
    

    def Write( self, Write_Chars ):
        if type(Write_Chars) == str :
            Write_Chars = Write_Chars.encode('UTF-8')
        
        self.ret_value = self.__write_shmem( self.__Shared_Memory_Name_pointer,             # const char* 
                                        ctypes.create_string_buffer(Write_Chars)            # a buffer containing a NUL terminated string (char* )
                                       ) 

    def Write_2(self, Write_Chars):
        self.ret_value = self.__write_shmem( self.__Shared_Memory_Name_pointer,             # const char* 
                                             Write_Chars                                    # a buffer containing a NUL terminated string (char* )
                                       ) 
    def Read( self ):
        self.ret_value = self.__read_shmem( self.__Shared_Memory_Name_pointer,              # const char* 
                                       self.Buffer                                          # a buffer containing a NUL terminated string (char* )
                                      )
    def Read_2( self , Read_Chars):
        self.ret_value = self.__read_shmem( self.__Shared_Memory_Name_pointer,              # const char* 
                                       Read_Chars                                              # a buffer containing a NUL terminated string (char* )
                                      )
      
##SH=Shared_Memory_Wrapper(size=4,name='P0W', create=True)
##SH.Write('6') ## 56
##SH.Read()
##SH.Buffer.value #Displays Read Value
##SH.ret_value # Return Status of last operation
##del SH ## SH=0 Don't forget to deallocate
