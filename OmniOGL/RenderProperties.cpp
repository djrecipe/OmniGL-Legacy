#include "RenderProperties.h"
namespace OmniOGL{

MaterialLighting::MaterialLighting(void):
    black_only(false),
    enabled(false),
    shininess(100)
{
    this->diffuse_factor[0]=0.5;
    this->diffuse_factor[1]=0.5;
    this->diffuse_factor[2]=0.5;
    this->diffuse_factor[3]=1.0;
    this->specular_factor[0]=0.5;
    this->specular_factor[1]=0.5;
    this->specular_factor[2]=0.5;
    this->specular_factor[3]=1.0;
}
void MaterialLighting::Update(OGLUniforms & uniforms_in)
{
    glUniform1i(uniforms_in.cast_all_black(),this->black_only);
    glUniform1i(uniforms_in.lighting_enabled(),this->enabled);
    if(!this->black_only && this->enabled)
    {
        std::string block_name_str="MaterialLighting";
        glBindBuffer(GL_UNIFORM_BUFFER,uniforms_in.GetUniformBlockBuffer(block_name_str));
        std::string uniform_name_str="diffuse";
        glBufferSubData(GL_UNIFORM_BUFFER,uniforms_in.GetOffset(block_name_str,uniform_name_str),uniforms_in.GetSize(block_name_str,uniform_name_str),this->diffuse_factor);
        uniform_name_str="specular";
        glBufferSubData(GL_UNIFORM_BUFFER,uniforms_in.GetOffset(block_name_str,uniform_name_str),uniforms_in.GetSize(block_name_str,uniform_name_str),this->specular_factor);
        uniform_name_str="shininess";
        glBufferSubData(GL_UNIFORM_BUFFER,uniforms_in.GetOffset(block_name_str,uniform_name_str),uniforms_in.GetSize(block_name_str,uniform_name_str),&this->shininess);
    }
}
void MaterialLighting::StaticUpdate(OGLUniforms & uniforms_in)
{
    glUniform1i(uniforms_in.cast_all_black(),false);
    glUniform1i(uniforms_in.lighting_enabled(),false);
}

DepthFunction::DepthFunction(void):
    function(GL_LEQUAL),
    mask(true),
    enabled(true)
{
}

StencilFunction::StencilFunction(void):
    mask(0xFF),
    enabled(true)
{
    this->function[0]=GL_EQUAL; this->function[1]=1; this->function[2]=0xFF;
    this->operation[0]=GL_REPLACE; this->operation[1]=GL_KEEP; this->operation[2]=GL_KEEP;
}

BlendFunction::BlendFunction(void):
    enabled(true)
{
    memset(this->function,GL_LEQUAL,2); 
}
GLuint TextureMaster::textures[MAX_TEX];
std::string TextureMaster::filenames[MAX_TEX];
GLint TextureMaster::index=0;
GLint TextureMaster::MakeTexture(std::string file_in,bool repeat)
{
    if(TextureMaster::index>=MAX_TEX)
        return _BAD;
    for(GLuint i=0; i<MAX_TEX; i++)
    {
        if(file_in==filenames[i])
            return i;
    }
    GLuchar* txr_img;
    GLint txr_w,txr_h;
    // ~ Texture Config
    glGenTextures(1,&textures[TextureMaster::index]);
    if(TextureMaster::index>0)
        glActiveTexture(GL_TEXTURE0+TextureMaster::index);
    glBindTexture(GL_TEXTURE_2D,textures[TextureMaster::index]);
    // ~ Retrieve Image
    txr_img=SOIL_load_image(file_in.c_str(),&txr_w,&txr_h,0,SOIL_LOAD_RGBA);
    if((GLint)txr_img==_BAD || (GLint)txr_img==0)
    {
        dprint("Error loading image file '%s'.\n",file_in.c_str());
        dprint("Last SOIL error message: '%s'\n",SOIL_last_result());
        return _BAD;
    }
    glTexImage2D(GL_TEXTURE_2D,0,4,txr_w,txr_h,
        0,GL_RGBA,GL_UNSIGNED_BYTE,txr_img);
    SOIL_free_image_data(txr_img);
    // II(c). Set Texture Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    if(!repeat)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    }
    dprint("Processed texture file '%s' successfully.\n",file_in.c_str());
    // --- Return --- //
    filenames[TextureMaster::index]=file_in;
    return TextureMaster::index++;
}
GLint TextureMaster::active_texture_index=_BAD;
bool TextureMaster::textures_enabled=false;
GLint TextureMaster::FreeTextures(void)
{
    try
    {
        glDeleteTextures(TextureMaster::index,TextureMaster::textures);
        TextureMaster::index=0;
    }
    catch(GLint e)
    {
        return _BAD;
    }

    return TextureMaster::index;
}
GLint TextureMaster::CurrentTextureIndex(void)
{
    return TextureMaster::active_texture_index;
}
bool TextureMaster::TexturesEnabled(void)
{
    return TextureMaster::textures_enabled;
}
void TextureMaster::SetCurrentTextureIndex(GLint index_in)
{
    TextureMaster::active_texture_index=index_in;
    return;
}
void TextureMaster::SetTexturesEnabled(bool enabled_in)
{
    TextureMaster::textures_enabled=enabled_in;
    return;
}
}