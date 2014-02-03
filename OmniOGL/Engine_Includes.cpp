/*
    Engine_Includes.cpp
    Source File for Global/General Functions & Variables
    02/26/2013
*/

//--- Includes ---//
#include "Engine_Includes.h"
namespace OmniOGL
{
#pragma region Global Variable Definitions
//--- Global Variables ---//
// ~ Window Properties
///<summary>Simple pair of GLints which represent the width and height, respectively, of the target window.</summary>
GLint winsize[2]={1024,768};
// ~ Memory Allocation
///<summary>Represents how many bytes are allocated at a given time. This variable is only used if 
/// <c>\#define RECORD_MEMORY</c> is set to a non-zero number.</summary>
signed long int bytes_allocated=0;
/// <summary>Represents the maximum number of bytes that were allocated at any point since execution began.
/// This variable is only used if <c>\#define RECORD_MEMORY</c> is set to a non-zero number.</summary>
signed long int max_allocated=0;
#pragma endregion
#pragma region Global Function Definitions
//--- Global Functions ---//
/// <summary>A "clone" of the well-known <c>printf()</c> function, with the debug routed to the
/// <c>OutputDebugString()</c> function, to be output to the console.</summary>
/// <param name="format">A character array which holds the string which is to-be-output.
/// This string can reference additional variables via the '%' character.</param>  
/// <param name="...">Any number of parameters which are referenced in the first parameter, the format string.
/// This function will iterate through each argument and place its contents in the format string, as denoted in
/// the original format string.</param>
/// <returns>This function does not return a value.</returns>
/// <remarks>This function cannot be compiled as managed code. This function is intended for
/// development purposes only and should be "disabled" (commented-out) for final release builds.</remarks>
void __cdecl dprint(const GLchar *format, ...)
{
    // --- Construct String --- //
    GLchar buf[8192], *p = buf;
    // ~ Parse Arguements
    va_list args;
    va_start(args, format);
    int n = _vsnprintf(p, sizeof buf - 3, format, args); // buf-3 is room for CR/LF/NUL
    va_end(args);
    p += (n < 0) ? sizeof buf - 3 : n;
    // ~ Fill Blankspace w/ Nulls
    while (p>buf && isspace(p[-1]))
        *--p = '\0';
    // ~ "Attach Rear" to String
    *p++ = '\r';
    *p++ = '\n';
    *p   = '\0';
    // --- Output String --- //
    OutputDebugString(buf);
    return;
}
/// <summary>If <c>\#define RECORD_MEMORY</c> is set to a non-zero number, this function will return
/// the number of bytes allocated via <c>_New()</c> at that point in time.</summary>
/// <returns>Returns the number of bytes allocated via <c>_New()</c> at that moment, via a signed long integer.
/// If <c>\#define RECORD_MEMORY</c> is set to zero, this function will return 0.</returns>
/// <remarks></remarks>
signed long int GetBytesAllocated(void)
{
    return bytes_allocated;
}
/// <summary>If <c>\#define RECORD_MEMORY</c> is set to a non-zero number, this function will return
/// the maxmimum \# of bytes allocated at any one time prior to the call.</summary>
/// <returns>Returns the maxmimum number of bytes allocated at any one point in time prior to calling this function.
/// If <c>\#define RECORD_MEMORY</c> is set to zero, this function will return 0.</returns>
/// <remarks></remarks>
signed long int GetMaxAllocated(void)
{
    return max_allocated;
}
/// <summary>Returns the contents of a file as a string.</summary>
/// <param name="file_in">The name of the file to read. If no absolute path is specified, the file must be located
/// in the working directory of the program.</param>
/// <returns>Returns an std::string which holds the entire contents of the desired file. If there is an error then
/// a blank string is returned.</returns>
/// <remarks>Used to read the contents of the Vertex and Fragment Shader code, and to read the list of vertices needed
/// to form a <c>ComplexObj</c>.</remarks>
std::string GetFileContents(std::string file_in)
{
    FILE* this_file=nullptr;
    std::string filebuff="";
    GLchar tempbuff[_SMALLBUF]="\0";
    if(fopen_s(&this_file,file_in.c_str(),"rt")!=0 || this_file==nullptr || this_file==0)
        return "";
    while(fgets(tempbuff,sizeof(tempbuff),this_file) != NULL)
    {
        filebuff+=tempbuff;
    }
    fclose(this_file);
    return filebuff;
}
/// <summary>Prints to the console all "recorded" memory allocations which have not yet been subject to <c>_Delete()</c>.</summary>
/// <returns>Returns the value 0 as an integer in all cases.</returns>
/// <remarks>If run at the end of the program immediately before the program is terminated, the function should NOT print anything
/// to the console. If it does, then the source code does not have a matching <c>_Delete()</c> for every <c>_New()</c>.</remarks>
int DumpMemoryLeaks(void)
{
    std::map<void*,memory_info>::iterator i;
    dprint("Number of pointer entries left-over: %u\n",allocated_memory.size());
    for(i=allocated_memory.begin(); i!=allocated_memory.end(); i++)
    {
        dprint("Pointer: %i| Size: %u | Note: %s\n",int((*i).first),(*i).second.size,(*i).second.info.c_str());
    }
    return _OK;
}
#pragma endregion
}