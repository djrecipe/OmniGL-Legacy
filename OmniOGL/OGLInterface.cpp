#pragma region Includes
#include "OGLInterface.h"
#pragma endregion
namespace OmniOGL
{
#pragma region OGLAttributes Class
OGLAttributes::OGLAttributes(void)
{
    this->valid=false;
    return;
}
OGLAttributes::OGLAttributes(const OGLMatrices & matrices_in)
{
    this->valid=true;
    try
    {
        this->position=this->ConfigureAttribute("position",OGLBuffers::vbo(0),0,3,GL_FLOAT);
        this->color=this->ConfigureAttribute("color",OGLBuffers::vbo(1),0,4,GL_FLOAT);
        this->texcoord=this->ConfigureAttribute("texcoord",OGLBuffers::vbo(2),0,2,GL_FLOAT);
        this->normal=this->ConfigureAttribute("normal",OGLBuffers::vbo(4),0,3,GL_FLOAT);
        // ~ Update SRT Attribute
        //  
        glBindVertexArray(OGLBuffers::vao(0));
        GLint temp_glint = glGetAttribLocation(ShaderProgram::id(),"srt_combined");
        for(GLint i=0; i<4; i++)
        {
            this->srt[i]=temp_glint+i;
            glEnableVertexAttribArray(this->srt[i]);
        }
        glBindBuffer(GL_ARRAY_BUFFER,OGLBuffers::vbo(3));
        glBufferData(GL_ARRAY_BUFFER,sizeof(glm::mat4),&(matrices_in.translation_rotation_scale),GL_DYNAMIC_DRAW);
        for(GLint i=0; i<4; i++)
        {
            glVertexAttribPointer(this->srt[i],4,GL_FLOAT,GL_FALSE,sizeof(GLfloat)*16,(void*)(sizeof(GLfloat)*i*4));
            glVertexAttribDivisor(this->srt[i],1);
        }
        glBindVertexArray(0);
    }
    catch(GLint e)
    {
        this->valid=false;
    }
}
GLint OGLAttributes::ConfigureAttribute(std::string name_in, GLuint vbo_in, GLint divisor_in, GLuint num_components_in, GLint type_in)
{
    glBindVertexArray(OGLBuffers::vao(0));
    GLint retval=glGetAttribLocation(ShaderProgram::id(),name_in.c_str());
	glEnableVertexAttribArray(retval);
    glVertexAttribDivisor(retval,divisor_in);
	glBindBuffer(GL_ARRAY_BUFFER,vbo_in);
    glVertexAttribPointer(retval,num_components_in,type_in,GL_FALSE,0,0);// [Index],[Size],[type],[normalized],[stride],[pointer]
    glBindVertexArray(0);
    return retval;
}
OGLAttributes& OGLAttributes::operator=(const OGLAttributes& copy_source)
{
    this->position=copy_source.position;
    this->color=copy_source.color;
    this->texcoord=copy_source.texcoord;
    this->normal=copy_source.normal;
    this->valid=copy_source.valid;
    memcpy(this->srt,copy_source.srt,4*sizeof(GLint));
    return *this;
}
OGLAttributes::OGLAttributes(const OGLAttributes & copy_source):
    position(copy_source.position),
    color(copy_source.color),
    texcoord(copy_source.texcoord),
    normal(copy_source.normal),
    valid(copy_source.valid)
{
    memcpy(this->srt,copy_source.srt,4*sizeof(GLint));
    return;
}
#pragma endregion
#pragma region OGLMatrices Class
OGLMatrices::OGLMatrices(void)
{
	this->scale = glm::mat4(1.0);
	this->rotation = glm::mat4(1.0);
	this->translation = glm::mat4(1.0);
    this->translation_rotation_scale = this->translation*this->rotation*this->scale;
    this->projection_view = this->projection*this->view;
    this->vbo=(GLuint)_BAD;
    return;
}
void OGLMatrices::SetSRTVbo(const GLuint& vbo_in)
{
    this->vbo=vbo_in;
    return;
}
GLint OGLMatrices::sTranslate(glm::vec3 trans_vec)
{
    if(this->vbo==-1)
        return _BAD;
    //
	this->translation = glm::translate(glm::mat4(1.0),trans_vec);
    // ~ Update SRT Attribute
    this->translation_rotation_scale=this->translation*this->rotation*this->scale;
    this->UpdateSRT(&(this->translation_rotation_scale), this->vbo);
    //
	return _OK;
}

GLint OGLMatrices::UpdateSRT(glm::mat4 * srt_in, const long int& count)
{
    if(this->vbo==-1)
        return _BAD;
    //
    glBindBuffer(GL_ARRAY_BUFFER,this->vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(glm::mat4)*count,srt_in,GL_DYNAMIC_DRAW);
    //
    return _OK;
}
#pragma endregion
#pragma region OGLUniformBlockElement Class
OGLUniformBlockElement::OGLUniformBlockElement(void):
    index(_BAD),
    offset(_BAD),
    size(_BAD),
    type(_BAD)
{
}
OGLUniformBlockElement::OGLUniformBlockElement(GLuint index_in, GLint offset_in, GLint size_in, GLint type_in):
    index(index_in),
    offset(offset_in),
    size(size_in),
    type(type_in)
{
}
#pragma endregion
#pragma region OGLUniformBlock Class
OGLUniformBlock::OGLUniformBlock(void):
    index(_BAD),
    name("none"),
    count(_BAD),
    size(_BAD),
    binding(_BAD)
{
}
GLint OGLUniformBlock::block_count=1;
OGLUniformBlock::OGLUniformBlock(const std::string & name_in, std::vector<std::string> names_in)
{
    this->name=name_in;
    GLuint counter=0;

    this->size=0;
    //
    this->index=glGetUniformBlockIndex(ShaderProgram::id(),name_in.c_str());
    this->binding=OGLUniformBlock::block_count++;
    glGetActiveUniformBlockiv(ShaderProgram::id(),this->index,GL_UNIFORM_BLOCK_DATA_SIZE,&this->size);
    GLuint index;
    GLint offset, size, type;
    //
    std::map<const std::string,OGLUniformBlockElement>::iterator j;
    this->elements.clear();
    for(auto i=names_in.begin(); i!=names_in.end(); i++)
    {
        const char * chars[1]={(*i).c_str()};
        glGetUniformIndices(ShaderProgram::id(), 1, chars, &index);
        glGetActiveUniformsiv(ShaderProgram::id(), 1, &index,GL_UNIFORM_OFFSET,&offset);
        glGetActiveUniformsiv(ShaderProgram::id(), 1, &index,GL_UNIFORM_SIZE,&size);
        glGetActiveUniformsiv(ShaderProgram::id(), 1, &index,GL_UNIFORM_TYPE,&type);
        j=this->elements.end();
        this->elements.insert(j,std::pair<const std::string,OGLUniformBlockElement>((*i),OGLUniformBlockElement(index,offset,size,type)));
        counter++;
    }
    this->count=names_in.size();
}
OGLUniformBlock& OGLUniformBlock::operator=(const OGLUniformBlock& copy_source)
{
    this->count=copy_source.count;
    this->size=copy_source.size;
    this->name=copy_source.name;
    this->index=copy_source.index;
    this->binding=copy_source.binding;
    this->elements=copy_source.elements;
    return *this;
}
OGLUniformBlock::~OGLUniformBlock(void)
{
};
#pragma endregion
#pragma region OGLUniforms Class
const std::string OGLUniforms::_block_names[2]={"MaterialLighting","SourceLighting"};
const std::string OGLUniforms::_source_lighting_block_names[3]={"lightposn","lightcolor","ambient"};
const std::string OGLUniforms::_material_lighting_block_names[3]={"diffuse","specular","shininess"};
std::vector<std::string> OGLUniforms::block_names(OGLUniforms::_block_names,OGLUniforms::_block_names+sizeof(OGLUniforms::_block_names)/sizeof(OGLUniforms::_block_names[0]));
std::vector<std::string> OGLUniforms::source_lighting_block_names(OGLUniforms::_source_lighting_block_names,OGLUniforms::_source_lighting_block_names + sizeof(OGLUniforms::_source_lighting_block_names) / sizeof(OGLUniforms::_source_lighting_block_names[0]));
std::vector<std::string> OGLUniforms::material_lighting_block_names(OGLUniforms::_material_lighting_block_names,OGLUniforms::_material_lighting_block_names + sizeof(OGLUniforms::_material_lighting_block_names) / sizeof(OGLUniforms::_material_lighting_block_names[0]));
OGLUniforms::OGLUniforms(void)
{
    memset(this->uniform_buffers,-1,2*sizeof(GLuint));
    if(ShaderProgram::id()==_BAD)
        return;
    _projection_view_matrix=glGetUniformLocation(ShaderProgram::id(),"pv_combined");
    _cast_all_black=glGetUniformLocation(ShaderProgram::id(),"isblack");
    _is_a_voxel=glGetUniformLocation(ShaderProgram::id(),"isvoxel");
    _textures_enabled=glGetUniformLocation(ShaderProgram::id(),"istex");
    _texture=glGetUniformLocation(ShaderProgram::id(),"tex0");
    _bumpmapping_enabled=glGetUniformLocation(ShaderProgram::id(),"isbumpmap");
    _bumpmap=glGetUniformLocation(ShaderProgram::id(),"tex1");
    _lighting_enabled=glGetUniformLocation(ShaderProgram::id(),"islight");
    _diffuse_enabled=glGetUniformLocation(ShaderProgram::id(),"islambert");
    _specular_enabled=glGetUniformLocation(ShaderProgram::id(),"isphong");
    _eye_position=glGetUniformLocation(ShaderProgram::id(),"eyepos");
    _fog_start=glGetUniformLocation(ShaderProgram::id(),"fog_start");
    _fog_end=glGetUniformLocation(ShaderProgram::id(),"fog_end");
    for(int i=0; i<OGLUniforms::block_names.size(); i++)
        this->uniform_buffers[i]=OGLBuffers::CreateVBO();
    glBindBuffer(GL_UNIFORM_BUFFER,this->uniform_buffers[0]);
    this->blocks.clear();
    std::map<std::string,OGLUniformBlock>::iterator j=this->blocks.end();
    this->blocks.insert(j,std::pair<std::string,OGLUniformBlock>(OGLUniforms::_block_names[0],OGLUniformBlock(OGLUniforms::_block_names[0],OGLUniforms::material_lighting_block_names)));
    glBufferData(GL_UNIFORM_BUFFER, this->blocks[OGLUniforms::_block_names[0]].size, nullptr, GL_DYNAMIC_DRAW);
    glUniformBlockBinding(ShaderProgram::id(),this->blocks[OGLUniforms::_block_names[0]].index,this->blocks[OGLUniforms::_block_names[0]].binding);
    glBindBufferBase(GL_UNIFORM_BUFFER,this->blocks[OGLUniforms::_block_names[0]].binding,this->uniform_buffers[0]);
    //
    glBindBuffer(GL_UNIFORM_BUFFER,this->uniform_buffers[1]);
    j=this->blocks.end();
    this->blocks.insert(j,std::pair<std::string,OGLUniformBlock>(OGLUniforms::_block_names[1],OGLUniformBlock(OGLUniforms::_block_names[1],OGLUniforms::source_lighting_block_names)));
    glBufferData(GL_UNIFORM_BUFFER, this->blocks[OGLUniforms::_block_names[1]].size, nullptr, GL_STREAM_DRAW);
    glUniformBlockBinding(ShaderProgram::id(),this->blocks[OGLUniforms::_block_names[1]].index,this->blocks[OGLUniforms::_block_names[1]].binding);
    glBindBufferBase(GL_UNIFORM_BUFFER,this->blocks[OGLUniforms::_block_names[1]].binding,this->uniform_buffers[1]);
    
    return;
}
OGLUniforms& OGLUniforms::operator=(const OGLUniforms& copy_source)
{
    memcpy(const_cast<GLint*>(&(this->_texture)),&copy_source._texture,sizeof(GLint));
    memcpy(const_cast<GLint*>(&(this->_bumpmap)),&copy_source._bumpmap,sizeof(GLint));
    memcpy(const_cast<GLint*>(&(this->_eye_position)),&copy_source._eye_position,sizeof(GLint));
    memcpy(const_cast<GLint*>(&(this->_is_a_voxel)),&copy_source._is_a_voxel,sizeof(GLint));
    memcpy(const_cast<GLint*>(&(this->_cast_all_black)),&copy_source._cast_all_black,sizeof(GLint));
    memcpy(const_cast<GLint*>(&(this->_textures_enabled)),&copy_source._textures_enabled,sizeof(GLint));
    memcpy(const_cast<GLint*>(&(this->_lighting_enabled)),&copy_source._lighting_enabled,sizeof(GLint));
    memcpy(const_cast<GLint*>(&(this->_diffuse_enabled)),&copy_source._diffuse_enabled,sizeof(GLint));
    memcpy(const_cast<GLint*>(&(this->_specular_enabled)),&copy_source._specular_enabled,sizeof(GLint));
    memcpy(const_cast<GLint*>(&(this->_bumpmapping_enabled)),&copy_source._bumpmapping_enabled,sizeof(GLint));
    memcpy(const_cast<GLint*>(&(this->_projection_view_matrix)),&copy_source._projection_view_matrix,sizeof(GLint));
    memcpy(const_cast<GLint*>(&(this->_fog_start)),&copy_source._fog_start,sizeof(GLint));
    memcpy(const_cast<GLint*>(&(this->_fog_end)),&copy_source._fog_end,sizeof(GLint));
    memcpy(this->uniform_buffers,copy_source.uniform_buffers,2*sizeof(GLuint));
    this->blocks=copy_source.blocks;
    return *this;

}
GLint OGLUniforms::GetUniformBlockIndex(const std::string & name_in)
{
    if(this->blocks.find(name_in)!=this->blocks.end())
        return this->blocks[name_in].index;
    assert(1==2);
    return _BAD;
}
GLuint OGLUniforms::GetUniformBlockBuffer(const std::string & block_in)
{
    for(int i=0; i<OGLUniforms::block_names.size(); i++)
    {
        if(block_in==OGLUniforms::block_names[i])
            return this->uniform_buffers[i];
    }
    assert(1==2);
    return _BAD;
}
GLint OGLUniforms::projection_view_matrix(void)
{
    return this->_projection_view_matrix;
}
GLint OGLUniforms::cast_all_black(void)
{
    return this->_cast_all_black;
}
GLint OGLUniforms::is_a_voxel(void)
{
    return this->_is_a_voxel;
}
GLint OGLUniforms::textures_enabled(void)
{
    return this->_textures_enabled;
}
GLint OGLUniforms::texture(void)
{
    return this->_texture;
}
GLint OGLUniforms::bumpmapping_enabled(void)
{
    return this->_bumpmapping_enabled;
}
GLint OGLUniforms::bumpmap(void)
{
    return this->_bumpmap;
}
GLint OGLUniforms::lighting_enabled(void)
{
    return this->_lighting_enabled;
}
GLint OGLUniforms::diffuse_enabled(void)
{
    return this->_diffuse_enabled;
}
GLint OGLUniforms::specular_enabled(void)
{
    return this->_specular_enabled;
}
GLint OGLUniforms::eye_position(void)
{
    return this->_eye_position;
}
GLint OGLUniforms::fog_start(void)
{
    return this->_fog_start;
}
GLint OGLUniforms::fog_end(void)
{
    return this->_fog_end;
}
GLint OGLUniforms::fog_density(void)
{
    return this->_fog_density;
}
/*
GLint OGLUniforms::MallocFor(const std::string & name_in)
{
    for(int i=0; i<2; i++)
    {
        if(this->blocks[i].name==name_in)
            return this->blocks[i].size;
    }
    return _BAD;
}*/
GLint OGLUniforms::GetOffset(const std::string & block_in, const std::string & uniform_in)
{
    if(this->blocks.find(block_in)!=this->blocks.end() && this->blocks[block_in].elements.find(uniform_in)!=this->blocks[block_in].elements.end())
        return this->blocks[block_in].elements[uniform_in].offset;
    return _BAD;
}
GLint OGLUniforms::GetSize(const std::string & block_in, const std::string & uniform_in)
{
    if(this->blocks.find(block_in)!=this->blocks.end() && this->blocks[block_in].elements.find(uniform_in)!=this->blocks[block_in].elements.end())
        return this->blocks[block_in].elements[uniform_in].size*TypeSize(this->blocks[block_in].elements[uniform_in].type);
    return _BAD;
}

void OGLUniforms::ReleaseBuffers(void)
{
    for(int i=0; i<OGLUniforms::block_names.size(); i++)
    {
        if(this->uniform_buffers[i]!=_BAD)
            OGLBuffers::FreeVBO(this->uniform_buffers[i]);
    }
}
#pragma endregion
#pragma region OGLBuffers_Data Class
std::vector<GLuint> OGLBuffers_Data::vbo;
std::vector<GLuint> OGLBuffers_Data::vao;
std::vector<GLuint> OGLBuffers_Data::ebo;
#pragma endregion
#pragma region OGLBuffers Class
GLuint OGLBuffers::CreateVAO(void)
{
    // --- Initialize Vertex Arrays --- //
    // ~ Generate
    if(OGLBuffers::data.vao.size()>=MAX_VAO)
        return (GLuint)_BAD;
    GLuint temp;
    try
    {
	    glGenBuffers(1,&temp);
        OGLBuffers::data.vao.push_back(temp);
    }
    catch (GLint e)
    {
        temp=(GLuint)_BAD;
    }

    return temp;
}
GLuint OGLBuffers::CreateVBO(void)
{
    // --- Generate/Bind Vertex Array Buffer(s) --- //
    // I. Generate & Bind Buffer
    if(OGLBuffers::data.vbo.size()+OGLBuffers::data.ebo.size()>=MAX_VBO)
        return (GLuint)_BAD;
    GLuint temp;
    try
    {
	    glGenBuffers(1,&temp);
        OGLBuffers::data.vbo.push_back(temp);
    }
    catch(GLint e)
    {
        temp=(GLuint)_BAD;
    }
    return temp;
}
GLuint OGLBuffers::CreateEBO(void)
{
    // --- Initialize Element Buffer Object --- //
    if(OGLBuffers::data.vbo.size()+OGLBuffers::data.ebo.size()>=MAX_VBO)
        return (GLuint)_BAD;
    GLuint temp;
    try
    {
        glGenBuffers(1,&temp);
        OGLBuffers::data.ebo.push_back(temp);
    }
    catch(int e)
    {
        temp=(GLuint)_BAD;
    }
    // --- Return --- //
    return temp;
}
GLuint OGLBuffers::vbo(GLuint index_in)
{
    if(index_in>=OGLBuffers::data.vbo.size())
        return (GLuint)_BAD;
    return OGLBuffers::data.vbo[index_in];
}
GLuint OGLBuffers::ebo(GLuint index_in)
{
    if(index_in>=OGLBuffers::data.ebo.size())
        return (GLuint)_BAD;
    return OGLBuffers::data.ebo[index_in];
}
GLuint OGLBuffers::vao(GLuint index_in)
{
    if(index_in>=OGLBuffers::data.vao.size())
        return (GLuint)_BAD;
    return OGLBuffers::data.vao[index_in];
}
bool OGLBuffers::FreeVBO(GLuint index_in)
{
    if(index_in==_BAD)
        return false;
    for(auto i=OGLBuffers::data.vbo.begin(); i!=OGLBuffers::data.vbo.end(); i++)
    {
        if((*i)==index_in)
        {
            glDeleteBuffers(1,&(*i));
            OGLBuffers::data.vbo.erase(i);
            return true;
        }
    }
    return false;

}
void OGLBuffers::Cleanup(void)
{
    for(auto i=OGLBuffers::data.vbo.begin(); i!=OGLBuffers::data.vbo.end(); i++)
    {
        glDeleteBuffers(1,&(*i));
    }
    for(auto i=OGLBuffers::data.ebo.begin(); i!=OGLBuffers::data.ebo.end(); i++)
    {
        glDeleteBuffers(1,&(*i));
    }
    for(auto i=OGLBuffers::data.vao.begin(); i!=OGLBuffers::data.vao.end(); i++)
    {
        glDeleteVertexArrays(1,&(*i));
    }
    OGLBuffers::data.vbo.clear();
    OGLBuffers::data.ebo.clear();
    OGLBuffers::data.vao.clear();
}
#pragma endregion
#pragma region ShaderProgram Class
// ~ Shader Program
std::vector<GLuint> ShaderProgram::vertex_shaders;
std::vector<GLuint> ShaderProgram::fragment_shaders;
std::map<GLuint,GLuint*> ShaderProgram::shader_programs;
GLuint ShaderProgram::_id=(GLuint)_BAD;
GLuint ShaderProgram::CompileVertexShader(std::string filename_in)
{
    // --- Variables --- //
    // I. Source Code Buffer
    std::string filebuff="";
    GLchar * vertexSource;
    _Init(vertexSource);
    GLchar ** allvertexSources=new GLchar*[1];
    GLint sz_vertexSource;
    GLint* ptr_sz_vertexSource=new GLint[1];
    
    // II. Vertex Shader
    GLuint vertexShader;
    // --- Compile Fragment Shader --- //
    // I. Extract Source File Contents
    filebuff=GetFileContents(filename_in);
    // II. Typecast Array
    _New(vertexSource,filebuff.length()+1);
    memcpy((GLchar*)vertexSource,filebuff.c_str(),filebuff.length());
    vertexSource[filebuff.length()]='\0';
    allvertexSources[0]=vertexSource;
    sz_vertexSource=filebuff.length();
    ptr_sz_vertexSource[0]=sz_vertexSource;
    // DBUG: show file contents in GLchar* format
    //  dprint("---Vertex Source: ---\n%s",(GLchar *)vertexSource);
    // III. Create & Compile Shader
    vertexShader=glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader,(GLsizei)1,(const GLchar**)allvertexSources,ptr_sz_vertexSource);
	glCompileShader(vertexShader);
    glGetShaderInfoLog(vertexShader,_BUFSIZ,&ptr_sz_vertexSource[0],(GLchar*)vertexSource);
    dprint("---Vertex Shader Info Log (size: %d): ---\n%s",ptr_sz_vertexSource[0],(GLchar *)vertexSource);
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&ptr_sz_vertexSource[0]);
    if(ptr_sz_vertexSource[0]==GL_TRUE)
        dprint("Vertex Shader Compilation Succesful.");
    else if(ptr_sz_vertexSource[0]==GL_FALSE)
    {
        dprint("Vertex Shader Compilation Failed.");
        return (GLuint)_BAD;
    }

    // --- Return --- //
    _Delete(vertexSource);
    delete[] allvertexSources;
    delete[] ptr_sz_vertexSource;
    ShaderProgram::vertex_shaders.push_back(vertexShader);
    return ShaderProgram::vertex_shaders.size()-1;
}
GLuint ShaderProgram::CompileFragmentShader(std::string filename_in)
{
    // --- Variables --- //
    // Buffers
    std::string filebuff="";
    GLchar tempbuff[_SMALLBUF]="\0";
    GLchar * fragmentSource = new GLchar[_BUFSIZ];
    GLchar ** allfragmentSources=new GLchar*[1];
    GLint sz_fragmentSource;
    GLint* ptr_sz_fragmentSource=new GLint[1];
    // Fragment Shader
    GLuint fragmentShader;
    // --- Compile Fragment Shader --- //
    // Open & Extract Source
    FILE* fragfile=nullptr;
    if(fopen_s(&fragfile,filename_in.c_str(),"rt")!=0 || fragfile==nullptr || fragfile==0)
        return (GLuint)_BAD;
    while(fgets(tempbuff,sizeof(tempbuff),fragfile) != NULL)
        filebuff+=tempbuff;
    if(fragfile)
        fclose(fragfile);
    // Typecast Array
    memcpy((GLchar*)fragmentSource,filebuff.c_str(),filebuff.length()+1);
    fragmentSource[filebuff.length()+1]='\0';
    allfragmentSources[0]=fragmentSource;
    sz_fragmentSource=filebuff.length();
    ptr_sz_fragmentSource[0]=sz_fragmentSource;
    // DBUG: show file contents in GLchar* format
    //  dprint("---Fragment Source: ---\n%s",(GLchar *)fragmentSource);
    // Create & Compile Shader
	fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader,(GLsizei)1,(const GLchar**)allfragmentSources,ptr_sz_fragmentSource);
	glCompileShader(fragmentShader);
    glGetShaderInfoLog(fragmentShader,_BUFSIZ,&ptr_sz_fragmentSource[0],(GLchar*)fragmentSource);
    dprint("---Fragment Shader Info Log (size: %d): ---\n%s",ptr_sz_fragmentSource[0],(GLchar *)fragmentSource);
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&ptr_sz_fragmentSource[0]);
    if(ptr_sz_fragmentSource[0]==GL_TRUE)
        dprint("Fragment Shader Compilation Succesful.");
    else if(ptr_sz_fragmentSource[0]==GL_FALSE)
    {
        dprint("Fragment Shader Compilation Failed.");
        return (GLuint)_BAD;
    }
    // --- Return --- //
    delete[] fragmentSource;
    delete[] allfragmentSources;
    delete[] ptr_sz_fragmentSource;
    ShaderProgram::fragment_shaders.push_back(fragmentShader);
    return ShaderProgram::fragment_shaders.size()-1;
}
GLuint ShaderProgram::CreateShaderProgram(GLuint vertexShader, GLuint fragmentShader)
{
    if(vertexShader>ShaderProgram::vertex_shaders.size() || fragmentShader>ShaderProgram::fragment_shaders.size())
        return (GLuint)_BAD;
    // ~ Create Program
    GLuint shaderProgram=glCreateProgram();
    try
    {
        // ~ Attach Shaders to Program
	    glAttachShader(shaderProgram,ShaderProgram::vertex_shaders[vertexShader]);
	    glAttachShader(shaderProgram,ShaderProgram::fragment_shaders[fragmentShader]);
        // ~ Link & Use Program
	    glLinkProgram(shaderProgram);
	    glUseProgram(shaderProgram);
        // ~ Link Miscellaneous "Shader Program Variables"
	    glBindFragDataLocation(shaderProgram,0,"outColor");
    }
    catch (int e)
    {
        return (GLuint)_BAD;
    }
    //
    GLuint * programs;
    _Init(programs);
    _New(programs,2);
    programs[0]=ShaderProgram::vertex_shaders[vertexShader]; programs[1]=ShaderProgram::fragment_shaders[fragmentShader];
    ShaderProgram::shader_programs.insert(std::pair<GLuint,GLuint*>(shaderProgram,programs));
    ShaderProgram::_id=shaderProgram;
    return shaderProgram;
}
bool ShaderProgram::FreeShader(GLuint shader_in)
{
    bool retval=false;
    std::map<GLuint,GLuint*>::iterator i;
    std::vector<std::map<GLuint,GLuint*>::iterator> k;
    std::vector<std::map<GLuint,GLuint*>::iterator>::iterator m;
    GLuint pair[2]={(GLuint)_BAD,(GLuint)_BAD};
    GLboolean found[2]={false,false};
    for(i=ShaderProgram::shader_programs.begin(); i!=ShaderProgram::shader_programs.end(); i++)
    {
        if((*i).first==shader_in && pair[0]==_BAD && pair[1]==_BAD)
        {
            memcpy(pair,(*i).second,2*sizeof(GLuint));
            glDeleteProgram((*i).first);
            _Delete((*i).second);
            k.push_back(i);
            retval=true;
        }
        else
        {
            if(pair[0]==(*i).second[0])
                found[0]=true;
            if(pair[1]==(*i).second[1])
                found[1]=true;
        }
    }
    for(m=k.begin(); m!=k.end(); m++)
    {
        ShaderProgram::shader_programs.erase(*m);
    }
    std::vector<GLuint>::iterator j;
    std::vector<std::vector<GLuint>::iterator> n;
    if(!found[0])
    {
        for(j=ShaderProgram::vertex_shaders.begin(); j!=ShaderProgram::vertex_shaders.end(); j++)
        {
            if((*j)==pair[0])
                n.push_back(j);
        }
        glDeleteShader(pair[0]);
    }
    std::vector<std::vector<GLuint>::iterator>::iterator p;
    for(p=n.begin(); p!=n.end(); p++)
    {
        ShaderProgram::vertex_shaders.erase(*p);
    }
    n.clear();
    if(!found[1])
    {
        for(j=ShaderProgram::fragment_shaders.begin(); j!=ShaderProgram::fragment_shaders.end(); j++)
        {
            if((*j)==pair[1])
                n.push_back(j);
        }
        glDeleteShader(pair[1]);
    }
    for(p=n.begin(); p!=n.end(); p++)
    {
        ShaderProgram::fragment_shaders.erase(*p);
    }
    if(_id==shader_in)
        _id=(GLuint)_BAD;
    return retval;
}
GLuint ShaderProgram::CreateShaderProgram(std::string vertexShader_in, std::string fragmentShader_in)
{
    return ShaderProgram::CreateShaderProgram(ShaderProgram::CompileVertexShader(vertexShader_in),ShaderProgram::CompileFragmentShader(fragmentShader_in));
}                                                   
bool ShaderProgram::SetShaderProgram(const GLuint & id_in)
{
    if(id_in==_BAD)
        return false;
    std::map<GLuint,GLuint*>::iterator i;
    for(i=ShaderProgram::shader_programs.begin(); i!=ShaderProgram::shader_programs.end(); i++)
    {
        if((*i).first==id_in)
        {
            _id=id_in;
            return true;
        }
    }
    return false;

}
GLuint ShaderProgram::id(void)
{
    return ShaderProgram::_id;
}
void ShaderProgram::FreeShaders(void)
{
    std::map<GLuint,GLuint*>::iterator i;
    std::map<GLuint,GLuint*> copy=ShaderProgram::shader_programs;
    for(i=copy.begin(); i!=copy.end(); i++)
    {
        ShaderProgram::FreeShader((*i).first);
    }
}
#pragma endregion
#pragma region InputEvents Class
#pragma region Members
InputEvents* InputEvents::activeinput=NULL;
bool InputEvents::initialized=false;
#pragma endregion
#pragma region Methods
#pragma region Initialization
InputEvents::InputEvents(void)
{
    this->firstMouse=true;
    memset(this->lastmouse,0,2);
    this->mouseDivConst[0]=25.0;this->mouseDivConst[1]=25.0;
    this->screenshot_index=0;
}
void InputEvents::Activate(void)
{
    if(!InputEvents::initialized)
    {
        glfwSetKeyCallback(InputEvents::_evnt_Keybrd);
        glfwSetMousePosCallback(InputEvents::_evnt_MouseMov);
        glfwSetMouseButtonCallback(InputEvents::_evnt_MouseBtn);
        InputEvents::initialized=true;
    }
    InputEvents::activeinput=this;
}
#pragma endregion
#pragma region Input Data Retrieval
InputUpdate InputEvents::GetInputUpdate(void)
{
    InputUpdate retval=this->input_info;
    this->input_info.pending_placement=false;
    this->input_info.pending_removal=false;
    if(this->input_info.direction==0)
        this->input_info.trigger_camera_update=false;
    this->input_info.toggle_bumpmapping=false;
    this->input_info.toggle_diffuse=false;
    this->input_info.toggle_specular=false;
    this->input_info.toggle_texture=false;
    this->input_info.toggle_wireframe=false;

    return retval;
}
#pragma endregion
#pragma region Static Event Handlers
void GLFWCALL InputEvents::_evnt_Keybrd(GLint keyID,GLint keyState)
{
    if(InputEvents::activeinput!=NULL)
        activeinput->evnt_Keybrd(keyID,keyState);
	return;
}
void GLFWCALL InputEvents::_evnt_MouseBtn(GLint button, GLint action)
{
    if(InputEvents::activeinput!=NULL)
        activeinput->evnt_MouseBtn(button,action);
    return;
}
void GLFWCALL InputEvents::_evnt_MouseMov(GLint xCoord, GLint yCoord)
{
    if(InputEvents::activeinput!=NULL)
        activeinput->evnt_MouseMov(xCoord,yCoord);
	return;
}
#pragma endregion
#pragma region Instance Event Handlers
void GLFWCALL InputEvents::evnt_Keybrd(GLint keyID,GLint keyState)
{
    BYTE * pixels;
    FIBITMAP* image;
    int new_dir=this->input_info.direction;
    if(keyState==GLFW_RELEASE)
    {
		switch(keyID){
        case 'S':					// ___STOP Move Backwards___
            new_dir &= 0xFFFB;// nullify 0x0004 (backwards)
            this->input_info.trigger_camera_update=true;
            break;
		case 'W':					// ___STOP Move Forwards___
			new_dir&=0xFFFE;// nullify 0x0001 (forwards)
            this->input_info.trigger_camera_update=true;
            break;
	    case 'A':					// ___STOP Strafe Left___
			new_dir&=0xFFF7;// nullify 0x0008 (left)
            this->input_info.trigger_camera_update=true;
			break;
	    case 'D':					// ___STOP Strafe Right___
			new_dir&=0xFFFD;// nullify 0x0002 (right)
            this->input_info.trigger_camera_update=true;
            break;
        default:
			break;
		}
    }
	else if (keyState==GLFW_PRESS)
	{
		switch(keyID){
        case 'S':					// ___Move Backwards___
			new_dir&=0xFFFE;// nullify 0x0001 (forwards)
            new_dir|=0x0004;// add 0x0004 (backwards)
            this->input_info.trigger_camera_update=true;
            break;
		case 'W':					// ___Move Forwards___
			new_dir&=0xFFFB;// nullify 0x0004 (backwards)
            new_dir|=0x0001;// add 0x0001 (forwards)
            this->input_info.trigger_camera_update=true;
            break;
	    case 'A':					// ___Strafe Left___
			new_dir&=0xFFFD;// nullify 0x0002 (right)
            new_dir|=0x0008;// add 0x0008 (left)
            this->input_info.trigger_camera_update=true;
			break;
	    case 'D':					// ___Strafe Right___
			new_dir&=0xFFF7;// nullify 0x0008 (left)
            new_dir|=0x0002;// add 0x0002 (right)
            this->input_info.trigger_camera_update=true;
            break;
		case 'Q':
            this->input_info.exitbool=true;
			break;
        case 'F':
            if(this->input_info.free_cam)
                this->input_info.free_cam=false;
            else
                this->input_info.free_cam=true;
            this->input_info.trigger_camera_update=true;
            break;
        case 'P':
            this->input_info.toggle_specular=true;
            break;
        case 'L':
            this->input_info.toggle_diffuse=true;
            break;
        case 'M':
            pixels = new BYTE[ 3 * winsize[0] * winsize[1]];
            glReadPixels(0, 0, winsize[0], winsize[1], GL_RGB, GL_UNSIGNED_BYTE, pixels);
            // Convert to FreeImage format & save to file
            image = FreeImage_ConvertFromRawBits(pixels, winsize[0], winsize[1], 3 * winsize[0], 24, 0x0000FF, 0x00FF00, 0xFF0000, false);
            FreeImage_Save(FIF_BMP, image, (std::string("C:/Users/DJRecipe/Desktop/ogl")+std::to_string(screenshot_index++)+std::string(".bmp")).c_str(), 0);
            // Free resources
            FreeImage_Unload(image);
            delete [] pixels;
        case 'R':
            this->input_info.pending_removal=true;
            break;
        case 'B':
            this->input_info.toggle_bumpmapping=true;
            break;
        case 'T':
            this->input_info.toggle_texture=true;
            break;
        case 'K':
            this->input_info.toggle_wireframe=true;
            break;
        default:
			break;
		}
	}
    this->input_info.direction=new_dir;
	return;
}
void GLFWCALL InputEvents::evnt_MouseBtn(GLint button, GLint action)
{
    switch(button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        if (action==GLFW_PRESS)
            this->input_info.pending_placement=true;
        break;
    default:
        break;
    }
    return;
}
void GLFWCALL InputEvents::evnt_MouseMov(GLint xCoord, GLint yCoord)
{
    this->input_info.trigger_camera_update=true;
    if(this->firstMouse)
    {
        this->lastmouse[0]=(GLfloat)xCoord;this->lastmouse[1]=(GLfloat)yCoord;
        this->firstMouse=false;
        return;
    }
    if(yCoord<(this->lastmouse[1]) && this->input_info.angle_up<75.0)
        this->input_info.angle_up+=(lastmouse[1]-yCoord)/mouseDivConst[1];
    else if(yCoord>(lastmouse[1]) && this->input_info.angle_up>-75.0)
        this->input_info.angle_up-=(yCoord-this->lastmouse[1])/mouseDivConst[1];
    if(xCoord<(this->lastmouse[0]))
        this->input_info.angle_side-=(xCoord-this->lastmouse[0])/mouseDivConst[0];
    else if(xCoord>(this->lastmouse[0]))
        this->input_info.angle_side+=(this->lastmouse[0]-xCoord)/mouseDivConst[0];
    this->lastmouse[0]=(GLfloat)xCoord;this->lastmouse[1]=(GLfloat)yCoord;

	return;
}
#pragma endregion
#pragma endregion
#pragma endregion
#pragma region InputUpdate Class
#pragma region Methods
#pragma region Constructor/Initialization
InputUpdate::InputUpdate(void)
{
    this->angle_side=0.0;
    this->angle_up=0.0;
    this->direction=0;
    this->pending_placement=false;
    this->trigger_camera_update=false;
    this->free_cam=true;
    this->exitbool=false;
}
#pragma endregion
#pragma region Configuration
#pragma endregion
#pragma endregion
#pragma endregion
};