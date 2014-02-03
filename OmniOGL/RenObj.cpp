#include "RenObj.h"
namespace OmniOGL
{
#pragma region RenObj Class
#pragma region Constructors
RenObj::RenObj(void):
    // ~ Spatial Flags
    en_internal_clipping(true),
    in_foreground(false),
    // ~ Spatial Data
    my_section((GLuint)_BAD),
    selection_radius(-1.0),
    // ~ Material Data
    texture_sel(_BAD),
    bump_map(_BAD),
    lighting((MaterialLighting*)_BAD),
    blending((BlendFunction*)_BAD),
    stencil((StencilFunction*)_BAD),
    depth(new DepthFunction()),
    // ~ Meta
    render_id(_BAD),
    // ~ Primary Data
    vertices((GLfloat*)_BAD),
    colors((GLfloat*)_BAD),
    texcoords((GLfloat*)_BAD),
    normals((GLfloat*)_BAD)
{
    for(GLint i=0; i<4; i++)
        this->lengths[i]=0;
    // ~ Physical Properties
    memset(this->origin,0,3);
    return;
}
RenObj::RenObj(const RenObj &copy_source):
    // ~ Spatial Flags
    en_internal_clipping(copy_source.en_internal_clipping),
    in_foreground(copy_source.in_foreground),
    // ~ Spatial Data
    my_section(copy_source.my_section),
    selection_radius(copy_source.selection_radius),
    // ~ Material Data
    texture_sel(copy_source.texture_sel),
    bump_map(copy_source.bump_map),
    // ~ Meta
    render_id(copy_source.render_id),
    // ~ Primary Data
    vertices((GLfloat*)_BAD),
    colors((GLfloat*)_BAD),
    texcoords((GLfloat*)_BAD),
    normals((GLfloat*)_BAD)
{
	// ~ Vertice Data
    memcpy(this->lengths,copy_source.lengths,sizeof(GLuint)*4);
    // ... Vertices
    _New(this->vertices,this->lengths[0]);
    if(this->lengths[0]!=0)
        memcpy(this->vertices,copy_source.vertices,sizeof(GLfloat)*this->lengths[0]);
    // ... Colors
    _New(this->colors,this->lengths[1]);
    if(this->lengths[1]!=0)
        memcpy(this->colors,copy_source.colors,sizeof(GLfloat)*this->lengths[1]);
    // ... Texture Coordinates
    _New(this->texcoords,this->lengths[2]);
    if(this->lengths[2]!=0)
        memcpy(this->texcoords,copy_source.texcoords,sizeof(GLfloat)*this->lengths[2]);
    // ... Normals
    _New(this->normals,this->lengths[3]);
    if(this->lengths[3]!=0)
        memcpy(this->normals,copy_source.normals,sizeof(GLfloat)*this->lengths[3]);
    // ~ Physical Properties
	for(GLint i=0; i<3; i++)
		this->origin[i]=copy_source.origin[i];
    //
    if(!_Valid(copy_source.lighting))
        _Init(this->lighting);
    else
        this->lighting=new MaterialLighting(*(copy_source.lighting));
    if(!_Valid(copy_source.depth))
        _Init(this->depth);
    else
        this->depth=new DepthFunction(*(copy_source.depth));
    if(!_Valid(copy_source.stencil))
        _Init(this->stencil);
    else
        this->stencil=new StencilFunction(*(copy_source.stencil));
    if(!_Valid(copy_source.blending))
        _Init(this->blending);
    else
        this->blending=new BlendFunction(*(copy_source.blending));
	return;
}
#pragma endregion
#pragma region Operators
RenObj & RenObj::operator=(const RenObj & copy_source)
{
	//--- Copy Protected Vars ---//
    // ~ Physical
    // ~ Rendering Properties
    this->en_internal_clipping=copy_source.en_internal_clipping;
    this->in_foreground=copy_source.in_foreground;
    this->my_section=copy_source.my_section;
    // ~ Selection
    this->selection_radius=copy_source.selection_radius;
    //--- Copy Public Vars ---//
	// ~ Vertice Data
    memcpy(this->lengths,copy_source.lengths,sizeof(GLuint)*4);
    // ... Vertices
    GLfloat * new_vertices;
    _Init(new_vertices);
    _New(new_vertices,this->lengths[0]);
    if(this->lengths[0]>0)
        memcpy(new_vertices,copy_source.vertices,sizeof(GLfloat)*this->lengths[0]);
    _Delete(this->vertices);
    this->vertices=new_vertices;
    // ... Colors
    GLfloat * new_colors;
    _Init(new_colors);
    _New(new_colors,this->lengths[1]);
    if(this->lengths[1]>0)
        memcpy(new_colors,copy_source.colors,sizeof(GLfloat)*this->lengths[1]);
    _Delete(this->colors);
    this->colors=new_colors;
    // ... Texture Coordinates
    GLfloat * new_texcoords;
    _Init(new_texcoords);
    _New(new_texcoords,this->lengths[2]);
    if(this->lengths[2]>0)
        memcpy(new_texcoords,copy_source.texcoords,sizeof(GLfloat)*this->lengths[2]);
    _Delete(this->texcoords);
    this->texcoords=new_texcoords;
    // ... Normals
    GLfloat * new_normals;
    _Init(new_normals);
    _New(new_normals,this->lengths[3]);
    if(this->lengths[3]>0)
        memcpy(new_normals,copy_source.normals,sizeof(GLfloat)*this->lengths[3]);
    _Delete(this->normals);
    this->normals=new_normals;
    // ~ Physical Properties
	for(GLint i=0; i<3; i++)
        this->origin[i]=copy_source.origin[i];
    // ~ Rendering
	this->texture_sel=copy_source.texture_sel;
    this->bump_map=copy_source.bump_map;
	this->render_id=copy_source.render_id;
    // ~ Lighting
    if(_Valid(this->lighting))
        _Delete(this->lighting);
    if(!_Valid(copy_source.lighting))
        _Init(this->lighting);
    else
        this->lighting=new MaterialLighting(*(copy_source.lighting));
     if(!_Valid(copy_source.depth))
        _Init(this->depth);
    else
        this->depth=new DepthFunction(*(copy_source.depth));
    if(!_Valid(copy_source.stencil))
        _Init(this->stencil);
    else
        this->stencil=new StencilFunction(*(copy_source.stencil));
    if(!_Valid(copy_source.blending))
        _Init(this->blending);
    else
        this->blending=new BlendFunction(*(copy_source.blending));
	return *this;
}
#pragma endregion
#pragma region Static Methods
const GLuint RenObj::vbo(GLuint index_in)
{
    return OGLBuffers::vbo(index_in);
}
const GLuint RenObj::ebo(GLuint index_in)
{
    return OGLBuffers::ebo(index_in);
}
const GLuint RenObj::vao(GLuint index_in)
{
    return OGLBuffers::vao(index_in);
}
#pragma endregion
#pragma region OpenGL Data Binding
GLint RenObj::BindBufferData(void)
{
	// ~ Vertices
	glBindBuffer(GL_ARRAY_BUFFER,OGLBuffers::vbo(0));
    glBufferData(GL_ARRAY_BUFFER,this->lengths[0]*sizeof(GLfloat),this->vertices,GL_DYNAMIC_DRAW);
    // ~ Color
	glBindBuffer(GL_ARRAY_BUFFER,OGLBuffers::vbo(1));
    glBufferData(GL_ARRAY_BUFFER,this->lengths[1]*sizeof(GLfloat),this->colors,GL_DYNAMIC_DRAW);
    // ~ Texture Coordinates
	glBindBuffer(GL_ARRAY_BUFFER,OGLBuffers::vbo(2));
    glBufferData(GL_ARRAY_BUFFER,this->lengths[2]*sizeof(GLfloat),this->texcoords,GL_DYNAMIC_DRAW);
    // ~ Normal
	glBindBuffer(GL_ARRAY_BUFFER,OGLBuffers::vbo(4));
    glBufferData(GL_ARRAY_BUFFER,this->lengths[3]*sizeof(GLfloat),this->normals,GL_DYNAMIC_DRAW);

    return _OK;
}
GLint RenObj::Prepare(OGLUniforms& uniforms_in, const bool& textures_en, const bool& bumpmap_en)
{
    // ~ Blend
    if(_Valid(this->blending) && this->blending->enabled)
    {
        if(!glIsEnabled(GL_BLEND))
            glEnable(GL_BLEND);
        glBlendFunc(this->blending->function[0],this->blending->function[1]);
    }
    else if(glIsEnabled(GL_BLEND))
        glDisable(GL_BLEND);
    // ~ Depth
    if(_Valid(this->depth) && this->depth->enabled)
    {
        if(!glIsEnabled(GL_DEPTH_TEST))
            glEnable(GL_DEPTH_TEST);
        glDepthFunc(this->depth->function);
        glDepthMask(this->depth->mask);
    }
    else if(glIsEnabled(GL_DEPTH_TEST))
        glDisable(GL_DEPTH_TEST);
    // ~ Stencil
    if(_Valid(this->stencil) && this->stencil->enabled)
    {
        if(!glIsEnabled(GL_STENCIL_TEST))
            glEnable(GL_STENCIL_TEST);
        glStencilMask(this->stencil->mask);
        glStencilFunc(this->stencil->function[0],this->stencil->function[1],this->stencil->function[2]);
        glStencilOp(this->stencil->operation[0],this->stencil->operation[1],this->stencil->operation[2]);
    }
    else if(glIsEnabled(GL_STENCIL_TEST))
        glDisable(GL_STENCIL_TEST);
    bool keep_enabled=false;
    //
    if(TextureMaster::TexturesEnabled()!=textures_en)
    {
        if(!textures_en)
            glDisable(GL_TEXTURE_2D);
        else if(textures_en)
            glEnable(GL_TEXTURE_2D);
        keep_enabled=textures_en;
        glUniform1i(uniforms_in.textures_enabled(),textures_en);
        TextureMaster::SetTexturesEnabled(textures_en);
    }
    if(TextureMaster::CurrentTextureIndex()!=this->texture_sel)
    {
        glUniform1i(uniforms_in.texture(),this->texture_sel);
        TextureMaster::SetCurrentTextureIndex(this->texture_sel);
    }
    if(this->bump_map!=_BAD && bumpmap_en)
    {
        if(!glIsEnabled(GL_TEXTURE_2D))
            glEnable(GL_TEXTURE_2D);
        glUniform1i(uniforms_in.bumpmapping_enabled(),true);
        glUniform1i(uniforms_in.bumpmap(),this->bump_map);
    }
    else
    {
        if(glIsEnabled(GL_TEXTURE_2D) && !keep_enabled)
            glDisable(GL_TEXTURE_2D);
        glUniform1i(uniforms_in.bumpmapping_enabled(),false);
    }
    if(_Valid(this->lighting))
        this->lighting->Update(uniforms_in);
    else
        MaterialLighting::StaticUpdate(uniforms_in);
    if(dynamic_cast<BlockArray*>(this)!=nullptr)
        glUniform1i(uniforms_in.is_a_voxel(),true);
    else
        glUniform1i(uniforms_in.is_a_voxel(),false);
    //
    return _OK;
}
#pragma endregion
#pragma region Getters
bool RenObj::GetForeground(void)
{
    return this->in_foreground;
}
bool RenObj::GetInternalClipping(void)
{
    return this->en_internal_clipping;
}
bool RenObj::GetLightingEnable(void)
{
    if(!_Valid(this->lighting))
        return false;
    return this->lighting->enabled;
}
GLuint RenObj::GetOccupiedSection(void)
{
    return this->my_section;
}
GLint RenObj::GetOrigin(GLfloat * const  & target_arr)
{
    try
    {
        memcpy(target_arr,this->origin,3*sizeof(GLfloat));
    }
    catch(GLint e)
    {
        return _BAD;
    }
    return _OK;
}
GLint RenObj::GetRenderID(void)
{
    return this->render_id;
}
GLfloat RenObj::GetSelectionRadius(void)
{
    return this->selection_radius;
}
GLint RenObj::GetTexture(void)
{
    return this->texture_sel;
}
#pragma endregion
#pragma region Setters
void RenObj::SetColor(const GLfloat & r_in, const GLfloat & g_in, const GLfloat & b_in, const GLfloat & a_in)
{
    for(GLuint i=0; i<this->lengths[1]; i++)
    {
        switch(i%4)
        {
        case 0:
            this->colors[i]=r_in;
            break;
        case 1:
            this->colors[i]=g_in;
            break;
        case 2:
            this->colors[i]=b_in;
            break;
        case 3:
            this->colors[i]=a_in;
            break;
        default:
            break;
        }
    }
}
bool RenObj::SetForeground(const bool & fgnd_en_in)
{
    this->in_foreground=fgnd_en_in;
    return this->in_foreground;
}
bool RenObj::SetInternalClipping(const bool & en_clipping_in)
{
    this->en_internal_clipping=en_clipping_in;
    return this->en_internal_clipping;
}
void RenObj::SetLighting(const GLfloat & diffuse_in, const GLfloat & specular_in, const GLfloat & shine_in)
{
    if(!_Valid(this->lighting))
        _New(this->lighting);
    factor_vec4(this->lighting->diffuse_factor,diffuse_in);
    factor_vec4(this->lighting->specular_factor,specular_in);
    this->lighting->shininess=shine_in;

    return;
}
void RenObj::SetLightingEnable(const bool & en_in)
{
    if(en_in && !_Valid(this->lighting))
        _New(this->lighting);
    if(_Valid(this->lighting))
        this->lighting->enabled=en_in;
}
GLuint RenObj::SetOccupiedSection(const GLuint & section_in)
{
    if(section_in!=_BAD)
        this->my_section=section_in;
    return this->my_section;
}
GLint RenObj::SetOrigin(const GLfloat & x_in, const GLfloat & y_in, const GLfloat & z_in)
{
    this->origin[0]=x_in;
    this->origin[1]=y_in;
    this->origin[2]=z_in;

    return _OK;
}
GLint RenObj::SetBumpMap(std::string file_in,bool repeat)
{
    GLint retval=TextureMaster::MakeTexture(file_in,repeat);
    if(retval!=_BAD)
    {
        this->bump_map=retval;
        return this->bump_map;
    }
    return _BAD;
}
GLint RenObj::SetBumpMap(GLint txr_index_in)
{
    if(txr_index_in<=TextureMaster::index && txr_index_in>-1)
    {
        this->bump_map=txr_index_in;
        return this->bump_map;
    }
    return _BAD;
}
GLint RenObj::SetTexture(const std::string & file_in,const bool & repeat)
{
    GLint retval=TextureMaster::MakeTexture(file_in,repeat);
    if(retval!=_BAD)
    {
        this->texture_sel=retval;
        return this->texture_sel;
    }
    return _BAD;
}
GLint RenObj::SetTexture(const GLint & txr_index_in)
{
    if(txr_index_in<=TextureMaster::index && txr_index_in>-1)
    {
        this->texture_sel=txr_index_in;
        return this->texture_sel;
    }
    return _BAD;
}
#pragma endregion
#pragma region Destruction
RenObj::~RenObj(void)
{
	// ~ Free Dynamically Allocated Memory
    _Delete(this->vertices);
    _Delete(this->colors);
    _Delete(this->texcoords);
    _Delete(this->normals);
    _Delete(this->lighting);
    _Delete(this->blending);
    _Delete(this->depth);
    _Delete(this->stencil);
    _Delete(this->lighting);
    _Delete(this->blending);
    _Delete(this->depth);
    _Delete(this->stencil);
    return;
}
#pragma endregion
#pragma endregion
#pragma region BlenderObj Class
#pragma region Constructors
BlenderObj::BlenderObj(std::string file_in,GLfloat scale_in)
{
    std::string contents=GetFileContents(file_in);
    if (contents=="")
        return;
    //
    this->render_id=COBJ_RID;
    this->en_internal_clipping=false;
    std::istringstream stream(contents.c_str());
    std::string line;
    std::vector<GLfloat*> vert_vec;
    std::vector<GLint> vec_indx;
    std::vector<GLfloat*> norm_vec;
    std::vector<GLint> norm_indx;
    while(std::getline(stream, line))
    {
        if (line[0]=='v' && line[1]!='n')
        {
            GLfloat * float_arr=new GLfloat[3];
            GLint pos = line.find(' ',2);
            std::string sub_str=line.substr(2,pos-2);
            float_arr[0]=(GLfloat)std::atof(sub_str.c_str());
            GLint pos2 = line.find(' ',pos+1);
            sub_str=line.substr(pos,pos2-pos);
            float_arr[1]=(GLfloat)std::atof(sub_str.c_str());
            pos = line.find('\n',pos2);
            sub_str=line.substr(pos2,pos-pos2);
            float_arr[2]=(GLfloat)std::atof(sub_str.c_str());
            vert_vec.push_back(float_arr);
        }
        else if(line[0]=='f')
        {
            GLint pos = line.find(' ',2);                                     // find first space on-or-after 3rd character
            std::string sub_str=line.substr(2,pos-2);                       // make substring starting at first digit, going to next space
            GLint pos3 = sub_str.find('/',0);                                 // find first slash in first word
            //
            std::string sub_str2=sub_str.substr(0,pos3);
            GLint temp_int=std::atoi(sub_str2.c_str());
            vec_indx.push_back(temp_int);
            //
            sub_str2=sub_str.substr(pos3+2,pos-(pos3+2));
            temp_int=std::atoi(sub_str2.c_str());
            norm_indx.push_back(temp_int);
            ////
            pos3=line.find(' ',pos+1);
            sub_str=line.substr(pos+1,pos3-(pos+1));
            pos=sub_str.find('/',0);
            //
            sub_str2=sub_str.substr(0,pos);
            temp_int=std::atoi(sub_str2.c_str());
            vec_indx.push_back(temp_int);
            //
            sub_str2=sub_str.substr(pos+2,pos3-(pos+2));
            temp_int=std::atoi(sub_str2.c_str());
            norm_indx.push_back(temp_int);
            ////
            pos = line.find(' ',pos3+1);                                     // find first space on-or-after 3rd character
            sub_str=line.substr(pos3+1,pos-(pos3+1));                       // make substring starting at first digit, going to next space
            pos3 = sub_str.find('/',0);                                 // find first slash in first word
            //
            sub_str2=sub_str.substr(0,pos3);
            temp_int=std::atoi(sub_str2.c_str());
            vec_indx.push_back(temp_int);
            //
            sub_str2=sub_str.substr(pos3+2,pos-(pos3+2));
            temp_int=std::atoi(sub_str2.c_str());
            norm_indx.push_back(temp_int);
        }
        else if(line[0]=='v' && line[1]=='n')
        {
            GLfloat * float_arr=new GLfloat[3];
            GLint pos = line.find(' ',3);
            std::string sub_str=line.substr(2,pos-2);
            float_arr[0]=(GLfloat)std::atof(sub_str.c_str());
            GLint pos2 = line.find(' ',pos+1);
            sub_str=line.substr(pos,pos2-pos);
            float_arr[1]=(GLfloat)std::atof(sub_str.c_str());
            pos = line.find('\n',pos2);
            sub_str=line.substr(pos2,pos-pos2);
            float_arr[2]=(GLfloat)std::atof(sub_str.c_str());
            norm_vec.push_back(float_arr);
        }
    }
    GLuint num_elements=vec_indx.size();
    this->lengths[0]=vec_indx.size()*3;
    this->lengths[1]=num_elements*4;
    this->lengths[2]=num_elements*2;
    this->lengths[3]=vec_indx.size()*3;
    _New(this->vertices,this->lengths[0]);
    _New(this->colors,this->lengths[1]);
    _New(this->texcoords,this->lengths[2]);
    _New(this->normals,this->lengths[3]);
    for(GLuint i=0; i<vec_indx.size(); i++)
    {
        GLfloat this_pt[3];
        memcpy(this_pt,vert_vec[vec_indx[i]-1],sizeof(GLfloat)*3);
        this->vertices[i*3]=this_pt[0]*scale_in;
        this->vertices[i*3+1]=this_pt[1]*scale_in;
        this->vertices[i*3+2]=this_pt[2]*scale_in;
    }
    for(GLuint i=0; i<this->lengths[1]; i++)
    {
        switch(i%4)
        {
        case 0:
            this->colors[i]=0.5;
            break;
        case 1:
            this->colors[i]=0.5;
            break;
        case 2:
            this->colors[i]=0.5;
            break;
        case 3:
            this->colors[i]=1.0;
            break;
        default:
            break;
        }
    }
    for(GLuint i=0; i<this->lengths[2]; i++)
    {
        this->texcoords[i]=0.0;
    }
    for(GLuint i=0; i<vec_indx.size(); i++)
    {
        GLfloat this_pt[3];
        memcpy(this_pt,norm_vec[norm_indx[i]-1],sizeof(GLfloat)*3);
        this->normals[i*3]=this_pt[0];
        this->normals[i*3+1]=this_pt[1];
        this->normals[i*3+2]=this_pt[2];
    }
    std::vector<GLfloat*>::iterator i;
    for(i=vert_vec.begin(); i!=vert_vec.end(); i++)
    {
        delete[] (*i);
    }
    for(i=norm_vec.begin(); i!=norm_vec.end(); i++)
    {
        delete[] (*i);
    }
    return;
}
BlenderObj::BlenderObj(const BlenderObj& copy_source)
    :RenObj(copy_source)
{
    return;
}
#pragma endregion
#pragma region Operators
BlenderObj& BlenderObj::operator=(const BlenderObj& copy_source)
{
    RenObj::operator=(copy_source);
    return *this;
}
#pragma endregion
#pragma region Rendering
GLint BlenderObj::Draw(GLint incoming_id, OGLUniforms& uniforms_in,OGLMatrices& matrices_in, const bool& textures_en, const bool& bumpmap_en)
{
    //--- Prepare for Rendering ---//
    // ~ Bind Data
    this->BindBufferData();                                                      // TODO: implement smarter rendering here; keep track of which BlenderObjs contain the same data and only bind the data if it is different
    this->Prepare(uniforms_in,textures_en,bumpmap_en);
    //--- Draw ---//
    // ~ Draw
    matrices_in.sTranslate(glm::vec3(this->origin[0],this->origin[1],this->origin[2]));
    glDrawArrays(GL_TRIANGLES, 0, this->lengths[0]/3);
    return _OK;
}
#pragma endregion
#pragma region Destruction
BlenderObj::~BlenderObj(void)
{
    return;
}
#pragma endregion
#pragma endregion
#pragma region Cube Class
#pragma region Cube Pre-Formed Data
GLfloat texCube[] = {
    // -Bottom- (-Z)
    //Texture
    //u    v
    0.0, 0.0, // tri1:pt1
	1.0, 0.0, // tri1:pt2 
	1.0, 1.0, // tri1:pt3
	1.0, 1.0, // tri2:pt1
	0.0, 1.0, // tri2:pt2
	0.0, 0.0, // tri2:pt3
    // -Top- (+Z)
    //Texture
    //u    v
	1.0, 0.0,
	0.0, 0.0,
	0.0, 1.0,
	0.0, 1.0,
	1.0, 1.0,
	1.0, 0.0,
    // -Left- (-X)
    //Texture
    //u    v
	0.0, 0.0,
	0.0, 1.0,
	1.0, 1.0,
	1.0, 1.0,
	1.0, 0.0,
	0.0, 0.0,
    // -Right- (+X)
    //Texture
    //u    v
	1.0, 0.0,
	1.0, 1.0,
	0.0, 1.0,
	0.0, 1.0,
	0.0, 0.0,
	1.0, 0.0,
    // -Back- (-Y)
    //Texture
    //u    v
	0.0, 1.0,
	1.0, 1.0,
	1.0, 0.0,
	1.0, 0.0,
	0.0, 0.0,
	0.0, 1.0,
    // -Front- (+Y)
    //Texture
    //u    v
	1.0, 1.0,
	0.0, 1.0,
	0.0, 0.0,
	0.0, 0.0,
	1.0, 0.0,
	1.0, 1.0,
};
GLfloat vertCube[] = {
    // -Bottom- (-Z)
    //  Position
    //x    y    z  
    -1.0,-1.0,-1.0,
     1.0,-1.0,-1.0,
     1.0, 1.0,-1.0,
     1.0, 1.0,-1.0,
    -1.0, 1.0,-1.0,
    -1.0,-1.0,-1.0,
    // -Top- (+Z)
    //  Position
    //x    y    z 
    -1.0,-1.0, 1.0,
     1.0,-1.0, 1.0,
     1.0, 1.0, 1.0,
     1.0, 1.0, 1.0,
    -1.0, 1.0, 1.0,
    -1.0,-1.0, 1.0,
    // -Left- (-X)
    //  Position
    //x    y    z 
    -1.0, 1.0, 1.0,
    -1.0, 1.0,-1.0,
    -1.0,-1.0,-1.0,
    -1.0,-1.0,-1.0,
    -1.0,-1.0, 1.0,
    -1.0, 1.0, 1.0,
    // -Right- (+X)
    //  Position
    //x    y    z  
     1.0, 1.0, 1.0,
     1.0, 1.0,-1.0,
     1.0,-1.0,-1.0,
     1.0,-1.0,-1.0,
     1.0,-1.0, 1.0,
     1.0, 1.0, 1.0,
    // -Back- (-Y)
    //  Position  
    //x    y    z 
    -1.0,-1.0,-1.0,
     1.0,-1.0,-1.0,
     1.0,-1.0, 1.0,
     1.0,-1.0, 1.0,
    -1.0,-1.0, 1.0,
    -1.0,-1.0,-1.0,
    // -Front- (+Y)
    //  Position  
    //x    y    z 
    -1.0, 1.0,-1.0,
     1.0, 1.0,-1.0,
     1.0, 1.0, 1.0,
     1.0, 1.0, 1.0,
    -1.0, 1.0, 1.0,
    -1.0, 1.0,-1.0,
};
GLfloat normCube[] = {
    // -Top- (-Z)
    //  Position
    //x    y    z  
     0.0, 0.0, -1.0,
     0.0, 0.0, -1.0,
     0.0, 0.0, -1.0,
     0.0, 0.0, -1.0,
     0.0, 0.0, -1.0,
     0.0, 0.0, -1.0,
    // -Bottom- (+Z)
    //  Position
    //x    y    z 
     0.0, 0.0, 1.0,
     0.0, 0.0, 1.0,
     0.0, 0.0, 1.0,
     0.0, 0.0, 1.0,
     0.0, 0.0, 1.0,
     0.0, 0.0, 1.0,
    // -Left- (-X)
    //  Position
     -1.0, 0.0, 0.0,
     -1.0, 0.0, 0.0,
     -1.0, 0.0, 0.0,
     -1.0, 0.0, 0.0,
     -1.0, 0.0, 0.0,
     -1.0, 0.0, 0.0,
    // -Right- (+X)
    //  Position
    //x    y    z  
     1.0, 0.0, 0.0,
     1.0, 0.0, 0.0,
     1.0, 0.0, 0.0,
     1.0, 0.0, 0.0,
     1.0, 0.0, 0.0,
     1.0, 0.0, 0.0,
    // -Back- (-Y)
    //  Position  
    //x    y    z 
     0.0, -1.0, 0.0,
     0.0, -1.0, 0.0,
     0.0, -1.0, 0.0,
     0.0, -1.0, 0.0,
     0.0, -1.0, 0.0,
     0.0, -1.0, 0.0,
    // -Front- (+Y)
    //  Position  
    //x    y    z 
     0.0, 1.0, 0.0,
     0.0, 1.0, 0.0,
     0.0, 1.0, 0.0,
     0.0, 1.0, 0.0,
     0.0, 1.0, 0.0,
     0.0, 1.0, 0.0
};
#pragma endregion
// ~ Cube Constructors
Cube::Cube(GLfloat diameter_in)
{
	//--- Set Cube Variables ---//
    this->diameter=diameter_in;
	//--- Set RenObj Variables ---//
	// ~ Allocate Memory
    _New(this->vertices,CUBE_V_3D); this->lengths[0]=CUBE_V_3D;
    _New(this->colors,CUBE_C_3D); this->lengths[1]=CUBE_C_3D;
    _New(this->texcoords,CUBE_T_3D); this->lengths[2]=CUBE_T_3D;
    _New(this->normals,CUBE_N_3D); this->lengths[3]=CUBE_N_3D;
	// ~ Populate Vertice Data
    for(GLint i=0; i<CUBE_C_3D; i++)
    {
        if(i<CUBE_V_3D)
        {
            this->vertices[i]=(GLfloat)(vertCube[i]*(diameter/2.0));
            this->normals[i]=normCube[i];
        }
		this->colors[i]=1.0;
    }
    memcpy(this->texcoords,texCube,sizeof(GLfloat)*CUBE_T_3D);
	// ~ Rendering Properties
	this->render_id=CUBE_RID;
    this->lighting=new MaterialLighting();
    this->lighting->enabled=true;
    this->lighting->shininess=100;
    factor_vec4(this->lighting->diffuse_factor,0.6f);
    factor_vec4(this->lighting->specular_factor,0.25);
    // ~ Selection
    this->selection_radius=(GLfloat)(diameter_in*1.05);
    return;
}

Cube::Cube(const Cube & copy_source)
	:RenObj(copy_source)
{
    // ~ Physical Properties
	this->diameter=copy_source.diameter;

	return;
}
// ~ Cube Operator(s)
Cube & Cube::operator=(const Cube & copy_source)
{
	// ~ Call Base Class Assignment Operator
	RenObj::operator=(copy_source);
	// ~ Physical Properties
	this->diameter=copy_source.diameter;

	return *this;
}
// ~ Cube Rendering
GLint Cube::Draw(GLint incoming_id, OGLUniforms& uniforms_in,OGLMatrices& matrices_in, const bool& textures_en, const bool& bumpmap_en)
{
	//--- Prepare for Rendering ---//
	if(incoming_id!=this->render_id) // only execute this section if last rendered item was different
	{
		// ~ Bind Data

	}
	this->BindBufferData();
    this->Prepare(uniforms_in,textures_en,bumpmap_en);
	//--- Draw Cube ---//
	// ~ Draw Real Cube
    matrices_in.sTranslate(glm::vec3(this->origin[0],this->origin[1],this->origin[2]));
	glDrawArrays(GL_TRIANGLES, 0, this->lengths[0]/3);
	// ~ Draw Reflection
    /*
	if(en_reflection)
	{
        glEnable( GL_STENCIL_TEST );
            glStencilFunc( GL_EQUAL, 1, 0xFF );
		    glStencilMask( 0x00 );
			glm::mat4 temp_scale=glm::scale(info_in->mat_scale,glm::vec3(1,1,-1));                   // scale -z, flipping z
			glm::mat4 temp_trans=glm::translate(info_in->mat_trans,glm::vec3(0,0,-this->diameter)); // translate down
            // ~ Update SRT Attribute
            glm::mat4 temp_srt=temp_scale*info_in->mat_rotate*temp_trans;
            Scene * local_scene_ptr=(Scene*)info_in->scene_ptr;
            local_scene_ptr->UpdateSRT(&temp_srt);
            // ~ Draw Dim, Reflected Cube
			glUniform3f(info_in->uni_ovclr, 0.3f, 0.3f, 0.3f );
               glDrawArrays(GL_TRIANGLES, 0, 36);                              // draw 36 starting at index 0 [cube]
			glUniform3f(info_in->uni_ovclr, 1.0f, 1.0f, 1.0f );
        glDisable( GL_STENCIL_TEST );
        // ~ Rollback SRT Attribute
        local_scene_ptr->UpdateSRT(&(info_in->mat_srt));
    }
    */
	return _OK;
}
// ~ Cube Destructor
Cube::~Cube(void)
{
    return;
}
#pragma endregion
#pragma region Floor Pre-Formed Data
GLfloat vertFloor[]={
    // -Floor-
    // Position
    -1.0,-1.0,-1.0,
     1.0,-1.0,-1.0,
    -1.0, 1.0,-1.0,
     1.0, 1.0,-1.0
};
GLfloat texFloor[]={
    // -Floor-
    //Texture
    0.0, 0.0, // tri1:pt1
    1.0, 0.0, // tri1:pt2 
    0.0, 1.0, // tri2:pt2
    1.0, 1.0 // tri1:pt3
};
GLfloat normFloor[]={
    0.0,0.0,1.0,
    0.0,0.0,1.0,
    0.0,0.0,1.0,
    0.0,0.0,1.0
};
#pragma endregion
// ~ Floor Constructor(s)
Floor::Floor(void)
{
	//--- Floor Variables ---//
	// ~ Set Variables
	this->length=(GLfloat)FLOOR_DEF_LENGTH, this->width=(GLfloat)FLOOR_DEF_WIDTH;

	//--- RenObj Variables ---//
	// ~ Allocate Memory
    _New(this->vertices,FLOOR_V_3D); this->lengths[0]=FLOOR_V_3D;
    _New(this->colors,FLOOR_C_3D); this->lengths[1]=FLOOR_C_3D;
    _New(this->texcoords,FLOOR_T_3D); this->lengths[2]=FLOOR_T_3D;
    _New(this->normals,FLOOR_N_3D); this->lengths[3]=FLOOR_N_3D;
	// ~ Populate with Data
    for(GLint i=0; i<FLOOR_C_3D; i++)
    {
        if(i<FLOOR_T_3D)
            this->texcoords[i]=texFloor[i];
        if(i<FLOOR_V_3D)
        {
			if(i%3==0)
				this->vertices[i]=vertFloor[i]*this->length;
			else if(i%3==1)
				this->vertices[i]=vertFloor[i]*this->width;
			else
				this->vertices[i]=vertFloor[i];
            this->normals[i]=normFloor[i];
        }
        if((i+1)%4==0)
		    this->colors[i]=1.0f;
        else
            this->colors[i]=0.1f;
    }
    this->SetOrigin(0.0,0.0,0.0);
	// ~ Remaining RenObj Variables
	this->texture_sel=_BAD;
	this->render_id=FLOOR_RID;
    this->en_internal_clipping=false;
    this->lighting=new MaterialLighting();
    this->lighting->enabled=true;
    this->lighting->shininess=800;
    factor_vec4(this->lighting->diffuse_factor,0.8f);
    factor_vec4(this->lighting->specular_factor,1.2f);
    // Enable Stencil
    /*
    this->en_stencil=true;
    this->stencil_mask=0xFF;
    this->stencil_func[0]=GL_ALWAYS;
    this->stencil_func[1]=1;
    this->stencil_func[2]=0xFF;
    this->stencil_op[0]=GL_KEEP;
    this->stencil_op[1]=GL_KEEP;
    this->stencil_op[2]=GL_REPLACE;
    */
	return;
}
Floor::Floor(GLfloat * orig_in, GLfloat length_in, GLfloat width_in, GLfloat * color_in)
{
	//--- Floor Variables ---//
	// ~ Set Variables
	this->length=length_in; this->width=width_in;

	//--- RenObj Variables ---//
	// ~ Allocate Memory
    _New(this->vertices,FLOOR_V_3D); this->lengths[0]=FLOOR_V_3D;
    _New(this->colors,FLOOR_C_3D); this->lengths[1]=FLOOR_C_3D;
    _New(this->texcoords,FLOOR_T_3D); this->lengths[2]=FLOOR_T_3D;
    _New(this->normals,FLOOR_N_3D); this->lengths[3]=FLOOR_N_3D;
	// ~ Populate with Data
    for(GLint i=0; i<FLOOR_C_3D; i++)
    {
        if(i<FLOOR_T_3D)
            this->texcoords[i]=texFloor[i];
        if(i<FLOOR_V_3D)
        {
			if(i%3==0)
				this->vertices[i]=vertFloor[i]*length;
			else if(i%3==1)
				this->vertices[i]=vertFloor[i]*width;
			else
				this->vertices[i]=vertFloor[i];
            this->normals[i]=normFloor[i];
        }
		this->colors[i]=color_in[i%4];
    }
    memcpy(this->origin,orig_in,sizeof(GLfloat)*3);
	// ~ Remaining RenObj Variables
	this->texture_sel=_BAD;
	this->render_id=FLOOR_RID;
    this->en_internal_clipping=false;
    this->lighting=new MaterialLighting();
    this->lighting->enabled=true;
    this->lighting->shininess=800;
    factor_vec4(this->lighting->diffuse_factor,0.8f);
    factor_vec4(this->lighting->specular_factor,1.2f);
    // Enable Stencil
    /*
    this->en_stencil=true;
    this->stencil_mask=0xFF;
    this->stencil_func[0]=GL_ALWAYS;
    this->stencil_func[1]=1;
    this->stencil_func[2]=0xFF;
    this->stencil_op[0]=GL_KEEP;
    this->stencil_op[1]=GL_KEEP;
    this->stencil_op[2]=GL_REPLACE;
    */
	return;
}
Floor::Floor(const Floor & copy_source)
	:RenObj(copy_source)
{
	this->length=copy_source.length;
	this->width=copy_source.width;

	return;
}
// ~ Floor Operator(s)
Floor& Floor::operator=(const Floor & copy_source)
{
	RenObj::operator=(copy_source);
	this->length=copy_source.length;
	this->width=copy_source.width;
	return *this;
}
// ~ Floor Rendering
GLint Floor::Draw(GLint incoming_id, OGLUniforms& uniforms_in,OGLMatrices& matrices_in, const bool& textures_en, const bool& bumpmap_en)
{
    if(incoming_id!=render_id)
    {
        this->BindBufferData();
        this->Prepare(uniforms_in,textures_en,bumpmap_en);
    }
    glClear(GL_STENCIL_BUFFER_BIT);
    // Draw
    matrices_in.sTranslate(glm::vec3(this->origin[0],this->origin[1],this->origin[2]));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, this->lengths[0]/3);
	return _OK;
}
// ~ Floor Destructor
Floor::~Floor(void)
{

	return;
}
//--- RenObj : TextOverlay ---//
#pragma region TextOverlay Pre-Formed Data
GLfloat TextOverlay::vert_letter[]={
    // Position
    //x   y   z
      0.0,  0.0, 0.0,
      1.0,  0.0, 0.0,
      1.0,  1.5, 0.0,
      1.0,  1.5, 0.0,
      0.0,  1.5, 0.0,
      0.0,  0.0, 0.0
};
GLfloat TextOverlay::txtur_letter[]={
    // -Bottom- (-Z)
    //Texture
    //u    v
    0.0, 0.0, // tri1:pt1
    (GLfloat)(1.0/12.0), 0.0, // tri1:pt2 
    (GLfloat)(1.0/12.0), (GLfloat)(1.0/11.0), // tri1:pt3
    (GLfloat)(1.0/12.0), (GLfloat)(1.0/11.0), // tri2:pt1
    0.0, (GLfloat)(1.0/11.0), // tri2:pt2
    0.0, 0.0, // tri2:pt3
};
#pragma endregion
// ~ TextOverlay Constructor(s)
TextOverlay::TextOverlay(GLfloat size_in)
{
    //--- RenObj Variables ---//
	// ~ Render Properties
	this->render_id=TEXTOVER_RID;
    this->in_foreground=true;
    // ~ Blending
    this->depth->enabled=false;
    _New(this->blending);
    this->blending->function[0]=GL_SRC_ALPHA;
    this->blending->function[1]=GL_ONE_MINUS_SRC_ALPHA;
    // ~ Rendering Flags
    this->en_internal_clipping=false;
    this->lighting=new MaterialLighting();
    //--- TextOverlay Variables ---//
    // ~ Private
    this->size=size_in;
    if(this->size<25.0)
        this->lighting->black_only=true;
    this->num_chars=0;
    _Init(this->ascii_buf);
	return;
}
TextOverlay::TextOverlay(const TextOverlay & textoverlay_in):
	RenObj(textoverlay_in)
{
    // ~ Private
    this->size=textoverlay_in.size;
    this->num_chars=textoverlay_in.num_chars;
    if(_Valid(textoverlay_in.ascii_buf))
    {
        _New(this->ascii_buf,this->num_chars);
        memcpy(this->ascii_buf,textoverlay_in.ascii_buf,sizeof(GLuint)*this->num_chars);
    }
    else
        _Init(this->ascii_buf);

	return;
}
// ~ TextOverlay Operator(s)
TextOverlay & TextOverlay::operator=(const TextOverlay & textoverlay_in)
{
	RenObj::operator=(textoverlay_in);

    this->size=textoverlay_in.size;
    _Delete(this->ascii_buf);
    this->num_chars=textoverlay_in.num_chars;
    if(_Valid(textoverlay_in.ascii_buf))
    {
        _New(this->ascii_buf,this->num_chars);
        memcpy(this->ascii_buf,textoverlay_in.ascii_buf,sizeof(GLuint)*this->num_chars);
    }

	return *this;
}
// ~ TextOverlay Getter(s)/Setter(s)
GLint TextOverlay::SetText(const GLchar * format, ...)
{
	GLchar buf[64]; GLchar * p = buf;
    GLuint bufout[128];
    GLuint new_char_count=0;
	// ~ Parse Arguments
    va_list args; GLint n;
    va_start(args, format); // start going through args
    n = _vsnprintf(p, sizeof buf - 3,format, args); // buf-3 is room for CR/LF/NUL
    va_end(args);
	p += (n < 0) ? sizeof buf - 1 : n;  // direct 'p' to point to end of array
    while (p>buf && isspace(p[-1]))
        *--p = '\0';
	// ~ Find Valid Chars
    for(GLint i=0; i<sizeof(buf)/sizeof(GLchar); i++)
    {
        if((GLuint)buf[i]>8 && (GLuint)buf[i]<127)
        {
            bufout[new_char_count++]=(GLint)buf[i];;
        }
        else
            break;
	}
	if(new_char_count==0)
		return _BAD;
    // ~ Get Existing String
    if(this->num_chars>0)
    {
        GLchar * char_buf;
        _Init(char_buf);
        _New(char_buf,this->num_chars);
        for(GLuint i=0; i<this->num_chars; i++)
            char_buf[i]=(GLchar)(this->ascii_buf[i]);
        std::string old_str(char_buf);
        _Delete(char_buf);
        _New(char_buf,new_char_count);
        for(GLuint i=0; i<new_char_count; i++)
            char_buf[i]=(GLchar)bufout[i];
        std::string new_str(char_buf);
        _Delete(char_buf);
        if (old_str.find(new_str,0)!=_BAD)
        {
            return _OK;
        }
    }
    // ~ Clear Buffer
    this->num_chars=new_char_count;
    _Delete(this->ascii_buf);
    _New(this->ascii_buf,this->num_chars);
    memcpy(this->ascii_buf,bufout,sizeof(GLuint)*this->num_chars);
    // ~ Set Vertices & Colors
	this->lengths[0]=18*this->num_chars;
    _Delete(this->vertices);
    _New(this->vertices,this->lengths[0]);
	this->lengths[1]=24*this->num_chars;
    _Delete(this->colors);
    _New(this->colors,this->lengths[1]);
	for(GLuint i=0; i<this->lengths[1]; i++)
	{
		if(i<this->lengths[0])
			this->vertices[i]=this->vert_letter[i-18*(GLint)(i/18)]*this->size;
		this->colors[i]=1.0;
	}
    // ~ Texture Coordinates
	this->lengths[2]=12*this->num_chars;
    _Delete(this->texcoords);
    _New(this->texcoords,this->lengths[2]);
    for(GLuint i=0; i<this->num_chars; i++)
    {
        for(GLint j=0; j<12; j++)
        {
            if(!(j%2) || !(j))
                this->texcoords[j+i*12]=this->txtur_letter[j]+(1.0f/12.0f)*(GLfloat)((this->ascii_buf[i])-12.0f*(GLint)(this->ascii_buf[i]/12.0f));
            else if(j%2)
                this->texcoords[j+i*12]=this->txtur_letter[j]+(1.0f/11.0f)*(GLfloat)((GLint)(this->ascii_buf[i]/12.0f));
        }
    }
    this->lengths[3]=this->lengths[0];
    _Delete(this->normals);
    _New(this->normals,this->lengths[3]);
    for(GLuint i=0; i<this->lengths[3]; i++)
        this->normals[i]=0.0;
	return _OK;
}
void TextOverlay::SetTextSize(GLfloat size_in)
{
    if(size_in<=0.0 || size_in==this->size)
        return;
    if(this->lengths[0]>0)
    {
        for(GLuint i=0; i<this->lengths[0]; i++)
        {
            this->vertices[i]/=this->size;
            this->vertices[i]*=size_in;
        }
    }
    this->size=size_in;
    if(this->size<25.0)
        this->lighting->black_only=true;
    else
        this->lighting->black_only=false;
    return;
}
// ~ TextOverlay Rendering
GLint TextOverlay::PushVerticalCompensation(GLuint char_in, glm::mat4 * temp_trans_in,OGLMatrices& matrices_in)
{
    if((char_in<'g' || char_in>'y') && char_in!='@')
        return _BAD;
    glm::mat4 temp_vert_shift;
    switch(char_in)
    {
        case 'j':
            temp_vert_shift=glm::translate(*temp_trans_in,glm::vec3(0.0,this->size/8.0,0.0));
            matrices_in.UpdateSRT(&temp_vert_shift);
            break;
        case 'g':
        case 'p':
        case 'q':
        case 'y':
        case '@':
            temp_vert_shift=glm::translate(*temp_trans_in,glm::vec3(0.0,this->size/4.0,0.0));
            matrices_in.UpdateSRT(&temp_vert_shift);
            break;
        default:
            return _BAD;
    }
    return _OK;
}
GLint TextOverlay::Draw(GLint incoming_id, OGLUniforms& uniforms_in,OGLMatrices& matrices_in, const bool& textures_en, const bool& bumpmap_en)
{
    // ~ Bind Data
	this->BindBufferData();
    this->Prepare(uniforms_in, true, bumpmap_en);
    glDisable(GL_CULL_FACE);
	GLuint num_characters=this->lengths[0]/18;
    matrices_in.sTranslate(glm::vec3(this->origin[0],this->origin[1],this->origin[2]));
    glm::mat4 temp_translation=matrices_in.translation;
    GLuint num_rows=0;
	for(GLuint i=0; i<num_characters; i++)
	{
        if(this->ascii_buf[i]=='\n')
        {
            temp_translation = glm::translate(glm::mat4(1.0),glm::vec3(this->origin[0],this->origin[1]+(++num_rows)*(1.5*this->size),0.0)); // translate
            matrices_in.UpdateSRT(&temp_translation);
        }
        else if(this->ascii_buf[i]>31 && this->ascii_buf[i]<127) // must be >= ' ' and <= '~'
        {
            this->PushVerticalCompensation(this->ascii_buf[i],&temp_translation,matrices_in);
		    glDrawArrays(GL_TRIANGLES,i*6,6);
		    temp_translation = glm::translate(temp_translation,glm::vec3(this->size,0.0,0.0)); // translate
            // ~ Update SRT Attribute
            matrices_in.UpdateSRT(&temp_translation);
        }
	}
    // ~ Change SRT Attribute to Previous Value
    glEnable(GL_CULL_FACE);
    matrices_in.UpdateSRT(&(matrices_in.translation));
	return _OK;
}
// ~ TextOverlay Destructor
TextOverlay::~TextOverlay(void)
{
    _Delete(this->ascii_buf);
	return;
}
//--- RenObj : LineStrip ---//
// ~ LineStrip Constructor(s)
LineStrip::LineStrip(void)
{
    //--- Set RenObj Variables ---//
    // ~ Render Variables
    this->render_id=LINE_RID;
	this->texture_sel=_BAD;
    this->en_internal_clipping=false;
    //--- Dynamic Allocation ---//
	// ~ Allocate Memory
    this->num_chunks=0;
    return;
}
GLint LineStrip::AddInstance(GLfloat start_pt[3],GLfloat end_pt[3])
{
    GLfloat all_points[6]={start_pt[0],start_pt[1],start_pt[2],end_pt[0],end_pt[1],end_pt[2]};
    GLfloat * old_values;
    // Vertices
    bool update_all=false;
    if(this->lengths[0]+6>=(GLuint)(this->num_chunks*LINE_ARR_CHUNK*3))
    {
        _Init(old_values);
        _New(old_values,LINE_ARR_CHUNK*3*(this->num_chunks+1));
        memcpy(old_values,this->vertices,this->lengths[0]*sizeof(GLfloat));
        _Delete(this->vertices);
        this->vertices=old_values;
        update_all=true;
    }
    memcpy(&this->vertices[this->lengths[0]],all_points,sizeof(GLfloat)*6);
    this->lengths[0]+=6;
    // Colors
    GLfloat new_vals[8]={0.1,0.8,0.5,1.0,0.1,0.8,0.5,1.0};
    if(this->lengths[1]+8>=(GLuint)(this->num_chunks*LINE_ARR_CHUNK*4) || update_all)
    {
        _Init(old_values);
        _New(old_values,LINE_ARR_CHUNK*4*(this->num_chunks+1));
        memcpy(old_values,this->colors,this->lengths[1]*sizeof(GLfloat));
        _Delete(this->colors);
        this->colors=old_values;
        update_all=true;
    }
    memcpy(&this->colors[this->lengths[1]],new_vals,8*sizeof(GLfloat));
    this->lengths[1]+=8;
    // Texcoords
    if(this->lengths[2]+4>=(GLuint)(this->num_chunks*LINE_ARR_CHUNK*2) || update_all)
    {
        _Init(old_values);
        _New(old_values,LINE_ARR_CHUNK*2*(this->num_chunks+1));
        memcpy(old_values,this->texcoords,this->lengths[2]*sizeof(GLfloat));
        _Delete(this->texcoords);
        this->texcoords=old_values;
        update_all=true;
    }
    this->texcoords[this->lengths[2]]=0.0; this->texcoords[this->lengths[2]+1]=0.0;
    this->texcoords[this->lengths[2]+2]=0.0; this->texcoords[this->lengths[2]+3]=0.0;
    this->lengths[2]+=4;
    // Normals
    if(this->lengths[3]+6>=(GLuint)(this->num_chunks*LINE_ARR_CHUNK*3) || update_all)
    {
        _Init(old_values);
        _New(old_values,LINE_ARR_CHUNK*3*(this->num_chunks+1));
        memcpy(old_values,this->normals,this->lengths[3]*sizeof(GLfloat));
        _Delete(this->normals);
        this->normals=old_values;
        this->num_chunks++;
    }
    for(GLint i=0; i<6; i++)
        this->normals[this->lengths[3]+i]=0.0;
    this->lengths[3]+=6;
    //
    return _OK;
}
LineStrip::LineStrip(const LineStrip & copy_source)
    :RenObj(copy_source)
{
    this->num_chunks=copy_source.num_chunks;
    return;
}
// ~ LineStrip Operator(s)
LineStrip & LineStrip::operator=(const LineStrip & copy_source)
{
	RenObj::operator=(copy_source);
    this->num_chunks=copy_source.num_chunks;
    return *this;
}
// ~ LineStrip Rendering
GLint LineStrip::Draw(GLint incoming_id, OGLUniforms& uniforms_in,OGLMatrices& matrices_in, const bool& textures_en, const bool& bumpmap_en)
{
    this->BindBufferData();
    this->Prepare(uniforms_in,textures_en, bumpmap_en);
    matrices_in.sTranslate(glm::vec3(0.0f,0.0f,0.0f));
    glDrawArrays(GL_LINES,0,this->lengths[0]/3);
    matrices_in.UpdateSRT(&(matrices_in.translation_rotation_scale));
    return _OK;
}

