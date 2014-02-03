#pragma once
#pragma region Preprocesser Defines
#pragma region Function Returns
///<summary>Indicates that either an error has occured, the returning function failed,
///a solution was not found, or the element is intended to be "non-existant" or "blank".</summary>
#define _BAD         -1
///<summary>Indicates that the returning function succeeded. Generally indicates a "positive" or
///"affirmative" outcome.</summary>
#define _OK           0
#pragma endregion
#pragma region Buffer Sizes
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
#define _BUFSIZ   30000
#pragma endregion
#pragma region Constants
/// <summary>A crude representation of the trigonometric constant, PI. For the lazy only.</summary>
#define PI 3.14159265
#pragma endregion
#pragma region Memory Configuration
#define _VARIADIC_MAX 10
#pragma endregion
#pragma region OpenGL Configuration
/// <summary>Should be set to '1' if using the GFLW DLL file.</summary>
#define GLFW_DLL
/// <summary>Prevents the GLFW library from including <c>glu.h</c>, since it is already included by
/// the GLEW library.</summary>
#define GLFW_NO_GLU
/// <summary>Tells the GLFW library to enable OpenGL 3.0+ functionality</summary>
#define GLFW_INCLUDE_GL3
#pragma endregion
#pragma region Flags
#define EN_GRIDLINES 0
#pragma endregion
#pragma endregion
#pragma region Linker Options
//#pragma comment(linker, "/NODEFAULTLIB:MSVCRTD")
#pragma endregion                                           
#pragma region Library Inclusion
#pragma comment(lib,"SOIL.lib")                     // Importing Images
#pragma comment(lib,"GLFWDLL.lib")                  // Windows window context
#pragma comment(lib,"opengl32.lib")                 // OpenGL
#pragma comment(lib,"glew32.lib")                   // OpenGL Extensions Wrapper
#pragma comment(lib,"irrKlang.lib")                 // 3D Audio
#pragma comment(lib,"FreeImage.lib")
#pragma comment(lib,"sqlite3.lib")
#pragma endregion
#pragma region Header File Inclusion
#pragma region Standard Libraries
#include <iostream>
#include <fstream>                                  // file input (for vertex/fragment shaders)
#include <functional>
#include <list>                                     // STL container, list (fast modifications, slow random access)
#include <map>                                      // STL container, map (convenient index using keys)
#include <memory>                                   // for shared_ptrs
#include <mutex>
#include <sstream>                                  // string stream
#include <string>                                   // strings!
#include <sqlite3.h>
#include <thread>
#include <time.h>                                   // time keeping / time differentials
#include <vector>                                   // STL container, vector (just a managed array)
#include <windows.h>                                // debug console output
#pragma endregion
#pragma region Non-Standard Libraries
#include <FreeImage.h>
#include <GL/glew.h>                                // OpenGL extensions wrapper (must-have)
#include <glm/glm.hpp>                              // matrices, vectors, essential math, noise, etc.
#include <glm/gtc/matrix_transform.hpp>             // provides OpenGL matrix transform functions, such as 'lookAt'
#include <glm/gtc/random.hpp>                       // for controlled random number functions
#include <glm/gtc/type_ptr.hpp>                     // for sending matrices through the pipeline (probably not necessary)
#include <GL/glfw.h>                                // initial window creation
#include <irK/irrKlang.h>                           // audio
#include <irK/ik_vec3d.h>                           // for 3D operations on audio
#include <SOIL/SOIL.h>
#pragma endregion
#pragma endregion
///<summary>The primary namespace which encompases all the functions and variables necessary to render and manipulate
///a scene using OpenGL. This namespace spans several files and deals not only with the issue of rendering, but also
///the issue of user input, environment generation and interaction, and [eventually] physics.</summary>
namespace OmniOGL
{
#pragma region Math
static class OGLMath
{
public:
	static glm::vec3 Normal(glm::vec3 vec0_in, glm::vec3 vec1_in, bool normalize = false);
	static GLfloat Angle(glm::vec3 vec0_in, glm::vec3 vec1_in);
};
#pragma endregion
#pragma region Custom Types
///<summary>Simply a wrapper around the built-in type, 'unsigned char'. Feigned to appear like one
///of the native OpenGL cross-platform types.</summary>
typedef unsigned char GLuchar;
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
#pragma region Forward Declarations
class RenObj; class Section; class Scene; class Camera; class OGLMatrices;
#pragma endregion
#pragma region Global Variables
extern GLint winsize[2];
#pragma endregion
#pragma region Global Function Declarations
#pragma region Debugging
void __cdecl dprint(const GLchar *format="> DEBUG\n", ...);
std::string GetFileContents(const std::string file_in);
#pragma endregion
/// <summary>A simple method which returns four floats in an array, the first three of which will contain the same number - a value between 0.0 and 1.0. The
/// fourth element will always be 1.0. This method is used primarily for lighting calculations - it provides an easy way to produce a diffuse or specular
/// component (which is a material property, NOT a light source property) which is homogenous across all colors (i.e. grayscale).
/// <seealso cref="RenObj::Prepare()"/></summary>
/// <param name="vec4_in">An array of four floats to which the factor will be copied. The memory is owned and initialized by the caller.</param>
/// <param name="factor_in">A GLfloat which will be copied to all four elements of the target array. When used in lighting calculations the result is materials which
/// "reflect" light uniformly.</param>
/// <returns>In order to assist the caller, this method returns the pointer (first parameter) passed to it.</returns>
/// <remarks>Currently the diffuse and specular components of any given rendered object are stored as a single floating point number. This function can be
/// used to convert the one number into the four floating point numbers required by the shader program to wholey describe the lighting characteristics
/// of the object/material. In the future this function may be deperecated should it be desired to be able to tweak the specular and diffuse components
/// of each color seperately.</remarks>
GLfloat * factor_vec4(GLfloat * vec4_in,const GLfloat & factor_in);
size_t TypeSize(GLenum type);
// ~ Memory Allocation (Templated)
template <typename T>
void _Init(T *&);
template <typename T>
bool _New(T *& pointer, GLuint size_in=1);
template <typename T>
bool _Delete(T*&);
template <typename T>
bool _Valid(T*&);
template <typename T>
bool _Valid(T* const &);
#pragma endregion
#pragma region Templated Global Function Definitions
#pragma region Memory Management
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
bool _Valid(T*& pointer)
{
    if(pointer==(T*)_BAD)
        return false;
    return true;
}
template <typename T>
bool _Valid(T * const & pointer)
{
    if(pointer==(T*)_BAD)
        return false;
    return true;
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
bool _New(T *& pointer, GLuint size_in)
{
    assert(pointer==(T*)_BAD);
    // ~ Check for Anomolies
    if(size_in==0)
    {
        pointer=(T*)_BAD;
        return false;
    }
    pointer=new T[size_in];
    return true;
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
bool _Delete(T *& pointer)
{
    if(pointer==(T*)_BAD)
        return false;
    // ~ Free Memory
    delete[] pointer;
    pointer=(T*)_BAD;

    return true;
}
#pragma endregion
#pragma endregion
}