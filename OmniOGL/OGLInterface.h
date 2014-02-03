#ifndef OGLINTERFACE_H
#define OGLINTERFACE_H
#pragma region Includes
#include "EngineIncludes.h"
#pragma endregion
namespace OmniOGL
{
#pragma region Type Definitions
/// <summary>A shared pointer to a <c>RenObj</c> class. Using shared pointers to store all of our rendering objects will allow us to have
/// multiple pointers to the same object which willl automatically managing the memory for us.</summary>
typedef std::shared_ptr<RenObj> ptrRenObj;
typedef std::shared_ptr<Section> ptrSection;
#pragma endregion
#pragma region Shader Program Classes
#pragma region OGLAttributes Class
/// <summary>Provides a means to send large amounts of data to the graphics pipeline via interaction with shader program "attributes". <seealso cref="Scene::attributes"/></summary>
class OGLAttributes
{
private:
    /// <summary>A flag indicating whether or not the class and its data are valid.</summary>
    bool valid;
    // ~ Attributes
    /// <summary>A shader program attribute used to update the vertices of any object-to-be-rendered. This attribute is perhaps the most crucial as it directly supplies the vertices of the
    /// polygons which are to-be-drawn.</summary>
    /// <remarks>This attribute is bound to a buffer object (probably object index zero) and vertices are sent to the graphics card via calls to <c>glBindBuffer()</c> and <c>glBufferData()</c>.</remarks>
	GLint position;
    /// <summary>A shader program attribute which supplies one color (r,g,b,a) for every vertex sent. Note that color data will only be used if textures are disabled.</summary>
    /// <remarks>This attribute is bound to a buffer object and colors are sent to the graphics card via calls to <c>glBindBuffer()</c> and <c>glBufferData()</c>.</remarks>
	GLint color;
    /// <summary>A shader program attribute for transmitting texture coordinates. Texture coordinates are updated on a per-vertex basis, so it is essential to use an attribute.</summary>
	GLint texcoord;
    /// <summary>A shader program attribute for transmitting the combined translation / rotation / scale matrix. This matrix is updated on an as-needed basis, but it is updated often, so it is essential
    /// to transfer the data via an attribute.</summary>
    /// <remarks>This matrix is calculated by multiplying translation matrix * rotation matrix * scale matrix. Note that matrix multiplication is non-communative, so the order in which
    /// the matrices are multiplied will have an impact on the result. When instanced rendering is being utilized, this attribute is updated once per object.</remarks>
    GLint srt[4];
    /// <summary>A shader program attribute for transmitting the normal vector on a per-vertex basis.</summary>
    /// <remarks>During the first pass, one normal is computed for every triangle (every three vertices), then the computed normal is assigned to each
    /// vertex of the triangle so each vertex has its own normal. On the second pass, any normals which share a vertex (i.e. two adjacent triangles would
    /// share two vertices and therefore two normals) are then averaged together.</remarks> 
    GLint normal;
    GLint ConfigureAttribute(std::string name_in, GLuint vbo_in, GLint divisor_in, GLuint num_components_in, GLint type_in);
public:
    /// <summary>Constructor for this class which takes a number of parameters which are required to configure and initialize attributes.</summary>
    OGLAttributes(void);
    OGLAttributes(const OGLMatrices & matrices_in);
    OGLAttributes& operator=(const OGLAttributes& copy_source);
    OGLAttributes::OGLAttributes(const OGLAttributes & copy_source);

};
#pragma endregion
#pragma region OGLUniforms Class
/// <summary>Provides a means to send data to the graphics pipepline via interaction with shader program "uniforms/uniform blocks". <seealso cref="Scene::uniforms"/></summary>
class OGLUniformBlockElement
{
public:
    const GLuint index;
    const GLint offset;
    const GLint size;
    const GLint type;
    OGLUniformBlockElement(void);
    OGLUniformBlockElement(GLuint index_in, GLint offset_in, GLint size_in, GLint type_in);
};
class OGLUniformBlock
{
private:
    static GLint block_count;
public:
    std::string name;
    GLint size;
    GLint count;
    GLint index;
    GLint binding;
    std::map<const std::string,OGLUniformBlockElement> elements;
    OGLUniformBlock(const std::string & name_in, std::vector<const std::string> names_in);
    OGLUniformBlock(void);
    OGLUniformBlock& operator=(const OGLUniformBlock& copy_source);
    OGLUniformBlock::~OGLUniformBlock(void);
};
class OGLUniforms
{
private:
    static const std::string _block_names[];
    static std::vector<const std::string> block_names;
    static const std::string _source_lighting_block_names[];
    static std::vector<const std::string> source_lighting_block_names;
    static const std::string _material_lighting_block_names[];
    static std::vector<const std::string> material_lighting_block_names;
    // ~ View Uniform
    /// <summary>The unique identifier which is used to send the combined projection & view matrices to the shader program on the graphics card.</summary>
    GLint _projection_view_matrix;
    // ~ Texture Uniforms
    /// <summary>A shader program uniform location. If set to true (a non-zero number), forces all colors to black.</summary>
    GLint _cast_all_black;
    /// <summary>A shader program uniform location. If set to true (a non-zero number), enables triplanar texturing. If set to false (zero), textures
    /// are mapped using standard (u,v) texture coordinates.</summary>
    GLint _is_a_voxel;
    /// <summary>A shader program uniform location. If set to true (a non-zero number), texturing will be enabled (either triplanar or uv mapping, depending on the value set to <c>uni_isvoxel</c>). If
    /// set to false (zero), texturing will be disabled and the color data for the object will ultimately determine the fragment colors.</summary>
    GLint _textures_enabled;
    /// <summary>A shader program uniform location. This uniform acts as a texture index - it determines which texture is mapped to the given object.</summary>
    GLint _texture;
    /// <summary>A shader program uniform location. If set to true (a non-zero number), bump mapping will be enabled (either triplanar or uv mapping,
    /// depending on the value set to <c>uni_isvoxel</c>). If set to false (zero), bump mapping will be disabled.
    GLint _bumpmapping_enabled;
    /// <summary>A shader program uniform location. This uniform acts as a texture index - it determines which texture is used to generate the highlights which
    /// characterize bump mapping.</summary>
    GLint _bumpmap;
    // ~ Lighting Uniforms (Flags)
    /// <summary>A shader program uniform location. This uniform determines whether or not ANY lighting will be calculated and added to each fragment. TRUE is represented by
    /// a non-zero number whereas FALSE is represented by zero.</summary>
    GLint _lighting_enabled;
    /// <summary>A shader program uniform location. If set to true (a non-zero number), diffuse lighting calculations will be enabled in the fragment shader. If false, diffuse
    /// lighting calculations are disabled.</summary>
    GLint _diffuse_enabled;
    /// <summary>A shader program uniform location. If set to true (a non-zero number), specular lighting calculations will be enabled in the fragment shader. If false, specular
    /// lighting calculations are disabled.</summary>
    GLint _specular_enabled;
    // ~ Lighting Uniforms (Source, independent of object data)
    /// <summary>A shader program uniform location. This uniform holds the position of the camera eye in the scene - three floats - used for lighting calculations.</summary>
    GLint _eye_position;
    GLint _fog_start;
    GLint _fog_end;
    GLint _fog_density;
    //
public:
    GLuint uniform_buffers[2];
    std::map<const std::string,OGLUniformBlock> blocks;
    /// <summary>The unique identifier which is used to send the combined projection & view matrices to the shader program on the graphics card.</summary>
    GLint projection_view_matrix(void);
    // ~ Texture Uniforms
    /// <summary>A shader program uniform location. If set to true (a non-zero number), forces all colors to black.</summary>
    GLint cast_all_black(void);
    /// <summary>A shader program uniform location. If set to true (a non-zero number), enables triplanar texturing. If set to false (zero), textures
    /// are mapped using standard (u,v) texture coordinates.</summary>
    GLint is_a_voxel(void);
    /// <summary>A shader program uniform location. If set to true (a non-zero number), texturing will be enabled (either triplanar or uv mapping, depending on the value set to <c>uni_isvoxel</c>). If
    /// set to false (zero), texturing will be disabled and the color data for the object will ultimately determine the fragment colors.</summary>
    GLint textures_enabled(void);
    /// <summary>A shader program uniform location. This uniform acts as a texture index - it determines which texture is mapped to the given object.</summary>
    GLint texture(void);
    /// <summary>A shader program uniform location. If set to true (a non-zero number), bump mapping will be enabled (either triplanar or uv mapping,
    /// depending on the value set to <c>uni_isvoxel</c>). If set to false (zero), bump mapping will be disabled.
    GLint bumpmapping_enabled(void);
    /// <summary>A shader program uniform location. This uniform acts as a texture index - it determines which texture is used to generate the highlights which
    /// characterize bump mapping.</summary>
    GLint bumpmap(void);
    // ~ Lighting Uniforms (Flags)
    /// <summary>A shader program uniform location. This uniform determines whether or not ANY lighting will be calculated and added to each fragment. TRUE is represented by
    /// a non-zero number whereas FALSE is represented by zero.</summary>
    GLint lighting_enabled(void);
    /// <summary>A shader program uniform location. If set to true (a non-zero number), diffuse lighting calculations will be enabled in the fragment shader. If false, diffuse
    /// lighting calculations are disabled.</summary>
    GLint diffuse_enabled(void);
    /// <summary>A shader program uniform location. If set to true (a non-zero number), specular lighting calculations will be enabled in the fragment shader. If false, specular
    /// lighting calculations are disabled.</summary>
    GLint specular_enabled(void);
    /// <summary>A shader program uniform location. This uniform holds the position of the camera eye in the scene - three floats - used for lighting calculations.</summary>
    GLint eye_position(void);
    GLint fog_start(void);
    GLint fog_end(void);
    GLint fog_density(void);
    OGLUniforms(void);
    OGLUniforms& operator=(const OGLUniforms& copy_source);
    GLint GetUniformBlockIndex(const std::string & name_in);
    GLint GetOffset(const std::string & block_in,const std::string & uniform_in);
    GLint GetSize(const std::string & block_in, const std::string & uniform_in);
    GLuint GetUniformBlockBuffer(const std::string & block_in);
    void OGLUniforms::ReleaseBuffers(void);

};
#pragma endregion
#pragma region OGLMatrices Class
/// <summary>Stores and manipulates matrices which are used to describe various various rendering parameters (i.e. translation, rotation, etc.). <seealso cref="Scene::matrices"/></summary>
class OGLMatrices
{
private:
    GLuint vbo;
public:
    OGLMatrices(void);
        //--- Matrices ---//
    // ~ SRT Matrices
    /// <summary>The scale matrix. Holds x,y,z scaling information for the current rendering context.</summary>
	glm::mat4 scale;
    /// <summary>The rotation matrix. Holds arbitrary rotation information for the curent rendering context.</summary>
	glm::mat4 rotation;
    /// <summary>The translation matrix. Holds arbitrary translation information for the current rendering context which determines "where" the next
    /// object will be drawn.</summary>
	glm::mat4 translation;
    /// <summary>The combined scale, rotation, translation matrix which is found by taking translation * rotation * scale.</summary>
    glm::mat4 translation_rotation_scale;
    // ~ View Matrices
    /// <summary>Projection matrix which determines how the scene as a whole is projected onto the window to which it is rendered. Determines
    /// such things as aspect ratio.</summary>
	glm::mat4 projection;
    /// <summary>View matrix which determines which part of the scene the "viewer" is looking at. Determines such things as "camera" location and
    /// field of view.</summary>
	glm::mat4 view;
    /// <summary>The combined project & view matrices, found by taking projection * view.</summary>
    glm::mat4 projection_view;
    void SetSRTVbo(const GLuint& vbo_in);
    // ~ Matrices
    /// <summary>This method is intended to offer a simple way to set the translation matrix via a set of x,y,z coordinates. In other words, this method takes a three component vector as an input
    /// then calls the necessary methods to convert the vector into a translation matrix which is then sent to the shader program. This operation IGNORES the previous translation matrix. The x,y,z 
    /// coordinates passed to this method are the coordinates from which all vertices will be relative to should a draw call be made.
    /// <seealso cref="Scene::info.mat_srt"/><seealso cref="Scene::info.mat_trans"/><seealso cref="Scene::info.mat_rotate"/><seealso cref="Scene::info.mat_scale"/>
    /// <seealso cref="Scene::UpdateSRT()"/></summary>
    /// <param name="trans_vec"/>A vector from the external GLM library which represents the x,y,z coordinates in 3D space that you want the next draw call to be made from.</param>
    /// <returns>An integer intended to indicate the success of the method. Currently always returns _OK (0).</returns>
    /// <remarks>This method invokes <c>Scene::UpdateSRT()</c>. It should also be noted that this method updates the matrices stored in <c>Scene::info</c>.</remarks>
	GLint sTranslate(glm::vec3 trans_vec);
    /// <summary>This method sends a combined translation*rotation*scale 4x4 matrix to the shader program, effectively configuring the size, orientation, and position of each vertex which is a candidate
    /// for rendering. The combined "srt" matrix is an attribute in the shader program, so it is sent via a vertex buffer object. Note that this method is capable of taking an array of matrices if
    /// a count is specified (second parameter), in order to accomodate instanced rendering. If no second parameter is passed, it is assumed that the first parameter only points to a single matrix.
    /// <seealso cref="Scene::info.vbo"/></summary>
    /// <param name="srt_in"/>A pointer either to a single 4x4 matrix or an array of 4x4 matrices.</param>
    /// <param name="count"/>An optional parameter specifying the number of matrices pointed to by the first parameter. Defaults to 1.</param>
    /// <returns>An integer intended to indicate the success of the method. Currently always returns _OK (0).</returns>
    /// <remarks>Unlike <c>Scene::sTranslate()</c>, this method does NOT update <c>Scene::info.mat_srt</c>.</remarks>
    GLint UpdateSRT(glm::mat4 * srt_in,const long int & count=1);
};
#pragma endregion
#pragma region OGLBuffers & OGLBuffers_Data Classes
class OGLBuffers_Data
{
    friend class OGLBuffers;
private:
    static std::vector<const GLuint> vbo;
    static std::vector<const GLuint> ebo;
    static std::vector<const GLuint> vao;
};
class OGLBuffers
{
    friend class Scene; friend class OGLAttributes; friend class OGLUniforms; friend class RenObj;
private:
    static OGLBuffers_Data data;
protected:
    /// <summary>A simple wrapper around <c>glGenBuffers()</c> which also increments a global counter keeping track of how many Vertex Buffer Objects have been generated. Vertex Buffer Objects are used as "pipelines"
    /// to "stream" frequently changing data to the graphics processor / shader program. Generally there is one VBO for each important attribute - for example the vertices which need to be rendered.
    /// <seealso cref="vboindx"/><seealso cref="MakVao()"/><seealso cref="MakEbo()"/></summary>
    /// <param name="vbo_in">A pointer to an array of unsigned integers in which the newly generated Vertex Buffer Object index will be placed. The element index at which the VBO will be placed is
    /// determined by <c>vboindx</c>.</param>
    /// <returns>An unsigned integer representing the number of VBOs generated thus far, including the one generated via this method call (if one was generated). If the maximum number of buffers have
    /// already been generated or there is an exception when invoking <c>glGenBuffers()</c> then this function will simply return <c>vboindx</c> without making any modifications.</returns>
    /// <remarks>The maximum number of VBOs and EBOs (combined) which can be generated is specified by <c>MAX_VBO</c>. This function will return MAX_VBO if the maximum number of buffers have already been generated.</remarks>
    static GLuint CreateVBO(void);
    /// <summary>A simple wrapper around <c>glGenBuffers()</c> which also increments a global counter keeping track of how many Element Buffer Objects have been generated. Element buffer objects are used to represent a series of elements
    /// as indices into a pool of unique elements - a process which ultimately promotes effeciency.
    /// <seealso cref="eboindx"/><seealso cref="MakVao()"/><seealso cref="MakVbo()"/></summary>
    /// <param name="ebo_in">A pointer to an array of unsigned integers in which the newly generated Element Buffer Object index will be placed. The element index at which the EBO will be placed is
    /// determined by <c>eboindx</c>.</param>
    /// <returns>An unsigned integer representing the number of EBOs generated thus far, including the one generated via this method call (if one was generated). If the maximum number of buffers have
    /// already been generated or there is an exception when invoking <c>glGenBuffers()</c> then this function will simply return <c>eboindx</c> without making any modifications.</returns>
    /// <remarks>The maximum number of VBOs and EBOs (combined) which can be generated is specified by <c>MAX_VBO</c>. This function will return MAX_VBO if the maximum number of buffers have already been generated.</remarks>
    static GLuint CreateEBO(void);
    static GLuint CreateVAO(void);
    static GLuint vbo(GLuint index_in);
    static GLuint ebo(GLuint index_in);
    static GLuint vao(GLuint index_in);
    static bool FreeVBO(GLuint index_in);
    static void Cleanup(void);
};
#pragma endregion
#pragma region ShaderProgram Class
class ShaderProgram
{
private:
    static std::vector<GLuint> vertex_shaders;
    static std::vector<GLuint> fragment_shaders;
    static std::map<GLuint,GLuint*> shader_programs;
    static GLuint CompileFragmentShader(std::string filename_in);
    static GLuint CompileVertexShader(std::string filename_in);
    static GLuint CreateShaderProgram(GLuint vertexShader, GLuint fragmentShader);
    static bool FreeShader(GLuint shader_in);
    static GLuint _id;
public:
    static bool SetShaderProgram(const GLuint & id_in);
    static GLuint id(void);
    static GLuint CreateShaderProgram(std::string vertexShader_in, std::string fragmentShader_in);
    static void FreeShaders(void);
};
#pragma endregion
#pragma endregion
#pragma region Input Classes
#pragma region InputUpdate Class
/// <summary>Holds input-event-related information which needs to be relayed to the Scene. <seealso cref="InputEvents::input_info"/></summary>
class InputUpdate
{
#pragma region Friends
friend class InputEvents;
friend class Scene;
friend class Camera;
#pragma endregion
private:
    #pragma region Flags
    bool pending_placement;
    bool pending_removal;
    bool trigger_camera_update;
    bool free_cam;
    bool exitbool;
    #pragma endregion
    #pragma region Toggle Flags
    bool toggle_diffuse;
    bool toggle_specular;
    bool toggle_bumpmapping;
    bool toggle_texture;
    bool toggle_wireframe;
    #pragma endregion
    #pragma region Data
    GLfloat angle_side;
    GLfloat angle_up;
    GLint direction;
    #pragma endregion
public:
    InputUpdate();
};
#pragma endregion
#pragma region InputEvents Class
/// <summary>Handles input events and relays relevant information back to the Scene as appropriate. <seealso cref="InputUpdate"/></summary>
/// <remarks>In order for the instance events to be triggered, the instance method <c>InputEvents::Activate()</c> needs to be called first.</remarks>
class InputEvents
{
private:
    static InputEvents* activeinput;
    static bool initialized;
    InputUpdate input_info;
    void GLFWCALL evnt_MouseBtn(GLint button, GLint action);
    static void GLFWCALL _evnt_MouseBtn(GLint button, GLint action);
    void GLFWCALL evnt_Keybrd(GLint keyID,GLint keyState);
    static void GLFWCALL _evnt_Keybrd(GLint keyID,GLint keyState);
    void GLFWCALL evnt_MouseMov(GLint xCoord, GLint yCoord);
    static void GLFWCALL _evnt_MouseMov(GLint xCoord, GLint yCoord);
    bool firstMouse;
    float lastmouse[2];
    GLfloat mouseDivConst[2];
    int screenshot_index;
public:
    InputEvents();
    InputUpdate GetInputUpdate(void);
    void Activate(void);

};
#pragma endregion
#pragma endregion
}
#endif