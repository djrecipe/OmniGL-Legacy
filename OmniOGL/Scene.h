#ifndef SCENE_H
#define SCENE_H
#pragma region Includes
#include "Blocks.h"
#pragma endregion
namespace OmniOGL
{
#pragma region Camera Class
/// <summary>Provides a viewport for the player to view the scene. Stores the location and orientation of the camera and handles updating of the camera position and orientation.</summary>
class Camera
{
protected:
    //--- Protected Variables ---//
    // ~ Perspective
    /// <summary>A GLfloat which represents the near clipping plane. This value should be greater than one. The greater this value is, the further the distance a vertex must be from
    /// the camera in order for the vertex to be rendered. If this value is too small, depth-testing artifacts may occur.<seealso cref="clip_far"/></summary>
    /// <remarks>Initialized to 0.1 in the Camera constructor.</remarks>
    GLfloat clip_near;
    /// <summary>A GLfloat which represents the far clipping plane. This value should be greater than one. The smaller this value is, the closer the distance a vertex must be from
    /// the camera in order for the vertex to be rendered. If this value is too large, depth-testing artifacts may occur.<seealso cref="clip_near"/></summary>
    /// <remarks>Initialized to 5000.0 in the Camera constructor.</remarks>
    GLfloat clip_far;
    /// <summary>A GLfloat which represents the field of view, in degrees. The field of view determines the width of the area in front of the camera that will be rendered to the
    /// screen. <seealso cref="winsize"/></summary>
    /// <remarks>Initialized to 60 degrees in the Camera constructor. The member is used in conjunction with the screen size in order to determine the rectangular "window"
    /// which will be rendered to the screen.</remarks>
    GLfloat fov;
    // ~ View
    /// <summary>An array of three floats which represent the original look vector which was defined at the time the class was instantiated. This vector is used in conjunction with the
    /// current left/right or up/down angle to determine the new look vector (should the camera orientation change).</summary>
    /// <remarks>The initialization of this three element array in the Camera constructor is derived from the calculation <c>Camera::center - Camera::eye</c></remarks>
    GLfloat orig_look_vec[3];
    /// <summary>An array of three floats representing which direction is "up". This vector ultimately determines which direction in the rendered scene's 3D space matches the y (up/down)
    /// axis of the computer screen. This array will indirectly determine how the camera is rotated when the player looks around (i.e. as the mouse moves).</summary>
    /// <remarks>Initialized to [0.0,0.0,1.0] in the Camera constructor which implies that the upwards direction of the scene is the scene's positive z-axis. Be sure to keep the up
    /// vector normalized.</remarks>
    GLfloat up[3];
    //--- Protected Methods ---//
    /// <summary>This method rotates a 3D vector (represented by a three element GLfloat array) around another 3D vector by a specified number of degrees.
    /// <seealso cref="Camera::Update()"/></summary>
    /// <param name="arr_in"/>An array of three floats which represent the x,y,z components of the vector will be rotated. This method will store the resulting rotated vector
    /// back into this array.</param>
    /// <param name="axis"/>An array of three floats which represent the x,y,z components of the rotation vector. The rotation vector represents the axis around which
    /// the target vector will be rotated. For example, to rotate around the x-axis, pass [1.0,0.0,0.0] for this parameter.</param>
    /// <param name="degrees"/>The number of degrees the target vector will be rotated. This parameter may be positive or negative.</param>
    /// <returns>A 3 x 3 matrix (from the glm library) which holds various values used to update/rotate the target vector. The calculations which utilize this matrix
    /// have already been performed at the time this method returns, thus this matrix is returned for reference only.</returns>
    /// <remarks>When rotating about multiple axis, this method only seems to work when the rotations are split up into their different axis. For example, rotate about
    /// the x-axis first, then the y-axis, rather than trying to rotate about both axis at the same time.</remarks>
    glm::mat3 raw_rotate(GLfloat arr_in[3], const glm::vec3& axis,const GLfloat degrees);

public:
    //--- Public Variables ---//
    // ~ View
    /// <summary>A three element array of floats representing the position of the camera in 3D space. The camera position is updated whenever the player moves.
    /// <seealso cref="Camera::center"/><seealso cref="Camera::Update()"/></summary>
    /// <remarks>Initialized to [0.0,0.0,0.0] by default.</remarks>
    GLfloat eye[3];
    /// <summary>An array of three floats which represent the point in 3D space at which the camera eye is looking at. This point should be in relatively close proximity
    /// to the camera compared to objects in the scene and generally the distance between the camera eye and this member is fixed. The center point at which the camera
    /// is looking at is calculated using the camera eye, a known "up" direction, and the number of horizontal and vertical degrees the camera has rotated via the mouse.
    /// <seealso cref="Camera::eye"/><seealso cref="Camera::angle_side"/><seealso cref="Camera::angle_up"/><seealso cref="Camera::Update()"/></summary>
    /// <remarks>Initialization of this array is dependant upon <c>INITIAL_LOOK_DISTANCE</c> and <c>Camera::angle_side</c> and <c>Camera::angle_up</c>.</remarks>
    GLfloat center[3];
    /// <summary>A GLfloat which represents the number of degrees (either positive or negative) that the camera has rotated left/right (usually via horizontal mouse movement).
    /// This value is used to derive the Camera's new center value whenever an update is needed.
    /// <seealso cref="Camera::angle_up"/><seealso cref="Camera::raw_rotate()"/><seealso cref="Camera::center"/></summary>
    /// <remarks>Initialized to 0.0 in the Camera default constructor.</remarks>
    GLfloat angle_side;
    /// <summary>A GLfloat which represents the number of degrees (either positive or negative) that the camera has rotated up/down (usually via vertical mouse movement).
    /// This value is used to derive the Camera's new center value whenever and update is needed.</summary>
    /// <seealso cref="Camera::angle_side"/><seealso cref="Camera::raw_rotate()"/><seealso cref="Camera::center"/></summary>
    /// <remarks>Initialized to 0.0 in the Camera default constructor.</remarks>
    GLfloat angle_up;
    // ~ Movement
    /// <summary>An integer which represents the [sum] of the directions that the camera is moving in. Movement is generally incurred by user input or perhaps the environment.
    /// This integer is essentially a bit field (a set of flags) which can represent all the possible combinations of directions so that pressing multiple keys at one time
    /// can be handled properly. 0=no movement ; 1=forward ; 2=right ; 4=backward ; 8=left
    /// <seealso cref="Camera::Update()"/></summary>
    /// <remarks>Initialized to 0 in the Camera default constructor.</remarks>
    GLint curr_direction;
    /// <summary>A boolean value which restricts the camera/player to zero movement on the z-axis if set to <c>False</c>. If set to <c>True</c> then the player will
    /// move forward in the direction that the camera is facing, even if that direction has an upwards or downwards tilt.
    /// <seealso cref="Camera::Update()"/></summary>
    /// <remarks>Initialized to <c>False</c> in the Camera default constructor.</remarks>
    bool free_cam;
    // ~ Flags
    //--- Public Methods ---//
    // ~ Constructor(s)
    /// <summary>The default constructor for the Camera class which is responsible for initializing all Camera members including the eye and center.</summary>
    /// <remarks>For debugging purposes this method will output the Camera's starting eye and center to the debug console.</remarks>
    Camera(void);
    // ~ Rendering
    /// <summary>A comprehensive method which checks to see if camera movement or camera orientation change is pending. If a movement operation is pending then
    /// the camera eye position will move accordingly, and a Section change can potentially occur. If a camera orientation change is pending then the center
    /// position (the point the camera eye is looking at) will be updated based on <c>Camera::angle_side</c> and <c>Camera::angle_up</c>. This method updates
    /// the relevant shader program uniforms and attributes with new data as appropriate.
    /// <seealso cref="Camera::raw_rotate()"/><seealso cref="Camera::mouse_update_available"/></summary>
    /// <param name="info_in"/>A pointer to a structure which contains various pointers and data which are essential to accomplishing anything rendering-related.
    /// In this method it is primarily used to update matrices, send some data to the shader program, and indicate that a camera change has occured.</param>
    /// <returns>An unsigned integer representing the index of the Section that the camera has crossed in to. If the pending movement did not cause the camera
    /// to change Sections and the method encountered no errors, _OK (0) will be returned. If the pending movement DID cause a Section change but a new
    /// valid Section index could not be resolved, _BAD (-1) will be returned.</returns>
    /// <remarks></remarks>
    GLint Update(glm::mat4& proj_matrix_in, glm::mat4& view_matrix_in, const GLint & uniform_eye, const GLint & uniform_pv_matrix,const InputUpdate& update_data_in,irrklang::ISoundEngine* sound_engine_ptr=NULL);
    // ~ Destructor
    /// <summary>The destructor for the Camera class which is responsible for freeing any memory owned by the class. The RenObj destructor is automatically invoked immediately prior to this method
    /// whenever a Camera is destroyed/deleted. Currently this particular destructor is empty.</summary>
    /// <remarks>Declared virtual as a stylistic choice.</remarks>
    ~Camera(void);
};
#pragma endregion
#pragma region Section Class
/// <summary>A 3D subsection of the rendered environment. All subsections share the same fixed size and the subjections should be contiguous in 3D space. A Section stores all objects to-be-rendered
/// as a std::list of shared pointers to RenObj class instances. Generally speaking an object will only be rendered if the camera viewport is in a Section that is in close proximity to the object-in-
/// question.</summary>
/// <remarks>Each instance of the Section class holds pointers to its neighbors on all axis (if any exists), for a total of 6 neighbors.</remarks>
class Section
{
protected:
    // ~ Rendering
    /// <summary>An unsigned integer which represents the ID, or index, of the Section. All Sections should have a unique index so-as to avoid conflictions. This number is used to indentify the
    /// Section and subsequently interact with it.
    /// <seealso cref="Section::QueryID()"/></summary>
    /// <remarks>Set to -1 in the Section constructor. Since the member is unsigned, -1 translates to 4,294,967,295. The ID/index of a Section should match its location index in whichever array
    /// contains it.</remarks>
    GLuint id;
    bool voxels_loaded;
    bool adjacents_loaded;
    std::vector<GLuint> adjacent_sections;
public:
    GLint x_index, y_index, z_index;
    /// <summary>An linked list of shared pointers to the base class which all rendered objects inherit from. This list of pointers contains the objects that are within the Section's boundaries.</summary>
    /// <remarks>This member does not need to be initialized in the constructor.</remarks>
    std::list<ptrRenObj> objects;
    /// <summary>A GLfloat representing the positive and negative boundaries of the Section on the x-axis. If an object does not fall within these boundaries then it has left the Section and shoud be
    /// 'handed-over' to the appropriate Section neighbor (using the neighbor pointer members).<seealso cref="Seciton::Section()"/></summary>
    /// <remarks>Initialized to -1 in the Section default constructor, but there is another constructor available which initializes this member via parameter.</remarks>
    GLfloat x_limits[2];
    /// <summary>A GLfloat representing the positive and negative boundaries of the Section on the y-axis. If an object does not fall within these boundaries then it has left the Section and shoud be
    /// 'handed-over' to the appropriate Section neighbor (using the neighbor pointer members).<seealso cref="Seciton::Section()"/></summary>
    /// <remarks>Initialized to -1 in the Section default constructor, but there is another constructor available which initializes this member via parameter.</remarks>
    GLfloat y_limits[2];
    /// <summary>A GLfloat representing the positive and negative boundaries of the Section on the z-axis. If an object does not fall within these boundaries then it has left the Section and shoud be
    /// 'handed-over' to the appropriate Section neighbor (using the neighbor pointer members).<seealso cref="Seciton::Section()"/></summary>
    /// <remarks>Initialized to -1 in the Section default constructor, but there is another constructor available which initializes this member via parameter.</remarks>
    GLfloat z_limits[2];
    /// <summary>An array of two Section pointers which store the pointers to this Section's positive and negative neighbors on the x-axis. These pointers ensure that objects can be transfered
    /// to-and-from Sections easily whenever they cross a boundary on the x-axis.</summary>
    /// <remarks>Initialized to -1 in any of the Section constructors. Currently the way this array is populated/initialized is via direct access from an external caller.</remarks>
    GLuint x_neighbors[2];
    /// <summary>An array of two Section pointers which store the pointers to this Section's positive and negative neighbors on the y-axis. These pointers ensure that objects can be transfered
    /// to-and-from Sections easily whenever they cross a boundary on the y-axis.</summary>
    /// <remarks>Initialized to -1 in any of the Section constructors. Currently the way this array is populated/initialized is via direct access from an external caller.</remarks>
    GLuint y_neighbors[2];
    /// <summary>An array of two Section pointers which store the pointers to this Section's positive and negative neighbors on the z-axis. These pointers ensure that objects can be transfered
    /// to-and-from Sections easily whenever they cross a boundary on the z-axis.</summary>
    /// <remarks>Initialized to -1 in any of the Section constructors. Currently the way this array is populated/initialized is via direct access from an external caller.</remarks>
    GLuint z_neighbors[2];
    //--- Public Methods ---//
    // ~ Constructor(s)
    /// <summary>The default constructor for the Section class which is responsible for initializing all members owned by this class (and not by its base class).
    /// <seealso cref="Section::Section(GLfloat,GLfloat,GLfloat)"/></summary>
    /// <remarks>The RenObj default constructor is automatically invoked immediately prior to the invocation of this method. If this constructor is used then the x,y, and z limits & neighbors
    /// all need to be set via direct access.</remarks>
    Section(void);
    /// <summary>A constructor for the Section class which requires three two-element GLfloat arrays as parameters. These parameters represent the boundaries which make up the cuboid area of
    /// the Section.</summary>
    /// <param name="x_limits_in"/>An array of two floats which contain the negative and positive boundaries of the Section on the x-axis. These boundaries will be used when determining
    /// whether or not an object [still] resides in a Section.</param>
    /// <param name="y_limits_in"/>An array of two floats which contain the negative and positive boundaries of the Section on the y-axis. These boundaries will be used when determining
    /// whether or not an object [still] resides in a Section.</param>
    /// <param name="z_limits_in"/>An array of two floats which contain the negative and positive boundaries of the Section on the z-axis. These boundaries will be used when determining
    /// whether or not an object [still] resides in a Section.</param>
    /// <remarks>The RenObj default constructor is automatically invoked immediately prior to the invocation of this method. If this constructor is used then only the x,y, and z neighbors
    /// need to be set via direct access.</remarks>
    Section(GLint x_index_in, GLint y_index_in, GLint z_index_in, GLfloat x_limits_in[2], GLfloat y_limits_in[2], GLfloat z_limits_in[2]);
    // ~ Getter(s)/Setter(s)
    /// <summary>A method used to either set or retrieve the unique ID/index of the Section. This method will always retrieve the Section's ID, but it will only set the Section's ID
    /// if a parameter is passed and not equal to -1.<seealso cref="Section::id"/></summary>
    /// <param name="id_in"/>An optional unsigned integer which if passed and not equal to -1 (+4,294,967,295) will be set as the Section's ID.</param>
    /// <returns>An unsigned integer representing the unique ID/index of the Section. If the Section ID was changed by this method, the changed is reflected.</returns>
    /// <remarks>The member associated with this method is initialized to -1 in the Section's constructors.</remarks>
    GLuint QueryID(GLuint id_in=_BAD);
    /// <summary>A method which retrieves an unsigned integer representing the total number of rendered objects which reside in teh Section's boundaries.
    /// <seealso cref="Section::obj_count"/><seealso cref="Section::objects"/></summary>
    /// <returns>An unsigned integer representing the total number of objects in the Section and thus the total number of objects in <c>Section::objects</c>.</returns>
    /// <remarks>The member associated with this method is initialized to 0 in the Section's constructors.</remarks>
    GLuint GetObjCount(void);
    bool AreVoxelsLoaded(void);
    void SetVoxelsLoaded(void);
    // ~ Object Population
    /// <summary>Adds an object to the Section's collection (linked list) of objects. This method also increments <c>Section::obj_count</c>.
    /// <seealso cref="Section::objects"/><seealso cref="Section::obj_count"/></summary>
    /// <param name="renobj_toadd"/>A shared pointer to a class instance derived from the RenObj class which will be added to the Section. This object should be an object
    /// which resides within the Section's boundaries.</param>
    /// <returns>A boolean value indicating the success of the method. Currently always returns <c>True</c>.</returns>
    /// <remarks></remarks>
    bool AddObject(ptrRenObj renobj_toadd);
    /// <summary>Removes an object from the Section's collection (linked list) of objects. This method also decrements <c>Section::obj_count</c>.
    /// <seealso cref="Section::objects"/><seealso cref="Section::obj_count"/></summary>
    /// <param name="renobj_in"/>A shared pointer to a class instance derived from the RenObj class which will be removed from the Section. This object should be an object
    /// which already has been previously added to the Section.</param>
    /// <returns>A boolean value indicating the success of the method. Currently always returns <c>True</c>.</returns>
    /// <remarks>Currently I'm unsure if trying to delete an object which does not exist in the Section will cause an error. If so, there is a custom try/catch clause "protecting" it
    /// and the cause of the error should be obvious when it occurs.</remarks>
    GLuint RemoveObject(ptrRenObj renobj_in);
    bool AdjacentsLoaded(void);
    std::vector<GLuint> GetAdjacentSections(void);
    void SetAdjacentSections(std::vector<GLuint> sections_in);
};
#pragma endregion
#pragma region SectionCollection Class
class SectionCollection
{
private:
    //
    std::mutex mtxSections;
    GLuint width;
    GLuint populated_sections;
    GLuint section_count;
    BlockArray temp_barr;
    //
    /// <summary>A pointer to all Sections which partition the Scene's 3D space and allow chunks which are not in close proximity to the player to be hidden. In <c>Scene::Draw()</c>, certain elements
    /// in this array will be accessed via their index and their objects will rendered. When an object is added to the Scene, <c>RenObj::origin</c> and <c>RenObj::en_internal_clipping</c> are used to
    /// determine whether or not the object should belong to a Section, and if so, which Section the object is added to.
    /// <seealso cref="Scene::section_count"/><seealso cref="Scene::AddObject()"/></summary>
    /// <remarks>Initialized to be an array of X number of Sections within the Scene constructor. This pointer is dynamically allocated at runtime and must be deleted in the Scene's
    /// destructor. All sections should be contiguous.</remarks>
    Section * sections;
    //
    GLuint current_section;
    std::vector<GLuint> adjacent_sections;
    std::vector<GLuint> new_adjacent_sections;
    std::vector<GLuint> temp_adjacent_sections;
    bool looking_x[2], looking_y[2];
    bool UpdateAdjacentSectionsArray(GLfloat angle_side_in=0.0);
    bool AddAdjacentSection(Section * adj_sec_ptr, std::vector<GLuint>& new_sections);
    void AddAdjacentSections(GLint i_in);
    void MinorUpdateAdjacentSections(GLfloat angle_side_in, bool update_pending=false);
    void InstallVoxels(std::vector<GLint> x_load_in, std::vector<GLint> y_load_in, std::vector<GLint> z_load_in, BlockArray barr_in, GLuint width_in);
public:
    std::vector<GLint> x_load;
    std::vector<GLint> y_load;
    std::vector<GLint> z_load;
    //
    SectionCollection(GLint width_in);
    //
    bool AddObject(ptrRenObj renobj_toadd);
    bool RemoveNearObject(float x_in, float y_in, float z_in);
    bool RemoveObject(ptrRenObj obj_in);
    //
    GLuint GetCurrentSection(void);
    void GetOccupiedSectionIndices(GLint indices_out[3]);
    bool UpdateCurrentSection(float x_in, float y_in, float z_in, GLfloat angle_side_in);
    //
    LineStrip GetGridlines(int which);
    //
    void Draw(GLint & id_in, OGLUniforms & uniforms_in, OGLMatrices & matrices_in, const bool & textures_en_in, const bool & bumpmap_en_in);
    ~SectionCollection(void);
};
#pragma endregion
#pragma region Scene Class
/// <summary>Contains meta-information for the 3D scene and handles sorting/addition/removal of objects to/from the scene and configuration for elements such as lighting and sound.
/// This class is considered the "owner" of the translation matrix and the combined scale/rotation/translation matrix and thus updating these matrices is accomplished via instance methods
/// of this class.</summary>
/// <remarks>In the future this class may be split up into several smaller, more concise and specialized classes.</remarks>
class Scene
{
    friend GLint RenObj::Draw(GLint,OGLUniforms&, OGLMatrices&,const bool&,const bool&);
private:
    OGLAttributes attributes;
    OGLUniforms uniforms;
    OGLMatrices matrices; 
    InputEvents in_events;
    bool exitbool;
    ptrRenObj placement_obj;
    static bool globally_initialized;
    static void GlobalInitialization(void);
    SectionCollection sections;
protected:
    //--- Protected Members ---//
    // ~ Rendering Objects
    /// <summary>A signed long integer which represents the total number of renderable/rendered objects managed by the Scene. This count [should] include any objects residing in any partitions (Sections)
    /// created by the Scene and also any objects part of a collection which is rendered as a single object (Group class).
    /// <seealso cref="Scene::GetObjCount()"/><seealso cref="Scene::IncrementTotal()"/><seealso cref="Scene::AddObject()"/></summary>
    /// <remarks>Initialized to 0 in the Scene constructor. Incremented any time an object is added to the Scene or any time the IncrementTotal() method is called (so object collections which have already
    /// been added to the Scene may modify the Scene's object count whenever their own object count changes).</remarks>
    long int total_object_count;
    /// <summary>A list of all objects which will be rendered after 3D objects and using an orthogonal project matrix with depth testing disabled. More specifically, this member is a linked list of
    /// shared pointers to the base class (RenObj, the class from which all rendered objects are derived from). Storing all rendered objects as a pointer to their base class makes the process of iterating
    /// through them trivial and ambiguous to the number of unique classes. Storing the list of pointers as a linked list allows for speedy removal and insertion/addition.
    /// <seealso cref="RenObj::in_foreground"/><seealso cref="Scene::Draw()"/><seealso cref="Scene::AddObject()"/></summary>
    /// <remarks>This member does not need to be initialized in the constructor.</remarks>
    std::list<ptrRenObj> foregnd_objs;
    /// <summary>A std::vector of sounds whom will behave different based off of the proximity to the user. In other words this is a list of sounds and their associated properties, including their
    /// position in 3D space. Note that 'irrklang' is an external library developed by someone other than myself.
    /// <seealso cref="Scene::AddSound()"/></summary>
    /// <remarks>This member does not need to be initialized in the constructor. Any sounds added via AddSound() need to be disposed of properly in the Scene destructor.</remarks>
    std::vector<irrklang::ISound*> sound_objs;
    // ~ Rendering Properties
    /// <summary>An integer representing the unique identification number of the most recently rendered object. This value is passed to the Draw() method of each rendered object in the rendering
    /// loop to help the object determine if it needs to bind data to shader program attributes and/or alter the OpenGL state machine.
    /// <seealso cref="Scene::Draw()"/><seealso cref="RenObj::render_id"/></summary>
    /// <remarks>Initialized to _BAD (-1) in the Scene constructor.</remarks>
    GLint current_id;
    /// <summary>A boolean value representing whether or not lambertian lighting elements are enabled in the Scene. This boolean value directly affects a fragment shader uniform which enables/disables
    /// lambertian (diffuse) lighting. Diffuse lighting is the most basic of all dynamic lighting effects.
    /// <seealso cref="Scene::ToggleLambert()"/><seealso cref="Scene::info.uni_islambert"/><seealso cref="Scene::AddLight()"/></summary>
    /// <remarks>Initialized to <c>False</c> in the Scene constructor but is enabled <c>True</c> in <c>Scene::AddLight()</c>. This boolean is may used for debugging and may be toggled via a
    /// keyboard key, for example.</remarks>
    bool islambert;
    /// <summary>A boolean value representing whether or not phong shading elements are enabled in the Scene. This boolean value directly affects a fragment shader uniform which enables/disables
    /// phong (specular) shading/lightinglighting. Specular lighting is a basic "shiny" lighting effect.
    /// <seealso cref="Scene::TogglePhong()"/><seealso cref="Scene::info.uni_isphong"/><seealso cref="Scene::AddLight()"/></summary>
    /// <remarks>Initialized to <c>False</c> in the Scene constructor but is enabled <c>True</c> in <c>Scene::AddLight()</c>. This boolean may be used for debugging and may be toggled via a
    /// keyboard key, for example.</remarks>
    bool isphong;
    /// <summary>A boolean value representing whether or not normals will be affeted by bump-mapping. Bump-mapping takes a preprocessed file and uses the color data to alter the normals which
    /// are used to determine the lighting characterstics of an object. The result is a collection of brightened and darkened areas on the surface of the object which give the illusion of
    /// fine 3D bumps/detail on the surface of the object. This is a highly effective and relatively cheap method of greatly increasing the apparent detail of your objects.
    /// <seealso cref="Scene::ToggleBumpMapping()"/><seealso cref="Scene::info.uni_isbumpmap"/><seealso cref="Scene::AddLight()"/></summary>
    /// <remarks>Initialized to <c>False</c> in the Scene constructor but is enabled <c>True</c> in <c>Scene::AddLight()</c>. This boolean may be used for debugging and may be toggled via a
    /// keyboard key, for example.</remarks>
    bool isbumpmap;
    /// <summary>A boolean value representing whether or not textures are enabled. This boolean value will not affect bump mapping - only the original texture.
    /// <seealso cref="Scene::ToggleTextures()"/><seealso cref="Scene::GlobalTextureEn()"/><seealso cref="Scene::info.uni_istex"/></summary>
    /// <remarks>Initialized to <c>False</c> in the Scene constructor but is enabled <c>True</c> in <c>Scene::AddLight()</c>. This boolean may be used for debugging and may be toggled via a
    /// keyboard key, for example.</remarks>
    bool global_texture_enable;
    bool wireframe_enabled;
    /// <summary>A boolean value which is true if - and only if - the camera (or look) position has changed in that frame. This value is NOT cleared (set to false) each frame - it is cleared
    /// by calling <c>Scene::CameraQuery()</c>. <seealso cref="Scene::CameraQuery()"/></summary>
    bool camera_changed;
    // ~ Sound
    /// <summary>A pointer to an <c>irrklang</c> object which is used for 3D sound effects such as panning and volume derived from proximity.</summary>
    irrklang::ISoundEngine* sound_engine;
    //--- Protected Methods ---//
    void HandleInput(void);
    //
public:
    //--- Public Members ---//
    // ~ Camera
    /// <summary>A Camera instance which acts as the player's means to view the Scene. This object determines the projection matrix and thus it determines where the Camera eye is located and
    /// how the Camera is oriented. This Camera class instance also handles Camera movement and assists in determining the Section that the player occupies at any given moment. The Camera is
    /// updating upon instantiation of the Scene and also once per rendered frame. Camera orientation is altered via mouse movement whereas Camera position is altered via keyboard keys.
    /// <seealso cref="Camera::Update()"/><seealso cref="Scene::Draw()"/><seealso cref="evnt_Keybrd()"/></summary>
    /// <remarks>Initialized in the Scene constructor by passing <c>Scene::info</c> to <c>Camera::Update()</c>. In the future perhaps multiple Camera classes could co-exist in the same Scene
    /// allowing the player to cycle through multiple viewports with ease.</remarks>
    Camera sc_camera;
    // ~ Objects
    /// <summary>A linked list of shared pointers to the base class from which all rendered objects are derived from. This specific list contains objects whom are not subject to "Sectioning"
    /// (they do not belong to a Section) and thus will be visible [regardless of their proximity to the player] as long as they lie between the near and far clipping planes. Any objects which
    /// will be moving very quickly through Sections and whom do not need to be subject to collision rules are good candidates for this list. When an object is added to the Scene, <c>RenObj::origin</c>
    /// and <c>RenObj::en_internal_clipping</c> determine whether or not the object will be placed in a Section or in this list.
    /// <seealso cref="RenObj::origin"/><seealso cref="RenObj::en_internal_clipping"/><seealso cref="Scene::AddObject()"/><seealso cref="Scene::Draw()"/></summary>
    /// <remarks>This member does not need to initialized in the Constructor. Rather, this member can potentially have an element added to it each time <c>Scene::AddObject()</c> is called. Note that
    /// "persistant" objects are drawn after "regular" objects but before "foreground objects".</remarks>
    std::list<ptrRenObj> persistant_objs;
    //--- Public Methods ---//
    // ~ Constructor(s)
    /// <summary>The constructor for the Scene class. This is a fairly busy constructor since it's responsible for initializing several core OpenGL elements, populating <c>Scene::info</c>, and
    /// overall setting-up a large number of program elements. More specifically, this constructor initializes "pointers" to attributes and uniforms in the shader program, partitions 3D space
    /// into Sections, initializes the scale, rotation, translation, and projection matrices, configuring the OpenGL state machine, etc.
    /// <seealso cref="Scene::Draw()"/><seealso cref="Scene::sections"/><seealso cref="matrix_info"/></summary>
    /// <param name="shader_pgm_in"/>An unsigned integer which acts as an identifier for the active shader program. More specifically, it is the unsigned integer that is returned from
    /// <c>glCreateProgram()</c>. This shader program will be the shader program which the Scene will interact with via uniforms, attributes, draw calls, etc.</param>
    /// <param name="vbo_in"/>An array of unsigned integers, each representing a vertex buffer object which is essentially an input [a way to send data] to the shader program. Each unsigned integer is
    /// generated via <c>glGenBuffers()</c>. Vertex buffer objects should be reserved for data which is going to be updated frequently.</param>
    /// <param name="vao_in"/>An array of unsigned integers, each representing a vertex array object which allows the programmer (you =p) to quickly switch between "vertex array configurations"
    /// via calls to <c>glBindVertexArray()</c>. Binding a vao will affect subsequent calls to <c>glVertexAttribPointer()</c> and <c>glEnableVertexAttribArray()</c>.</param>
    /// <remarks>Currently there is no alternative to this constructor. Expect the structure and function of this method to change drastically as the engine becomes more developed.</remarks>
    Scene(void);
    // ~ Getter(s)/Setter(s)
    /// <summary>A getter which returns an unsigned integer representing the index of the Section that the player/camera eye occupies at the time. Useful for debugging, object interactions, proximity-
    /// based events, determining which Sections to render, etc. <seealso cref="Scene::info.current_section"/><seealso cref="Camera::Update()"/></summary>
    /// <returns>An unsigned integer representing the index of the currently occupied section. If this method returns 4,294,967,295 then the Section occupied by the player/camera eye is probably not
    /// valid.</returns>
    /// <remarks>The member associated with this member is initialized to 440 in the Scene constructor.</remarks>
    GLuint GetOccupiedSectionIndex(void);
    /// <summary>A getter which returns a signed long integer representing the total number of objects in the Scene, which is a sum of all of the objects owned/managed by the Scene as well as the
    /// objects owned/managed by the Sections which partition the Scene's 3D space. <seealso cref="Scene::total_object_count"/><seealso cref="Scene::AddObject()"/>
    /// <seealso cref="Scene::IncrementTotal()"/></summary>
    /// <returns>A signed long integer representing the total count of [any and all] objects in the Scene.</returns>
    /// <remarks>The member associated with this method is initialized to 0 in the Scene constructor. Note that each instance of an instanced rendering call is counted as an object.</remarks>
    long int GetObjCount(void);
    /// <summary>Increments the total object count in the Scene and returns the post-incremented value. Useful for incrementing the object count without adding an object to the Scene (i.e. the Group class).
    /// <seealso cref="Scene::total_object_count"/><seealso cref="Scene::GetObjCount()"/></summary>
    /// <returns>A signed long integer representing the number of objects in the Scene (and the Sections managed by the Scene), post-increment.</returns>
    /// <remarks>The member associated with this method is initialized to 0 in the Scene constructor. Note that each instance of an instanced rendering call is counted as an object.</remarks>
    long int IncrementTotal(void);
    /// <summary>Toggles phong shading (a.k.a. specular highlights) on/off. Primarily used for debugging purposes.
    /// <seealso cref="Scene::isphong"/><seealso cref="Scene::info.uni_isphong"/><seealso cref="Scene::AddLight()"/></summary>
    /// <returns>A boolean value representing whether or not phong shading is enabled, post-toggle.</returns>
    /// <remarks>The member associated with this method is initialized to <c>False</c> in the Scene constructor but is enabled <c>True</c> in <c>Scene::AddLight()</c>. Some logic is
    /// implemented so-as to guarantee that this method will never result in a redundant state change - in other words, the shader program is only notified of the toggle if the toggle results
    /// in a state change, which it always should.</remarks>
    bool TogglePhong(void);
    /// <summary>Toggles lambertian reflectance (a.k.a. diffuse highlights) on/off. Primarily used for debugging purposes.
    /// <seealso cref="Scene::islambert"/><seealso cref="Scene::info.uni_islambert"/><seealso cref="Scene::AddLight()"/></summary>
    /// <returns>A boolean value representing whether or not lambertian reflectance is enabled, post-toggle.</returns>
    /// <remarks>The member associated with this method is initialized to <c>False</c> in the Scene constructor but is enabled <c>True</c> in <c>Scene::AddLight()</c>. Some logic is
    /// implemented so-as to guarantee that this method will never result in a redundant state change - in other words, the shader program is only notified of the toggle if the toggle results
    /// in a state change, which it always should.</remarks>
    bool ToggleLambert(void);
    /// <summary>Toggles bump mapping via [specially formatted] textures on/off. Primarily used for debugging purposes.
    /// <seealso cref="Scene::isbumpmap"/><seealso cref="Scene::info.uni_isbumpmap"/><seealso cref="Scene::AddLight()"/></summary>
    /// <returns>A boolean value representing whether or not bump mapping textures are enabled, post-toggle.</returns>
    /// <remarks>The member associated with this method is initialized to <c>False</c> in the Scene constructor but is enabled <c>True</c> in <c>Scene::AddLight()</c>. A bump map texture must be
    /// assigned to the object-to-be-rendered (via <c>RenObj::SetBumpMap()</c>) in order for bump mapping to be implemented on that object.</remarks>
    bool ToggleBumpMapping(void);
    /// <summary>Toggles texture mapping on/off. Does not affect textures used for bump mapping. Primarily used for debugging purposes.
    /// <seealso cref="Scene::global_texture_enable"/><seealso cref="Scene::GlobalTextureEn()"/><seealso cref="Scene::info.uni_istex"/><seealso cref="RenObj::SetTexture()"/></summary>
    /// <returns>A boolean value representing whether or not [regular] textures are enabled, post-toggle.</returns>
    /// <remarks>The member associated with this method is initialized to <c>True</c> in the Scene constructor. A texture must be assigned to the object-to-be-rendered (via <c>RenObj::SetTexture()</c>)
    /// in order for textures to show-up on that object.</remarks>
    bool ToggleTextures(void);
    bool ToggleWireframe(void);
    /// <summary>A getter which retrieves a boolean value representing whether or not [regular] textures are enabled. This value does not reflect the state of bump mapping.
    /// <seealso cref="Scene::global_texture_enable"/><seealso cref="Scene::ToggleTextures()"/><seealso cref="Scene::info.uni_istex"/><seealso cref="RenObj::SetTexture()"/></summary>
    /// <returns>A boolean value which is <c>True</c> if textures are enabled and <c>False</c> if textures are disabled.</returns>
    /// <remarks>The member associated with this method is initialized to <c>True</c> in the Scene constructor. A texture must be assigned to the object-to-be-rendered (via <c>RenObj::SetTexture()</c>)
    /// in order for textures to show-up on that object.</remarks>
    bool GlobalTextureEn(void);
    /// <summary>A getter which retrieves a boolean value representing whether or not the Camera's position or orientation has been changed since the last frame (generally via mouse or keyboard input).
    /// <seealso cref="Camera::Update()"/><seealso cref="Camera::mouse_update_available"/></summary>
    /// <returns>A boolean value representing whether or not there has been a Camera update since this method has last been called.</returns>
    /// <remarks>The member associated with this method is initialized to <c>False</c> in the Scene constructor. Note that this flag is an enitrely seperate entity than the flags which are used to actually
    /// trigger Camera updates.</remarks>
    bool CameraQuery(void);
    bool ExitQuery(void);
    /// <summary>A getter which returns a pointer to the Section object currently being occupied by the player/camera eye. This pointer is derived from the index of the currently occupied Section which
    /// is stored in the <c>matrix_info</c> struct. Useful for exposing the currently occupied Section to external methods/functions (i.e. iterating through objects in a section in order to select, remove,
    /// etc.). <seealso cref="Scene::info.current_section"/> <seealso cref="Scene::sections"/></summary>
    /// <returns>A Section pointer to the Section which has the index that matches the index of the currently occupied Section. In other words, this method returns a pointer to the currently occupied
    /// Section provided that <c>Scene::info.current_section</c> is not _BAD (-1) - in which case this method will return _BAD (-1).</returns>
    /// <remarks>The member associated with this method is initialized to _BAD (-1) in the Scene constructor.</remarks>
    Section & GetOccupiedSection(void);
    void GetOccupiedSectionIndices(GLint indices_out[3]);
    /// <summary>A getter which retrieves a boolean value representing whether or not bump mapping is enabled. Primarily used for debugging purposes.
    /// <seealso cref="Scene::isbumpmap"/><seealso cref="Scene::info.uni_isbumpmap"/><seealso cref="Scene::AddLight()"/></summary>
    /// <returns>A boolean value representing whether or not bump mapping textures are enabled.</returns>
    /// <remarks>The member associated with this method is initialized to <c>False</c> in the Scene constructor but is enabled <c>True</c> in <c>Scene::AddLight()</c>. A bump map texture must be
    /// assigned to the object-to-be-rendered (via <c>RenObj::SetBumpMap()</c>) in order for bump mapping to be implemented on that object.</remarks>
    bool BumpMapEn(void);
    // ~ Rendering
    /// <summary>Configures lighting parameters for the Scene including the sole light's position and color, the global ambient color of the Scene, and the object (BlenderObj) which will be used to denote
    /// the location of the light. Data will be sent to the shader program when this method is invoked.
    /// <seealso cref="Scene::islambert"/><seealso cref="Scene::isphong"/><seealso cref="Scene::isbumpmap"/><seealso cref="Scene::info"/></summary>
    /// <param name="light_position"/>An array of floating point values which is expected to contain three elements which represent the x,y, and z location of the light in 3D space.</param>
    /// <param name="light_color"/>An array of floating point values which is expected to contain four elements which represent the r,g,b, and a components of the [dynamic] light's color.</param>
    /// <param name="ambient_color"/>An array of floating point values which is expected to contain four elements which represent the r,g,b, and a components of the ambient color which will be
    /// applied globally to every surface/object in the Scene.</param>
    /// <param name="renobj_in"/>A pointer to an instance of the base class from which all rendered objects are derived. This object will be placed at the same location as the light to denote
    /// the light's position.</param>
    /// <returns>An integer indicating the success of the method. _OK (0) if there were no errors, and _BAD (-1) if there were errors. Currently always return _OK (0).</returns>
    /// <remarks>The members associated with this method are initialized so that lighting effects are disabled prior to the invocation of this method. Diffuse lighting, specular highlighting, and bump
    /// mapping will all be globally enabled via this method. Currently only one light is supported by the engine. This will be changing very soon.</remarks>
    GLint AddLight(GLfloat * light_position, GLfloat * light_color, GLfloat * ambient_color, BlenderObj* renobj_in=nullptr);
    void UpdateLightPosition(GLfloat * light_position);
    /// <summary>A template method which adds an renderable object of type T to the scene. It is assumed that T is a derivative of the RenObj class. This method includes logic which uses the <c>RenObj::origin</c>
    /// property of the object to determine which Section the object belongs in (and will be managed by), if any. Note that when objects are added to the Scene they are essentially copied, so changes made to
    /// the instance which was passed to the Scene after it was passed will have no ramifications for the instance owned by the Scene.
    /// <seealso cref="Scene::Draw()"/><seealso cref="Scene::total_object_count"/><seealso cref="Scene::RemoveObject()"/></summary>
    /// <param name="renobj_in"/>A pointer (of any type, but MUST be a RenObj derivative) to the object which will be added to the Scene. This object's origin member will be used to determine if the
    /// object will be placed in and owned by a Section or not.</param>
    /// <returns>A pointer to the same type as the parameter passed to this method. This pointer points to the object that is owned by the Scene, NOT the original object passed to this method. This
    /// pointer is used by any methods outside the scope of the Scene which may need access to an object which has already been added to the Scene.</returns>
    /// <remarks>Since this is a template function, its definition must lie within this header file.</remarks>
    template <typename T>
    T* AddObject(T* renobj_in);
    template <typename T>
    void SetPlacementObject(T* renobj_in);
    /// <summary>Removes a RenObj (or one of its derivates) from the Scene, only if the object [pointer] already existed in the Scene. This method checks all foreground objects, objects owned by Sections
    /// within the Scene, and "persistant" objects and decrements <c>Scene::total_object_count</c> if the object is found and removed.
    /// <seealso cref="Scene::total_object_count"/><seealso cref="Scene::AddObject()"/></summary>
    /// <param name="renobj_in"/>A shared pointer to a derivative to the RenObj class which is desired to be removed from the Scene. This should be an object which already has been added to the Scene via
    /// <c>Scene::AddObject()</c>.</param>
    /// <returns>If an object was removed from the Scene then this method returns _OK (0). Returns _BAD (-1) otherwise.</returns>
    /// <remarks>Unlike <c>Scene::AddObject()</c> this method is not a template method.</remarks>

