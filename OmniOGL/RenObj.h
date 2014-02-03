#ifndef RENOBJ_H
#define RENOBJ_H
#pragma region Includes
#include "RenderProperties.h"
#pragma endregion
//--- Custom Types ---//
namespace OmniOGL
{
#pragma region Defines
// ~ BlenderObj Constants
/// <summary>The minimum render ID of a <c>BlenderObj</c>. The first <c>BlenderObj</c> will be given this render ID and subsequent <c>BlenderObj</c>s will have their
/// render ID incremented by one.</summary>
#define COBJ_RID 1000
/// <summary>The maxmimum render ID of a <c>BlenderObj</c>.</summary>
#define COBJ_RID_MAX 9999
// ~ Cube Constants
/// <summary>The render ID for a <c>Cube</c>.</summary>
#define CUBE_RID 101 
/// <summary>The number of vertices needed to represent a single <c>Cube</c>.</summary>
#define CUBE_V_3D 108               
/// <summary>The number of color elements needed to represent a single <c>Cube</c>.</summary>
#define CUBE_C_3D 144
/// <summary>The number of texture coordinate elements needed to represent a single <c>Cube</c>.</summary>
#define CUBE_T_3D 72
/// <summary>The number of vertice elements needed to represent all of the normals for a single <c>Cube</c>.</summary>
#define CUBE_N_3D 108
// ~ Floor Constants
/// <summary>The render ID for a <c>Floor</c>.</summary>
#define FLOOR_RID 102
/// <summary>The number of vertices needed to represent a single <c>Floor</c>.</summary>
#define FLOOR_V_3D 12
/// <summary>The number of color elements needed to represent a single <c>Floor</c>.</summary>
#define FLOOR_C_3D 16
/// <summary>The number of texture coordinate elements needed to represent a single <c>Cube</c>.</summary>
#define FLOOR_T_3D 8
/// <summary>The number of vertice elements needed to represent all of the normals for a single <c>Cube</c>.</summary>
#define FLOOR_N_3D 12
/// <summary>The default "length" (x-axis) of the <c>Floor</c> class.</summary>
#define FLOOR_DEF_LENGTH 10000.0f
/// <summary>The default "width" (y-axis) of the <c>Floor</c> class.</summary>
#define FLOOR_DEF_WIDTH 10000.0f
// ~ Text Overlay Constants
#define TEXTOVER_RID 103
// ~ Line Constants
/// <summary>The render ID for a <c>LineStrip</c>.</summary>
#define LINE_RID 104
/// <summary>The number of vertices needed to represent a single <c>LineStrip</c>.</summary>
#define LINE_V_3D 6
/// <summary>The number of color elements needed to represent a single <c>LineStrip</c>.</summary>
#define LINE_C_3D 8
/// <summary>The number of texture coordinate elements needed to represent a single <c>LineStrip</c>.</summary>
#define LINE_T_3D 4
/// <summary>The number of vertice elements needed to represent all of the normals for a single <c>LineStrip</c>.</summary>
#define LINE_N_3D 6
/// <summary>The "chunk" size of the vertice array for a <c>LineStrip</c>. The larger this number is, the less often the vertice
/// array will have to resize itself (due to adding additional line segments).</summary>
#define LINE_ARR_CHUNK 5000
// ~ Group Constants
/// <summary>The maximum number of object instances rendered by one <c>Group</c>.</summary>
#define MAX_GROUP_SIZE 1000000
// ~ Section Constants
/// <summary>The size, in one dimension, of a single <c>Section</c>.</summary>
#define SECTION_SIZE 30000.0f
// ~ Camera Constants
/// <summary>The Z coordinate (height) at which the camera "spawns".</summary>
#define DEF_CAMERA_HEIGHT 3.0f
/// <summary>The default speed (# of units) traveled by hitting 'w','a','s', or 'd'.</summary>
#define DEFAULT_SPEED 50.0f
/// <summary>The default distance from the camera eye to the point that the camera is looking at.
/// <seealso cref="Camera::Camera()"/></summary>
#define INITIAL_LOOK_DISTANCE 10.0f
// ~ Block Array Constants
/// <summary>The render ID for a <c>BlockArray</c>.</summary>
#define BARR_RID 10101
/// <summary>The length of the normal lines of a <c>BlockArray</c>, should the user wish to draw them.</summary>
#define BARR_NORMAL_LEN 4.0f
#pragma endregion
#pragma region RenObj Class
/// <summary>The purely virtual base class which is used to represent a single object which will potentially be rendered. This class is responsible for harboring and
/// initializing any data which is common to all rendered objects. Data should be stored and intialized in a way that that allows any ambiguous object to be described/stored.</summary>
/// <remarks>Note that any derived class must implement a Draw() method.</remarks>
class RenObj
{
#pragma region Friends
    /// <summary>Declares the Group class as a friend so that the Group's constructor may clone all of the RenObj's data.</summary>
    friend class Group;
#pragma endregion
#pragma region Members
    #pragma region Proected Members
    protected:
    #pragma region Optional Extensions
    MaterialLighting * lighting;
    BlendFunction * blending;
    DepthFunction * depth;
    StencilFunction * stencil;
    #pragma endregion
    #pragma region Partitioning
    /// <summary>Indicates whether or not the rendered object should be subject to rendering sections. Objects which are subject to sectioning will only be seen when
    /// in relatively close proximity to the camera. Objects which are not subject to sectioning (i.e. <c>en_internal_clipping=False;</c>) can be referred to as
    /// "persistant" objects, and they will be rendered as long as they lie between the near and far clipping planes.
    /// <seealso cref="Section"/></summary>
    /// <remarks>Initialized to <c>True</c> by default, which means rendered objects will be sorted into sections by default.</remarks>
    bool en_internal_clipping;
    /// <summary>Indicates whether or not the rendered object is a 2D object in the foreground. Objects with this property set to true will most likely be something
    /// UI or debug related. <seealso cref="Scene::Draw()"/></summary>
    /// <remarks>Initialized to <c>False</c> by default, which means rendered objects will be 3D by default.</remarks>
    bool in_foreground;
    /// <summary>An unsigned integer which indicates which section the object occupies in 3D spaces. Only applicable if <c>RenObj::en_internal_clipping</c> is set to <c>True</c>.
    /// <seealso cref="RenObj::en_internal_clipping/></summary>
    /// <remarks>Generally speaking, if an object is in a section which is in close proximity to the section occupied by the player/camera, then the object will be rendered.
    /// Initialized to -1 by default, which will resolve to 4294967295.</remarks>
    GLuint my_section;
    #pragma endregion
    #pragma region Interaction
    /// <summary>The radius of the object should the object be approximated to a crude sphere. This value is used in conjuction with the objects position in
    /// 3D space to determine whether or not the camera is looking at the object, and thus whether the player/user can potentially "select" the object.</summary>
    /// <remarks>Initialized to -1. If it is desired for the object to have selection/modification capabilties, a technique must be formulated in a method
    /// in a derived class which will determine the approximate overall "radius" of the object and store that value in this member.</remarks>
    GLfloat selection_radius;
    #pragma endregion
    #pragma region Data
    /// <summary>An array of four unsigned integers which represent the length of each of the [currently four] essential data arrays associated with any given rendered object. Each of
    /// the four elements of this member represent the length of the <c>RenObj::vertices</c>, <c>RenObj::colors</c>, <c>RenObj::texcoords</c>, and <c>RenObj::normals</c> arrays, respectively.</summary>
    /// <remarks>Array lengths are determined at run time and are dependant on what type of derived class is instantiated. As a result, all lengths are initiated to 0 in the RenObj constructor.</remarks>
	GLuint lengths[4];
    /// <summary>The bread-and-butter of all the rendering data. Holds the vertices which "describe" the object in 3D space. Remember that these vertices will be subject to the various transformation
    /// matrices. All vertices for a given object are stored in a single one-dimensional array of floats. There are three [GLfloat] components to one vertex. In most drawing operations you will be
    /// drawing triangles - three vertices make up a triangle - so there will be nine floats for every triangle in the format [A0x, A0y, A0z, A1x, A1y, A1z, A2x, A2y, A2z, ...] where A is a triangle,
    /// 0, 1, and 2 are the three vertices that form the triangle, and x, y, and z represent the three primary axis in 3D space.</summary>
    /// <remarks>Vertex data will not be known at the time the RenObj constructor is invoked, so the pointer is initialized to the value -1. It is possible to store vertices as a type other than a
    /// GLfloat. The length of this array, once resolved, is/should be stored in the first element of <c>RenObj::lengths</c>.</remarks>
    GLfloat * vertices;
    /// <summary>An array of floats which hold one color (r,g,b,a) for each vertex specified in the rendering process. If interpolation is enabled then specifying different colors at different adjacent
    /// vertices will cause a gradient to appear. This array is not used all that often since generally textures will provide more visually appealing colors and have the advantage of being able to
    /// easily fake complicated features such as shadows or bumps. Even if textures are being used for a given object, there must be enough elements in this array to satisfy the requirement of
    /// 4 floats for every 3 floats in the <c>RenObj::vertices</c> array. Essentially there must be color data (r,g,b,a) for each vertex (x,y,z) that is rendered, even if the color data is not
    /// being used and is filled with random garbage.</summary>
    /// <remarks>Color data will not be known at the time the RenObj constructor is invoked, so the pointer is initialized to the value -1. It is possible to store colors as a type other than a GLfloat.
    /// The length of this array, once resolved, is/should be stored in the second element of <c>RenObj::lengths</c>. Perhaps this data/array/associated shader program components should be done away
    /// with?</remarks>
    GLfloat * colors;
    /// <summary>An array of floating point numbers which are used to map textures (essentially pictures) to an object in 3D space. For every vertex (3 floats; x,y,z) passed to the active shader program
    /// there must be one pair of floats in this array. Texture coordinates are generally refered to by two letters: u and v. "u" refers to the x-axis of the texture, while "v" refers to the y-axis of
    /// the texture. For each location in 3D space passed to the shader program, a texture coordinate will be passed as well which will "map" the specified point in the texture to the specified location
    /// in 3D space. Voxels (the terrain) use interpolation (in the fragment shader) on all axis in order to determine the color for a given fragment.<seealso cref="FragmentShader.glsl"/></summary>
    /// <remarks>Texture coordinates will not be known at the time the RenObj constructor is invoked, so the pointer is initialized to the value -1. When color data is used (<c>RenObj::colors</c>),
    /// texture coordinates will be irrelevant but this array must still contain dummy data. Texture coordinates are not used for voxels. The length of this array is/should be stored in the third element
    /// of <c>RenObj::lengths</c>.</remarks>
    GLfloat * texcoords;
    /// <summary>An array of floats which represent the normal vector - one per vertex. There are three components to a normal vector: the x,y, and z components of the vector. Normal vectors are
    /// the root of most lighting calculations (remember that the normal vector represents the direction parallel to the objects surface at that vertex). A normal vector requires three vertices in
    /// order to be calculated and the order the vertices are entered into the calculation will have an effect on the result. Since a normal requires three vertices in order to be calculated (1:3)
    /// but each vertex sent to the shader program requires a normal (1:1), generally every all three vertices of a triangle will all have identical normals. Currently this rendering engine supports
    /// an option, when enabled, averages any normals which share a vertex - as is often an occurance when rendering voxels - resulting in smoother lighting over arbitrary surfaces.
    /// <seealso cref="FragmentShader.glsl"/></summary>
    /// <remarks>Normal vectors will not be known at the time the RenObj constructor is invoked, so this pointer is initialized to the value -1. The length of this array is/should be stored in the fourth
    /// element of <c>RenObj::lengths</c>.</remarks>
    GLfloat * normals;
    #pragma endregion
    #pragma region Render Properties
    /// <summary>Three floats which represent the origin (location) of the object in 3D space. This origin is the location from which all vertices are relative. This location is translated to
    /// in the <c>Draw()</c> function of the derived class during the rendering loop.<seealso cref="RenObj::GetOrigin()"/></summary>
    /// <remarks>The array size is fixed at 3 and all elements are intialized to 0.0 in the RenObj constructor by default. I will probably be making this member private, since there are public
    /// getters and setters available for this member.</remarks>
	GLfloat origin[3];
    /// <summary>An unsigned integer intended to be used as a sort of identification to distinguish unique objects from eachother. It is suggested that any objects which can be made the same object via
    /// matrix manipulation (translation, rotation, and scaling) should have the same rendering identification number. Identifying which objects are the same and rendering them together
    /// (at the same time, consecutively) can free up a lot of resources for other operations and help prevent redundant state changes. Currently, code optimization as previously described is
    /// spotty at best - some of the classes derived from RenObj attempt to implement rendering identification, but some do not. See the <c>Draw()</c> function of a derived class for an example
    /// implementation.</summary>
    /// <remarks>Initialized to -1 in the RenObj constructor.</remarks>
	GLint render_id;
    /// <summary>An integer which specifies which texture should be used to dress the object, if any. If this member is set to -1 it will signify to the rendering engine that no texture image will
    /// be used so that optimizations can be made where available.<seealso cref="RenObj::MakeTexture()"/><seealso cref="RenObj::SetTexture()"/></summary>
    /// <remarks>Initialized to -1 in the RenObj constructor.</remarks>
	GLint texture_sel;
    /// <summary>An integer which specifies which texture should be used for bump mapping on the object, if any. If this member is set to -1 it will signify to the rendering engine and shader program
    /// that no texture image will be used for bump-mapping and thus bump-mapping-related code should be disabled/avoided.<seealso cref="RenObj::MakeTexture()"/>
    /// <seealso cref="RenObj::SetTexture()"/></summary>
    /// <remarks>Initialized to -1 in the RenObj constructor.</remarks>
    GLint bump_map;
    #pragma endregion
    #pragma endregion
#pragma endregion
#pragma region Methods
    #pragma region Static Methods
    protected:
    static const GLuint vbo(GLuint index_in);
    static const GLuint ebo(GLuint index_in);
    static const GLuint vao(GLuint index_in);
    #pragma endregion
    #pragma region Protected Methods
    protected:
    #pragma region OpenGL Data Binding
    /// <summary>This method attempts to handle all of the necessary OpenGL state changes in one place. The desired value of various states are stored as a local variable owned by the object.
    /// This method is called for each object each frame and simply sets each state to the position specified by the object.</summary>
    /// <param name="info_in">A pointer to a structure which contains a large amount of relevant information necessary for the rendering process. For this particular method the structure is
    /// primarily used to update uniform variables in the shader program.</param>
    /// <returns>Returns an integer indicating whether or not the method performed successfully. Currently this always returns <c>_OK</c>, which is 0.</returns>
    /// <remarks>Currently this method results in an inefficient implementation. This method will blindly set each option in the OpenGL state machine without regards to its previous state.
    /// Such action results in many redundant state changes - meaning that options are being set to values that they were already set to. A wholey efficient implementation would store
    /// the state of each OpenGL option in a local variable and only update each option upon encountering a change in state.</remarks>
    GLint Prepare(OGLUniforms& uniforms_in, const bool& textures_en=true, const bool& bumpmap_en=true);
    /// <summary>This method lies at the heart of the rendering process, as it binds the object's relevant data to the appropriate shader program attribute (i.e. buffer object), then "commits"
    /// all of the [currently four] attributes to the shader program via calls to <c>glBufferData()</c>.</summary>
    /// <param name="info_in">A pointer to a structure which contains a large amount of relevant information necessary for the rendering process. For this particular method the structure
    /// is used ot access the vertex buffer objects in order to send attribute data to the shader program.</param>
    /// <returns>Returns an integer indicating whether or not the method performed successfully. Currently this always returns <c>_OK</c>, which is 0.</returns>
    GLint BindBufferData();
    #pragma endregion
    #pragma endregion
    #pragma region Public Methods
    public:
    #pragma region Constructors
    /// <summary>The default constructor for the RenObj class. All variables/data which is shared by all dervied classes should be initialized here. This constructor is intended to be the go-to
    /// centralized location for finding and tweaking the many properties common to all rendered objects. Remember that the RenObj class is a purely abstract class since its draw function is
    /// set equal to 0, thus it cannot be directly instantiated. The RenObj constructor is invoked automatically upon instantiation of a derived class, prior to the derived class's constructor.</summary>
    /// <returns>Constructors do not return a value.</returns>
    /// <remarks>Generally any members which are undetermined at the time this constructor is invoked (such as dynamically allocated arrays, the size of which will not be known) will be initialized
    /// to the value -1.</remarks>
    RenObj(void);
    /// <summary>The copy constructor for the RenObj class. This class is responsible for initializing all of the RenObj data using data from another RenObj instance, effectively copying that instance.
    /// Unlike the default RenObj constructor, this contructor MAY have to dynamically allocate memory for data, such as the vertices. This copy constructor will be called prior to the copy
    /// constructor of the derived class.</summary>
    /// <param name="copy_source">An instance of a class derived from RenObj which will be cloned by this method.</param>
    /// <returns>Constructors do not return a value.</returns>
    /// <remarks>It is important to account for all potential cases when copying array data (i.e. the arrays-to-be-copied could be uninitialized, yet they could also potentially contain 10,000
    /// elements.</remarks>
    RenObj(const RenObj &copy_source);
    #pragma endregion
    #pragma region Operators
    /// <summary>The '=' operator for the RenObj class. This method is called any time a derived class instance of a RenObj finds itself on the left side of a single equals sign. The object on the right
    /// side of the equals sign is passed as the parameter to this method and used as the copy source. This method is very similar to the RenObj copy constructor in that it must set all of its own
    /// members to contain the same data as the copy source. The biggest difference is that by the time this method is invoked, the destination object will have already be instantiated and thus could already
    /// contain data. As a result we must check our dynamic arrays and free memory when appropriate before attempting to resize and store new data into them.</summary>
    /// <param name="copy_source">An instance of a class derived from RenObj which will be cloned by this method.</param>
    /// <returns>Returns a reference to the destination object (the instance the method was called on).</returns> 
    /// <remarks>Since RenObj is a purely abstract class and cannot be instantiated, this method will only be called prior to a call to an '=' operator of a derived class. If I'm not mistaken you must
    /// explicitly call this function (via code) at the beginning of each derived classes' '=' operator methods.</remarks>
    RenObj & operator=(const RenObj &copy_source);
    #pragma endregion
    #pragma region Getters
    /// <summary>A getter for determining whether or not the object should be subject to "sectioning", which groups objects into seperate sections in 3D space. Objects which are sectioned will only render
    /// in sections which are in close proximity to the section occupied by the player/camera.<seealso cref="RenObj::en_internal_clipping"/></summary>
    /// <returns>A boolean value, <c>RenObj::en_internal_clipping</c>, which indicates whether or not the object will (automatically) be sorted into a section or not.</returns>
    /// <remarks>The member returned by this method is initialized to true by default. Objects which need to be seen far away and foreground objects (2D,UI,etc.) should have this option disabled.</remarks>
    bool GetInternalClipping(void);
    /// <summary>A setter for configuring whether or not the object will be subject to "sectioning", which groups objects into seperate sections in 3D space. Objects which are sectioned will only render
    /// in sections which are in close proximity to the section occupied by the player/camera.<seealso cref="RenObj::en_internal_clipping"/></summary>
    /// <param name="en_clipping_in">A boolean value which determines whether or not the object will (automatiaclly) be sorted into a section.</param>
    /// <returns>The boolean value passed to the method as a sanity check.</returns>
    /// <remarks>The member set by this method is initialized to true by default, in the RenObj default constructor. Objects which need to be seen far away and foreground objects (2D,UI,etc.) should have
    /// this option disabled.</remarks>
    bool GetLightingEnable(void);
    /// <summary>A getter which attempts to copy the objects current origin (x,y,z location in 3D space to which all vertices are relative) into a pointer whose memory is owned by the caller. The
    /// copying is performed via <c>memcpy()</c> and is protected by a try/catch clause.<seealso cref="RenObj::origin"/><seealso cref="RenObj::SetOrigin()"/></summary>
    /// <param name="target_arr">A GLfloat array of at least 3 elements, created and owned by the caller. The first three elements of this array will be filled with the x,y, and z coordinates
    /// of the object, respectively.</param>
    /// <returns>In the case that the memcpy call results in an error, the error number will be returned. If no errors occur, this function returns _OK, which is equal to zero.</returns>
    /// <remarks>Consider changing <c>RenObj::origin</c> from a public member to a private member.</remarks>
    GLint GetOrigin(GLfloat * const  & target_arr);
    /// <summary>A getter that retrieves a boolean value which represents whether or not the object is a foreground object and thus should be rendered on the "front" of the screen.
    /// <seealso cref="RenObj::in_foreground"/><seealso cref="RenObj::SetForeground()"/></summary>
    /// <returns>True if the object should be in the foreground and False if the object should be rendered as a 3D object with an orthoganal projection matrix.</returns>
    /// <remarks>The member associated with this getter is initialized to False in the RenObj default constructor.</remarks>
    bool GetForeground(void);
    /// <summary>A setter which sets a boolean value which represents whether or not the object is a foreground object and thus should be rendered on the "front" of the screen.
    /// <seealso cref="RenObj::in_foreground"/><seealso cref="RenObj::GetForeground()"/></summary>
    /// <param name="fgnd_en_in">A bool which determines whether or not the object is intended to be [most likely] a 2D object rendered "on top"/"in front" of all other objects.
    /// An orthogonal projection matrix will be used for this object.</param>
    /// <returns>The value of the member after it has been set by the parameter passed to the method. This is done as a sanity check and also for easy code integration.</returns>
    /// <remarks>The member associated with this setter is initialized to False in the RenObj default constructor.</remarks>
    GLuint GetOccupiedSection(void);
    /// <summary>Gets a GLfloat which is used to create a rough approximation of the spherical area occupied by the object. More specifically, it is the maximum distance (in any direction)
    /// from the center [origin] of the object that the coordinates the camera is looking at need to be in order to warrant a selection or modification of the object.
    /// <seealso cref="RenObj::selection_radius"/></summary>
    /// <returns>A GLfloat which ultimately represents the spherical area around the center of the object that, when looked at, can potentially trigger an interaction of some sort (i.e. selection,
    /// modification, etc.). If this method returns -1 then the selection radius is uninitialized.</returns>
    /// <remarks>The member associated with this getter is set to -1.0 by default and thus it is up to RenObj's derived classes to develop an algorithm to determine the selection radius and also
    /// calculate and store the selection radius. Note that there is no setter method at this time.</remarks>
    GLint GetRenderID(void);
    GLfloat GetSelectionRadius(void);
    /// <summary>A getter which retrieves the texture index assigned to the object, if any. Currently this method is not being utilized.
    /// <seealso cref="RenObj::texture_sel"/><seealso cref="RenObj::SetTexture(string,bool)"/><seealso cref="RenObj::SetTexture(GLint)"/><seealso cref="RenObj::MakeTexture()"/></summary>
    /// <returns>Returns an integer which corresponds to the index of the texture that should be used to determine the object's colors.</returns>
    /// <remarks>The member retrieved by this getter is initialized to -1 in the RenObj default constructor which means the object's color data will be used instead of a texture image.</remarks>
    GLint GetTexture(void);
    #pragma endregion
    #pragma region Setters
    void SetColor(const GLfloat & r_in, const GLfloat & g_in, const GLfloat & b_in, const GLfloat & a_in);
    bool SetForeground(const bool & fgnd_en_in);
    bool SetInternalClipping(const bool & en_clipping_in);
    /// <summary>A getter for determining whether the lighting calculations will be performed in the fragment shader at the time the object is rendered. If the member associated with this getter is set to
    /// true then diffuse and specular lighting calculations will be performed in the fragment shader, provided that valid normals, material data, and light source data are passed to the shader program.
    /// <seealso cref="RenObj::en_lighting"/><seealso cref="FragmentShader.glsl"/></summary>
    /// <returns>A boolean value which indicates whether or not lighting calculations will be performed for the object.</returns>
    /// <remarks>The member returned by this method is initialized to true by default, which means objects will have lighting enabled by default. Lighting should be disabled when it is not being used,
    /// as lighting is a very expensive calculation that needs to be performed for every fragment.</remarks>
    /// <summary>Gets an unsigned integer which represents the section id that the object is currently occupying. The member returned by this method will be set to -1 in cases where the object does
    /// not belong to a section (i.e. the object is persistant through all sections). This section id keeps track of what section the object is in so that methods which have access to the object
    /// can easily perform certain operations such as remove the object from its section.
    /// <seealso cref="RenObj::my_section"/><seealso cref="RenObj::SetOccupiedSection()"/><seealso cref="RenObj::en_internal_clipping"/></summary>
    /// <returns>An unsigned integer which will be 0 or a positive number if the object resides in a section, or -1 if the object is a persistant object. Any other return value is a bad thing.</returns>
    /// <remarks>The member associated with this getter is initialized to -1 in the RenObj default constructor.</remarks>
    /// <summary>A setter which is more-or-less just a wrapper around the static function <c>RenObj::MakeTexture()</c>. This method sets the appropriate member with the value returned by MakeTexture().
    /// The texture of an object is the image that is used to color the object's face(s).
    /// <seealso cref="RenObj::SetTexture(std::string,bool)"/><seealso cref="RenObj::MakeTexture()"/><seealso cref="RenObj::texture_sel"/></summary>
    /// <param name="file_in">The full path of an image/texture file. There are restrictions to what file types and resolutions will work, but I suggest a .png with an alpha layer.</param>
    /// <param name="repeat">A boolean value which represents whether the image will be tiled over surfaces or stretched. Stretch (false) is generally better for static / smaller objects
    /// whereas repeating (true) might be better for terrain, for example.</param>
    /// <returns>The texture index which was assigned to the object - regardless if a new texture was bound or the given file path was already bound. Returns -1 in error cases.</returns>
    /// <remarks>The member associated with this method is initialized to -1 in the RenObj default constructor which means no texture will be used for the object and the object's color data
    /// will be used instead. Note there is an overloaded version of this method which takes an integer instead.</remarks>
    /// <summary>A setter which takes three floats as a parameter and derives three GLfloat arrays of size four to be used as the material lighting information for the object. The first parameter is used
    /// as the r,g,b, and a diffuse components of the material which determine what/how diffuse lighting will show up on the object. The second parameter is used as the r,g,b, and a specular components
    /// of the material. The last parameter is used as the shininess factor (exponent), which affects how the specular lighting will appear.
    /// <seealso cref="RenObj::diffuse_factor"/><seealso cref="RenObj::specular_factor"/><seealso cref="RenObj::factor_vec4()"/></summary>
    /// <param name="diffuse_in">A GLfloat which is converted into an array of four matching floats and is used in the fragment shader when calculating diffuse lighting for the object.</param>
    /// <param name="specular_in">A GLfloat which is converted into an array of four matching floats and is used in the fragment shader when calculating diffuse lighting for the object.</param>
    /// <param name="shine_in">A GLfloat which is used as the shinniness factor when calculating the specular lighting of an object. The shinniness factor is the exponent that the dot
    /// product is raised to. See FragmentShader.glsl for more information.</param>
    /// <remarks>The diffuse and specular components are all set to 1.0 by default in the RenObj default constructor. Shininniness is initialized to 100 by default. Soon this method will most
    /// likely be modified to take GLfloat arrays as parameters so that the r,g,b,a specular and diffuse components can all be specified seperatley by an external caller.</remarks>
    void SetLighting(const GLfloat & diffuse_in, const GLfloat & specular_in, const GLfloat & shine_in);
    void SetLightingEnable(const bool & en_in);
    /// <summary>Sets an unsigned integer which represents the section id that the object is currently occupying. This section id keeps track of what section the object is in so that methods which
    /// have access to the object can easily perform certain operations such as remove the object from its section. This method will only be called upon initial creation of the object or when the
    /// object crosses a boundary to another section.
    /// <seealso cref="RenObj::my_section"/><seealso cref="RenObj::GetOccupiedSection()"/><seealso cref="RenObj::en_internal_clipping"/><seealso cref="Scene::AddObject()"/></summary>
    /// <param name="section_in">An unsigned integer designating the index of the new section the object resides in, or -1 if the object is intended to be persistant across all sections.</param>
    /// <returns>An unsigned integer which should match the parameter passed to this method. Any other return value is a bad thing.</returns>
    /// <remarks>The member associated with this getter is initialized to -1 in the RenObj default constructor. Setting the occupied section to -1 after initialization will result in
    /// undefined behavior and thus it is not allowed (it will result in an instant return).</remarks>
    GLuint SetOccupiedSection(const GLuint & section_in);
    /// <summary>A setter which sets the origin of the object in 3D space, to which all vertices are relative. All parameters are optional, and the default origin will be 0,0,0. An objects origin
    /// will determine which section it is placed in and may also be used to determine if the player is in close proximity to the object (i.e. for modification).
    /// <seealso cref="RenObj::origin"/><seealso cref="RenObj::GetOrigin()"/><seealso cref="Group::SetOrigin()"/></summary>
    /// <param name="x_in">A GLfloat which represents the object's location on the x-axis.</param>
    /// <param name="y_in">A GLfloat which represents the object's location on the y-axis.</param>
    /// <param name="z_in">A GLfloat which represents the object's location on the z-axis.</param>
    /// <returns>An integer which indicates whether or not the function was successful. Currently always returns zero, although the overloaded version of this function available in
    /// the Group class does potentially return a -1.</returns>
    /// <remarks>Also note that this method is virtual, although a default RenObj implementation exists. This is primarily because I needed to overload the function to accomodate the derived class,
    /// Group, which needs to update the transformation matrix for each item in the group any time that the origin changes.</remarks>
    virtual GLint SetOrigin(const GLfloat & x_in=0.0, const GLfloat & y_in=0.0, const GLfloat & z_in=0.0);
    GLint SetTexture(const std::string & file_in,const bool & repeat=false);
    /// <summary>A setter which directly sets the texture index of the object, provided that it is a positive number that is less than the number of textures bound thus far. The texture of an
    /// object is the image that is used to color the object's face(s).</summary>
    /// <seealso cref="RenObj::SetTexture(GLint)"/><seealso cref="RenObj::MakeTexture()"/><seealso cref="RenObj::texture_sel"/></summary>
    /// <param name="txr_index_in">An integer which represents the desired texture index to be assigned to the object. A check will be made to ensure the number is positive and less than the
    /// total texture count.</param>
    /// <returns>If there were no errors, returns the texture index, post-assignment. Returns -1 in error cases (i.e. bad parameter or an issue loading the image).</returns>
    /// <remarks>The member associated with this method is initialized to -1 in the RenObj default constructor which means no texture will be used for the object and the object's color data
    /// will be used instead. Note there is an overloaded version of this method which takes a file path (string; and optional bool) instead.</remarks>
    GLint SetTexture(const GLint & txr_index_in);
    /// <summary>A setter which is more-or-less just a wrapper around the static function <c>RenObj::MakeTexture()</c>. This method sets the appropriate member with the value returned by MakeTexture().
    /// The bump map is an image whose color components are used to alter the normals of each object to simulate small differences in lighting.<seealso cref="RenObj::SetBumpMap(GLint)"/>
    /// <seealso cref="RenObj::MakeTexture()"/><seealso cref="RenObj::bump_map"/></summary>
    /// <param name="file_in">The full path of an image/bump map file. There are restrictions to what file types and resolutions will work, but I suggest a .png with an alpha layer.</param>
    /// <param name="repeat">A boolean value which represents whether the image will be tiled over surfaces or stretched. Stretch (false) is generally better for static / smaller objects
    /// whereas repeating (true) might be better for terrain, for example.</param>
    /// <returns>The texture [bump map] index which was assigned to the object - regardless if a new texture was bound or the given file path was already bound. Returns -1 in error cases.</returns>
    /// <remarks>The member associated with this method is initialized to -1 in the RenObj default constructor which means no bump map will be used for the object. Note there is an overloaded
    /// version of this method which takes an integer instead.</remarks>
    GLint SetBumpMap(std::string file_in,bool repeat=false);
    /// <summary>A setter which directly sets the texture index of the object, provided that it is a positive number that is less than the number of textures bound thus far.
    /// The bump map is an image whose color components are used to alter the normals of each object to simulate small differences in lighting.<seealso cref="RenObj::SetBumpMap(std::string,bool)"/>
    /// <seealso cref="RenObj::MakeTexture()"/><seealso cref="RenObj::bump_map"/></summary>
    /// <param name="txr_index_in">An integer which represents the desired texture index to be assigned to be the bump map of the object. A check will be made to ensure the number is positive and less
    /// than the total texture count.</param>
    /// <returns>If there were no errors, returns the texture index, post-assignment. Returns -1 in error cases (i.e. bad parameter or an issue loading the image).</returns>
    /// <remarks>The member associated with this method is initialized to -1 in the RenObj default constructor which means no bump map will be used for the object. Note there is an
    /// overloaded version of this method which takes a file path (as a string) and an optional boolean value.</remarks>
    GLint SetBumpMap(GLint txr_index_in);
    #pragma endregion
    #pragma region Rendering
    /// <summary>A purely virtual method which must be defined by any derived classes in order for the derived class to be considered valid. This method is called on in <c>Scene::Draw()</c> and
    /// must implement all of the necessary code needed to send the object's information to the shader program and execute the drawing of the object. In order to save bandwith, objects should only
    /// send their data to the graphics card if their data differs from the previously rendered object's data. Similiarily, OpenGL states should only be committed if the state has changed.
    /// <seealso cref="BlockArray::Draw()"/><seealso cref="BlenderObj::Draw()"/><seealso cref="Cube::Draw()"/><seealso cref="Floor::Draw()"/><seealso cref="LineStrip::Draw()"/>
    /// <seealso cref="TextOverlay::Draw()"/><seealso cref="RenObj::BindBufferData()"/><seealso cref="RenObj::Prepare()"/></summary>
    /// <param name="incoming_id">An integer which represents the unique identification number of the most recently rendered object. For some derived classes (i.e. Cube), the render id will
    /// be the same for every instance. Other derived classes (i.e. BlenderObj) will assign a new render id to each instance. Derived classes' Draw() methods can minimize the amount of
    /// bandwith used if they only send data when the new object data differs from the previous object data.</param>
    /// <param name="info_in">A pointer to a structure which holds the majority of the information necessary to interact with the shader program, alter matrices, call Scene methods, etc.</param>
    /// <returns>An integer which will be 0 in most cases but may be -1 if an error is encountered.</returns>
    /// <remarks>This method is called on every object every frame so all implementations must be as concise and efficient as possible. It is important not to be redundant - don't send
    /// data the shader program already has, don't mess with the OpenGL state machine unless a state has actually changed, etc.</remarks>
    virtual GLint Draw(GLint incoming_id, OGLUniforms& uniforms_in, OGLMatrices& matrices_in, const bool& textures_en, const bool& bumpmap_en)=0;
    #pragma endregion
    #pragma region Destruction
    /// <summary>The destructor for the RenObj class which is responsible for freeing any memory which was dynamically allocated in a RenObj method - namely the vertices, colors, normals, and texture
    /// coordinates.<c>_Delete()</c> is used to ensure maximum safety. Note that this method is virtual and any derived classes should provide their own destructors which are responsible for deleting
    /// any dynamically allocated memory owned by the derived class.</summary>
    /// <remarks>The pointers which are deleted by this method are set to -1 in the RenObj default constructor.</remarks>
    virtual ~RenObj(void);
    #pragma endregion
    #pragma endregion
#pragma endregion
};
#pragma endregion
#pragma region BlenderObj Class
/// <summary>A class derived from a RenObj which gets is vertices and normals from a file generated by the program Blender. In the future this class might be able to extract additional externally-generated
/// data such as baked occlusion maps. See the BlenderObj constructor for more information on the file format expected.</summary>
class BlenderObj : public RenObj
{
#pragma region Methods
    #pragma region Public Methods
    public:
    #pragma region Constructors
    /// <summary>The sole BlenderObj constructor for the time being (except for the copy constructor) which requires a file path to a file with vertex and normal data generated by the Blender
    /// software. This default constructor must initialize any RenObj data which is not initialized by the RenObj constructor(s) (i.e. vertices, etc.). <seealso cref="RenObj::RenObj()"/></summary>
    /// <param name="file_in">A string representing the file path of the file which contains vertex and normal data. Currently there is very little logic involved with this so be careful and
    /// be sure the file is formatted/generated properly/as expected.</param>
    /// <param name="scale_in">A percentage to which the object will be scaled - use this number to quickly tweak the scale of the object without having to re-generate the .obj file over-and-
    /// over in Blender.</param>
    /// <remarks>This class has not been tested/used much, and thus it is in its very early stages and has been subject to very little optimization / error-guarding. Remember that by invoking
    /// this constructor the RenObj default constructor is invoked as well (first).</remarks>
    BlenderObj(std::string file_in,GLfloat scale_in=1.0);
    /// <summary>The copy constructor for the BlenderObj class. Because the BlenderObj class has no unique members (for the time being), this method simply invokes the base class (RenObj)
    /// copy constructor. <seealso cref="RenObj::RenObj(const RenObj&)"/></summary>
    /// <param name="copy_source">The BlenderObj which is passed to the RenObj constructor to be copied.</param>
    BlenderObj(const BlenderObj& copy_source);
    #pragma endregion
    #pragma region Operators
    /// <summary>The assignment operator method. Since the BlenderObj class has no unique members this method simply invokes the base class (RenObj) assignment operator and then returns
    /// a copy of "this". <seealso cref="RenObj::operator=()"/></summary>
    /// <param name="copy_source">The BlenderObj which is passed to the RenObj assignment operator to be copied.</param>
    /// <returns>A copy of what "this" is pointing to.(a.k.a. "*this").</returns>
    BlenderObj& operator=(const BlenderObj& copy_source);
    #pragma endregion
    #pragma region Rendering
    /// <summary>The draw method which must be implemented in order for the class to be instantiable. This method is responsible for sending the vertices, normals, etc. necessary to draw the object to the
    /// shader program, set all necessary options to their desired state, and finally invoke the desired OpenGL draw call. At the time being this method implementation is fairly rudimentary as it contains
    /// no logic to determine whether or not it needs send its data to the graphics card (it sends its data blindly).</summary>
    /// <param name="incoming_id">An integer passed to this method by the Scene class and intended to be used as a means of reducing bandwith used. Currently this parameter is not being utililzed
    /// as the BlenderObj class has not yet implemented a way to keep track of which BlenderObjs should have the same render id.</param>
    /// <param name="info_in">A pointer to a structure which contains a wide variety of essential variables which are passed around and modified by the various draw methods of the various objects
    /// present in the scene. This pointer is required to access shader program variables, change the mvp matrices, alter the OpenGL state machine, etc.</param>
    /// <returns>An integer indicating whether or not there was an error in the method. Currently this return value isn't being utilized and the method always returns _OK, which is 0.</returns>
    /// <remarks>Consider implementing a way to determine if the previously rendered BlenderObj is the same as the current one so-as to avoid redundant data transfers.</remarks>
    GLint Draw(GLint incoming_id, OGLUniforms& uniforms_in,OGLMatrices& matrices_in, const bool& textures_en, const bool& bumpmap_en);
    #pragma endregion
    #pragma region Destruction
    /// <summary>The destructor for the BlenderObj class. Since the BlenderObj class has no unique members, this method currently does nothing. The RenObj destructor is invoked prior to the
    /// invocation of this destructor.<seealso cref="RenObj::~RenObj()"/></summary>
    virtual ~BlenderObj(void);
    #pragma endregion
    #pragma endregion
#pragma endregion
};
#pragma endregion
#pragma region Cube Class
/// <summary>A simple perfect cube which can be scaled in size. Vertex, normals, and texture coordinates are all hard-coded and all that needs to be supplied in order to complete the look is a 
/// texture. <seealso cref="RenObj"/></summary>
/// <remarks>It is possible to render imperfect cuboids by using this class to seed a Group class, then drawing various Cube instances which have been subject to an arbitrary scale, rotation, and
/// translation matrix.</remarks>
class Cube : public RenObj
{
#pragma region Members
    #pragma region Private Members
    private:
    /// <summary>A GLfloat which represents the width of each side of the cube.</summary>
    GLfloat diameter;
#pragma endregion
#pragma endregion
#pragma region Methods
    #pragma region Public Methods
    public:
    #pragma region Constructors
    /// <summary>The default constructor for the Cube class. This method is responsible for initializing all members specific to the Cube class. This constructor
    /// must also allocate memory for and assign values to any RenObj class data that is not initialized by the RenObj constructor(s) (i.e. vertices, texture
    /// coordinates, etc.).<seealso cref="Cube::Cube(const Cube &)"/></summary>
    /// <param name="diameter_in">The desired width of the cube (equal width on all sides). This value will be multiplied by each vertex in order to yield the final vertices which will be
    /// passed to the shader program via an attribute.</param>
    /// <remarks>The sole member initialized by this method has no default initialization value since its initial value is determined by the parameter passed to the constructor.</remarks>
    Cube(GLfloat diameter_in);
    /// <summary>The copy constructor for the Cube class. This method initializes all of the Cube class members by using the values of the Cube instance passed to this method.
    /// This copy constructor does not have to initialize/copy RenObj data like the Cube default constructor since that data is copied via the RenObj copy constructor.
    /// <seealso cref="Cube::Cube(GLfloat)"/><seealso cref="RenObj::RenObj(RenObj &)"/><seealso cref="Cube::operator=()"/></summary>
    /// <param name="copy_source">The source from which all values will be copied.</param>
    /// <remarks>This method is invoked by the code <c>Cube foo(bar);</c> where bar is a perviously defined Cube and is not to be confused by the operator= method.</remarks>
    Cube(const Cube & copy_source);
    #pragma endregion
    #pragma region Operators
    /// <summary>The "= operator" for the Cube class. This method sets all of the Cube class members of one Cube to be equal to the class members of another Cube. In other words, it copys the
    /// data from the Cube on the right-hand side of the operator over to the Cube on the left-hand side of the operator. Note that the <c>RenObj::operator=</c> is invoked immediately prior to
    /// the execution of the code in this method.<seealso cref="Cube::Cube(const Cube &)"/><seealso cref="RenObj::operator=()"/></summary>
    /// <param name="copy_source">The Cube whose data will be cloned. Be sure to check for anomolies such as self-assignment.</param>
    /// <returns>Trivially returns a reference to the type. Return type could be void as well.</returns>
    /// <remarks>This method is not to be confused with the copy constructor. Be sure to check for anomolies (such as self-assignment) when coding this method. An example invocation of this
    /// method is <c>foo=bar;</c> where foo and bar are both predefined Cubes.</remarks>
    Cube& operator=(const Cube & copy_source);
    #pragma endregion
    #pragma region Rendering
    /// <summary>A method which is called each frame in order to render the object to the screen. This method is responsible for binding data to the shader program and calling the draw
    /// call(s). Both the rendering id of the last rendered object and a structure which holds a plethora of data are passed to this method.
    /// <seealso cref="RenObj::BindBufferData()"/><seealso cref="RenObj::Prepare()"/><seealso cref="Scene::Draw()"/></summary>
    /// <param name="incoming_id">An integer which represents the rendering id of the most recently rendered object. This id should be utilized to determine whether or not data needs to be
    /// sent to the shader program or OpenGL states need to be set.</param>
    /// <param name="info_in">A structure which provides access to various shader program attributes & uniforms, pointers relevant to the rendering process, etc. This structure is essential
    /// to perform most of the basic rendering tasks.</param>
    /// <returns>An integer which will be 0 if the method was successful or -1 (or possibly another number) if there was an error. Currently this always returns 0.</returns>
    GLint Draw(GLint incoming_id, OGLUniforms& uniforms_in,OGLMatrices& matrices_in, const bool& textures_en, const bool& bumpmap_en);
    #pragma endregion
    #pragma region Destruction
    /// <summary>The destructor for the Cube class which is responsible for freeing any memory owned by the class. The RenObj destructor is automatically invoked immediately prior to this method
    /// whenver a Cube is destroyed/deleted.</summary>
    /// <remarks>Declared virtual as a stylistic choice.</remarks>
    virtual ~Cube(void);
    #pragma endregion
    #pragma endregion
#pragma endregion
};
#pragma endregion
#pragma region Floor Class
/// <summary>A basic flat and wide floor. This class will have little use later on, but for now it serves as a good-enough plane to serve as a floor.
/// This class is derived from the RenObj class which means that it inherits all of the RenObj members and methods.</summary>
class Floor : public RenObj
{
private:
    //--- Protected Variables ---//
    // ~ Physical Properties
    /// <summary>A GLfloat representing the dimensions of the floor plane on the x-axis. All of the floor's vertices which lie on the x-axis are multiplied by
    /// this number in order to yield the vertices which will be rendered.<seealso cref="FLOOR_DEF_LENGTH"/><seealso cref="Floor::width"/></summary>
    /// <remarks>This member is set to 10,000.0 in the Floor default constructor or can be set via the 2nd parameter to the other Floor constructor.</remarks>
    GLfloat length;
    /// <summary>A GLfloat representing the dimensions of the floor plane on the x-axis. All of the floor's vertices which lie on the x-axis are multiplied by
    /// this number in order to yield the vertices which will be rendered.<seealso cref="FLOOR_DEF_WIDTH"/><seealso cref="Floor::length"/></summary>
    /// <remarks>This member is set to 10,000.0 in the Floor default constructor or can be set via the 3rd parameter to the other Floor constructor.</remarks>
    GLfloat width;
public:
    //--- Public Methods ---//
    // ~ Constructor(s)
    /// <summary>Default constructor for the Floor class - responsible for initializing any members owned by this class and also any RenObj data which is not
    /// initialized by the RenObj constructor(s) (i.e. vertices, etc.). This default constructor will result in a large/wide black floor with a shiny material (light settings).
    /// <seealso cref="Floor::Floor(GLfloat*,GLfloat,GLfloat,GLfloat*)"/><seealso cref="FLOOR_DEF_LENGTH"/><seealso cref="FLOOR_DEF_WIDTH"/></summary>
    /// <remarks>The length and width are initialized to the preprocessor definitions <c>FLOOR_DEF_LENGTH</c> and <c>FLOOR_DEF_WIDTH</c>, respectively. Texture coordinates
    /// are filled with garbage data and all normals will always be aligned with the z-axis on the positive side.</remarks>
    Floor(void);
    /// <summary>Verbose constructor for the Floor class - responsible for initializing any members owned by this class and also any RenObj data which is not initialized 
    /// by the RenObj constructor(s) (i.e. vertices, etc.) - all by utilizing the parameters passed to this method.<seealso cref="Floor::Floor()"/></summary>
    /// <param name="orig_in">A GLfloat array of three elements which will determine the origin at which the Floor will be drawn (x,y,z). All vertices will be rendered
    /// relative to this origin.</param>
    /// <param name="length_in">A GLfloat which represents the length of the floor, or in other words, the size of the floor on the x-axis. All x-axis vertices will be
    /// multiplied by this number to yield the vertices which will be rendered.</param>
    /// <param name="width_in">A GLfloat which represents the width of the floor, or in other words, the size of the floor on the y-axis. All y-axis vertices will be
    /// multiplied by this number to yield the vertices which will be rendered.</param>
    /// <param name="color_in">An array of four floats which represent the color of the entire floor (r,g,b,a).</param>
    /// <remarks>Texture coordinates are filled with garbage data and all normals will always be aligned with the z-axis on the positive side.</remarks>
    Floor(GLfloat * orig_in, GLfloat length_in, GLfloat width_in, GLfloat * color_in);
    /// <summary>Copy constructor for the Floor class which is responsible for initializing all members owned by and exclusive to this class. Note that this constructor is
    /// not responsible handling uninitialized RenObj members like the other Floor constructors are since the RenObj copy constructor takes care of that. This copy constructor
    /// copies the data of the existing Floor instance in order to initialize its own data.<seealso cref="Floor::Floor()"/><seealso cref="Floor::Floor(GLfloat*,GLfloat,GLfloat,GLfloat*)"/>
    /// <seealso cref="Floor::operator=()"/></summary>
    /// <param name="copy_source">The existing Floor instance whose data will be used to intitialize the new instance.</param>
    /// <remarks>This method is invoked by the code <c>Floor foo(bar);</c> where "bar" is a perviously defined Floor and is not to be confused by the operator= method.</remarks>
    Floor(const Floor & copy_source);
    // ~ Operator(s)
    /// <summary>Assignment operator method. This method is called whenever a Floor is on the left-hand side of an '=' operator and is responsible for responsibley freeing any
    /// memory exclusively owned by the Floor class (and not RenObj) and then copying the data from the Floor instance found on the right-hand side of the '=' operator into
    /// its own members.<seealso cref="Floor::Floor(Floor&)"/></summary>
    /// <param name="copy_source">The Floor instance which will be copied to the target (invoking) instance.</param>
    /// <returns>Trivially returns a reference to the type. Return type could be void as well.</returns>
    /// <remarks>This method is not to be confused with the copy constructor. Be sure to check for anomolies (such as self-assignment) when coding this method. An example invocation of this
    /// method is <c>foo=bar;</c> where foo and bar are both predefined Floor instances.</remarks>
    Floor& operator=(const Floor & copy_source);
    // ~ Rendering Methods
    /// <summary>A method which will be called each frame for each Floor object and is responsible for binding relevant data to the appropriate shader program attributes, configuring the
    /// OpenGL state machine, and executing draw calls to render the object. The render id passed to the function can be used to prevent unneccesary calls while the structure passed to this
    /// method serves as the primary means of accessing/interacting with OpenGL.<seealso cref="RenObj::BindBufferData()"/><seealso cref="RenObj::Prepare()"/><seealso cref="Scene::Draw()"/></summary>
    /// <param name="incoming_id">The render id of the most recently rendered object. This id can be used to prevent unneccesary data transfers or redundant state changes.</param>
    /// <param name="info_in">A structure which provides access to various shader program attributes & uniforms, pointers relevant to the rendering process, etc. This structure is essential
    /// to perform most of the basic rendering tasks.</param>
    /// <returns>An integer which is _OK (0) if the method executed without errors or _BAD (-1) if the method encountered an error. Currently this ALWAYS returns 0.</returns>
    /// <remarks>This method translates the current drawing location to the Floor's origin before the draw call is made.</remarks>
    GLint Draw(GLint incoming_id, OGLUniforms& uniforms_in,OGLMatrices& matrices_in, const bool& textures_en, const bool& bumpmap_en);
    // ~ Destructor
    /// <summary>The destructor for the Floor class which is responsible for freeing any memory owned by the class. The RenObj destructor is automatically invoked immediately prior to this method
    /// whenever a Floor is destroyed/deleted.</summary>
    /// <remarks>Declared virtual as a stylistic choice.</remarks>
    virtual ~Floor(void);
};
#pragma endregion
#pragma region TextOverlay Class
/// <summary>A series of 2D quads with letters on them. All letters are stored in one texture file and thus ultimately each character of the string to-be-rendered is translated into
/// a set of texture coordinates, each ranging from 0.0 to 1.0. All text is fixed width and characters such as 'j' are translated downwards a bit. Nothing too fancy. Different font sizes are
/// achieved simply by multiplying all vertices by a number.</summary>
/// <remarks>In order to save clock cycles, <c>SetText()</c> will only generate new vertices if the text is different than the string already stored by the class. This method contains some
/// of the last remaining natively compiled code. Most text will be black with a white outline - unless the text is relatively small - in which case it will simply be all black with no
/// white outline (in order to avoid artifacts).</remarks>
class TextOverlay : public RenObj
{
private:
    //--- Protected Variables ---//
    // ~ Static Rendering Data
    /// <summary>A static set of vertices which will serve as the template for drawing a letter (which is essentially just a 2D quad). These vertices
    /// make a rectangle which is 1.5 times tall as it is wide. These vertices will be multiplied by the "font size" to yield the final set of
    /// vertices which will be renderd.</summary>
    /// <remarks>There are an x,y, and z component to each vertex. There are 3 components to each vertex, 3 vertices to each triangle, and 2 triangles
    /// needed to render a single rectangular area which will contain one letter.</remarks>
    static GLfloat vert_letter[18];
    /// <summary>A set of fractional texture coordinates which represent the area that a single character takes up on the texture which contains
    /// all letters. Each letter which is to-be-rendered to the screen will ultimately be translated into an ascii value. This ascii value
    /// has an offset subtracted from it and the resulting value is then multiplied by each element in this set of texture coordinates to yield
    /// the final set of texture coordinates which will be rendered.</summary>
    /// <remarks>The texture which contains all the characters is expected to contain the characters in ascii order, with no gaps.</remarks>
    static GLfloat txtur_letter[12];
    // ~ Font Characteristics
    /// <summary>The "font size" which determines how large the letters appear on the screen. This value is multiplied by all vertices to yield
    /// the vertices which will be rendered to the screen. If this size is less than a certain value then the letters will be rendered as
    /// all black, as opposed to having a white outline. <seealso cref="TextOverlay::SetTextSize()"/></summary>
    /// <remarks>This member has no default initialization value, since its default value is determined via parameter.</remarks>
    GLfloat size;
    /// <summary>Stores the set of ascii values which represent the string which is to-be-rendered. This array will be updated anytime the
    /// text rendered by this class changes. Ultimately this array determines the texture indices (and thus the letters) that are used to
    /// render the desired text to the screen.<seealso cref="TextOverlay::num_chars"/></summary>
    /// <remarks>This pointer is initialized to the value -1 in the TextOverlay constructor(s).</remarks>
    GLuint * ascii_buf;
    /// <summary>The number of characters present in the string and thus the number of valid elements in the ascii_buf member. This count is updated
    /// each time the string-to-be-rendered changes. This value is used in many operations, such as when determining how many quads need to be rendered.
    /// <seealso cref="TextureOverlay::ascii_buf"/></summary>
    /// <remarks>Initialized to 0 in the TextOverlay constructor(s).</remarks>
    GLuint num_chars;
    //--- Protected Methods ---//
    // ~ Special Characters
    /// <summary>This method is called for each "special character" that needs to be adjusted vertically each frame (i.e. 'j'). The transformation
    /// matrix is updated to reflect the desired change. <seealso cref="TextOverlay::Draw()"/></summary>
    /// <param name="char_in">The character which needs to be vertically compensated, represented by an unsigned integer.</param>
    /// <param name="temp_trans_in">A pointer to the translation matrix, so that it may be modified.</param>
    /// <param name="scene_ptr_in">A pointer to the active scene which contains the text. This scene pointer is used to update the translation matrix.</param>
    /// <returns>An integer which will be either _OK (0) if the method resulted in a success, or _BAD (-1) if the method did not compensate.</returns>
    /// <remarks>The letters adjusted by this method include j,g,p,q,y, and the AT symbol. Note that this method is a bit of a hack and will most likely be done
    /// away with when true glyphs are implemented.</remarks>
    GLint PushVerticalCompensation(GLuint char_in, glm::mat4 * temp_trans_in, OGLMatrices& matrices_in);
public:
    //--- Public Methods ---//
    // ~ Constructor(s)
    /// <summary>The default constructor for the TextOverlay class which is invoked whenever the object is instantiated and is responsible for initiating
    /// members owned by this class. This method initializes any data which can be initialized ambiguosly to the string which will be rendered, since that
    /// string is not known at the time this class's constructor is called.
    /// <seealso cref="TextOverlay::TextOverlay(TextOverlay&)"/></summary>
    /// <param name="size_in">A GLfloat which will determine the size of the text as it is rendered to the screen.</param>
    /// <remarks>Note that there is no constructor available which sets the text. Text must be set by <c>SetText()</c>.</remarks>
    TextOverlay(GLfloat size_in);
    /// <summary>The copy constructor for the TextOverlay class, invoked whenever the object is instantiated by copying another instance (i.e. 
    /// <c>TextOverlay foo(bar);</c>). The method is responsible for initiating (and allocating memory for, if appropraite) all TextOverlay members using
    /// the data of an already existing instance of the object. In other words, this method "copies" an existing TextOverlay class, including the
    /// text assigned to it, if any. <seealso cref="TextOverlay::TextOverlay(GLfloat)"/><seealso cref="TextOverlay::operator=()"/></summary>
    /// <param name="textoverlay_in">The existing instance which will be copied.</param>
    /// <remarks>This method is not to be confused with the "=" operator method which is invoked by <c>foo=bar</c>.</remarks>
    TextOverlay(const TextOverlay & textoverlay_in);
    // ~ Operator(s)
    /// <summary>The assignment operator method for this class. This method is responsible for setting all of the members of the calling instance to
    /// be equal to the target instance (passed via parameter). This method differs from the copy constructor in that it must properly handle the
    /// potential existance of data already allocated-for. <seealso cref="TextOverlay::TextOverlay(TextOverlay &)"/></summary>
    /// <param name="textoverlay_in">The instance whose data will be copied. It may or may not contain text data.</param>
    /// <returns>Trivially returns a reference to the instance modified by this method (the calling object).</returns>
    /// <remarks>This method is invoked via <c>foo=bar;</c> which is not to be confused by the copy constructor which is invoked via
    /// <c>TextOverlay foo(bar);</c>.</remarks>
    TextOverlay& operator=(const TextOverlay & textoverlay_in);
    // ~ Getter(s)/Setter(s)
    /// <summary>This method populates the vertex, color, and texture coordinate arrays with data necessary to render the desired text to the scren.
    /// This method must be called at least once in order for any text to be rendered to the screen. When invoked, this method only updates relevant
    /// data members if the newly desired text differes from the existing text (or if there is no existing text).<seealso cref="TextOverlay::ascii_buf"/></summary>
    /// <param name = "format">This parameter is the string which will be rendered to the screen. Only basic characters are accepted at this time. This
    /// string can contain substrings such as <c>%d</c> which will result in the substitution of variable values (whom are also passed via parameter).</param>
    /// <param name="...">Any number of parameters which are variables referenced in the format string (the first parameter). These parameters must be
    /// referenced using the correct substring (i.e. integers are reperesented by "%d").</param>
    /// <returns>An integer which will be _BAD (-1) if the render data was not updated or _OK (0) if it was updated.</returns>
    /// <remarks>This method is one of the last remaining natively compiled sections of code, due to its usage of <c>va_list</c>, or arguement lists.</remarks>
    GLint SetText(const GLchar * format, ...);
    /// <summary>Sets the text size according to the parameter passed to the method. This method will modify vertices (if any already exist) so that they
    /// are scaled to the correct size. <seealso cref="TextOverlay::size"/></summary>
    /// <param name="size_in">The text size that is desired. This parameter will be checked to ensure that it is greater than 0 and different than
    /// the presently stored size.</param>
    /// <remarks>It is more effecient to set text size before populating the class with any text data.</remarks>
    void SetTextSize(GLfloat size_in);
    // ~ Rendering Methods
    /// <summary>A method which will be called each frame for each TextOverlay object and is responsible for binding relevant data to the appropriate shader program attributes, configuring the
    /// OpenGL state machine, and executing draw calls to render the object. The render id passed to the function can be used to prevent unneccesary calls while the structure passed to this
    /// method serves as the primary means of accessing/interacting with OpenGL. Note that each letter is rendered with a separate draw call in order to allow for different letter heights. In
    /// the future all letters could be rendered with an instanced rendering call (each letter would have a different translation matrix).
    /// <seealso cref="RenObj::BindBufferData()"/><seealso cref="RenObj::Prepare()"/><seealso cref="Scene::Draw()"/></summary>
    /// <param name="incoming_id">The render id of the most recently rendered object. This id can be used to prevent unneccesary data transfers or redundant state changes.</param>
    /// <param name="info_in">A structure which provides access to various shader program attributes & uniforms, pointers relevant to the rendering process, etc. This structure is essential
    /// to perform most of the basic rendering tasks.</param>
    /// <returns>An integer which is _OK (0) if the method executed without errors or _BAD (-1) if the method encountered an error. Currently this ALWAYS returns 0.</returns>
    /// <remarks>This method translates the current drawing location to the TextOverlay's origin before the draw call(s) are made. <c>TextOverlay::PushVerticalCompensation()</c> is
    /// called in this method for each letter whose height needs to be adjusted.</remarks>
    GLint Draw(GLint incoming_id, OGLUniforms& uniforms_in,OGLMatrices& matrices_in, const bool& textures_en, const bool& bumpmap_en);
    // ~ Destructor
    /// <summary>The destructor for the TextOverlay class which is responsible for freeing any memory owned by the class. The RenObj destructor is automatically invoked immediately prior to this method
    /// whenever a TextOverlay is destroyed/deleted.</summary>
    /// <remarks>Declared virtual as a stylistic choice.</remarks>
    virtual ~TextOverlay(void);
};
#pragma endregion
#pragma region LineStrip Class
/// <summary>A series of points, every two of which will be used as the start and endpoints of a line. All lines are converted into
/// vertex data which stores the lines in absolute coordinates. The object's origin is not translated to come <c>Draw()</c> time.</summary>
/// <remarks>Enabled depth testing in order to render 3D lines. <c>RenObj::origin</c> is not relevant for this class.</remarks>
class LineStrip : public RenObj
{
protected:
    //--- Protected Variables ---//
    /// <summary>The number of lines which are stored in the data members of the class.</summary>
    /// <remarks>This member is initialized to 0 in the LineStrip default constructor.</remarks>
    GLushort num_chunks;
public:
    //--- Public Methods ---//
    // ~ Constructor(s)
    /// <summary>The default constructor for the LineStrip class. This constructor is responsible for initializing all data owned by this class
    /// which is known at the time of the constructor.<seealso cref="LineStrip(LineStrip &)"/></summary>
    /// <remarks>No line data is added at the time that this class's constructor is invoked and lines should be added via <c>LineStrip::AddInstance()</c>.</remarks>
    LineStrip(void);
    /// <summary>The copy constructor for the LineStrip class. This constructor is responsible for initializing all data owned by this class which is
    /// known at the time of the constructor using an existing instance of the class.
    /// <seealso cref="LineStrip::LineStrip()"/><seealso cref="LineStrip::operator=()"/></summary>
    /// <param name="copy_source">The existing instance whose data will be copied in order to populate the new instance.</param>
    /// <remarks>Line data is not known at the time this class's constructor is invoked. This method is not to be confused with the "=" operator method.</remarks>
    LineStrip(const LineStrip & copy_source);
    // ~ Operator(s)
    /// <summary>The assignment operator method which is responsible for copying the data of one instance over to the current instance. Unlike the
    /// copy constructor, this method is responsible for properly releasing the (potentially) existing data held by the invoking instance.
    /// <seealso cref="LineStrip::LineStrip(LineStrip &)"/></summary>
    /// <param name="copy_source">The existing class instance which will be copied. This instance may or may not contain line segment data.</param>
    /// <returns>Trivially returns a reference to the LineStrip class.</returns>
    /// <remarks>This assignment operator method is invoked via <c>foo=bar;</c> and is not to be confused with the class's copy constructor.</remarks>
    LineStrip & operator=(const LineStrip & copy_source);
    // ~ Getter(s)/Setter(s)
    /// <summary>Adds a line segment to the class instance using absolute coordinates for a start and end point (i.e. [x1,y1,z1,x2,y2,z2]). The RenObj data
    /// arrays (i.e. vertices, etc.) are all allocated a (medium-to-large) static number of elements so that the arrays do not have to be resized (and thus
    /// copied) each time a new line segment is added. The data arrays are [only] automatically resized when they are about to reach capacity.
    /// <seealso cref="LineStrip::num_chunks"/><seealso cref="LINE_ARR_CHUNK"/></summary>
    /// <param name="start_pt">A GLfloat array of three elements which represent the absolute x,y, and z coordinates of the beginning of the line segment.</param>
    /// <param name="end_pt">A GLfloat array of three elements which represent the absolute x,y, and z coordinates of the end of the line segment.</param>
    /// <returns>An integer which will be _OK (0) if there are no errors and _BAD (-1) if an error occured. Currently this method ALWAYS returns 0.</returns>
    GLint AddInstance(GLfloat start_pt[3],GLfloat end_pt[3]);
    // ~ Rendering Methods
    /// when commenting, note that we translate to 0,0,0 instead of the object's origin
    // ~ Rendering Methods
    /// <summary>A method which will be called each frame for each LineStrip object and is responsible for binding relevant data to the appropriate shader program attributes, configuring the
    /// OpenGL state machine, and executing draw calls to render the object. The render id passed to the function can be used to prevent unneccesary calls while the structure passed to this
    /// method serves as the primary means of accessing/interacting with OpenGL.<seealso cref="RenObj::BindBufferData()"/><seealso cref="RenObj::Prepare()"/><seealso cref="Scene::Draw()"/></summary>
    /// <param name="incoming_id">The render id of the most recently rendered object. This id can be used to prevent unneccesary data transfers or redundant state changes.</param>
    /// <param name="info_in">A structure which provides access to various shader program attributes & uniforms, pointers relevant to the rendering process, etc. This structure is essential
    /// to perform most of the basic rendering tasks.</param>
    /// <returns>An integer which is _OK (0) if the method executed without errors or _BAD (-1) if the method encountered an error. Currently this ALWAYS returns 0.</returns>
    /// <remarks>This method translates the current drawing location to [0.0,0.0,0.0] before the draw call is made, thus all line start and end points should be
    /// given in absolute terms.</remarks>
    GLint Draw(GLint incoming_id, OGLUniforms& uniforms_in,OGLMatrices& matrices_in, const bool& textures_en, const bool& bumpmap_en);
    // ~ Destructor
    /// <summary>The destructor for the LineStrip class which is responsible for freeing any memory owned by the class. The RenObj destructor is automatically invoked immediately prior to this method
    /// whenever a LineStrip is destroyed/deleted.</summary>
    /// <remarks>Declared virtual as a stylistic choice.</remarks>
    virtual ~LineStrip(void);
};
#pragma endregion
#pragma region Group Class
/// <summary>A group of identical objects which may differ by scale, rotation, and translation - all of which are rendered with a single draw call. The class
/// is instantiated by "seeding" it with a pointer to an existing RenObj (or one of its derivatives). Within the Group, an instance with a specific scale,
/// rotation, and translation can be added via <c>Group::AddInstance()</c>, and a large number of instances can be added via <c>Group::AddRandomInstances()</c>.
/// The method which adds random instances does so using scale/rotation/translation matrices within a ranges specified by various methods available to this
/// class.</summary>
/// <remarks>All objects are rendered using a single set of vertices, colors, and texture coordinates, which is why this class is capable of rendering
/// hundreds of thousands of objects at a much lower cost then rendering them individually. Each member of the Group can be rendered with a different
/// scale, rotation, or translation component which makes it a very suitable container for objects which repeat often with slight varying characteristics.</remarks>
class Group : public RenObj
{
protected:
    //--- Protected Variables ---//
    // ~ Per Instance Data
    /// <summary>A collection of scale matrices which represent the x, y, and z scale of each object in the Group. These matrices are stored separately from
    /// the combined matrix so that the scale of each instance in the Group may be manipulated separately (currently such an operation is not implemented).
    /// These matrices are stored in a vector for easy addition/removal. <seealso cref="Group::gp_srt"/></summary>
    /// <remarks>This vector does not need to be initialized in the constructor.</remarks>
    std::vector<glm::mat4> gp_scale;
    /// <summary>A collection of scale matrices which represent the x, y, and z rotation of each object in the Group. These matrices are stored separately from
    /// the combined matrix so that the rotation of each instance in the Group may be manipulated separately (currently such an operation is not implemented).
    /// These matrices are stored in a vector for easy addition/removal. <seealso cref="Group::gp_srt"/></summary>
    /// <remarks>This vector does not need to be initialized in the constructor.</remarks>
    std::vector<glm::mat4> gp_rotate;
    /// <summary>A collection of three-component vectors which represent the x, y, and z location of each object in the Group. This collection of vectors is stored
    /// separately from the combined matrix so that the location of each instance in the Group may be manipulated separately (currently such an operation is not
    /// implemented). These matrices are stored in a vector for easy addition/removal. <seealso cref="Group::gp_srt"/></summary>
    /// <remarks>This vector does not need to be initialized in the constructor.</remarks>
    std::vector<glm::vec3> gp_trans;
    /// <summary>A collection of matrices which are calculated via <c>srt = translation * rotation * scale</c>. This combined collection of matrices is used
    /// when the instanced draw method is called and determines the orientation, location, and size of each member of the Group. This pointer is used to
    /// pass the collection of matrices to the shader program each frame. <seealso cref="Group::instance_count"/></summary>
    /// <remarks>Initialized to the value -1 in this class's constructor(s).</remarks>
    glm::mat4 * gp_srt;
    /// <summary>The number of members in the group. This number indicates how many elements are in each vector of matrices, how many elements are stored 
    /// in the combined matrix array, and ultimately how many objects will be rendered to the scene.<seealso cref="Group::gp_srt"/></summary>
    /// <remarks>Initialized to 0 in the Group default constructor.</remarks>
    long int instance_count;
    // ~ Randomn Instance Generator Limits
    /// <summary>The minimum amount of rotation on the x-axis, in degrees, that elements added randomly to the Group will be subject to. When members are
    /// added randomly their rotations are determined randomly within a minimum and maximum range. Negative values are accepted.
    /// <seealso cref="Group::max_x_rot"/><seealso cref="Group::SetRotationRanges()"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <remarks>Initialized to -45 degrees in the Group constructor.</remarks>
    GLfloat min_x_rot;
    /// <summary>The maximum amount of rotation on the x-axis, in degrees, that elements added randomly to the Group will be subject to. When members are
    /// added randomly their rotations are determined randomly within a minimum and maximum range. Negative values are accepted.
    /// <seealso cref="Group::min_x_rot"/><seealso cref="Group::SetRotationRanges()"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <remarks>Initialized to +45 degrees in the Group constructor.</remarks>
    GLfloat max_x_rot;
    /// <summary>The minimum amount of rotation on the y-axis, in degrees, that elements added randomly to the Group will be subject to. When members are
    /// added randomly their rotations are determined randomly within a minimum and maximum range. Negative values are accepted.
    /// <seealso cref="Group::max_y_rot"/><seealso cref="Group::SetRotationRanges()"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <remarks>Initialized to -45 degrees in the Group constructor.</remarks>
    GLfloat min_y_rot;
    /// <summary>The maximum amount of rotation on the y-axis, in degrees, that elements added randomly to the Group will be subject to. When members are
    /// added randomly their rotations are determined randomly within a minimum and maximum range. Negative values are accepted.
    /// <seealso cref="Group::min_y_rot"/><seealso cref="Group::SetRotationRanges()"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <remarks>Initialized to +45 degrees in the Group constructor.</remarks>
    GLfloat max_y_rot;
    /// <summary>The minimum amount of rotation on the z-axis, in degrees, that elements added randomly to the Group will be subject to. When members are
    /// added randomly their rotations are determined randomly within a minimum and maximum range. Negative values are accepted.
    /// <seealso cref="Group::max_z_rot"/><seealso cref="Group::SetRotationRanges()"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <remarks>Initialized to -45 degrees in the Group constructor.</remarks>
    GLfloat min_z_rot;
    /// <summary>The minimum amount of rotation on the z-axis, in degrees, that elements added randomly to the Group will be subject to. When members are
    /// added randomly their rotations are determined randomly within a minimum and maximum range. Negative values are accepted.
    /// <seealso cref="Group::min_z_rot"/><seealso cref="Group::SetRotationRanges()"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <remarks>Initialized to +45 degrees in the Group constructor.</remarks>
    GLfloat max_z_rot;
    /// <summary>The minimum size on the x-axis, as a ratio, that elements added randomly to the Group will be scaled to. When members are
    /// added randomly their scale is determined randomly within a minimum and maximum range.
    /// <seealso cref="Group::max_x_scale"/><seealso cref="Group::SetScaleRanges()"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <remarks>Initialized to 0.1 in the Group constructor which implies object will be as small as 1/10th their original size on the x-axis.</remarks>
    GLfloat min_x_scale;
    /// <summary>The maximum size on the x-axis, as a ratio, that elements added randomly to the Group will be scaled to. When members are
    /// added randomly their scale is determined randomly within a minimum and maximum range.
    /// <seealso cref="Group::min_x_scale"/><seealso cref="Group::SetScaleRanges()"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <remarks>Initialized to 2.0 in the Group constructor which implies object will be as large as twice their original size on the x-axis.</remarks>
    GLfloat max_x_scale;
    /// <summary>The minimum size on the y-axis, as a ratio, that elements added randomly to the Group will be scaled to. When members are
    /// added randomly their scale is determined randomly within a minimum and maximum range.
    /// <seealso cref="Group::max_y_scale"/><seealso cref="Group::SetScaleRanges()"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <remarks>Initialized to 0.1 in the Group constructor which implies object will be as small as 1/10th their original size on the y-axis.</remarks>
    GLfloat min_y_scale;
    /// <summary>The maximum size on the y-axis, as a ratio, that elements added randomly to the Group will be scaled to. When members are
    /// added randomly their scale is determined randomly within a minimum and maximum range.
    /// <seealso cref="Group::min_y_scale"/><seealso cref="Group::SetScaleRanges()"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <remarks>Initialized to 2.0 in the Group constructor which implies object will be as large as twice their original size on the y-axis.</remarks>
    GLfloat max_y_scale;
    /// <summary>The minimum size on the z-axis, as a ratio, that elements added randomly to the Group will be scaled to. When members are
    /// added randomly their scale is determined randomly within a minimum and maximum range.
    /// <seealso cref="Group::max_z_scale"/><seealso cref="Group::SetScaleRanges()"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <remarks>Initialized to 0.1 in the Group constructor which implies object will be as small as 1/10th their original size on the z-axis.</remarks>
    GLfloat min_z_scale;
    /// <summary>The maximum size on the z-axis, as a ratio, that elements added randomly to the Group will be scaled to. When members are
    /// added randomly their scale is determined randomly within a minimum and maximum range.
    /// <seealso cref="Group::min_z_scale"/><seealso cref="Group::SetScaleRanges()"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <remarks>Initialized to 2.0 in the Group constructor which implies object will be as large as twice their original size on the z-axis.</remarks>
    GLfloat max_z_scale;
    /// <summary>The minimum location on the x-axis that elements added randomly to the Group will be scaled to. When members are added randomly
    /// their location is determined randomly within a minimum and maximum range. Location is also referred to as "translation" through-out this code.
    /// <seealso cref="Group::max_x_scale"/><seealso cref="Group::SetScaleRanges()"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <remarks>Initialized to -100 in the Group constructor which implies randomly added objects may be as far as 100 units away in the
    /// negative direction on the x-axis.</remarks>
    GLfloat min_x_trans;
    /// <summary>The maximum location on the x-axis that elements added randomly to the Group will be scaled to. When members are added randomly
    /// their location is determined randomly within a minimum and maximum range. Location is also referred to as "translation" through-out this code.
    /// <seealso cref="Group::min_x_scale"/><seealso cref="Group::SetScaleRanges()"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <remarks>Initialized to +100 in the Group constructor which implies randomly added objects may be as far as 100 units away in the
    /// positive direction on the x-axis.</remarks>
    GLfloat max_x_trans;
    /// <summary>The minimum location on the y-axis that elements added randomly to the Group will be scaled to. When members are added randomly
    /// their location is determined randomly within a minimum and maximum range. Location is also referred to as "translation" through-out this code.
    /// <seealso cref="Group::max_y_scale"/><seealso cref="Group::SetScaleRanges()"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <remarks>Initialized to -100 in the Group constructor which implies randomly added objects may be as far as 100 units away in the
    /// negative direction on the y-axis.</remarks>
    GLfloat min_y_trans;
    /// <summary>The maximum location on the y-axis that elements added randomly to the Group will be scaled to. When members are added randomly
    /// their location is determined randomly within a minimum and maximum range. Location is also referred to as "translation" through-out this code.
    /// <seealso cref="Group::min_y_scale"/><seealso cref="Group::SetScaleRanges()"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <remarks>Initialized to +100 in the Group constructor which implies randomly added objects may be as far as 100 units away in the
    /// positive direction on the y-axis.</remarks>
    GLfloat max_y_trans;
    /// <summary>The minimum location on the z-axis that elements added randomly to the Group will be scaled to. When members are added randomly
    /// their location is determined randomly within a minimum and maximum range. Location is also referred to as "translation" through-out this code.
    /// <seealso cref="Group::max_z_scale"/><seealso cref="Group::SetScaleRanges()"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <remarks>Initialized to -100 in the Group constructor which implies randomly added objects may be as far as 100 units away in the
    /// negative direction on the z-axis.</remarks>
    GLfloat min_z_trans;
    /// <summary>The maximum location on the z-axis that elements added randomly to the Group will be scaled to. When members are added randomly
    /// their location is determined randomly within a minimum and maximum range. Location is also referred to as "translation" through-out this code.
    /// <seealso cref="Group::min_z_scale"/><seealso cref="Group::SetScaleRanges()"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <remarks>Initialized to +100 in the Group constructor which implies randomly added objects may be as far as 100 units away in the
    /// positive direction on the z-axis.</remarks>
    GLfloat max_z_trans;
    // ~ Etc
    /// <summary>A pointer to the active scene. Storing this pointer allows this class to update the Scene class's object count - debugging functionality
    /// for the most part.</summary>
    /// <remarks>Initialized to the value -1 in the Group default constructor.</remarks>
    Scene * gp_scene_ptr;
public:
    //--- Public Methods ---//
    // ~ Constructors
    /// <summary>The sole constructor for the Group class which requires a shared pointer to a RenObj (or one of its derived classes). Vertices, color, texture
    /// coordinates and other data is copied from the source RenObj and used to form the "base object" - all objects rendered by this Group will be a "clone" of the
    /// "base object", although each member can vary by scale, rotation, and location (a.k.a. translation).<seealso cref="Group::Group(Group &)"/></summary>
    /// <param name="renobj_seed"/>A shared pointer to a RenObj derived class which serves as the "original" object from which all Group members will be "cloned".</param>
    /// <remarks>No members (a.k.a. instances) are added at the time of the constructor and must be added via <c>Group::AddInstance()</c> or
    /// <c>Group::AddRandomInstance()</c>.</remarks>
    Group(ptrRenObj renobj_seed);
    /// <summary>The copy constructor for the Group class which copies an existing Group instance. All data is copied including members of the Group.
    /// <seealso cref="Group::Group(ptrRenObj)"/><seealso cref="Group::operator=()"/></summary>
    /// <param name="group_in"/>The existing Group class instance whom will be copied.</param>
    /// <remarks>This method is invoked via <c>Group foo(bar);</c> and is not to be confused with the "=" operator which is invoked via <c>foo=bar;</c>.</remarks>
    Group(const Group & group_in);
    // ~ Operators
    /// <summary>The assignment operator method which is responsible for copying the data of one instance over to the current instance. Unlike the
    /// copy constructor, this method is responsible for properly releasing the (potentially) existing data held by the invoking instance.
    /// <seealso cref="Group::Group(Group &)"/></summary>
    /// <param name="group_in"/>The existing class instance which will be copied. This instance of the Group class may or may not be populated with members.</param>
    /// <returns>Trivially returns a reference to the Group class.</returns>
    /// <remarks>This assignment operator method is invoked via <c>foo=bar;</c> and is not to be confused with the class's copy constructor.</remarks>
    Group& operator=(const Group & group_in);
    // ~ Getter(s)/Setter(s)
    /// <summary>A very simplistic "getter" which retrieves the total number of members in the Group. <seealso cref="Group::AddRandomInstances()"/>
    /// <seealso cref="Group::AddInstance()"/></summary>
    /// <returns>A long integer which indicates the number of objects in the Group.</returns>
    /// <remarks>The member associated with this method is initialized to 0 in the Group default constructor.</remarks>
    long int GetObjCount(void);
    /// <summary>A very simplistic "setter" which stores a pointer to the active scene. This pointer is used to update the Scene's total object
    /// count whenever a new object is added to the Group.</summary>
    /// <param name="scene_ptr_in"/>A pointer to the active scene </param>
    /// <returns>Returns an integer which indicates the success of the method. Currently this method will ALWAYS return _OK (0).</returns>
    /// <remarks>The member associated with this method is initialized to -1 in the Group default constructor. This method may be removed in the future
    /// should a more elegant solution come along to replace the Scene pointer.</remarks>
    GLint SetScenePtr(Scene * scene_ptr_in);
    /// <summary>A method native to this class's base class, RenObj. This base class's default behavior for this method is overridden so that setting
    /// the origin for a Group class will set the origin for all members which are rendered by this class. Setting the origin for the entire class
    /// requires modification of every translation matrix and "combined matrix" for each member in the Group, thus it is a rather expensive operation.
    /// <seealso cref="RenObj::origin"/></summary>
    /// <param name="x_in"/>The desired location of the entire Group on the x-axis. In other words, all members in the Group will be offset on the x-axis
    /// by this magnitude.</param>
    /// <param name="y_in"/>The desired location of the entire Group on the y-axis. In other words, all members in the Group will be offset on the y-axis
    /// by this magnitude.</param>
    /// <param name="z_in"/>The desired location of the entire Group on the z-axis. In other words, all members in the Group will be offset on the z-axis
    /// by this magnitude.</param>
    /// <returns>An integer which indicates the success of the method. Returns <c>_OK</c> (0) if there were no errors or <c>_BAD</c> (-1) if for some
    /// reason there are more members in the Group than indicated by <c>Group::instance_count</c>.</returns>
    /// <remarks>Each of the parameters sent to this method have a default value of 0.0 which means that if this method is invoked without
    /// passing any parameters then the default origin of the group will be (0,0,0). If possible, always set the origin of the Group before adding
    /// any members to it!</remarks>
    GLint SetOrigin(GLfloat x_in=0.0, GLfloat y_in=0.0, GLfloat z_in=0.0);
    /// <summary>Sets the minimum and maximum scaling factors (on all three axis) of any members which are added "randomly" to the Group. All minimum
    /// ranges must be less-than-or-equal-to their associated maximum range or else the method will not be executed.
    /// <seealso cref="Group::min_x_scale"/><seealso cref="Group::max_x_scale"/><seealso cref="Group::min_y_scale"/><seealso cref="Group::max_y_scale"/>
    /// <seealso cref="Group::min_z_scale"/><seealso cref="Group::max_z_scale"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <param name="x_min_in"/>Regarding objects added via <c>Group::AddRandomInstances()</c>, a GLfloat which determines the minimum size of the object
    /// on the x-axis. Must be less-than-or-equal-to the parameter "x_max_in".</param>
    /// <param name="x_max_in"/>Regarding objects added via <c>Group::AddRandomInstances()</c>, a GLfloat which determines the maximum size of the object
    /// on the x-axis. Must be greater-than-or-equal-to the parameter "x_min_in".</param>
    /// <param name="y_min_in"/>Regarding objects added via <c>Group::AddRandomInstances()</c>, a GLfloat which determines the minimum size of the object
    /// on the y-axis. Must be less-than-or-equal-to the parameter "y_max_in".</param>
    /// <param name="y_max_in"/>Regarding objects added via <c>Group::AddRandomInstances()</c>, a GLfloat which determines the maximum size of the object
    /// on the y-axis. Must be greater-than-or-equal-to the parameter "y_min_in".</param>
    /// <param name="z_min_in"/>Regarding objects added via <c>Group::AddRandomInstances()</c>, a GLfloat which determines the minimum size of the object
    /// on the z-axis. Must be less-than-or-equal-to the parameter "z_max_in".</param>
    /// <param name="z_max_in"/>Regarding objects added via <c>Group::AddRandomInstances()</c>, a GLfloat which determines the maximum size of the object
    /// on the z-axis. Must be greater-than-or-equal-to the parameter "z_min_in".</param>
    /// <remarks>In this class's default constructor the "minimum" members associated with this method are initialized to 0.1 and the "maximum" members
    /// are initialized to 2.0.</remarks>
    void SetScaleRanges(GLfloat x_min_in, GLfloat x_max_in, GLfloat y_min_in, GLfloat y_max_in, GLfloat z_min_in, GLfloat z_max_in);
    /// <summary>Sets the minimum and maximum rotation ranges (on all three axis; in degrees) of any members which are added "randomly" to the Group. All minimum
    /// ranges must be less-than-or-equal-to their associated maximum range or else the method will not be executed. Negative degrees may be used and
    /// they are equivalent to <c>360 - value</c>.
    /// <seealso cref="Group::min_x_rot"/><seealso cref="Group::max_x_rot"/><seealso cref="Group::min_y_rot"/><seealso cref="Group::max_y_rot"/>
    /// <seealso cref="Group::min_z_rot"/><seealso cref="Group::max_z_rot"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <param name="x_min_in"/>Regarding objects added via <c>Group::AddRandomInstances()</c>, a GLfloat which determines the minimum rotation of the object
    /// about the x-axis. Must be less-than-or-equal-to the parameter "x_max_in".</param>
    /// <param name="x_max_in"/>Regarding objects added via <c>Group::AddRandomInstances()</c>, a GLfloat which determines the maximum rotation of the object
    /// about the x-axis. Must be greater-than-or-equal-to the parameter "x_min_in".</param>
    /// <param name="y_min_in"/>Regarding objects added via <c>Group::AddRandomInstances()</c>, a GLfloat which determines the minimum rotation of the object
    /// about the y-axis. Must be less-than-or-equal-to the parameter "y_max_in".</param>
    /// <param name="y_max_in"/>Regarding objects added via <c>Group::AddRandomInstances()</c>, a GLfloat which determines the maximum rotation of the object
    /// about the y-axis. Must be greater-than-or-equal-to the parameter "y_min_in".</param>
    /// <param name="z_min_in"/>Regarding objects added via <c>Group::AddRandomInstances()</c>, a GLfloat which determines the minimum rotation of the object
    /// about the z-axis. Must be less-than-or-equal-to the parameter "z_max_in".</param>
    /// <param name="z_max_in"/>Regarding objects added via <c>Group::AddRandomInstances()</c>, a GLfloat which determines the maximum rotation of the object
    /// about the z-axis. Must be greater-than-or-equal-to the parameter "z_min_in".</param>
    /// <remarks>In this class's default constructor the "minimum" members associated with this method are initialized to -45 degrees and the "maximum" members
    /// are initialized to positive 45 degrees.</remarks>
    void SetRotationRanges(GLfloat x_min_in, GLfloat x_max_in, GLfloat y_min_in, GLfloat y_max_in, GLfloat z_min_in, GLfloat z_max_in);
    /// <summary>Sets the minimum and maximum translation ranges (on all three axis) of any members which are added "randomly" to the Group. All minimum
    /// ranges must be less-than-or-equal-to their associated maximum range or else the method will not be executed. An object's translation is analogous to
    /// its location in 3D space.
    /// <seealso cref="Group::min_x_trans"/><seealso cref="Group::max_x_trans"/><seealso cref="Group::min_y_trans"/><seealso cref="Group::max_y_trans"/>
    /// <seealso cref="Group::min_z_trans"/><seealso cref="Group::max_z_trans"/><seealso cref="Group::AddRandomInstances()"/></summary>
    /// <param name="x_min_in"/>Regarding objects added via <c>Group::AddRandomInstances()</c>, a GLfloat which determines the minimum location of the object
    /// on the x-axis. Must be less-than-or-equal-to the parameter "x_max_in".</param>
    /// <param name="x_max_in"/>Regarding objects added via <c>Group::AddRandomInstances()</c>, a GLfloat which determines the maximum location of the object
    /// on the x-axis. Must be greater-than-or-equal-to the parameter "x_min_in".</param>
    /// <param name="y_min_in"/>Regarding objects added via <c>Group::AddRandomInstances()</c>, a GLfloat which determines the minimum location of the object
    /// on the y-axis. Must be less-than-or-equal to the parameter "y_max_in".</param>
    /// <param name="y_max_in"/>Regarding objects added via <c>Group::AddRandomInstances()</c>, a GLfloat which determines the maximum location of the object
    /// on the y-axis. Must be greater-than-or-equal to the parameter "y_min_in".</param>
    /// <param name="z_min_in"/>Regarding objects added via <c>Group::AddRandomInstances()</c>, a GLfloat which determines the minimum location of the object
    /// on the z-axis. Must be less-than-or-equal to the parameter "z_max_in".</param>
    /// <param name="z_max_in"/>Regarding objects added via <c>Group::AddRandomInstances()</c>, a GLfloat which determines the maximum location of the object
    /// on the z-axis. Must be greater-than-or-equal to the parameter "z_min_in".</param>
    /// <remarks>In this class's default constructor the "minimum" members associated with this method are initialized to -100 and the "maximum" members
    /// are initialized to 100.</remarks>
    void SetTranslationRanges(GLfloat x_min_in, GLfloat x_max_in, GLfloat y_min_in, GLfloat y_max_in, GLfloat z_min_in, GLfloat z_max_in);
    // ~ Instancing
    /// <summary>Adds a specified number of "cloned" objects to the Group which will be differ by scale, rotation, and translation/location. The scale, rotation,
    /// and translation for each object is determined randomly but within a range specified by members of this class. If no parameter is passed to this method
    /// then it will generate one object by default.
    /// <seealso cref="Group::SetScaleRanges()"/><seealso cref="Group::SetRotationRanges()"/><seealso cref="Group::SetTranslationRanges()"/>
    /// <seealso cref="Group::AddInstance()"/></summary>
    /// <param name="how_many"/>A long integer which determines the number of objects generated with semi-random scale, rotation, and translation matrices. This
    /// parameter must be greater-than-or-equal to 1 for this method to run.</param>
    /// <returns>A long integer representing the number of objects in the Group including the objects just added. Returns _BAD (-1) if the parameter passed
    /// to this method is not valid (it must be greater-than-or-equal to 1).</returns>
    /// <remarks>The object which populates the group is determined by the parameter passed to this class's constructor.</remarks>
    long int AddRandomInstances(long int how_many=1);
    /// <summary>Adds a single object to the Group with the specified scale, rotation, and translation/location. There is very little logic protecting
    /// the inputs to this method so it is up to the caller to make sure that scale and rotation matrices are valid. The translation vector is converted
    /// to a matrix before it is stored. All three matrices are multiplied together to yield the combined matrix which is used by the instanced rendering
    /// call each frame.<seealso cref="Group::AddRandomInstances()"/><seealso cref="Group:gp_srt"/></summary>
    /// <param name="scale_in"/>The 4 x 4 scale matrix which represents the scale of the object on all three axis. There is no logic implemented by this
    /// method which ensures this matrix is in valid format.</param>
    /// <param name="rotate_in"/>The 4 x 4 rotation matrix which represents the rotation of the object on all three axis. There is no logic implemented by this
    /// method which ensures this matrix is in valid format.</param>
    /// <param name="trans_vec_in"/>The 3 element translation vector which represents the x,y,z location of the object in 3D space. This vector is converted
    /// into a 4 x 4 matrix before it is stored.</param>
    /// <returns>A long integer which represents the number of objects in the Group including the recently added object.</returns>
    /// <remarks>The object which populates the group is determined by the parameter passed to this class's constructor.</remarks>
    long int AddInstance(glm::mat4 scale_in, glm::mat4 rotate_in, glm::vec3 trans_vec_in);
    // ~ Rendering
    /// <summary>A method which will be called each frame for each LineStrip object and is responsible for binding relevant data to the appropriate shader program attributes, configuring the
    /// OpenGL state machine, and executing draw calls to render the object. The render id passed to the function can be used to prevent unneccesary calls while the structure passed to this
    /// method serves as the primary means of accessing/interacting with OpenGL. This implementation of the Draw() method differs from other dervied classes since it makes a call to
    /// <c>glDrawArraysInstanced()</c> and requires an array of combined matrices (translation * rotation * scale).
    /// <seealso cref="RenObj::BindBufferData()"/><seealso cref="RenObj::Prepare()"/><seealso cref="Scene::Draw()"/><seealso cref="Group:gp_srt"/></summary>
    /// <param name="incoming_id"/>The render id of the most recently rendered object. This id can be used to prevent unneccesary data transfers or redundant state changes.</param>
    /// <param name="info_in"/>A structure which provides access to various shader program attributes & uniforms, pointers relevant to the rendering process, etc. This structure is essential
    /// to perform most of the basic rendering tasks.</param>
    /// <returns>An integer which is _OK (0) if the method executed without errors or _BAD (-1) if the the number of objects in the Group is less than 1.</returns>
    /// <remarks>This method does NOT translate to <c>RenObj::origin</c> before executing a draw call since it must send an array of translation * rotation * scale matrices to
    /// the shader program immediately prior to making the call.</remarks>
    GLint Draw(GLint incoming_id, OGLUniforms& uniforms_in,OGLMatrices& matrices_in, const bool& textures_en, const bool& bumpmap_en);
    // ~ Destructor
    /// <summary>The destructor for the Group class which is responsible for freeing any memory owned by the class. The RenObj destructor is automatically invoked immediately prior to this method
    /// whenever a Group is destroyed/deleted.</summary>
    /// <remarks>Declared virtual as a stylistic choice.</remarks>
    virtual ~Group(void);
};
#pragma endregion
#pragma region BlockArray class
/// <summary>A class intended as a means for rendering voxels. Currently voxel implementation is incomplete so this class simply holds the data required to render the voxels, namely the vertices.
/// <seealso cref="Voxel"/><seealso cref="VoxelField"/></summary>
/// <remarks>This method averages any normals which share an edge segment.</remarks>
class BlockArray : public RenObj
{
private:
    /// <summary>A collection (std::vector) of 3-element vectors which represent the location of the origin of the normal vector of each and every vertex in the entire collection.
    /// <seealso cref="BlockArray::GetNormals()"/><seealso cref="BlockArray::normal_lines"/></summary>
    /// <remarks>This collection is not initialized in this class's constructor but in <c>BlockArray::UpdateVertices()</c>.</remarks>
    std::vector<glm::vec3> normal_starts;
    /// <summary>A collection (std::vector) of 3-element vectors which represent the direction of the normal vector of each and every vertex in the entire collection.
    /// <seealso cref="BlockArray::GetNormals()"/><seealso cref="BlockArray::normal_starts"/></summary>
    /// <remarks>This collection is not initialized in this class's constructor but in <c>BlockArray::UpdateVertices()</c>.</remarks>
    std::vector<glm::vec3> normal_lines;
    GLuint * element_indices;
    GLuint element_count;
    GLfloat render_origin[3];
public:
    // ~ Constructor(s)
    /// <summary>The default constructor for this class which takes no parameters and does little but initialize various RenObj members.
    /// <seealso cref="BlockArray::BlockArray(BlockArray&)"/></summary>
    BlockArray(void);
    /// <summary>The copy constructor for this class which is responsible for copying data from an existing BlockArray object in order to create a new instance of the class. The RenObj copy
    /// constructor is invoked immediately prior to this method.
    /// <seealso cref="BlockArray::BlockArray(void)"/></summary>
    /// <param name="copy_source"/>The existance BlockArray instance whose data will be copied in order to create a new instance.</param>
    /// <remarks>This method is invoked via <c>BlockArray foo(bar)</c> and is not to be confused with the "=" operator.</remarks>
    BlockArray(const BlockArray& copy_source);
    // ~ Operator(s)
    /// <summary>The "=" operator method for the BlockArray class. This method copies all data from one existing BlockArray instance over to another. Unlike the copy constructor,
    /// this method is responsible for correctly disposing of any data which is potentially already stored with the existing class instance.</summary>
    /// <param name="copy_source"/>The existance BlockArray instance whose data will be copied over to another already-existing instance. In other words, it is the element
    /// on the right-hand side of the equals sign.</param>
    /// <returns>Trivially returns a reference to BlockArray instance which was modified, a.k.a. <c>*this</c>.</returns>
    /// <remarks>This method is invoked via <c>foo=bar</c> and is not to be confused with the copy constructor.</remarks>
	BlockArray& operator=(const BlockArray & copy_source);
    /// <summary>This method takes various critical parameters and populates its class members with their data. This method currently is also responsible for averaging
    /// any normals which share an edge index/segment.</summary>
    /// <param name="vertices_in"/>An array of floats which represents the pool of unique vertices to which the element indices will be indexed.</param>
    /// <param name="arr_len_in"/>An unsigned integer which represents the size of the first parameter, the array of vertices. This parameter must be greater than 8 and divisible
    /// by 9 or else this method will return _BAD (-1) before executing its body.</param>
    /// <param name="edges_in"/>An array of unsigned integers which represent the index of the gridline segment on which each vertex rests. Due to the nature of the Marching Cubes algorithm,
    /// all vertices must lie somewhere on the gridlines which define the area in which the voxels are rendered. This information is used to average any normals which share a gridline segment.
    /// This array is expected to contain one element for every one vertex (x,y,z), thus this array size should be exactly one-third the size of the vertex array in order for this method
    /// to operate properly.</param>
    /// <param name="special_edge_arr_length"/>An unsigned integer which represents the maximum value that any element in the <c>edges_in</c> parameter will contain. This parameter assists in
    /// normal averaging.</param>
    /// <remarks>This method performs the majority of the "work" necessary in order to convert a collection of vertices and other voxel data into a form usable by our
    /// rendering loop (which is a class dervied from RenObj).</remarks>
    GLint UpdateVertices(const std::vector<glm::vec3> & vertices_in,const std::vector<glm::vec3> & normals_in,const std::vector<GLuint> & elements_in, const std::vector<glm::vec4> & colors_in, const GLfloat render_origin_in[3], const GLfloat middle_in[3], const GLfloat radius_in);
    /// <summary></summary>
    /// <summary>A method which will be called each frame for each BlockArray object and is responsible for binding relevant data to the appropriate shader program attributes, configuring the
    /// OpenGL state machine, and executing draw calls to render the object. The render id passed to the function can be used to prevent unneccesary calls while the structure passed to this
    /// method serves as the primary means of accessing/interacting with OpenGL. Since the functionality associated with this method is still in development, this method is rather barren.
    /// <seealso cref="RenObj::BindBufferData()"/><seealso cref="RenObj::Prepare()"/><seealso cref="Scene::Draw()"/></summary>
    /// <param name="incoming_id"/>The render id of the most recently rendered object. This id can be used to prevent unneccesary data transfers or redundant state changes.</param>
    /// <param name="info_in"/>A structure which provides access to various shader program attributes & uniforms, pointers relevant to the rendering process, etc. This structure is essential
    /// to perform most of the basic rendering tasks.</param>
    /// <returns>An integer which is _OK (0) if the method executed without errors or _BAD (-1) if the method encountered an error. Currently this ALWAYS returns 0.</returns>
    /// <remarks>As it is currently implemented, this method will always call <c>RenObj::BindBufferData()</c> and <c>RenObj::Prepare()</c> regardless of parameters passed to it.</remarks>
    GLint Draw(GLint incoming_id, OGLUniforms& uniforms_in,OGLMatrices& matrices_in, const bool& textures_en, const bool& bumpmap_en);
    /// <summary>Processes the normals stored in this class's members and converts all normal lines into a single LineStrip instance, which is returned to the caller.
    /// <seealso cref="normal_starts"/> <seealso cref="normal_lines"/><seealso cref="BARR_NORMAL_LEN"/></summary>
    /// <returns>A LineStrip which contains all normal lines for each face on the set of polygons represented by the voxel values.</returns>
    /// <remarks>The length of each normal is determined by the value assigned to <c>BARR_NORMAL_LEN</c>.</remarks>
    LineStrip GetNormals(void);
    /// <summary>The destructor for the BlockArray class which is responsible for freeing any memory owned by the class. The RenObj destructor is automatically invoked immediately prior to this method
    /// whenever a BlockArray is destroyed/deleted. Currently this particular destructor is empty.</summary>
    /// <remarks>Declared virtual as a stylistic choice.</remarks>
    ~BlockArray(void);
};
#pragma endregion
}
#endif