// ~ LineStrip Destructor
LineStrip::~LineStrip(void)
{
    return;
}
//--- RenObj : BlockArray ---//
// ~ Constructor(s)
BlockArray::BlockArray(void)
{
    this->en_internal_clipping=true;
    this->render_id=BARR_RID;
    this->lighting=new MaterialLighting();
    this->lighting->enabled=true;
    this->lighting->shininess=200.0f;
    factor_vec4(this->lighting->diffuse_factor,0.55f);
    factor_vec4(this->lighting->specular_factor,0.9f);
    _Init(this->element_indices);
    this->element_count=0;
    memset(this->render_origin,0,3*sizeof(GLfloat));
    return;
}

BlockArray::BlockArray(const BlockArray & copy_source)
    :RenObj(copy_source)
{
    this->normal_lines=copy_source.normal_lines;
    this->normal_starts=copy_source.normal_starts;
    this->element_count=copy_source.element_count;
    _Init(this->element_indices);
    _New(this->element_indices,this->element_count);
    if(this->element_count>0)
    {
        memcpy(this->element_indices,copy_source.element_indices,this->element_count*sizeof(GLuint));
    }
    memcpy(this->render_origin,copy_source.render_origin,3*sizeof(GLfloat));
    return;
}
BlockArray& BlockArray::operator=(const BlockArray & copy_source)
{
	RenObj::operator=(copy_source);
    this->normal_lines=copy_source.normal_lines;
    this->normal_starts=copy_source.normal_starts;
    this->element_count=copy_source.element_count;
    _Delete(this->element_indices);
    if(this->element_count>0)
    {
        _New(this->element_indices,this->element_count);
        memcpy(this->element_indices,copy_source.element_indices,this->element_count*sizeof(GLuint));
    }
    memcpy(this->render_origin,copy_source.render_origin,3*sizeof(GLfloat));
    return *this;
}
GLint BlockArray::UpdateVertices(const std::vector<glm::vec3> & vertices_in,const std::vector<glm::vec3> & normals_in, const std::vector<GLuint> & elements_in, const std::vector<glm::vec4> & colors_in, const GLfloat render_origin_in[3],const GLfloat center_in[3], const GLfloat radius_in)
{
    memcpy(this->origin,center_in,3*sizeof(GLfloat));
    memcpy(this->render_origin,render_origin_in,3*sizeof(GLfloat));
    // Vertices
    this->lengths[0]=vertices_in.size()*3;
    _Delete(this->vertices);
    if(_New(this->vertices,this->lengths[0]))
        memcpy(this->vertices,vertices_in.data(),this->lengths[0]*sizeof(GLfloat));
    //
    this->element_count=elements_in.size();
    _Delete(this->element_indices);
    if(_New(this->element_indices,this->element_count))
        memcpy(this->element_indices,&elements_in[0],this->element_count*sizeof(GLuint));
    // Colors
    this->lengths[1]=colors_in.size()*4;
    _Delete(this->colors);
    if(_New(this->colors,this->lengths[1]))
        memcpy(this->colors,colors_in.data(),this->lengths[1]*sizeof(GLfloat));
    // Tex Coords
    this->lengths[2]=vertices_in.size()*2;
    _Delete(this->texcoords);
    if(_New(this->texcoords,this->lengths[2]))
        memset(this->texcoords,0,this->lengths[2]);
    // Normals
    this->lengths[3]=normals_in.size()*3;
    _Delete(this->normals);
    if(_New(this->normals,this->lengths[3]))
        memcpy(this->normals,normals_in.data(),this->lengths[3]*sizeof(GLfloat));
    this->normal_lines.clear();
    this->normal_starts.clear();
    return _OK;
}
GLint BlockArray::Draw(GLint incoming_id, OGLUniforms& info_in,OGLMatrices& matrices_in, const bool& textures_en, const bool& bumpmap_en)
{

    this->BindBufferData();
    this->Prepare(info_in,textures_en, bumpmap_en);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,RenObj::ebo(0));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,this->element_count*sizeof(GLuint),this->element_indices,GL_DYNAMIC_DRAW);
    //for(int i=0; i<this->lengths[0]/3; i++)
    //    dprint("VERTEX #%i: %f,%f,%f",i,this->vertices[i*3],this->vertices[i*3+1],this->vertices[i*3+2]);
    matrices_in.sTranslate(glm::vec3(this->render_origin[0],this->render_origin[1],this->render_origin[2]));
    //glDrawArrays(GL_TRIANGLES, 0, this->lengths[0]/3);
    glDrawElements(GL_TRIANGLES,this->element_count,GL_UNSIGNED_INT,0);
    return _OK;
}
LineStrip BlockArray::GetNormals(void)
{
    std::vector<glm::vec3>::iterator iter;
    GLfloat floata[3];
    GLfloat floatb[3];
    LineStrip normlines;
    GLuint local_counter=0;
    for(iter=this->normal_lines.begin(); iter!=this->normal_lines.end(); iter++)
    {
        floata[0]=this->origin[0]+this->normal_starts[local_counter].x;
        floata[1]=this->origin[1]+this->normal_starts[local_counter].y;
        floata[2]=this->origin[2]+this->normal_starts[local_counter].z;
        floatb[0]=floata[0]+(*iter).x*BARR_NORMAL_LEN; floatb[1]=floata[1]+(*iter).y*BARR_NORMAL_LEN; floatb[2]=floata[2]+(*iter).z*BARR_NORMAL_LEN;
        normlines.AddInstance(floata,floatb);
        if(local_counter%1000==0)
            dprint("Processing normal vector for voxel #%u\n",local_counter);
        local_counter++;
    }
    return normlines;
}
BlockArray::~BlockArray(void)
{
    _Delete(this->element_indices);
    return;
}
//--- RenObj : Group ---//
// ~ Group Constructor(s)
Group::Group(ptrRenObj renobj_seed)
{
    // ~ Allocate Memory
    GLuint local_largest=0;
    for(GLint i=0; i<4; i++)
    {
        this->lengths[i]=renobj_seed->lengths[i];
        if(this->lengths[i]>local_largest)
            local_largest=lengths[i];
    }
    _New(this->vertices,this->lengths[0]);
    _New(this->colors,this->lengths[1]);
    _New(this->texcoords,this->lengths[2]);
    _New(this->normals,this->lengths[3]);
    // ~ Populate Data
    for(GLuint i=0; i<local_largest; i++)
    {
        if(i<lengths[0])
            this->vertices[i]=renobj_seed->vertices[i];
        if(i<lengths[1])
            this->colors[i]=renobj_seed->colors[i];
        if(i<lengths[2])
            this->texcoords[i]=renobj_seed->texcoords[i];
        if(i<lengths[3])
            this->normals[i]=renobj_seed->normals[i];
    }
    //
    this->instance_count=0;
    this->texture_sel=renobj_seed->texture_sel;
    this->render_id=renobj_seed->render_id;
    // ~ Lighting
    if(_Valid(renobj_seed->lighting))
        this->lighting=new MaterialLighting(*(renobj_seed->lighting));
    if(_Valid(renobj_seed->blending))
        this->blending=new BlendFunction(*(renobj_seed->blending));
    if(_Valid(renobj_seed->stencil))
        this->stencil=new StencilFunction(*(renobj_seed->stencil));
    if(_Valid(renobj_seed->depth))
        this->depth=new DepthFunction(*(renobj_seed->depth));
    _Init(this->gp_srt);

    this->min_x_rot=-45.0f; this->max_x_rot=45.0f;
    this->min_y_rot=-45.0f; this->max_x_rot=45.0f;
    this->min_z_rot=-45.0f; this->max_x_rot=45.0f;

    this->min_x_scale=0.1f; this->max_x_scale=2.0f;
    this->min_y_scale=0.1f; this->max_y_scale=2.0f;
    this->min_z_scale=0.1f; this->max_z_scale=2.0f;

    this->min_x_trans=-100.0f; this->max_x_trans=100.0f;
    this->min_y_trans=-100.0f; this->max_y_trans=100.0f;
    this->min_z_trans=0.0f; this->max_z_trans=200.0f;

    _Init(this->gp_scene_ptr);

    return;
}
Group::Group(const Group & group_in)
	:RenObj(group_in)
{
    this->instance_count=group_in.instance_count;
    std::vector<glm::mat4> temp_mat_vec=group_in.gp_scale;
    this->gp_scale=temp_mat_vec;
    temp_mat_vec=group_in.gp_rotate;
    this->gp_rotate=temp_mat_vec;
    std::vector<glm::vec3> temp_vec_vec=group_in.gp_trans;
    this->gp_trans=temp_vec_vec;
    _Init(this->gp_srt);
    _New(this->gp_srt,this->instance_count);
    for(long int i=0; i<this->instance_count; i++)
    {
        this->gp_srt[i]=group_in.gp_srt[i];
    }

    this->min_x_rot=group_in.min_x_rot; this->max_x_rot=group_in.max_x_rot;
    this->min_y_rot=group_in.min_y_rot; this->max_x_rot=group_in.max_y_rot;
    this->min_z_rot=group_in.min_z_rot; this->max_x_rot=group_in.max_z_rot;

    this->min_x_scale=group_in.min_x_scale; this->max_x_scale=group_in.max_x_scale;
    this->min_y_scale=group_in.min_y_scale; this->max_y_scale=group_in.max_y_scale;
    this->min_z_scale=group_in.min_z_scale; this->max_z_scale=group_in.max_z_scale;

    this->min_x_trans=group_in.min_x_trans; this->max_x_trans=group_in.max_x_trans;
    this->min_y_trans=group_in.min_y_trans; this->max_y_trans=group_in.max_y_trans;
    this->min_z_trans=group_in.min_z_trans; this->max_z_trans=group_in.max_z_trans;

    this->gp_scene_ptr=group_in.gp_scene_ptr;

    return;
}
// ~ Group Operator(s)
Group & Group::operator=(const Group & group_in)
{
	RenObj::operator=(group_in);

    this->instance_count=group_in.instance_count;
    this->gp_scale=group_in.gp_scale;
    this->gp_rotate=group_in.gp_rotate;
    this->gp_trans=group_in.gp_trans;
    glm::mat4 * temp_srt;
    _Init(temp_srt);
    _New(temp_srt,this->instance_count);
    for(long int i=0; i<this->instance_count; i++)
    {
        temp_srt[i]=group_in.gp_srt[i];
    }
    _Delete(this->gp_srt);
    _New(this->gp_srt,this->instance_count);
    for(long int i=0; i<this->instance_count; i++)
    {
        this->gp_srt[i]=temp_srt[i];
    }
    _Delete(temp_srt);
    this->min_x_rot=group_in.min_x_rot; this->max_x_rot=group_in.max_x_rot;
    this->min_y_rot=group_in.min_y_rot; this->max_x_rot=group_in.max_y_rot;
    this->min_z_rot=group_in.min_z_rot; this->max_x_rot=group_in.max_z_rot;

    this->min_x_scale=group_in.min_x_scale; this->max_x_scale=group_in.max_x_scale;
    this->min_y_scale=group_in.min_y_scale; this->max_y_scale=group_in.max_y_scale;
    this->min_z_scale=group_in.min_z_scale; this->max_z_scale=group_in.max_z_scale;

    this->min_x_trans=group_in.min_x_trans; this->max_x_trans=group_in.max_x_trans;
    this->min_y_trans=group_in.min_y_trans; this->max_y_trans=group_in.max_y_trans;
    this->min_z_trans=group_in.min_z_trans; this->max_z_trans=group_in.max_z_trans;

    this->gp_scene_ptr=group_in.gp_scene_ptr;

    return *this;
}
// ~ Group Getter(s)/Setter(s)
long int Group::GetObjCount(void)
{
    return this->instance_count;
}
GLint Group::SetScenePtr(Scene * scene_ptr_in)
{
    this->gp_scene_ptr=scene_ptr_in;
    return _OK;
}
GLint Group::SetOrigin(GLfloat x_in, GLfloat y_in, GLfloat z_in)
{
    GLfloat x_diff=x_in-this->origin[0];
    GLfloat y_diff=y_in-this->origin[1];
    GLfloat z_diff=z_in-this->origin[2];
    this->origin[0]=x_in;
    this->origin[1]=y_in;
    this->origin[2]=z_in;
    std::vector<glm::vec3>::iterator i;
    long int srt_i=0;
    for(i=this->gp_trans.begin(); i!=this->gp_trans.end(); i++)
    {
        if(srt_i>=this->instance_count)
            return _BAD;
        glm::vec3 temp_vec=(*i);
        (*i)=glm::vec3(temp_vec.x+x_diff,temp_vec.y+y_diff,temp_vec.z+z_diff);
        this->gp_srt[srt_i]=glm::translate(glm::mat4(1.0),(*i))*this->gp_rotate[srt_i]*this->gp_scale[srt_i];
        srt_i++;
    }
    return _OK;
}
void Group::SetScaleRanges(GLfloat x_min_in, GLfloat x_max_in, GLfloat y_min_in, GLfloat y_max_in, GLfloat z_min_in, GLfloat z_max_in)
{
    if(x_max_in<x_min_in || max_y_scale<min_y_scale || max_z_scale<min_z_scale)
        return;
    this->min_x_scale=x_min_in; this->max_x_scale=x_max_in;
    this->min_y_scale=y_min_in; this->max_y_scale=y_max_in;
    this->min_z_scale=z_min_in; this->max_z_scale=z_max_in;

    return;
}
void Group::SetRotationRanges(GLfloat x_min_in, GLfloat x_max_in, GLfloat y_min_in, GLfloat y_max_in, GLfloat z_min_in, GLfloat z_max_in)
{
    this->min_x_rot=x_min_in; this->max_x_rot=x_max_in;
    this->min_y_rot=y_min_in; this->max_y_rot=y_max_in;
    this->min_z_rot=z_min_in; this->max_z_rot=z_max_in;

    return;
}
void Group::SetTranslationRanges(GLfloat x_min_in, GLfloat x_max_in, GLfloat y_min_in, GLfloat y_max_in, GLfloat z_min_in, GLfloat z_max_in)
{
    this->min_x_trans=x_min_in; this->max_x_trans=x_max_in;
    this->min_y_trans=y_min_in; this->max_y_trans=y_max_in;
    this->min_z_trans=z_min_in; this->max_z_trans=z_max_in;

    return;
}
// ~ Group Instancing
long int Group::AddRandomInstances(long int how_many)
{
    if(how_many<1)
        return _BAD;
    // ~ Re-Size Dynamic Array
    glm::mat4 * new_srt_array;
    _Init(new_srt_array);
    _New(new_srt_array,this->instance_count+how_many);
    memcpy(new_srt_array,this->gp_srt,sizeof(glm::mat4)*this->instance_count);
    _Delete(this->gp_srt);
    this->gp_srt=new_srt_array;
    // ~ Add New Instances
    for(long int i=0; i<how_many; i++)
    {
        glm::mat4 temp_rot_mat=glm::mat4(1.0);
        GLfloat rand0;
        if(this->max_x_rot!=0 && this->min_x_rot!=0)
        {
            rand0 = this->min_x_rot + (GLfloat)rand()/((GLfloat)RAND_MAX/(this->max_x_rot-this->min_x_rot));
            temp_rot_mat=glm::rotate(temp_rot_mat,rand0,glm::vec3(1.0,0.0,0.0));
        }
        if(this->max_y_rot!=0 && this->min_y_rot!=0)
        {
            rand0 = this->min_y_rot + (GLfloat)rand()/((GLfloat)RAND_MAX/(this->max_y_rot-this->min_y_rot));
            temp_rot_mat=glm::rotate(temp_rot_mat,rand0,glm::vec3(0.0,1.0,0.0));
        }
        if(this->max_z_rot!=0 && this->min_z_rot!=0)
        {
            rand0 = this->min_z_rot + (GLfloat)rand()/((GLfloat)RAND_MAX/(this->max_z_rot-this->min_z_rot));
            temp_rot_mat=glm::rotate(temp_rot_mat,rand0,glm::vec3(0.0,0.0,1.0));
        }
        this->gp_rotate.push_back(temp_rot_mat);
        rand0 = this->min_x_scale + (GLfloat)rand()/((GLfloat)RAND_MAX/(this->max_x_scale-this->min_x_scale));
        GLfloat rand1 = this->min_y_scale + (GLfloat)rand()/((GLfloat)RAND_MAX/(this->max_y_scale-this->min_y_scale));
        GLfloat rand2 = this->min_z_scale + (GLfloat)rand()/((GLfloat)RAND_MAX/(this->max_z_scale-this->min_z_scale));
        glm::mat4 temp_scale_mat=glm::scale(glm::mat4(1.0),glm::vec3(rand0,rand1,rand2));
        this->gp_scale.push_back(temp_scale_mat);
        // ~ Add Instance
        rand0 = this->min_x_trans + (GLfloat)rand()/((GLfloat)RAND_MAX/(this->max_x_trans-this->min_x_trans));
        rand1 = this->min_y_trans + (GLfloat)rand()/((GLfloat)RAND_MAX/(this->max_y_trans-this->min_y_trans));
        rand2 = this->min_z_trans + (GLfloat)rand()/((GLfloat)RAND_MAX/(this->max_z_trans-this->min_z_trans));
        glm::vec3 temp_vec=glm::vec3(rand0+this->origin[0],rand1+this->origin[1],rand2+this->origin[2]);
        this->gp_trans.push_back(temp_vec);
        this->gp_srt[this->instance_count]=glm::translate(glm::mat4(1.0),temp_vec)*temp_rot_mat*temp_scale_mat;
        this->instance_count++;
    }
    return this->instance_count;
}
long int Group::AddInstance(glm::mat4 scale_in, glm::mat4 rotate_in, glm::vec3 trans_vec_in)
{
    // ~ Add Instance
    this->gp_scale.push_back(scale_in);
    this->gp_rotate.push_back(rotate_in);
    glm::vec3 temp_vec=glm::vec3(trans_vec_in.x+this->origin[0],trans_vec_in.y+this->origin[1],trans_vec_in.z+this->origin[2]);
    this->gp_trans.push_back(temp_vec);
    this->gp_srt[this->instance_count]=glm::translate(glm::mat4(1.0),temp_vec)*rotate_in*scale_in;
    return this->instance_count++;
}

// ~ Group Rendering
GLint Group::Draw(GLint incoming_id, OGLUniforms& uniforms_in,OGLMatrices& matrices_in, const bool& textures_en, const bool& bumpmap_en)
{
    if(this->instance_count<1)
        return _BAD;
    if(incoming_id!=this->render_id)
    {
        this->BindBufferData();
    }
    this->Prepare(uniforms_in,textures_en, bumpmap_en);
	//--- Draw Cube ---//
    // ~ Update SRT Attribute
    matrices_in.UpdateSRT(this->gp_srt,this->instance_count);
    // ~ Draw Call
	glDrawArraysInstanced(GL_TRIANGLES,0,this->lengths[0]/3,this->instance_count);
    // ~ Rollback SRT Attribute
    matrices_in.UpdateSRT(&(matrices_in.translation_rotation_scale));
    return _OK;
}
// ~ Group Destructor
Group::~Group(void)
{
    _Delete(this->gp_srt);
}
};