    GLint RemoveObject(ptrRenObj renobj_in);
    /// <summary>The core of the rendering loop! This method iterates through all possible containers of objects-to-be-rendered (derivatives of the RenObj class) and invokes their Draw() method.
    /// <c>RenObj::render_id</c> of each object rendered is passed to the next object so that traffic to the shader program and redundant state changes are minimized. This method also accomplishes
    /// various matrix and OpenGL state machine "prep work" as needed. Lastly, the Camera and associated data is updated.
    /// <seealso cref="Scene::current_id"/><seealso cref="RenObj::Draw()"/><seealso cref="Scene::info"/><seealso cref="Scene::AddObject()"/></summary>
    /// <returns>An integer intended to indicate the success of the method. Currently always returns _OK (0).</returns>
    /// <remarks>This method resets all matrices and "clears the screen" before drawing anything.</remarks>
	GLint Draw(void);
    // ~ Sound
    /// <summary>Adds a "sound" to the Scene which can be subject to 3D effects (the most common being panning and volume control) using the external "irrklang" library. This method takes
    /// a file name of a sound as a parameter then performs all of the necessary setup to initialize the sound as an irrklang object and add it to the Scene.
    /// <seealso cref="Scene::sound_objs"/><seealso cref="Scene::info.sound_engine"/></summary>
    /// <param name="file_in"/>A string representing the file path of the sound file which will be added to the Scene. Accepts most common file formats. Currently very little eerror handling is implemented so
    /// be careful.</param>
    /// <param name="location_in"/>A vector representing the x,y,z coordinates in 3D space at which the sound will be located. This value is used to give the sound natural 3D properties (i.e. sound gets quieter
    /// as it is farther away, sound comes out of left speaker when it is to the camera's left, etc.).</param>
    /// <param name="min_in"/>A GLfloat representing the minimum distance the camera has to be from the sound in order for the sound to be heard. In other words, it is the maximum distance the camera can be
    /// from the sound and still have the sound be heard.</param>
    /// <returns>A pointer to the irrklang object created by the external library.</returns>
    /// <remarks>This method is barely developed and my not even work properly. =]</remarks>
    irrklang::ISound* AddSound(std::string file_in,glm::vec3 location_in,GLfloat min_in);
    /// <summary>Iterates through all objects in the Section which the player/user is currently in, as well as all of the persistant objects in the Scene, and attempts to find which object is within closest
    /// proximity to the camera CENTER (the point the camera is looking at). Only objects which are within a close-enough proximity will qualify. The nearest qualifying object is removed from the Scene (or from
    /// the Section it occupied, if it occupied one). <seealso cref="evnt_Keybrd()"/></summary>
    /// <returns>An integer representing the success of the function. Returns _OK (0) if a qualifying object was found and subsequently removed from the Scene succesfully - returns _BAD (-1) if
    /// <c>active_scene_ptr</c> is not set (via a call to <c>CaptureInput()</c>) or if an object was not found within a close-enough distance from the camera center.</returns>
    /// <remarks><c>CaptureInput()</c> must be called prior to the invocation of this method for this method to operate.</remarks>
    GLint RemoveNearObject(void);
    /// <summary>In the current implementation this function must determine the derived type of the shared pointer passed to this function. If the derived class is determined then the object is placed at the
    /// camera center. The object-to-be-placed must first have been defined via <c>SetPlacementObject()</c>.
    /// <seealso cref="SetPlacementObject()"/><seealso cref="evnt_MouseBtn"/><seealso cref="Scene::AddObject()"/><seealso cref="RemoveNearObject()"/><seealso cref="CaptureInput()"/></summary>
    /// <param name="scene_in">A pointer to the Scene to which the object will be added. This pointer is originally obtained via <c>CaptureInput()</c>.</param>
    /// <param name="obj_in">The object which will be added to the Scene (or technically one of its Sections, if appropriate). This parameter comes in the form of a shared pointer to the base class of all
    /// rendered objects and thus this function must determine the object type using <c>std::dynamic_pointer_cast<>()</c> then add it to the Scene via <c>Scene::AddObject()</c> once the type is determined.</param>
    /// <remarks>Generally this method is invoked as a result of user input. This method will probably change a lot in the near future.</remarks>
    void AddDerivedObj(ptrRenObj obj_in);
	// Destructor
    /// <summary>The destructor for the Scene class which is responsible for freeing any memory owned by the class. This method must delete all rendered objects.
    /// </summary>
    /// <remarks>Declared virtual as a stylistic choice.</remarks>
	~Scene(void);
};
//--- Templated Methods ---//
template <typename T>
T* Scene::AddObject(T* renobj_in)
{
    this->total_object_count+=1;
    ptrRenObj renobj_out=std::make_shared<T>(*renobj_in);
    if(!renobj_out)
        return (T*)&*renobj_out;
    if(renobj_out->GetForeground())
       this->foregnd_objs.push_back(renobj_out);
    else if(!renobj_out->GetInternalClipping() || this->sections.AddObject(renobj_out))
        this->persistant_objs.push_back(renobj_out);
    return dynamic_cast<T*>(renobj_out.get());
}
/// <summary>A templated function which takes any type of pointer as a parameter then returns a shared pointer to the base class of the renderable object (if applicable). If it is not found to be a derivative
/// of the RenObj class then the value _BAD (-1) is returned instead. Ultimately this function determines what [object] will be placed when <c>AddDerivedObj()</c> is invoked, generally because of user input.
/// <seealso cref="AddDerivedObj()"/><seealso cref="Scene::AddObject()"/></summary>
/// <param name="renobj_in">The pointer of any type (T) which will be attempted to be made into a shared pointer to a RenObj (or one of its derivatives).</param>
/// <returns>A shared pointer to a RenObj (or one of its derivatives) which represents the object which will be placed any time <c>AddDerivedObj()</c> is invoked (via user input).</returns>
/// <remarks>This function may be vastly restructured in the future as it may introduce unneccesary complications.</remarks>
template <typename T>
void Scene::SetPlacementObject(T* renobj_in)
{
    try
    {
        this->placement_obj=std::make_shared<T>(*renobj_in);
    }
    catch(GLint e)
    {
    }
}
#pragma endregion
}
#endif