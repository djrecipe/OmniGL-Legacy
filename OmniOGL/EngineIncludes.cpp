#pragma region Includes
#include "EngineIncludes.h"
#pragma endregion
namespace OmniOGL
{
#pragma region Math
	glm::vec3 OGLMath::Normal(glm::vec3 vec0_in, glm::vec3 vec1_in, bool normalize)
	{
		glm::vec3 retval = glm::cross(vec0_in, vec1_in);
		return (normalize && glm::length(retval)>0.0) ? retval / glm::length(retval) : retval;
	}
	GLfloat OGLMath::Angle(glm::vec3 vec0_in, glm::vec3 vec1_in)
	{
		return glm::degrees(glm::acos(glm::dot(vec0_in, vec1_in) / (glm::length(vec0_in)*glm::length(vec1_in))));
	}
#pragma endregion
#pragma region Global Variable Definitions
// ~ Window Properties
///<summary>Simple pair of GLints which represent the width and height, respectively, of the target window.</summary>
GLint winsize[2]={1500,844};
#pragma endregion
#pragma region Global Function Definitions
#pragma region Debugging
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
/// <summary>Returns the contents of a file as a string.</summary>
/// <param name="file_in">The name of the file to read. If no absolute path is specified, the file must be located
/// in the working directory of the program.</param>
/// <returns>Returns an std::string which holds the entire contents of the desired file. If there is an error then
/// a blank string is returned.</returns>
/// <remarks>Used to read the contents of the Vertex and Fragment Shader code, and to read the list of vertices needed
/// to form a <c>BlenderObj</c>.</remarks>
#pragma endregion
std::string GetFileContents(const std::string file_in)
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
GLfloat * factor_vec4(GLfloat * vec4_in, const GLfloat & factor_in)
{
    GLfloat protected_factor=0.0;
    if(factor_in>1.0)
        protected_factor=1.0;
    else if(factor_in>0.0)
        protected_factor=factor_in;

    for(GLint i=0; i<4; i++)
    {
        if(i==3)
            vec4_in[i]=1.0;
        else
            vec4_in[i]=protected_factor;
    }
    return vec4_in;
}
size_t TypeSize(GLenum type)
{
    size_t size;
    #define CASE(Enum,Count,Type) \
    case Enum: size = Count * sizeof(Type); break
    switch (type) {
        CASE(GL_FLOAT, 1, GLfloat);
        CASE(GL_FLOAT_VEC2, 2, GLfloat);
        CASE(GL_FLOAT_VEC3, 3, GLfloat);
        CASE(GL_FLOAT_VEC4, 4, GLfloat);
        CASE(GL_INT, 1, GLint);
        CASE(GL_BOOL, 1, GLboolean);
    #undef CASE
    default:
        assert(1==2);
        break;
    }
    return size;
}
#pragma endregion
}