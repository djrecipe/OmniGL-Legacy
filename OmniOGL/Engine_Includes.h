/*
    Engine_Includes.h
    Header File for Global/General Functions & Variables
    02/26/2013
*/
#pragma once
#ifndef ENGINE_INCLUDES
#pragma region Preprocesser Defines
//--- Defines ---//
///<summary>Include guard for 'Engine_Includes.h'.</summary>
#define ENGINE_INCLUDES
// ~ Function Returns
///<summary>Indicates that either an error has occured, the returning function failed,
///a solution was not found, or the element is intended to be "non-existant" or "blank".</summary>
#define _BAD         -1
///<summary>Indicates that the returning function succeeded. Generally indicates a "positive" or
///"affirmative" outcome.</summary>
#define _OK           0
// ~ Buffer Sizes
///<summary>Determines the maximum number of Vertex Attribute Objects which can be created. VAOs
///are used to quickly set-up/swap various vertex attribute configurations.</summary>
#define MAX_VAO       2
///<summary>Determines the maximum number of Vertex Buffer Objects which can be created. VBOs are
///one of the primary means of sending large amounts of data to the graphics card.</summary>
#define MAX_VBO      32
///<summary>Determines the maximum number of Element Buffer Objects which can be created. Not
///currently being utilized as of 02/24/2013.</summary>
#define MAX_EBO      32
///<summary>Determines the maximum number of different textures which can be loaded into memory.</summary>
#define MAX_TEX      32
///<summary>A somewhat inconsequential size parameter for functions which read from a file. This buffer
///size essentially represents the "chunk" size which is processed each loop iteration.</summary>
#define _SMALLBUF   256
// ~ Math Constants
/// <summary>A crude representation of the trigonometric constant, PI. For the lazy only.</summary>
#define PI 3.14159265
// ~ OpenGL Config
/// <summary>Should be set to '1' if using the GFLW DLL file.</summary>
#define GLFW_DLL
/// <summary>Prevents the GLFW library from including <c>glu.h</c>, since it is already included by
/// the GLEW library.</summary>
#define GLFW_NO_GLU
/// <summary>Tells the GLFW library to enable OpenGL 3.0+ functionality</summary>
#define GLFW_INCLUDE_GL3
// ~ Project-Wide Flags
/// <summary>Enables "logging" of memory acquisitions and releases via the '_New' and
/// '_Delete' templated functions. <para>This functionality is somewhat of a hack, and should only
/// be used for debugging purposes only. Incurrs a performance loss.</para></summary>
#define RECORD_MEMORY 0
/// <summary>Enables rendering of the "grid lines" which denote the boundaries of each <seealso cref="Section"/> class.</summary>
#define EN_GRIDLINES 1
/// <summary>Enables a custom smoothing algorithm that finds which normals share edges, then averages them together.</summary>
#define SMOOTH_NORMALS 1
#pragma endregion
#pragma region Linker Options
//--- Linker Options ---//
//#pragma comment(linker, "/NODEFAULTLIB:MSVCRTD")
#pragma endregion
#pragma region .lib Inclusion
//--- Library Includes ---//
#pragma comment(lib,"SOIL.lib")                     // Importing Images
#pragma comment(lib,"GLFWDLL.lib")                  // Windows window context
#pragma comment(lib,"opengl32.lib")                 // OpenGL
#pragma comment(lib,"glew32.lib")                   // OpenGL Extensions Wrapper
#pragma comment(lib,"irrKlang.lib")                 // 3D Audio
#pragma endregion
#pragma region Header File Inclusion
//--- C++ Includes ---//
// ~ Standard Libraries
#include <fstream>                                  // file input (for vertex/fragment shaders)
#include <list>                                     // STL container, list (fast modifications, slow random access)
#include <map>                                      // STL container, map (convenient index using keys)
#include <memory>                                   // for shared_ptrs
#include <sstream>                                  // string stream
#include <string>                                   // strings!
#include <time.h>                                   // time keeping / time differentials
#include <vector>                                   // STL container, vector (just a managed array)
#include <windows.h>                                // debug console output
// ~ Non-Standard Libraries
#include <GL/glew.h>                                // OpenGL extensions wrapper (must-have)
#include <glm/glm.hpp>                              // matrices, vectors, essential math, noise, etc.
#include <glm/gtc/matrix_transform.hpp>             // provides OpenGL matrix transform functions, such as 'lookAt'
#include <glm/gtc/type_ptr.hpp>                     // for sending matrices through the pipeline (probably not necessary)
#include <GL/glfw.h>                                // initial window creation
#include <SOIL/SOIL.h>                              // image loading (redundant functionality with GLFW)
#include <irK/irrKlang.h>                           // audio
#include <irK/ik_vec3d.h>                           // for 3D operations on audio
#pragma endregion
///<summary>The primary namespace which encompases all the functions and variables necessary to render and manipulate
///a scene using OpenGL. This namespace spans several files and deals not only with the issue of rendering, but also
///the issue of user input, environment generation and interaction, and [eventually] physics.</summary>
namespace OmniOGL
{
#pragma region Custom Types
//--- Custom Types ---//
// ~ Portability
///<summary>Simply a wrapper around the built-in type, 'unsigned char'. Feigned to appear like one
///of the native OpenGL cross-platform types.</summary>
typedef unsigned char GLuchar;
// ~ Memory Management
///<summary>Structure which is only used when <c>\#define RECORD_MEMORY</c> is set to a non-zero number.</summary>
struct memory_info
{
    ///<summary>The size (in bytes) of the allocation.</summary>
    GLuint size;
    ///<summary>A string associated with the allocation.
    ///<para>By default this will be the name of the type which the allocation was for.</para></summary>
    std::string info;
};
#pragma endregion
#pragma region Global Variable Declarations
//--- Global Variables ---//
// ~ Window Config
extern GLint winsize[2];
/// <summary>Records memory allocations into a container which uses the pointer address as a key.
/// The allocation size is recorded in bytes, and a custom string is stored for each key.
/// This variable is only used if <c>\#define RECORD_MEMORY</c> is set to a non-zero number.</summary>
static std::map<void*,memory_info> allocated_memory;
extern long int bytes_allocated;
extern long int max_allocated;
#pragma endregion
#pragma region Global Function Declarations
//--- Global Functions (Prototypes) ---//
// ~ Debugging
void __cdecl dprint(const GLchar *format="> DEBUG\n", ...);
signed long int GetBytesAllocated(void);
signed long int GetMaxAllocated(void);
std::string GetFileContents(std::string);
GLint DumpMemoryLeaks(void);
// ~ Memory Allocation (Templated)
template <typename T>
void _Init(T *&);
template <typename T>
signed long int _New(T*&,GLuint,std::string);
template <typename S,typename T>
signed long int _Copy(S*&,T*&);
template <typename T>
signed long int _Delete(T*&);
template <typename T>
signed long int _Size(T*&);
#pragma endregion
#pragma region Templated Global Function Definitions
//--- Templated Function Definitions ---//
template <typename T>
/// <summary>Sets the pointer to the value -1.</summary>
/// <param name="pointer">The pointer which will be set to the value -1.</param>
/// <returns>This function does not return a value (void).</returns>
/// <remarks>This wrapper of a function simply exists for clarity, and as a potential sanity
/// check if <c>\#define RECORD_MEMORY</c> is set to a non-zero number, in which case <c>_New()</c>
/// will only accept pointers which are assigned the value of -1.</remarks>
void _Init(T *& pointer)
{
    pointer=(T*)_BAD;
    return;
}
template <typename T>
/// <summary>A wrapper around the <c>new[]</c> keyword. Allocates the desired number of elements to the given pointer.
/// Pointers allocated via this function should be deleted using <c>_Delete()</c>. Even in the case of 1 element, <c>new[]</c>
/// is used instead of <c>new</c>.</summary>
/// <param name="pointer">A pointer reference at which memory will be dynamically allocated</param>
/// <param name="size_in">An unsigned integer which determines how many elements the resulting array can hold.
/// Note that even if this number is 1, the <c>new []</c> operator is still used</param>
/// <param name="info_in">On applicable if <c>\#define RECORD_MEMORY</c> is not set to 0.
/// An optional string to help denote the allocation. Defaults to the name of the type of the pointer</param>
/// <returns>If <c>\#define RECORD_MEMORY</c> is set to a non-zero number and the memory address referenced by the "pointer" parameter
/// is found in <c>allocated_memory</c>, returns the value -1. If the previous statement(s) fail to result in a return statement and the
/// "pointer" parameter is not equal to the value -1 or the "size_in" parameter is equal to zero, returns the value -1. 
/// If the previous statement(s) fail to result in a return statement and <c>\#define RECORD_MEMORY</c> is set to a non-zero number 
/// and the pointer is not found in <c>allocated_memory</c>, returns the number of bytes allocated at that moment, post-allocation, 
/// as a signed long integer. In all other cases returns the value 0.</returns>
/// <remarks>If <c>\#define RECORD_MEMORY</c> is set to a non-zero number, the pointer address is stored as the key to an STL map element.
/// The value associated with the pointer address is a structure which holds the number of bytes allocated and a string (the 3rd optional
/// parameter to this function).</remarks>
signed long int _New(T *& pointer, GLuint size_in, std::string info_in=typeid(T).name())
{
    // ~ Check for Existing Allocation
    if(RECORD_MEMORY!=0)
    {
        std::map<void*,memory_info>::iterator i;
        i=allocated_memory.find((void*)pointer);
        if(i!=allocated_memory.end())
        {
            dprint("Potential '%s' pointer is already allocated as type '%s'.",typeid(T).name(),allocated_memory[(void*)pointer].info.c_str());
            //assert(0==1);
            return _BAD;
        }
    }
    if(pointer!=(T*)_BAD)
    {
        dprint("Potential %s pointer already allocated, but allocation was not documented.\nTry calling '_Init()' on your pointer first.",typeid(T).name());
        //assert(1==2);
        return _BAD;
    }
    // ~ Check for Anomolies
    if(size_in==0)
    {
        pointer=(T*)_BAD;
        return _BAD;
    }
    pointer=new T[size_in];
    // ~ Remember New Allocation
    if(RECORD_MEMORY!=0)
    {
        memory_info mem_info;
        mem_info.info=info_in;
        mem_info.size=size_in*sizeof(T);
        allocated_memory[(void*)pointer]=mem_info;
        bytes_allocated+=mem_info.size;
        if(bytes_allocated>max_allocated)
            max_allocated=bytes_allocated;
        return bytes_allocated;
    }
    return _OK;
}
template <typename S,typename T>
/// <summary>Makes a copy of the second parameter into dynamically allocated memory at the first parameter, 
/// an operation which invokes the copy constructor of the target class. This function is NOT to be used with arrays.</summary>
/// <param name="destination">A pointer reference at which memory will be dynamically allocated by copying the class pointed 
/// to by the other (second) parameter labeled *source*.</param>
/// <param name="source">A pointer reference from which a copy will be made and placed into dynamically allocated memory at the address
/// represented by the other (first) parameter labeled *destination*.</param>
/// <returns>If the "source" parameter is equal to the value -1, returns -1. If the previous statement(s) fail to result in a return and
/// <c>\#define RECORD_MEMORY</c> is set to a non-zero number, returns the number of bytes allocated at that moment as a signed long integer.
/// In all other cases returns the value 0.</returns>
/// <remarks>If <c>\#define RECORD_MEMORY</c> is set to a non-zero number, the pointer address is stored as the key to an STL map element.
/// The value associated with the pointer address is a structure which holds the number of bytes allocated and a string which holds the
/// literal, "copy".</remarks>
signed long int _Copy(S *& destination, T *& source)
{
    // ~ Invoke Copy Constructor
    if(source==(T*)_BAD)
    {
        destination=(S*)_BAD;
        return _BAD;
    }
    destination=new T(*source);
    // ~ Remember Allocation
    if(RECORD_MEMORY!=0)
    {
        bytes_allocated+=sizeof(T);
        memory_info mem_info;
        mem_info.size=sizeof(T);
        mem_info.info="copy";
        allocated_memory[(void*)destination]=mem_info;
        return bytes_allocated;
    }
    return _OK;
}
template <typename T>
/// <summary>A wrapper for the <c>delete[]</c> keyword. Always uses <c>delete[]</c>, and never <c>delete</c>.</summary>
/// <param name="pointer">The pointer reference which will be deleted and set to -1.</param>
/// <returns>If <c>\#define RECORD_MEMORY</c> is set to a NON-zero number and the memory address referenced by the "pointer"
/// parameter is found in <c>allocated_memory</c>, returns the number of bytes allocated at that moment post-delete operation.
/// If <c>\#define RECORD_MEMORY</c> is set to a NON-zero number and the memory address referenced by the "pointer" parameter is
/// NOT found in <c>allocated_memory</c>, returns the value -1. If <c>\#define RECORD_MEMORY</c> is set to zero, returns the value
/// of zero. If the pointer is set to the value -1 then this function returns the value -1, regardless of all other [previously listed] circumstances.</returns>
/// <remarks>If <c>\#define RECORD_MEMORY</c> is set to a non-zero number, the pointer-to-be-deleted MUST had been previously
/// allocated by the <c>_New()</c> function and not yet <c>_Delete()</c> in order for the <c>delete[]</c> call to be made.</remarks>
signed long int _Delete(T *& pointer)
{
    if(pointer==(T*)_BAD)
    {
        //dprint("Pointer is bad, cannot delete.");
        return _BAD;
    }
    if(RECORD_MEMORY!=0)
    {
        // ~ Find Allocation
        std::map<void*,memory_info>::iterator i;
        i=allocated_memory.find((void*)pointer);
        if(sizeof(T)>=140)
            dprint("break");
        if(i==allocated_memory.end())
        {
            dprint("Pointer not found. (object size=%u; name=%s)",sizeof(T),typeid(T).name());
            //assert(2==3);
            return _BAD;
        }
        else
        {
            // ~ Forget Allocation
            bytes_allocated-=allocated_memory[(void*)pointer].size;
            allocated_memory.erase((void*)pointer);
        }
    }
    // ~ Free Memory
    delete[] pointer;
    pointer=(T*)_BAD;

    return bytes_allocated;
}
template <typename T>
/// <summary>Returns the number of bytes allocated to the given pointer,
/// only if <c>\#define RECORD_MEMORY</c> is set to a non-zero number. Returns the value <c>-1</c> if <c>\#define RECORD_MEMORY</c> is set to zero.</summary>
/// <param name="pointer">A pointer reference whose address will be used as a key into a STL map.</param>
/// <returns>If <c>\#define RECORD_MEMORY</c> is set to a non-zero number, returns the size of the allocation 
/// at the memory location denoted by the parameter labeled "pointer" via a signed long integer. If <c>\#define RECORD_MEMORY</c> is
/// set to zero, returns the value <c>-1</c>.</returns>
/// <remarks>The pointer MUST had been dynamically allocated via <c>_New()</c> and not yet been subject to <c>_Delete()</c>.</remarks>
signed long int _Size(T *& pointer)
{
    // ~ Find Allocation
    if(RECORD_MEMORY!=0)
    {
        std::map<void*,GLint>::iterator i;
        i=allocated_memory.find((void*)pointer);
        if(i==allocated_memory.end())
            return _BAD;
        return allocated_memory[(void*)pointer];
    }
    return _BAD;
}
#pragma endregion
}
#endif