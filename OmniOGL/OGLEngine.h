#ifndef OGLENGINE_H
#define OGLENGINE_H
#pragma region Includes
#include "Scene.h"
#pragma endregion
namespace OmniOGL
{
#pragma region Preprocessor Defines
// ~ Window Buffer Properties
/// <summary>The resolution of the red component of color, in bits. <seealso cref="MakeWindow()"/></summary>
/// <remarks>If set to 0, the resolution of the red component of color on the desktop will be used.</remarks>
#define RED_DEPTH     0
/// <summary>The resolution of the green component of color, in bits. <seealso cref="MakeWindow()"/></summary>
/// <remarks>If set to 0, the resolution of the green component of color on the desktop will be used.</remarks>
#define GRN_DEPTH     0
/// <summary>The resolution of the blue component of color, in bits. <seealso cref="MakeWindow()"/></summary>
/// <remarks>If set to 0, the resolution of the blue component of color on the desktop will be used.</remarks>
#define BLU_DEPTH     0
/// <summary>The resolution of the alpha component of color, in bits. <seealso cref="MakeWindow()"/></summary>
/// <remarks>If set to 0, the resolution of the alpha component of color on the desktop will be used.</remarks>
#define ALPHA_BUF     0
/// <summary>The resolution of the Depth Buffer, in bits. <seealso cref="MakeWindow()"/></summary>
#define DEPTH_BUF    24
/// <summary>The resolution of the Stencil Buffer, in bits. <seealso cref="MakeWindow()"/></summary>
#define STENC_BUF     8
/// <summary>A preprocesor "flag" which can either be set to GLFW_WINDOW or GLFW_FULLSCREEN. The state of this preprocessor "flag" determines whether or not OpenGL is fullscreen or windowed.
/// <seealso cref="MakeWindow()"/></summary>
/// <remarks>Valid values are GLFW_WINDOW and GLFW_FULLSCREEN.</remarks>
#define WINMODE  GLFW_WINDOW
#pragma endregion
#pragma region Function Declarations
// ~ Initialization
/// <summary>A method which calls upon various methods from an external library (GLFW) to create a Windows context to draw on. This method determines various properties of the window such as its resolution
/// and whether or not it is fullscreen. This method is also responsible for setting-up event handlers / function hooks for handling user input.
/// <seealso cref="MyGLEWInit()"/><seealso cref="WinMain()"/></summary>
/// <param name="title">A character array which will determine the text on the title bar of the window. This parameter is optional and will default to "OGL Engine".</param>
/// <param name="width">An integer specifying the width of the window (if in windowed mode) or the width of the target resolution if in fullscreen mode. This parameter is optional and will default to
/// the first element in <c>winsize</c>.</param>
/// <param name="height">An integer specifying the height of the window (if in windowed mode) or the height of the target resolution if in fullscreen mode. This parameter is optional and will default to
/// the second element in <c>winsize</c>.</param>
/// <returns>An integer indicating the success of the method - _OK (0) if a window context was created and configured correctly, or _BAD (-1) if errors occured during the process.</returns>
/// <remarks>This method calls <c>glfwInit()</c> prior to setting up the window. This method should be called prior to any other initialization methods (and obviously prior to starting the rendering
/// loop).</remarks>
GLint MakeWindow(GLchar* title="OGL Engine",int width=winsize[0],int height=winsize[1]);
/// <summary>A function which sets up the external GLEW library which provides various function wrappers for OpenGL extensions. This function is essential if any functionality beyond OpenGL 1.1 is
/// desired, which is obviously always will be. This function also does some additional logic checks to ensure the desired OpenGL version is supported and also that <c>glewInit()</c> reported no errors.</summary>
/// <returns>An integer representing the success of the function. Returns _OK (0) if GLEW was initialized sucessfully and the desired OpenGL version / extensions are supported - returns _BAD (-1) otherwise.</returns>
/// <remarks>Without this function, function pointers to OpenGL extensions can be found via <c>wglGetProcAddress()</c>.</remarks>
GLint MyGLEWInit(void);
// ~ Closure
/// <summary>A function which returns a boolean variable which tells the render loop whether or not to continue (each frame). Raising the flag associated with this function will cause the program
/// to exit (hopefully gracefully). <seealso cref="exitbool"/><seealso cref="evnt_Keybrd()"/></summary>
/// <returns>A boolean value, or more specifically the boolean value stored in <c>exitbool</c>.</returns>
/// <remarks>Currently the member associated with this function is initialized to <c>False</c> but then set to <c>True</c> upon pressing a certain keyboard key.</remarks>
bool ExitQuery(void);
#pragma endregion
}
#endif