#include "OGLEngine.h"
namespace OmniOGL
{
//--- Global Variables ---//
// ~ Input
//HHOOK hookMouse; HHOOK hookKeyboard;
bool glfwinit=false;
// Text
//--- Function Definitions ---//
// ~ Initialization
GLint MakeWindow(GLchar* title,GLint width,GLint height)
{
    // --- Variables --- //
    winsize[0]=width; winsize[1]=height;
    // --- GLFW Configuration --- //
    if(!glfwinit)
    {
        glfwInit();
        glfwinit=true;
    }
    // Set GLFW 'Hints'
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR,3);  // OpenGL v3.2
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR,2);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_COMPAT_PROFILE );
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE,GL_TRUE);
    // Initialize Window
    if(glfwOpenWindow( width,height,RED_DEPTH,GRN_DEPTH,BLU_DEPTH,
                    ALPHA_BUF,DEPTH_BUF,STENC_BUF,WINMODE)==GL_FALSE)
    {
            dprint("Attempt to create window context using GLFW has failed.\n");
            return _BAD;
    }
    glfwSetWindowTitle(title);
	// Hide Cursor
    glfwDisable(GLFW_MOUSE_CURSOR);
	// Handle Input Events
    glfwSwapInterval(1);
    if(!SetCursorPos((GLint)(winsize[0]/2.0f),(GLint)(winsize[1]/2.0f)))
    {
        dprint("Call to 'SetCursorPos' failed with error %x.\n",GetLastError());
        return _BAD;
    }
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_BLEND);
	//oglew=GetForegroundWindow();

    return _OK;
}
GLint MyGLEWInit(void)
{
	// ~ Init GLEW
	glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      dprint("GLEW initialization failed with error '%s'.\n",glewGetErrorString(err));
      return _BAD;
    }
    if (!glewIsSupported("GL_VERSION_3_2"))
    {
        dprint("Target system does not support OpenGL v3.2.\n");
        return _BAD;
    }
    return _OK;
}

}
