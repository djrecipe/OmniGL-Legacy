#ifndef BLOCKS_H
#define BLOCKS_H
#pragma region Preprocessor Defines
/// <summary>A somewhat arbitrary number which determines how often arrays need to be resized when generated voxel data. Larger numbers mean faster
/// voxel generation but more memory waste. Too large of a number and you will begin to encounter exceptions. <seealso cref="VoxelField::FillWithRandom()"/></summary>
/// <remarks>I may need to look deeper into how this is being used. I need to make sure that the final array of vertices and other data that is being passed to
/// to the shader program at render time do not contain extra garbage elements at their ends.</remarks>
#define BARR_ARR_SIZE 10000
/// <summary>Half of the minimum distance that any two NetNodes can be from eachother. This prevents two vertices from overlapping and various artifacts from
/// occuring should too many relaxation passes be made.</summary>
#define NETNODE_PROXIMITY_LIMIT 100.0
#pragma region Includes
#include "RenObj.h"
#pragma endregion
namespace OmniOGL
{
#pragma endregion
#pragma region NetNode Class
/// <summary>A single node in a collection of nodes (SurfaceNet). There is one node for each vertex in the mesh and "duplicate" vertices will be considered the same node.
/// This node holds data such as its coordinates, pointers to neighbors (any other NetNodes with whom this NetNode shared a triangle with), the element index of said neighbor, and
/// the node's constraint value. Currently spherical constraints are implemented which means the constraint value will be equal to half the distance to [this] NetNode's closest neighboring
/// NetNode. <seealso cref="SurfaceNet"/></summary>
/// <remarks>As of 05/22/13 this class is still in very early development and may be subject to heavy modification in the near future.</remarks>
class NetNode
{
friend class SurfaceNet;
private:
    bool pending_limit;
    bool limited;
    bool original_limit;
    bool simplified;
protected:
    /// <summary>An array of three floats which represent the original x,y, and z coordinate that the NetNode represents. This member will always hold the original vertex - relaxation via surface net techniques will
    /// not affect this array. <seealso cref="NetNode::new_vertex"/></summary>
    /// <remarks>This member is set directly via the three parameters passed to the constructor.</remarks>
    GLfloat vertex[3];
    /// <summary>An array of three floats which represent the current x,y, and z coordinate that the NetNode represents. This member holds the vertex which will be placed into the mesh should no further relaxation passes
    /// be performed.<seealso cref="NetNode::vertex"/></summary>
    /// <remarks>This member is set directly via the three parameters passed to the constructor and will be the same as <c>NetNode::vertex</c> until the first relaxation pass is performed.</remarks>
    GLfloat new_vertex[3];
    /// <summary>An std::vector of pointers to NetNodes which share a triangle with this NetNode which is populated at the time the vertices are generated. These pointers allow this node to easily access
    /// the vertices of neighboring nodes so-as to perform the relaxation process.
    /// <seealso cref="NetNode::neighbor_indices"/><seealso cref="NetNode::AddNeighbor()"/></summary>
    /// <remarks>This member does not need to be initialized in the constructor.</remarks>
    std::vector<GLuint> neighbors;
    /// <summary>A double representing the maximum distance this NetNode is permitted to travel from its original vertex. This value is only valid for a spherical constraint implementation and is defined
    /// as half the distance to this NetNode's closest neighbor. A check must be made each time a neighbor is added to determine if this member needs to be updated.
    /// <seealso cref="NetNode::AddNeighbor()"/><seealso cref="NetNode::neighbors"/></summary>
    /// <remarks>Initialized to _BAD (-1) in the NetNode constructor.</remarks>
    GLdouble constraint;
	GLfloat normal_dev;
    glm::vec3 blended_normal;
    std::vector<glm::vec3> normals;
public:
    /// <summary>The sole constructor for this class which initializes all members which need to be initialized. This constructor takes three floats as parameters which represent the x,y, and z
    /// components of the vertex at which this NetNode will lie.<seealso cref="NetNode::vertex"/> <seealso cref="SurfaceNet"/></summary>
    /// <param name="x_in">A float representing the x component of the vertex at which this NetNode will lie, prior to relaxation passes.</param>
    /// <param name="y_in">A float representing the y component of the vertex at which this NetNode will lie, prior to relaxation passes.</param>
    /// <param name="z_in">A float representing the z component of the vertex at which this NetNode will lie, prior to relaxation passes.</param>
    NetNode(GLfloat x_in, GLfloat y_in, GLfloat z_in,bool limited_in=false);
    /// <summary>This method should be invoked two times for each triangle that the vertex represented by this NetNode is present in. This method allows this NetNode to "remember" whom its neighbors are
    /// by storing a pointer to the neighbor and that neighbor's element index into an array of vertices. Duplicate neighbors are checked for and are not allowed. A NetNode's neighbors are used in order
    /// to determine which direction the NetNode will move during relaxation passes. <seealso cref="NetNode::neighbors"/><seealso cref="NetNode::neighbor_indices"/>
    /// <seealso cref="NetNode::Relax()"/></summary>
    /// <param name="neighbor_in">A pointer to the [already created] NetNode with whom this NetNode shares a triangle with. This pointer will be used to retrieve data from the neighbor when it comes
    /// time to relax.</param>
    /// <param name="index_in">An index into a vertex array that represents the vertex that the new neighbor (first parameter passed to this method) occupies. This index is essentially used as
    /// an identification number for each neighbor.</param>
    /// <returns><c>True</c> if the prospective neighbor was added or <c>False</c> if the neighbor was not added (due to it being a duplicate of a previously added neighbor).</returns>
	bool AddNeighbor(GLuint index_in, glm::vec3 vertex_in);
	void SetNormalDeviation(GLfloat deviation_in);
	GLfloat GetNormalDeviation(void);
    /// <summary>A method which returns a scalar distance between the original vertex represented by this NetNode and the vertex represented by the three floats passed to this method. This method
    /// is used by neighboring NetNodes in determining relaxation distances and spherical constraints.
    /// <seealso cref="NetNode::constraint"/></summary>
    /// <param name="x_in">A float representing the x component of the vertex from which a distance will be measured in respect to this NetNode.</param>
    /// <param name="y_in">A float representing the y component of the vertex from which a distance will be measured in respect to this NetNode.</param>
    /// <param name="z_in">A float representing the z component of the vertex from which a distance will be measured in respect to this NetNode.</param>
    /// <returns>A float representing the distance between the vertex passed to this method and the vertex represented by this NetNode.</returns>
    GLfloat GetDistance(GLfloat x_in, GLfloat y_in, GLfloat z_in);
    /// <summary>Retrieves the original vertex represented by this NetNode by placing them in a provided pointer. This value is not affected by relaxation passes but it is affected by
    /// <c>NetNode::CommitNewVertex()</c> - be sure to call the CommitNewVertex() method prior to calling this method if you want the vertex returned by this method to reflect
    /// any recent relaxation passes.<seealso cref="NetNode::vertex"/><seealso cref="NetNode::CommitNewVertex()"/><seealso cref="NetNode::Relax()"/></summary>
    /// <param name="ptr_in"/>A pointer to an array of floats which must have at least three elements. The first three elements of this array will be filled with the x,y, and z components
    /// of the vertex represented by this NetNode.</param>
	void SetVertex(glm::vec3 vertex_in);
    void GetVertex(GLfloat * ptr_in);
	glm::vec3 GetVertex(void);
    /// <summary>Copies three floats from <c>NetNode::new_vertex</c> to <c>NetNode::vertex</c> effectively "forgetting" the old vertex and placing a [potentially] newly relaxed vertex
    /// in its place. This method needs to be called after relaxation and prior to updating constraints. Also note that this method should be called prior to calling <c>NetNode::GetVertices()</c> if
    /// the caller wants the vertices returned by GetVertices() to reflect relaxation recent relaxation passes.
    /// <seealso cref="NetNode::vertex"/><seealso cref="NetNode::new_vertex"/><seealso cref="NetNode::Relax()"/><seealso cref="NetNode::UpdateConstraints()"/><seealso cref="NetNode::GetVertices()"/></summary>
    /// <remarks>This method should be called on all NetNodes in a given SurfaceNet after all calls to <c>NetNode::Relax()</c> are completed and before any calls to <c>NetNode::UpdateConstraints()</c>
    /// are made.</remarks>
	bool operator==(NetNode & node_in);
    void CommitNewVertex(void);
	int AddNormal(glm::vec3 normal_in);
    bool IsLimited(void);
    bool IsOriginalLimit(void);
    void SetBlendedNormal(glm::vec3 normal_in);
    glm::vec3 GetBlendedNormal(bool normalized=false);
    std::vector<GLuint> GetNeighbors(void);
    void SetSimplified(void);
    void ClearSimplified(void);
	bool IsSimplified(void);
	void UpdateNeighbors(GLuint old_in, GLuint new_in);
	void UpdateNeighbors(GLuint old_in, bool subtract, bool erase);
	void SetConstraint(GLfloat constraint_in);
	GLfloat GetConstraint(void);
	void ClearNormals(void);
    void Limit(void);
    void PendingLimit(void);
    /// <summary>The destructor for the NetNode class which is responsible for freeing any memory which was dynamically allocated in a "NetNode method. Currently empty.</summary>
    /// <remarks>Declared virtual as a stylistic choice. =]</remarks>
    virtual ~NetNode(void);
};
#pragma endregion
#pragma region SurfaceNet Class
/// <summary>A collection of NetNodes which facilitates the relaxation of these nodes to allow for a smoother, more appealing mesh. This class holds a collection of each of the unique vertices
/// that each NetNode represents and an array of indices into said vertex collection which represents the actual mesh (a la an element buffer). Nodes are added, accessed, and modified via methods of
/// this class and can be dumped for rendering via <c>SurfaceNet::GetVertices()</c>. <seealso cref="NetNode"/><seealso cref="BlockArray"/></summary>
/// <remarks>As of 05/22/13 this class is still in very early development and may be subject to heavy modification in the near future, namely to facilitate continuous level-of-detail, continuous
/// generation, etc.</remarks>
class SurfaceNet
{
friend class VoxelField;
private:
protected:
    /// <summary>An std::vector of pointers to unique NetNode objects. Each element in this vector has a paired element at the same index in <c>SurfaceNet::vertices</c> - both of which represent
    /// a single unique vertex in the mesh generated by Marching Cubes. The objects contained in this vector are created by (and must be deleted by) this class. This collection of pointers allows
    /// this class to swiftly iterate through all NetNodes and perform batch operations on all nodes, namely relaxation.
    /// <seealso cref="SurfaceNet::AddNode()"/><seealso cref="NetNode"/><seealso cref="SurfaceNet::vertices"/><seealso cref="SurfaceNet::element_indices"/></summary>
    /// <remarks>Does not need to be initialized in the constructor, but this collection needs to be iterated through and each object disposed of in this class's destructor.</remarks>
    std::vector<NetNode*> field;
    /// <summary>An std::vector of three component [mathematical] vectors (compliment of the external GLM library). Each element in this vector has a paired element at the same index in
    /// <c>SurfaceNet::field</c> - both of which represent a single unique vertex in the mesh generated by Marching Cubes. This collection of vertices is the pool of unique vertices that
    /// make-up the mesh, but the order of the vertices as they lie in the mesh is not maintained in this collection - the mesh can be reconstructed by iterating through <c>SurfaceNet::element_indices</c>
    /// and using each element as an index into this collection. <seealso cref="SurfaceNet::element_indices"/><seealso cref="SurfaceNet::field"/><seealso cref="SurfaceNet::AddNode()"/></summary>
    /// <remarks>Does not need to be initialized in the constructor.</remarks>
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec4> colors;
    /// <summary>An std::vector of unsigned integers which represent the mesh (with vertex order intact) as a collection of indices into an array of unique vertices. The functionality offered by this
    /// member is similar to an Element Buffer Object. In the current implementation of this class, <c>SurfaceNet::GetVertices()</c> does the work of looking up the vertices and placing them into
    /// an array in the order needed for the rendering/draw call. In the future it will probably make more sense to dump these indices into an Element Buffer Object.
    /// <seealso cref="SurfaceNet::vertices"/><seealso cref="SurfaceNet::field"/><seealso cref="SurfaceNet::AddNode()"/></summary>
    /// <remarks>Does not need to be initialized in the constuctor.</remarks>
    std::vector<GLuint> element_indices;
    GLuint valid_nodes;
public:
    /// <summary>The constructor for this class which is responsible for initializing any members which need initializing. Currently this method is empty since this class and its members are populated
    /// with calls to <c>SurfaceNet::AddNode()</c> and <c>SurfaceNet::AddNeighbors()</c> post-construction.
    /// <seealso cref="SurfaceNet::AddNode()"/><seealso cref="SurfaceNet::AddNeighbors()"/></summary>
    SurfaceNet(void);
    /// <summary>This method takes three floats as parameters which represent a point in 3D space which is added to the SurfaceNet if an indentical vertex does not already exist. This new NetNode is 
    /// added without any neighbors linked to it - they must be added via <c>SurfaceNet::AddNeighbors()</c>. Before the new NetNode is added, a check is made to see if any of the elements in 
    /// <c>SurfaceNet::vertices</c> share the same x,y, and z component as the prospective new NetNode - if so, the index of the existing identical NetNode into <c>SurfaceNet::vertices</c> and
    /// <c>SurfaceNet::field</c> is returned. Regardless of whether or not a new NetNode was added, an index will always be returned.
    /// <seealso cref="SurfaceNet::field"/><seealso cref="SurfaceNet::vertices"/><seealso cref="SurfaceNet::element_indices"/></summary>
    /// <param name="x_in">A float representing the x component of the vertex which will be added to the SurfaceNet as a NetNode, should no NetNode with an identical vertex already exist.</param>
    /// <param name="y_in">A float representing the y component of the vertex which will be added to the SurfaceNet as a NetNode, should no NetNode with an identical vertex already exist.</param>
    /// <param name="z_in">A float representing the z component of the vertex which will be added to the SurfaceNet as a NetNode, should no NetNode with an identical vertex already exist.</param>
    /// <returns>An integer representing the index of the NetNode into <c>SurfaceNet::field</c> and <c>SurfaceNet::vertices</c> - whether or not a new NetNode was added or an existing identical
    /// NetNode was found.</returns>
    /// <remarks>This method could probably be made faster by changing the parameter list to allow an array of vertices (NetNodes) to be added at one time.</remarks>
    GLuint AddNode(glm::vec3 vertex_in, bool limit_in=false, bool blind_add=false);
	bool RelaxNode(GLuint index_in);
    /// <summary>This method adds any number of neighbors to a NetNode at the specified index. All indices passed to this method are those returned by <c>SurfaceNet::AddNode()</c>. If the index
    /// at which the neighbors will be added is invalid, no neighbors will be added; invalid neighbor indices will be skipped as well. Ultimately this method is a wrapper around 
    /// <c>NetNode::AddNeighbor()</c>. <seealso cref="NetNode::AddNeighbor()"/><seealso cref="SurfaceNet::AddNode()"/></summary>
    /// <param name="target_index">An unsigned integer representing the index of the NetNode to which neighbors will be added (provided the index is valid). If this value exceeds
    /// the number of elements in <c>SurfaceNet::field</c> then no neighbors are added and _BAD (-1) is returned.</param>
    /// <param name="neighbors_in">A pointer to an array of unsigned integers, the count of which is specified by the 3rd parameter of this method. These numbers represent the indices of
    /// the NetNodes (into <c>SurfaceNet::field</c> and <c>SurfaceNet::vertices</c>) which are to-be-added as neighbors to a NetNode which already exists in the SurfaceNet. Any indices
    /// which exceed the count of elements in <c>SurfaceNet::field</c> will be skipped.</param>
    /// <param name="neighbor_count">An unsigned integer representing the number of elements in the second parameter passed to this method. In other words, this number represents the number
    /// of neighbors which are to-be-added to the specified NetNode. If this parameter is less than 1 then no neighbors will be added and _BAD (-1) will be returned.</param>
    /// <returns>An integer which represents the success of the method. Returns _OK (0) as long as the first and third parameters to this method are valid (target index and neighbor count) -
    /// returns _BAD (-1) otherwise.</returns>
    /// <remarks>If rendering triangles, this method will be called two times on each vertex in each triangle.</remarks>
    int AddNeighbors(GLuint target_index, GLuint * neighbors_in, GLuint neighbor_count);
    int AddNormal(GLuint target_index, glm::vec3 normal_in);
	glm::vec3 CalculateNormal(GLuint indices_in[3], bool normalize=false);
    int SetBlendedNormal(GLuint target_index, glm::vec3 normal_in);
    void BlendNormals(void); 
	GLint FindPartner(GLuint index_in, glm::vec3& vec_out);
	void SafeDeleteNodes(std::vector<GLuint> delete_us);
	void SafeDeleteNodes(std::vector<GLuint> delete_us, std::vector<GLuint> new_in);
	void SimplifyMesh(GLint iterations_in);
	bool SimplifyNode(GLint index_in);
	void SetSimplified(GLuint target_index);
	void UpdateConstraints(void);
	void DestroyInnerTriangles(void);
    /// <summary>Simply returns the number of NetNodes in the SurfaceNet. In other words, this method returns the number of elements in <c>SurfaceNet::field</c> and <c>SurfaceNet::vertices</c>.
    /// This number does not necessarily reflect the total number of vertices in the final mesh.
    /// <seealso cref="SurfaceNet::field"/><seealso cref="SurfaceNet::vertices"/><seealso cref="SurfaceNet::element_indices"/><seealso cref="SurfaceNet::AddNode()"/></summary>
    /// <returns>An integer representing the number of NetNodes in the SurfaceNet (the number of elements in <c>SurfaceNet::field</c> and <c>SurfaceNet::vertices</c>).</returns>
    int GetNodeCount();
    /// <summary>Retrieves all of the vertices that are represented by the SurfaceNet (and its NetNodes) in proper order needed to draw triangles. This method takes a float pointer
    /// as a parameter along with the start index and number of vertices that are desired to be retrieved. This method iterates th rough <c>SurfaceNet::element_indices</c> according to the
    /// input parameters and returns the values that the indices point to in <c>SurfaceNet::vertices</c>.
    /// <seealso cref="SurfaceNet::element_indices"/><seealso cref="SurfaceNet::vertices"/><seealso cref="SurfaceNet::AddNode()"/></summary>
    /// <param name="float_ptr_in">A pointer to a float array which must have room for the number of elements represented by the "count_in" parameter. All of the desired vertices will be copied
    /// into this array in the order needed to render triangles.</param>
    /// <param name="start_index_in">An unsigned integer representing the index at which the caller wants to begin retrieving vertices. If this value plus the vertex count exceeds the number of vertices
    /// available then this method will do nothing and return <c>False</c>.</param>
    /// <param name="count_in">An unsigned integer representing the number of vertices the caller wishes to retrieve. If this value combined with the start index parameter exceeds the
    /// number of vertices available for retrieval then this method will do nothing and return <c>False</c>.</param>
    /// <returns>A boolean value - true as long as the start index parameter plus the vertex count parameter does not exceed the number of vertices available. Returns false otherwise.</returns>
    /// <remarks>In the future this method may/should/will be changed to supply unique vertex data and element indices, as opposed to iterating through all element indices and returning
    /// the full mesh, as it does now.
    bool GetVertices(GLfloat * float_ptr_in, GLuint start_index_in,GLuint count_in);
    /// <summary>Iterates through all NetNodes in this SurfaceNet and adjusts them according to the position of each of their neighbors. This process can be repeated iteratively to achieve a smoother
    /// Marching-Cubes-based mesh of vertices. This method iterates through all NetNodes in order to relax them, iterates through all of the NetNodes again to actually update the vertices, then finally
    /// iterates through them again to update constraints. This method is responsible for updating <c>SurfaceNet::vertices</c> as well.
    /// <seealso cref="NetNode::Relax()"/><seealso cref="NetNode::CommitNewVertex()"/><seealso cref="NetNode::UpdateConstraints()"/></summary>
    /// <returns>A boolean value indicating the success of the method. Currently always returns <c>True</c>.</returns>
    bool RelaxNodes(GLuint count_in);
	bool SpreadLimitOfNode(GLuint index_in);
    void SpreadLimits(GLint count);
    void BurnData(GLuint indices_in[3], sqlite3_stmt * stmt_in);
    bool LoadData(GLuint indices_in[3], sqlite3_stmt * stmt_in);
    /// <summary>The destructor for the SurfaceNet class which is responsible for freeing any memory which was dynamically allocated in a SurfaceNet method. This destructor is responsible for 
    /// properly disposing of all NetNodes.</summary>
    /// <remarks>Declared virtual as a stylistic choice.</remarks>
    virtual ~SurfaceNet(void);
};
#pragma endregion
#pragma region Voxel Class
/// <summary>A class which represents a single perfectly cubic area of space. This area of space is represented by an origin, which is the very center of the cube, and
/// the "density" values at the volume's 8 corners. Density values which are greater than 0 are considered outside space, while density values less than or equal to 0 are considered
/// "in the ground". Voxel values are processed using a technique known as "Marching Cubes" which utilizes some look-up tables  - which I will not explain here - that result in fairly flexible
/// mesh arrangements whose limitations can be aleved via further processing / algorithmic enhancements.
/// <seealso cref="VoxelField"/><seealso cref="SurfaceNet"/><seealso cref="BlockArray"/></summary>
/// <remarks>A collection of instances of this class is managed via a VoxelField.</remarks>
class Voxel
{
protected:
    //--- Static Class Variables ---//
    /// <summary>Three collections of four unsigned short integers which represent the indices of all of the edges which are aligned on each of the three axis in 3D space. The first collection of
    /// four unsigned shorts represents the four edges which align with the x-axis, the second collection represents the edges which align with the y-axis, and the last collection represents
    /// the indices of the edges which align with the z-axis. This information is the same for every voxel and is used to determine which axis each generated vertex will be interpolated on.
    /// <seealso cref="Voxel::EdgesOfIndex"/><seealso cref="Voxel::SetMarchingCubesParameters()"/></summary>
    /// <remarks>The order of the four elements in each four element container is arbitrary and thus the edges are ordered in a logical order, as opposed to ordering them by rotation, etc.</remarks>
    static GLushort EdgesByAxis[3][4];
    /// <summary>Twelve collections of two unsigned short integers which represent the indices of the two corners which form the end points of each of the twelve possible edges of the voxel. Once it
    /// has been determined which edges the vertices lie on for the given voxel configuration, the indices of the edge that each vertex lies on is used to index into this collection of twelve pairs of
    /// [different] indices. These new pair of indices are used in order to index into <c>Voxel::corners</c> and subsequently generate a vertex by interpolating the vertices that lie at two corners
    /// according to the density values at those corners. <seealso cref="Voxel::EdgesByAxis"/><seealso cref="Voxel::corners"/><seealso cref="Voxel::SetMarchingCubesParameters()"/></summary>
    /// <remarks>Each index into the outer collection of this member corresponds to the edge index.</remarks>
    static GLushort CornersOfEdge[12][2];
    /// <summary>A collection of 256 8-bit numbers which indicate the number of vertices which result from each of the 256 possible voxel configurations - for quick creation of arrays.
    /// <seealso cref="Voxel::num_vertices"/><seealso cref="Voxel::EdgesOfIndex"/></summary>
    /// <remarks>There will never be more than 15 vertices (or 45 total numbers since there 3 components to each vertex) in a single voxel.</remarks>
    static GLchar VerticeCount[256];
    /// <summary>256 collections of fifteen indices which serve as the bread-and-butter to the Marching Cube algorithm - each of the collection of fifteen indices represents the fifteen edges on
    /// which the vertices of a given voxel arrangement lie, while the index into the outermost 256-element array represents each of the 256 (2^8 where 8 is the number of corners in a voxel) possible
    /// voxel configurations. The index into this outermost 256-element array can be determined by determining the sign of the density values in each corner of the voxel and performing a bitwise AND
    /// to combine the "inside/outside" status of all 8 corners into a single 8-bit number which can be used as the index.</summary>
    /// <remarks>Figuring out these indices was difficult and is dependent upon the specific Marching Cubes algorithm that is used. Our algorithm (courtesy of Eric Lengyel's dissertation) supposadely
    /// contains zero amiguous configurations which should theoretically render it "unbreakable". Indices are stored as chars since there will be no edge index above 15.</remarks>
    static GLchar EdgesOfIndex[256][15];
    /// <summary>An array of indices (into <c>Voxel::VerticeCount</c> and <c>Voxel::EdgesOfIndex</c>) which indicate which of the 256 possible voxel configurations must be inverted (from its "equivalent 
    /// case" - something I won't cover here). This member is currently not being utilized but may be utilized for debugging purposes if needed.</summary>
    static GLuchar InvertedIndices[];
    static GLushort Voxel::EdgesByRegion[3][2][4];
    //--- Protected Variables ---//
    /// <summary>A three element array of floats which represent the x,y, and z components of the very middle of the cubic area represented by the voxel. This center value is used in order to determine
    /// the vertices at the corner of the voxel and subsequently the generated interpolated vertices which lie on its edge. This value could also be used for operations such as proximity checks.
    /// <seealso cref="Voxel::corners"/><seealso cref="Voxel::Voxel()"/></summary>
    /// <remarks>This member is initialized directly via the parameters passed to the constructor of this class.</remarks>
    GLfloat origin[3];
    /// <summary>A collection of three element float arrays which represent the x,y, and z components of the vertices of the eights corners of the cubic area represented by the voxel. These vertices
    /// are used to generate vertices by interpolating two adjacent corners according to the voxel configuration.
    /// <seealso cref="Voxel::origin"/><seealso cref="Voxel::Voxel()"/></summary>
    /// <remarks>Initialized in the constructor by adding or subtracting <c>Voxel::radius</c> from <c>Voxel::origin</c> along various axis.</remarks>
    GLfloat corners[8][3];
    /// <summary>A float representing the distance from the center of the cubic area represented by the voxel and the middle of any of the voxel's faces. In other words, it is half of the width of the 
    /// cubic area represented by the voxel. <seealso cref="Voxel::Voxel()"/></summary>
    /// <remarks>Initialized directly via a parameter passed to the constructor of this class.</remarks>
    GLfloat radius;
    /// <summary>A signed short integer which represents the index of this voxel's configuration into Marching Cubes' 256 possible configurations (2^8). This number is the primary number that determines
    /// "what the voxel will look like" - or more specifically the edges on which vertices will lie and the direction normals will face. This number is calculated by iterating through the corners of the
    /// voxel and adding 2 to the power of the sign of the corner value (1 if the corner value is negative; 0 if the corner value is positive).</summary>
    /// <remarks>Initialized to _BAD (-1) in this class's constructor and set to a proper index in <c>Voxel::SetMarchingCubesParameters()</c>. If this value is 0 or 255 then no vertices will be rendered.</remarks>
    GLshort mc_index;
    /// <summary>An 8-bit number representing the number of vertex components this voxel currently contains. This number represents the number of floating point numbers (the x,y, and z components for each
    /// of the up-to 15 vertices) - not the number of vertices (which will be this value divided by three) - and will/should never exceed 45 (thus this number only need be 8-bit).
    /// <seealso cref="Voxel::SetMarchingCubesParameters()"/></summary>
    /// <remarks>Initialized to 0 in this class's constructor and modified again in <c>Voxel::SetMarchingCubesParameters()</c>.</remarks>
    GLchar num_vertices;
public:
    //--- Public Methods ---//
    // ~ Constructor(s)
    /// <summary>The sole constructor for this class which requires four floats as parameters, the first three of which represent the x,y, and z components of the very center of the voxel. This coordinate
    /// will determine where the corners of the voxel will be located and ultimately where the vertices will be generated. The fourth parameter required is the "radius" of the cube, or the half the width
    /// of any given edge - this parameter will determine how "large" the voxel is.
    /// <seealso cref="Voxel::SetMarchingCubesParameters()"/></summary>
    /// <param name="x_in">A float representing the x component of the coordinate at which the voxel will be located/centered on.</param>
    /// <param name="y_in">A float representing the y component of the coordinate at which the voxel will be located/centered on.</param>
    /// <param name="z_in">A float representing the z component of the coordinate at which the voxel will be located/centered on.</param>
    /// <param name="radius_in">A float representing the distance between the center of the voxel and the center of any of its faces. In other words, this is half the length of any of the edges
    /// of the cubic area represented by the voxel. Ultimately this value determines how large the voxel is.</param>
    /// <remarks>This constructor does not determine the voxel configuration or the actual vertices that will be rendered - that is determined in <c>Voxel::SetMarchingCubesParameters()</c>.</remarks>
    Voxel(GLfloat x_in, GLfloat y_in, GLfloat z_in, GLfloat radius_in);
    // ~ Operator(s)
    /// <summary>Assignment operator method. This method is called whenever a Voxel is on the left-hand side of an '=' operator and is responsible for responsibley freeing any memory exclusively owned
    /// by the Voxel class and then copying the data from the Voxel instance found on the right-hand side of the '=' operator into its own members.</summary>
    /// <param name="voxel_in">The Voxel instance which will be copied to the target (invoking) instance.</param>
    /// <returns>Trivially returns a reference to the type. Return type could be void as well.</returns>
    /// <remarks>Be sure to check for anomolies (such as self-assignment) when coding this method. An example invocation of this method is <c>foo=bar;</c> where foo and bar are both predefined Voxel
    /// instances.</remarks>
    Voxel& operator=(const Voxel & voxel_in);
    // ~ Configuration
    /// <summary>This method directly sets the density values at all eight corners of the voxel, determines the resulting Marching Cubes case index for this voxel, then adds the appropriate NetNodes
    /// and NetNode neighbors to a SurfaceNet passed as a parameter. The Marching Cubes case number is determined by iterating through the corner values and adding 2^(index into corner_vals array)
    /// for each corner value that is less than zero. Various look up tables are accessed / iterated through until the algorithm has determined the pairs of coordinates (voxel corners) which need to
    /// be interpolated and their interpolation ratio. The voxels corners are interpolated according to their density values so that a surface is created where f(x)=0. The maximum amount of vertices
    /// that will be generated for any given voxel is 15 (45 floats total). <seealso cref="VoxelField::FillWithRandom()"/><seealso cref="Voxel::EdgesOfIndex"/><seealso cref="Voxel::EdgesByAxis"/>
    /// <seealso cref="Voxel::CornersOfEdge"/></summary>
    /// <param name="corner_vals">An array of eight characters (actually 8-bit numbers) which represent the density values at each of the eight corners of the voxel. A positive density value means
    /// the corner lies outside "solid matter" whereas a zero or a negative number implies the corner lies inside "solid matter". Combined, the density values at all eight corners of the voxel
    /// determine the "voxel configuration" and "Marching Cubes casde index" for the voxel. It is expected that each corner value is within the range of -255 to +255, and if a nice-looking continuous
    /// mesh is desired, then a combination of noise functions should be used in order to generate the corner values.</param>
    /// <param name="target_edges">An array of eight unsigned integers which will be populated by this method with the fifteen indices of the edges on which the voxel's generated vertices will lie.
    /// If less than fifteen vertices are generated then the remaining elements are populated with the value '-1'. These edges are ultimately converted into a global edge index (an index that is 
    /// unique to all edges across all voxels) then the global edge index is used to blend normals which share an edge - a process which could easily be accomplished via the SurfaceNet class which
    /// already has the structure for such an operation. In other words, I expect this parameter to be removed/deprecated at some point in the near future.</param>
    /// <param name="net_in">A reference to an existing SurfaceNet to which the generated vertices (NetNodes) will be added. This method takes care of populating each NetNode with its appropriate
    /// neighbors, as well.</param>
    /// <returns>An unsigned integer representing the number of vertices generated by this method. This number will always range from 0 to 15.</returns>
    /// <remarks>Normal blending should be moved to the SurfaceNet class soon.</remarks>
    GLuint SetMarchingCubesParameters(GLchar corner_vals[8],SurfaceNet & net_in, bool limit[3][2]);
    /// <summary>A method which takes a LineStrip as a parameter and adds the 12 line segments which represent the edges of the voxel. This technique can be used in order to display the gridlines
    /// on which all vertices will fall (not counting post processing such as relaxation techniques).<seealso cref="LineStrip::AddInstance()"/><seealso cref="Voxel::SetMarchingCubesParameters()"/></summary>
    /// <param name="line_in">A reference to an existing LineStrip to which the voxel edge line segments will be added.</param>
    /// <returns>An integer indicating the success of the method. Currently always returns _OK (0).</returns>
    /// <remarks>Used primarily for debugging purposes. Note that this method will result in a very high number of line segments should the voxel gridlines be displayed for all voxels.</remarks>
    GLint AddCubeMesh(LineStrip & line_in);
    // ~ Destructor
    /// <summary>The destructor for the Voxel class which is responsible for freeing any memory which was dynamically allocated in a Voxel method.</summary>
    /// <remarks>Declared virtual as a stylistic choice.</remarks>
    virtual ~Voxel(void);//
};
#pragma endregion
#pragma region VoxelField Class
/// <summary>This class is responsible for managing a collection of Voxel class instances. This class creates a coniguous three dimensional array of voxels whom share edges/faces. This class is capable
/// populating the density values of the corners of each voxel via noise functions, taking advantage of the fact that neighboring voxels share corner density values and also the fact that the noise
/// functions tend to generate a continuous fractal-like structure.
/// <seealso cref="Voxel"/><seealso cref="BlockArray"/></summary>
/// <remarks>Currently this class is very rudimentary - in the future it may implement continuous level of detail, chunk loading, etc. This</remarks>
class VoxelField
{
private:
    GLuint index_offsets[3];
    GLfloat width_squared;
    GLfloat width_cubed;
    GLfloat v_width;
    GLfloat v_w_squared;
    GLuint widthplus;
    GLuint widthpluscubed;
protected:
    /// <summary>An unsigned integer representing the number of voxels in a single dimension of the voxel field (the size of each dimension should be the same). The total number of voxels will be this
    /// number to the power of three. This value is used widely through-out <c>VoxelField::FillWithRandom()</c> in order to iterate through the voxels, match corner density values to Voxel class
    /// instances, etc. <seealso cref="VoxelField::FillWithRandom()"/></summary>
    /// <remarks>This member is initialized directly via a required parameter passed to the constructor of this class.</remarks>
    GLuint width;
    /// <summary>A float representing the size of each individual voxel. This value is not only passed directly to the constructor of each voxel generated in <c>VoxelField::FillWithRandom()</c> but also
    /// indirectly determines the coordinates at which each voxel will be located. Currently our Marching Cubes algorithm implementation expects all voxels which need to be connected to each-other to be
    /// the same size.<seealso cref="Voxel::radius"/><seealso cref="VoxelField::FillWithRandom()"/></summary>
    /// <remarks>This member is initialized directly via a parameter passed to the constructor.</remarks>
    GLfloat vox_size;
    /// <summary>An array of three floats which represent the x,y, and z component of the coordinate that marks the 'origin' of the field of voxels. The origin is typically whichever corner of the first
    /// voxel which is also the "original" corner of the entire voxel field. This value will be passed onto the <c>BlockArray</c> instance passed to <c>VoxelField::FillWithRandom</c> and will be the
    /// starting coordinate from which are vertices rendered in this VoxelField will be relative to.
    /// <seealso cref="VoxelField::VoxelField()"/><seealso cref="VoxelField::FillWithRandom</c><seealso "BlockArray::origin"/></summary>
    /// <remarks>This member is initialized directly via parameters passed to the constructor of this class.</remarks>
    GLfloat origin[3];
    /// <summary>An std::vector (managed array) of Voxel class instances which holds all of the Voxels in this entire VoxelField. This member is populated via <c>VoxelField::FillWithRandom()</c> which uses
    /// a noise function to generate a three dimensional contiguous series of corner density values which are then passed to the Voxel constructor, thus configuring the Voxel and determining its vertices.
    /// <seealso cref="VoxelField::FillWithRandom()"/><seealso cref="VoxelField::vox"/></summary>
    /// <remarks>This member need not be initialized in the constructor. Currently this member does very little outside of the scope of the original generation of the Voxels, but in the future this
    /// member will be essential to modification operations.</remarks>
    std::vector<Voxel> voxel_vec;
    /// <summary>An std::vector of characters (8-bit numbers) which store the corner values of all of the voxels in their original logical order, with no duplicates. Currently this member has no use
    /// outside of <c>VoxelField::FillWithRandom()</c>, but it may be used by modification operations implemented in the future.
    /// <seealso cref="VoxelField::FillWithRandom()"/><seealso cref="VoxelField::voxel_vec"/></summary>
    /// <remarks>This member need not be initialized in the constructor.</remarks>
    std::vector<GLchar> vox;
    GLfloat center[3];
public:
    /// <summary>The constructor for the VoxelField class which [at the moment] does very little but directly initialize some members via parameters passed to the constructor. Most of the action is performed
    /// in <c>VoxelField::FillWithRandom()</c>.
    /// <seealso cref="VoxelField::origin"/><seealso cref="VoxelField::width"/><seealso cref="VoxelField::vox_size"/></summary>
    /// <param name="width_in">An unsigned integer representing the number of voxels in any given dimension of the three dimensional field of voxels - all dimensions will be of equal width,
    /// at least in the current implementation of Marching Cubes. This parameter is directly equated to <c>VoxelField::width</c> in this constructor.</param>
    /// <param name="each_vox_size">A float representing HALF the size of each individual voxel (each side/dimension of each individual voxel should be the same). This number will be directly set to
    /// <c>VoxelField::vox_size</c> and <c>Voxel::radius</c> and will also indirectly determine <c>Voxel::origin</c>.</param>
    /// <param name="orig_x_in">A float representing the x component from which all vertices generated by the VoxelField will be relative to. In other words, it's the smallest x component of the coordinate
    /// at which any given corner density value will lie. This value will be directly assigned to the BlockArray passed to <c>VoxelField::FillWithRandom()</c>.</param>
    /// <param name="orig_y_in">A float representing the y component from which all vertices generated by the VoxelField will be relative to. In other words, it's the smallest y component of the coordinate
    /// at which any given corner density value will lie. This value will be directly assigned to the BlockArray passed to <c>VoxelField::FillWithRandom()</c>.</param>
    /// <param name="orig_z_in">A float representing the z component from which all vertices generated by the VoxelField will be relative to. In other words, it's the smallest z component of the coordinate
    /// at which any given corner density value will lie. This value will be directly assigned to the BlockArray passed to <c>VoxelField::FillWithRandom()</c>.</param>
    /// <remarks></remarks>
    VoxelField(GLuint width_in, GLfloat each_vox_size, GLint index_x_in=0, GLint index_y_in=0,GLint index_z_in=0);
    /// <summary>The core method of this class which initializes all voxels and populates a given BlockArray with the NetNodes which were based on the vertices generated by a Marching Cubes algorithm
    /// seeded with a noise function. Once a three dimensional array of corner density values has been created via a noise function, the corner density values are used to create a contiguous three-dimensional
    /// array of voxels whose vertices are extracted (at the same time they are created) and then used to create NetNode instances and populate each NetNode with its appropriate neighbors (any vertices with
    /// share a triangle with the NetNode's vertex). Once all NetNodes are created, a SurfaceNet is created which relaxes the nodes. Finally, the relaxed vertices are dumped into a BlockArray instance
    /// which is then put into the rendering loop where it is rendered each frame.
    /// <seealso cref="Voxel::SetMarchingCubesParameters()"/><seealso cref="BlockArray::UpdateVertices()"/></summary>
    /// <param name="barr_in">A reference to an existing BlockArray instance at which the vertices will be dumped once they are generated. This BlockArrays vertices will be updated and its origin will
    /// be set to the origin of this VoxelField.</param>
    /// <param name="interpolate_normals">A boolean value which determines whether or not any normals which share the same voxel edge (or more specifically, the same "global voxel edge index") will
    /// be blended/averaged together - a process which results in smoother lighting effects on the mesh.</param>
    /// <returns>An integer intended to indicate the success of the method. Currently always returns _OK (0).</returns>
    /// <remarks>This method is still in very early development and its core structure will most likely be drastically altered as a more robust, effecient, and comprehensive voxel solution is
    /// implemented.</remarks>
    GLint FillWithRandom(BlockArray & barr_in, sqlite3_stmt * stmt_in);
    /// <summary>A method which simply adds all of the line segments representing the cubic area represented by each voxel to a LineStrip and then returns the LineStrip. This method actually interates
    /// through all Voxels contained in <c>VoxelField::voxel_vec</c> and invokes <c>Voxel::AddCubeMesh()</c> in order to add line segments to the LineStrip.
    /// <seealso cref="Voxel::AddCubeMesh()"/><seealso cref="LineStrip::AddInstance()"/><seealso cref="VoxelField::FillWithRandom()"/></summary>
    /// <returns>A LineStrip instance containing all of the line segments representing every edge of every voxel. The resulting LineStrip is a uniform grid on which all vertices will lie
    /// (pre-relaxation and such).</returns>
    /// <remarks>This method can be very costly unless the LineStrip implementation combines any parallel lines which share an end point. This method will result in a "blank" LineStrip if invoked prior
    /// to an invocation of <c>VoxelField::FillWithRandom()</c>.</remarks>
    LineStrip GetCubeMeshes(void);
    void GetCenter(GLfloat arr_in[3]);
    bool LoadData(BlockArray & barr_in, sqlite3_stmt * stmt_in);
    /// <summary>The destructor for the VoxelField class which is responsible for freeing any memory which was dynamically allocated in a VoxelField method.</summary>
    /// <remarks>Declared virtual as a stylistic choice.</remarks>
    virtual ~VoxelField(void);

};
#pragma endregion
#pragma region VoxelFieldMatrix
class VoxelFieldMatrix
{

};
#pragma endregion
}
#endif