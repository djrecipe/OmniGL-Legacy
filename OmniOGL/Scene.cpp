#include "Scene.h"
namespace OmniOGL{
#pragma region Camera Class
// ~ Camera Constructor(s)
Camera::Camera(void)
{
    for(GLint i=0; i<3; i++)
    {
        if(i==2)
        {
            eye[i]=DEF_CAMERA_HEIGHT; up[i]=1.0f;
        }
        else
        {
            eye[i]=0.0f; up[i]=0.0f;
        }
    }
    this->clip_near=0.1f; this->clip_far=100000.0f;
    this->fov=65.0f;
    this->free_cam=false;
    this->angle_side=0.0f; this->angle_up=0.0f;
    this->center[0]=(GLfloat)(this->eye[0]+INITIAL_LOOK_DISTANCE*cos(this->angle_side*PI/180.0f));
    this->center[1]=(GLfloat)(this->eye[1]+INITIAL_LOOK_DISTANCE*sin(this->angle_side*PI/180.0f));
    this->center[2]=(GLfloat)(this->eye[2]+INITIAL_LOOK_DISTANCE*sin(this->angle_up*PI/180.0f));
    for(GLint i=0;i<3;i++)
        this->orig_look_vec[i]=this->center[i]-this->eye[i];
    dprint("Starting Eye @ %f,%f,%f",this->eye[0],this->eye[1],this->eye[2]);
    dprint("Starting Center @ %f,%f,%f",this->center[0],this->center[1],this->center[2]);
    dprint("Starting Look Vector: %f,%f,%f",this->orig_look_vec[0],this->orig_look_vec[1],this->orig_look_vec[2]);
    this->curr_direction=0;

	return;
}
// ~ Camera Math
glm::mat3 Camera::raw_rotate(GLfloat arr_in[3],const glm::vec3& axis,const GLfloat degrees) 
{
    glm::mat3 mat_out=glm::mat3(1.0);
    GLfloat sin_theta=(GLfloat)sin(degrees*PI/180.0f);
    GLfloat cos_theta=(GLfloat)cos(degrees*PI/180.0f);
    // ~ Construct Matrix
    mat_out=cos_theta*mat_out;
    mat_out+=(1.0f-cos_theta)*glm::mat3(axis.x*axis.x, axis.x*axis.y, axis.x*axis.z,
                                       axis.x*axis.y, axis.y*axis.y, axis.y*axis.z,
                                       axis.x*axis.z, axis.y*axis.z, axis.z*axis.z);
    mat_out+=sin_theta*glm::mat3(0.0, -axis.z, axis.y,
                                 axis.z, 0.0, -axis.x,
                                 -axis.y, axis.x, 0.0);
    GLfloat arr_out[3];
    memcpy(arr_out,arr_in,sizeof(GLfloat)*3);
    arr_out[0]=arr_in[0]*mat_out[0][0]+arr_in[1]*mat_out[0][1]+arr_in[2]*mat_out[0][2];
    arr_out[1]=arr_in[0]*mat_out[1][0]+arr_in[1]*mat_out[1][1]+arr_in[2]*mat_out[1][2];
    arr_out[2]=arr_in[0]*mat_out[2][0]+arr_in[1]*mat_out[2][1]+arr_in[2]*mat_out[2][2];
    memcpy(arr_in,arr_out,sizeof(GLfloat)*3);
    return mat_out;
}
// ~ Camera Updating
GLint Camera::Update(glm::mat4& proj_matrix_in, glm::mat4& view_matrix_in, const int & uniform_eye, const int & uniform_pv_matrix,const InputUpdate& update_data_in,irrklang::ISoundEngine* sound_engine_ptr)
{
    GLfloat old_eye[3];
	// ~ Update Projection
    if(update_data_in.trigger_camera_update)
    {
        memcpy(old_eye,this->eye,3*sizeof(GLfloat));
        this->angle_side=update_data_in.angle_side;
        this->angle_up=update_data_in.angle_up;
        this->curr_direction=update_data_in.direction;
        this->free_cam=update_data_in.free_cam;
        //
        GLfloat speed = DEFAULT_SPEED;
        GLint mov_count=0;
        bool mov_forw=false, mov_backw=false, mov_left=false, mov_right=false;
        //
        if(this->curr_direction&0x0004)
        {
            mov_backw=true;
            mov_count++;
        }
        else if(this->curr_direction&0x0001)
        {
            mov_forw=true;
            mov_count++;
	    }
	    if(this->curr_direction&0x0008)
	    {
            mov_left=true;
            mov_count++;
	    }
	    else if(this->curr_direction&0x0002)
	    {
            mov_right=true;
            mov_count++;
	    }
        //
        GLfloat look_vec[3]={this->center[0]-this->eye[0],this->center[1]-this->eye[1],this->center[2]-this->eye[2]};
	    glm::vec3 normlook=glm::normalize(glm::vec3(look_vec[0],look_vec[1],look_vec[2]));
        glm::vec3 normup=glm::normalize(glm::vec3(this->up[0],this->up[1],this->up[2]));
        glm::vec3 normtrans = glm::normalize(glm::cross(normlook,normup));
        //
	    if(mov_count>0)
	    {
		    speed/=(GLfloat)mov_count;
		    if(mov_backw)
		    {
			    """ Move Backwards """;
			    // - Normalize & Apply - //
			    for (GLint i=0; i<3; i++)
			    {
                    if(!this->free_cam && i==2)
                        break;
				    this->eye[i]-=normlook[i]*speed;
			    }
		    }
		    else if(mov_forw)
		    {
			    """ Move Forwards """;
			    // - Normalize & Apply - //
			    for (GLint i=0; i<3; i++)
			    {
                    if(!this->free_cam && i==2)
                        break;
				    this->eye[i]+=normlook[i]*speed;
			    }
		    }
		    if(mov_left)
		    {
			    """ Strafe Left """;
			    for (GLint i=0; i<3; i++)
			    {
				    this->eye[i]-=normtrans[i]*speed;
			    }
		    }
		    else if(mov_right)
		    {
			    """ Strafe Right """;
			    for (GLint i=0; i<3; i++)
			    {
				    this->eye[i]+=normtrans[i]*speed;
			    }
		    }
	    }
	    // ~ Update Look View
        memcpy(this->center,this->orig_look_vec,sizeof(GLfloat)*3);
        while(this->angle_side>=360.0)
            this->angle_side-=360.0;
        while(this->angle_side<0)
            this->angle_side+=360.0;
        glm::mat3 rot_mat=this->raw_rotate(this->center,normup,this->angle_side);
	    normlook=glm::normalize(glm::vec3(this->center[0],this->center[1],this->center[2]));
        normup=glm::normalize(glm::vec3(this->up[0],this->up[1],this->up[2]));
        normtrans = glm::normalize(glm::cross(normlook,normup));
        rot_mat=this->raw_rotate(this->center,normtrans,this->angle_up);
        for(int i=0; i<3; i++)
            this->center[i]+=this->eye[i];
    }
    //
    view_matrix_in=glm::lookAt(glm::vec3(this->eye[0],this->eye[1],this->eye[2]),
			                   glm::vec3(this->center[0],this->center[1],this->center[2]),
			                   glm::vec3(this->up[0],this->up[1],this->up[2]));
    proj_matrix_in=glm::perspective(this->fov,(GLfloat)winsize[0]/(GLfloat)winsize[1],this->clip_near,this->clip_far);
    glm::mat4 proj_view_mat=proj_matrix_in*view_matrix_in;
    glUniform3f(uniform_eye,this->eye[0],this->eye[1],this->eye[2]);
	glUniformMatrix4fv(uniform_pv_matrix,1,GL_FALSE,glm::value_ptr(proj_view_mat));
    //
    if(update_data_in.trigger_camera_update)
    {
        // ~ Update Audio
        if(sound_engine_ptr!=NULL)
        {
            irrklang::vec3df irk_center(this->center[0]-this->eye[0],this->center[1]-this->eye[1],this->center[2]-this->eye[2]);
            sound_engine_ptr->setListenerPosition(irrklang::vec3df(this->eye[0],this->eye[1],this->eye[2]),irk_center,irrklang::vec3df((this->eye[0]-old_eye[0])/10.0,(this->eye[1]-old_eye[1])/10.0,(this->eye[2]-old_eye[2])/10.0),irrklang::vec3df(this->up[0],this->up[1],-this->up[2]));
        }
        return _OK;
    }
    return _BAD;
}
// ~ Camera Destructor
Camera::~Camera(void)
{
	return;
}
#pragma endregion
#pragma region Section Class
// ~ Section Constructor(s)
Section::Section(void):
    x_index(0),
    y_index(0),
    z_index(0),
    voxels_loaded(false),
    adjacents_loaded(false)
{
    for(GLint i=0; i<2; i++)
    {
        this->x_limits[i]=_BAD;
        this->y_limits[i]=_BAD;
        this->z_limits[i]=_BAD;
        this->x_neighbors[i]=0;
        this->y_neighbors[i]=0;
        this->z_neighbors[i]=0;
    }
    this->id=(GLuint)_BAD;
}
Section::Section(GLint x_index_in, GLint y_index_in, GLint z_index_in, GLfloat x_limits_in[2], GLfloat y_limits_in[2], GLfloat z_limits_in[2]):
    x_index(x_index_in),
    y_index(y_index_in),
    z_index(z_index_in),
    voxels_loaded(false),
    adjacents_loaded(false)
{
    for(GLint i=0; i<2; i++)
    {
        this->x_limits[i]=x_limits_in[i];
        this->y_limits[i]=y_limits_in[i];
        this->z_limits[i]=z_limits_in[i];
        this->x_neighbors[i]=0;
        this->y_neighbors[i]=0;
        this->z_neighbors[i]=0;
    }
    this->id=(GLuint)_BAD;
}
// ~ Section Populating
bool Section::AddObject(ptrRenObj renobj_toadd)
{
    try
    {
        if(dynamic_cast<BlockArray*>(renobj_toadd.get())!=0)
            this->voxels_loaded=true;
    }
    catch(int e)
    {
    }
    this->objects.push_back(renobj_toadd);
    return true;
}
GLuint Section::RemoveObject(ptrRenObj renobj_in)
{
    try
    {
        this->objects.remove(renobj_in);
    }
    catch (int e)
    {
        dprint("Custom Exception! Candidate for removal does not exist in the Section! Perhaps we need to check for this case?");
        throw;
    }
    return this->objects.size();
}
GLuint Section::QueryID(GLuint id_in)
{
    if(id_in!=_BAD)
        this->id=id_in;
    return this->id;
}
GLuint Section::GetObjCount(void)
{
    return this->objects.size();
}
bool Section::AreVoxelsLoaded(void)
{
    return this->voxels_loaded;
}
void Section::SetVoxelsLoaded(void)
{
    this->voxels_loaded=true;
}
bool Section::AdjacentsLoaded(void)
{
    return this->adjacents_loaded;
}
std::vector<GLuint> Section::GetAdjacentSections(void)
{
    return this->adjacent_sections;
}
void Section::SetAdjacentSections(std::vector<GLuint> sections_in)
{
    this->adjacent_sections=sections_in;
    this->adjacents_loaded=true;
}
#pragma endregion
#pragma region SectionCollection Class
SectionCollection::SectionCollection(GLint width_in):
    width(width_in),
    populated_sections(0),
    current_section(0),
    section_count(0)
{
    this->temp_barr.SetTexture("darkseamless.jpg",true);
    this->temp_barr.SetBumpMap("darkseamless_NRM.jpg",true);
    //
    this->looking_x[0]=false; this->looking_x[1]=false;
    this->looking_y[0]=true; this->looking_y[1]=true;
    //
    GLint half_width=width_in/2;
    GLint width_squared=this->width*this->width;
    GLint width_cubed=width_squared*this->width;
    _Init(this->sections);
    _New(this->sections,width_cubed);
    std::allocator<Section> alloc;
    for(GLint i=-half_width; i<half_width; i++)
    {
        for(GLint j=-half_width; j<half_width; j++)
        {
            for(GLint k=-half_width; k<half_width; k++)
            {
                GLfloat x_limits[2]={(GLfloat)i*SECTION_SIZE,(GLfloat)i*SECTION_SIZE+SECTION_SIZE};
                GLfloat y_limits[2]={(GLfloat)j*SECTION_SIZE,(GLfloat)j*SECTION_SIZE+SECTION_SIZE};
                GLfloat z_limits[2]={(GLfloat)k*SECTION_SIZE,(GLfloat)k*SECTION_SIZE+SECTION_SIZE};
                Section temp_section=Section(i,j,k,x_limits,y_limits,z_limits);
                temp_section.QueryID(this->section_count);
                this->sections[section_count]=temp_section;
                if(i==0 && j==0 && k==0)
                    this->current_section=this->sections[section_count].QueryID();
                if(k>-half_width)
                {
                    this->sections[section_count].z_neighbors[0]=section_count-1;
                    this->sections[section_count-1].z_neighbors[1]=section_count;
                }
                if(j>-half_width)
                {
                    this->sections[section_count].y_neighbors[0]=section_count-this->width;
                    this->sections[section_count-this->width].y_neighbors[1]=section_count;
                }
                if(i>-half_width)
                {
                    this->sections[section_count].x_neighbors[0]=section_count-width_squared;
                    this->sections[section_count-width_squared].x_neighbors[1]=section_count;
                }
                if(section_count%1000==0)
                    dprint("Generating Section # %i",section_count);
                this->section_count++;
            }
        }
    }
    bool need_to_update=this->UpdateAdjacentSectionsArray(0.0);
    if(need_to_update)
    {
        std::thread t(&SectionCollection::InstallVoxels,this,this->x_load,this->y_load,this->z_load,this->temp_barr,this->width/2);
        t.detach();
    }
}
GLuint SectionCollection::GetCurrentSection(void)
{
    return this->sections[this->current_section].QueryID();
}
void SectionCollection::GetOccupiedSectionIndices(GLint indices_out[3])
{
    indices_out[0]=this->sections[this->current_section].x_index;
    indices_out[1]=this->sections[this->current_section].y_index;
    indices_out[2]=this->sections[this->current_section].z_index;
    return;
}
void SectionCollection::InstallVoxels(std::vector<GLint> x_load_in, std::vector<GLint> y_load_in, std::vector<GLint> z_load_in, BlockArray barr_in, GLuint half_width_in)
{
    this->mtxSections.lock();
	GLfloat initial_time = (GLfloat)clock() / (GLfloat)CLOCKS_PER_SEC;
    GLuint start_section=this->current_section;
    //dprint("Started Loading Section %i,%i,%i",this->sections[start_section].x_index,this->sections[start_section].y_index,this->sections[start_section].z_index);
    this->mtxSections.unlock();
    GLint smallest_size=x_load_in.size();
    if(y_load_in.size()<smallest_size)
        smallest_size=y_load_in.size();
    if(z_load_in.size()<smallest_size)
        smallest_size=z_load_in.size();
    GLfloat voxel_count=10.0f;
    GLfloat voxel_size=(SECTION_SIZE/voxel_count)/2.0;
    char filename[256];
    sqlite3 *db;
    sqlite3_stmt * stmt=0;
	GLuint width_in = half_width_in * 2;
	GLuint half_width = half_width_in;
    std::string replace_string="REPLACE INTO SurfaceNets VALUES (?,?,?,?);";
    std::string select_string="SELECT * FROM SurfaceNets WHERE x=? AND y=? AND z=?;";
    GLint sprint_count=sprintf(filename,"SELECT * FROM SurfaceNets WHERE x=? AND y=? AND z=?;")+1;
    sqlite3_open_v2("SurfaceNets.ogldb", &db,SQLITE_OPEN_READWRITE|SQLITE_OPEN_FULLMUTEX,NULL);
    sqlite3_prepare_v2(db,filename,sprint_count,&stmt,nullptr);
	std::vector<GLint> sorted_x_load, sorted_y_load, sorted_z_load;
	std::vector<GLfloat> distances;
	for (GLint n = 0; n < smallest_size; n++)
	{
		GLfloat section_dist = glm::distance(glm::vec3((GLfloat)this->sections[this->current_section].x_index, (GLfloat)this->sections[this->current_section].y_index, (GLfloat)this->sections[this->current_section].z_index), glm::vec3((GLfloat)x_load_in[n], (GLfloat)y_load_in[n], (GLfloat)z_load_in[n]));
		if (distances.size() < 1)
		{
			sorted_x_load.push_back(x_load_in[n]);
			sorted_y_load.push_back(y_load_in[n]);
			sorted_z_load.push_back(z_load_in[n]);
			distances.push_back(section_dist);
		}
		else
		{
			for (GLint m = 0; m < distances.size(); m++)
			{
				if (section_dist <= distances[m])
				{
					sorted_x_load.insert(sorted_x_load.begin() + m, x_load_in[n]);
					sorted_y_load.insert(sorted_y_load.begin() + m, y_load_in[n]);
					sorted_z_load.insert(sorted_z_load.begin() + m, z_load_in[n]);
					distances.insert(distances.begin() + m, section_dist);
					break;
				}
				if (m == distances.size() - 1)
				{
					sorted_x_load.push_back(x_load_in[n]);
					sorted_y_load.push_back(y_load_in[n]);
					sorted_z_load.push_back(z_load_in[n]);
					distances.push_back(section_dist);
					break;
				}
			}
		}
		
	}
    for(GLint n=0; n<smallest_size; n++)
    {
        VoxelField vfield(voxel_count,voxel_size,sorted_x_load[n],sorted_y_load[n],sorted_z_load[n]);
        if(!vfield.LoadData(barr_in,stmt))
        {  
            if(sqlite3_finalize(stmt)!=SQLITE_OK)
            {
                dprint("Error message: %s\n", sqlite3_errmsg(db));
                assert(1==2);
            }
            sqlite3_prepare_v2(db,replace_string.c_str(),replace_string.size(), &stmt, nullptr);
            vfield.FillWithRandom(barr_in,stmt);
            if(sqlite3_finalize(stmt)!=SQLITE_OK)
            {
                dprint("Error message: %s\n", sqlite3_errmsg(db));
                assert(1==2);
            }
            sqlite3_prepare_v2(db,select_string.c_str(),select_string.size(),&stmt,nullptr);
        }
		this->mtxSections.lock();
		this->sections[sorted_z_load[n] + half_width + (sorted_y_load[n] + half_width)*width_in + (sorted_x_load[n] + half_width)*width_in*width_in].AddObject(std::make_shared<BlockArray>(barr_in));
		dprint("\nFinished Loading Section %i,%i,%i", sorted_x_load[n], sorted_y_load[n], sorted_z_load[n]);
		this->mtxSections.unlock();
		dprint(" in [[[   %f   ]]] Seconds\n", (GLfloat)clock() / (GLfloat)CLOCKS_PER_SEC - initial_time);
		dprint("--------------------------------------------------------------\n");
		initial_time = (GLfloat)clock() / (GLfloat)CLOCKS_PER_SEC;
    }
    if(sqlite3_finalize(stmt)!=SQLITE_OK)
    {
        dprint("Error message: %s\n", sqlite3_errmsg(db));
        assert(1==2);
    }
    if(sqlite3_close_v2(db)!=SQLITE_OK)
    {
        dprint("Error message: %s\n", sqlite3_errmsg(db));
        assert(1==2);
    }						 
}
bool SectionCollection::UpdateCurrentSection(GLfloat x_in, GLfloat y_in, GLfloat z_in, GLfloat angle_side_in)
{
    // ~ Section Control
    bool need_to_update=false;
        if(x_in<this->sections[this->current_section].x_limits[0])
    {
        this->current_section=this->sections[this->current_section].x_neighbors[0];
        need_to_update=true;
    }
    else if(x_in>=this->sections[this->current_section].x_limits[1])
    {
        this->current_section=this->sections[this->current_section].x_neighbors[1];
        need_to_update=true;
    }
    if(y_in<this->sections[this->current_section].y_limits[0])
    {
        this->current_section=this->sections[this->current_section].y_neighbors[0];
        need_to_update=true;

    }
    else if(y_in>=this->sections[this->current_section].y_limits[1])
    {
        this->current_section=this->sections[this->current_section].y_neighbors[1];
        need_to_update=true;
    }
    if(z_in<this->sections[this->current_section].z_limits[0])
    {
        this->current_section=this->sections[this->current_section].z_neighbors[0];
        need_to_update=true;
    }
    else if(z_in>=this->sections[this->current_section].z_limits[1])
    {
        this->current_section=this->sections[this->current_section].z_neighbors[1];
        need_to_update=true;
    }
    if(need_to_update)
    {
        need_to_update=this->UpdateAdjacentSectionsArray(angle_side_in);
        if(need_to_update)
        {
            std::thread t(&SectionCollection::InstallVoxels,this,this->x_load,this->y_load,this->z_load,this->temp_barr,this->width/2);
            t.detach();
        }
    }
    //else
    //    this->MinorUpdateAdjacentSections(angle_side_in);
    return need_to_update;
}
void SectionCollection::MinorUpdateAdjacentSections(GLfloat angle_side_in,bool update_pending)
{
    if((this->looking_x[0] || this->looking_x[1] || update_pending) && angle_side_in>=225 && angle_side_in <315.0)
    {
        this->looking_y[0]=true; this->looking_y[1]=false;
        this->looking_x[0]=false; this->looking_x[1]=false;
        update_pending=true;
    }
    else if((this->looking_x[0] || this->looking_x[1] || update_pending) && angle_side_in>=45 && angle_side_in <135.0)
    {
        this->looking_y[0]=false; this->looking_y[1]=true;
        this->looking_x[0]=false; this->looking_x[1]=false;
        update_pending=true;
    }
    else if((this->looking_y[0] || this->looking_y[1] || update_pending) && angle_side_in>=135 && angle_side_in <225.0)
    {
        this->looking_x[0]=true; this->looking_x[1]=false;
        this->looking_y[0]=false; this->looking_y[1]=false;
        update_pending=true;
    }
    else if((this->looking_y[0] || this->looking_y[1] || update_pending) && (angle_side_in>=315 || angle_side_in <45.0))
    {
        this->looking_x[0]=false; this->looking_x[1]=true;
        this->looking_y[0]=false; this->looking_y[1]=false;
        update_pending=true;
    }
    else
    {
        //dprint("Angle Rejected: %f",angle_side_in);
    }
    if(update_pending)
    {
        std::vector<GLuint>::iterator i;
        this->temp_adjacent_sections.clear();
        Section * adj_sec_ptr;
        _Init(adj_sec_ptr);
        Section * derived_ptr;
        _Init(derived_ptr);
        Section * curr_sec_ptr;
        _Init(curr_sec_ptr);
        for(i=this->new_adjacent_sections.begin(); i!=this->new_adjacent_sections.end(); i++)
        {
            curr_sec_ptr=&this->sections[*i];
            //if(!_Valid(curr_sec_ptr))
            //    continue;
            // ~ Check Y- Neighbor
            if(this->looking_y[0])
            {
                adj_sec_ptr=&this->sections[curr_sec_ptr->y_neighbors[0]];
                if(_Valid(adj_sec_ptr) && adj_sec_ptr->y_index<=this->sections[this->current_section].y_index  && abs(adj_sec_ptr->x_index-this->sections[this->current_section].x_index)<5 && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),adj_sec_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),adj_sec_ptr->QueryID())==this->temp_adjacent_sections.end())
                {
                    this->temp_adjacent_sections.push_back(adj_sec_ptr->QueryID());
                    derived_ptr=&this->sections[adj_sec_ptr->x_neighbors[0]];
                    if(_Valid(derived_ptr) && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),derived_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),derived_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(derived_ptr->QueryID());
                    derived_ptr=&this->sections[adj_sec_ptr->x_neighbors[1]];
                    if(_Valid(derived_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),derived_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),derived_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(derived_ptr->QueryID());
                    derived_ptr=&this->sections[adj_sec_ptr->z_neighbors[0]];
                    if(_Valid(derived_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),derived_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),derived_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(derived_ptr->QueryID());
                    derived_ptr=&this->sections[adj_sec_ptr->z_neighbors[1]];
                    if(_Valid(derived_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),derived_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),derived_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(derived_ptr->QueryID());
                    // Z-
                    adj_sec_ptr=&this->sections[curr_sec_ptr->z_neighbors[0]];
                    if(_Valid(adj_sec_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),adj_sec_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),adj_sec_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(adj_sec_ptr->QueryID());
                    // Z+
                    adj_sec_ptr=&this->sections[curr_sec_ptr->z_neighbors[1]];
                    if(_Valid(adj_sec_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),adj_sec_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),adj_sec_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(adj_sec_ptr->QueryID());
                }
            }
            else if(this->looking_y[1])
            {
                // ~ Check Y+ Neighbor
                adj_sec_ptr=&this->sections[curr_sec_ptr->y_neighbors[1]];
                if(_Valid(adj_sec_ptr)  && adj_sec_ptr->y_index>=this->sections[this->current_section].y_index && abs(adj_sec_ptr->x_index-this->sections[this->current_section].x_index)<5 && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),adj_sec_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),adj_sec_ptr->QueryID())==this->temp_adjacent_sections.end())
                {
                    this->temp_adjacent_sections.push_back(adj_sec_ptr->QueryID());
                    derived_ptr=&this->sections[adj_sec_ptr->x_neighbors[0]];
                    if(_Valid(derived_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),derived_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),derived_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(derived_ptr->QueryID());
                    derived_ptr=&this->sections[adj_sec_ptr->x_neighbors[1]];
                    if(_Valid(derived_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),derived_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),derived_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(derived_ptr->QueryID());
                    derived_ptr=&this->sections[adj_sec_ptr->z_neighbors[0]];
                    if(_Valid(derived_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),derived_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),derived_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(derived_ptr->QueryID());
                    derived_ptr=&this->sections[adj_sec_ptr->z_neighbors[1]];
                    if(_Valid(derived_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),derived_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),derived_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(derived_ptr->QueryID());
                    // Z-
                    adj_sec_ptr=&this->sections[curr_sec_ptr->z_neighbors[0]];
                    if(_Valid(adj_sec_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),adj_sec_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),adj_sec_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(adj_sec_ptr->QueryID());
                    // Z+
                    adj_sec_ptr=&this->sections[curr_sec_ptr->z_neighbors[1]];
                    if(_Valid(adj_sec_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),adj_sec_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),adj_sec_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(adj_sec_ptr->QueryID());
                }
            }
            else if(this->looking_x[0])
            {
                // ~ Check X- Neighbor
                adj_sec_ptr=&this->sections[curr_sec_ptr->x_neighbors[0]];
                if(_Valid(adj_sec_ptr)  && adj_sec_ptr->x_index<=this->sections[this->current_section].x_index && abs(adj_sec_ptr->y_index-this->sections[this->current_section].y_index)<5 && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),adj_sec_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),adj_sec_ptr->QueryID())==this->temp_adjacent_sections.end())
                {
                    this->temp_adjacent_sections.push_back(adj_sec_ptr->QueryID());
                    derived_ptr=&this->sections[adj_sec_ptr->y_neighbors[0]];
                    if(_Valid(derived_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),derived_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),derived_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(derived_ptr->QueryID());
                    derived_ptr=&this->sections[adj_sec_ptr->y_neighbors[1]];
                    if(_Valid(derived_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),derived_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),derived_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(derived_ptr->QueryID()); 
                    derived_ptr=&this->sections[adj_sec_ptr->z_neighbors[0]];
                    if(_Valid(derived_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),derived_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),derived_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(derived_ptr->QueryID());
                    derived_ptr=&this->sections[adj_sec_ptr->z_neighbors[1]];
                    if(_Valid(derived_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),derived_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),derived_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(derived_ptr->QueryID());
                    // Z-
                    adj_sec_ptr=&this->sections[curr_sec_ptr->z_neighbors[0]];
                    if(_Valid(adj_sec_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),adj_sec_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),adj_sec_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(adj_sec_ptr->QueryID());
                    // Z+
                    adj_sec_ptr=&this->sections[curr_sec_ptr->z_neighbors[1]];
                    if(_Valid(adj_sec_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),adj_sec_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),adj_sec_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(adj_sec_ptr->QueryID());
                }
            }
            else if(this->looking_x[1])
            {
                // ~ Check X+ Neighbor
                adj_sec_ptr=&this->sections[curr_sec_ptr->x_neighbors[1]];
                if(_Valid(adj_sec_ptr)  && adj_sec_ptr->x_index>=this->sections[this->current_section].x_index && abs(adj_sec_ptr->y_index-this->sections[this->current_section].y_index)<5 && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),adj_sec_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),adj_sec_ptr->QueryID())==this->temp_adjacent_sections.end())
                {
                    this->temp_adjacent_sections.push_back(adj_sec_ptr->QueryID());
                    derived_ptr=&this->sections[adj_sec_ptr->y_neighbors[0]];
                    if(_Valid(derived_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),derived_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),derived_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(derived_ptr->QueryID());
                    derived_ptr=&this->sections[adj_sec_ptr->y_neighbors[1]];
                    if(_Valid(derived_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),derived_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),derived_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(derived_ptr->QueryID());
                    derived_ptr=&this->sections[adj_sec_ptr->z_neighbors[0]];
                    if(_Valid(derived_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),derived_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),derived_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(derived_ptr->QueryID());
                    derived_ptr=&this->sections[adj_sec_ptr->z_neighbors[1]];
                    if(_Valid(derived_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),derived_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),derived_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(derived_ptr->QueryID());
                    // Z-
                    adj_sec_ptr=&this->sections[curr_sec_ptr->z_neighbors[0]];
                    if(_Valid(adj_sec_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),adj_sec_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),adj_sec_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(adj_sec_ptr->QueryID());
                    // Z+
                    adj_sec_ptr=&this->sections[curr_sec_ptr->z_neighbors[1]];
                    if(_Valid(adj_sec_ptr)  && std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),adj_sec_ptr->QueryID())==this->adjacent_sections.end() && std::find(this->temp_adjacent_sections.begin(),this->temp_adjacent_sections.end(),adj_sec_ptr->QueryID())==this->temp_adjacent_sections.end())
                        this->temp_adjacent_sections.push_back(adj_sec_ptr->QueryID());
                }
            }
        }
        //dprint("Angle Update! (%f) || Temp Adjacent Section Count: %i",angle_side_in,this->temp_adjacent_sections.size());
    }
}
bool SectionCollection::UpdateAdjacentSectionsArray(GLfloat angle_side_in)
{
    //
    this->x_load.clear();
    this->y_load.clear();
    this->z_load.clear();
    this->adjacent_sections.clear();
    this->new_adjacent_sections.clear();
    this->new_adjacent_sections.push_back(this->current_section);
    this->adjacent_sections.push_back(this->current_section);
    if(!this->sections[this->current_section].AreVoxelsLoaded())
    {
        this->x_load.push_back(this->sections[this->current_section].x_index);
        this->y_load.push_back(this->sections[this->current_section].y_index);
        this->z_load.push_back(this->sections[this->current_section].z_index);
    }
    //
    if(!this->sections[this->current_section].AdjacentsLoaded())
    {
        for(int i=0; i<3; i++)
            this->AddAdjacentSections(i);
        this->sections[this->current_section].SetAdjacentSections(this->adjacent_sections);
    }
    else
    {
        this->new_adjacent_sections=this->sections[this->current_section].GetAdjacentSections();
        this->adjacent_sections=this->new_adjacent_sections;
    }
    //this->MinorUpdateAdjacentSections(angle_side_in,true);
    return this->x_load.size()>0;
    //dprint("Adjacent Section Count: %i",this->adjacent_sections.size());
    //dprint("");
}
bool SectionCollection::AddAdjacentSection(Section * adj_sec_ptr, std::vector<GLuint>& new_sections)
{
    bool retval=false;
    if(_Valid(adj_sec_ptr))
    {
        GLuint temp_id=adj_sec_ptr->QueryID();
        if(std::find(this->adjacent_sections.begin(),this->adjacent_sections.end(),temp_id)==this->adjacent_sections.end())
        {
            new_sections.push_back(temp_id);
            this->adjacent_sections.push_back(temp_id);
            if(!this->sections[temp_id].AreVoxelsLoaded())
            {
                this->x_load.push_back(this->sections[temp_id].x_index);
                this->y_load.push_back(this->sections[temp_id].y_index);
                this->z_load.push_back(this->sections[temp_id].z_index);
                this->sections[temp_id].SetVoxelsLoaded();
                retval=true;
                //dprint("Section %i,%i,%i",this->x_load.back(),this->y_load.back(),this->z_load.back());
            }

        }
    }
    return retval;
}
void SectionCollection::AddAdjacentSections(GLint i_in)
{
    std::vector<GLuint>::iterator i;
    std::vector<GLuint> new_sections;
    GLuint temp_id=_BAD;
    for(i=this->new_adjacent_sections.begin(); i!=this->new_adjacent_sections.end(); i++)
    {
        //dprint("CONCERNING %i,%i,%i",this->sections[*i].x_index,this->sections[*i].y_index,this->sections[*i].z_index);
        Section * curr_sec_ptr=&this->sections[*i];
        //if(!_Valid(curr_sec_ptr))
        //    continue;
        Section * adj_sec_ptr;
        // ~ Check Z- Neighbor
        adj_sec_ptr=&this->sections[curr_sec_ptr->z_neighbors[0]];
        this->AddAdjacentSection(adj_sec_ptr, new_sections);
        // ~ Check Z+ Neighbor
        adj_sec_ptr=&this->sections[curr_sec_ptr->z_neighbors[1]];
        this->AddAdjacentSection(adj_sec_ptr, new_sections);
        // ~ Check Y- Neighbor
        adj_sec_ptr=&this->sections[curr_sec_ptr->y_neighbors[0]];
        this->AddAdjacentSection(adj_sec_ptr, new_sections);
        // ~ Check Y+ Neighbor
        adj_sec_ptr=&this->sections[curr_sec_ptr->y_neighbors[1]];
        this->AddAdjacentSection(adj_sec_ptr, new_sections);
        // ~ Check X- Neighbor
        adj_sec_ptr=&this->sections[curr_sec_ptr->x_neighbors[0]];
        this->AddAdjacentSection(adj_sec_ptr, new_sections);
        // ~ Check X+ Neighbor
        adj_sec_ptr=&this->sections[curr_sec_ptr->x_neighbors[1]];
        this->AddAdjacentSection(adj_sec_ptr, new_sections);
        //dprint("New Adjacent Section Iteration Number: %i",count++);
    }
    this->new_adjacent_sections=new_sections;
    return;
}
LineStrip SectionCollection::GetGridlines(int which)
{
    GLint half_width=this->width/2;
    LineStrip lines;
    if(which==0)
    {
        for(int j=-half_width; j<half_width; j++)
        {
            for(int i=-half_width; i<half_width; i++)
            {
                GLfloat start[3]={i*SECTION_SIZE,j*SECTION_SIZE,-half_width*SECTION_SIZE};
                GLfloat end[3]={i*SECTION_SIZE,j*SECTION_SIZE,half_width*SECTION_SIZE};
                lines.AddInstance(start,end);
            }
        }
    }
    else if(which==1)
    {
        for(int j=-half_width; j<half_width; j++)
        {
            for(int i=-half_width; i<half_width; i++)
            {
                GLfloat start[3]={-half_width*SECTION_SIZE,i*SECTION_SIZE,j*SECTION_SIZE};
                GLfloat end[3]={half_width*SECTION_SIZE,i*SECTION_SIZE,j*SECTION_SIZE};
                lines.AddInstance(start,end);
            }
        }
    }
    else if(which==2)
    {
        for(int j=-half_width; j<half_width; j++)
        {
            for(int i=-half_width; i<half_width; i++)
            {
                GLfloat start[3]={j*SECTION_SIZE,-half_width*SECTION_SIZE,i*SECTION_SIZE};
                GLfloat end[3]={j*SECTION_SIZE,half_width*SECTION_SIZE,i*SECTION_SIZE};
                lines.AddInstance(start,end);
            }
        }
    }
    return lines;
}
bool SectionCollection::AddObject(ptrRenObj renobj_in)
{
    bool retval=true;
    GLfloat temp_origin[3];
    this->mtxSections.lock();
    for(GLuint i=0; i<this->section_count; i++)
    {
        GLfloat xlim[2]={this->sections[i].x_limits[0],this->sections[i].x_limits[1]};
        GLfloat ylim[2]={this->sections[i].y_limits[0],this->sections[i].y_limits[1]};
        GLfloat zlim[2]={this->sections[i].z_limits[0],this->sections[i].z_limits[1]};
        renobj_in->GetOrigin(temp_origin);
        if(temp_origin[0]>=xlim[0] && temp_origin[0]<xlim[1] && temp_origin[1]>=ylim[0] && temp_origin[1]<ylim[1] && temp_origin[2]>=zlim[0] && temp_origin[2]<zlim[1])             // notice >= lower limit
        {
            renobj_in->SetOccupiedSection(this->sections[i].QueryID());
            this->sections[i].AddObject(renobj_in);
            if(this->sections[i].GetObjCount()==1)
                this->populated_sections++;
            retval=false;
            break;
        }
    }
    this->mtxSections.unlock();
    if(retval)
        renobj_in->SetInternalClipping(false);
    return retval;
}
bool SectionCollection::RemoveNearObject(GLfloat x_in, GLfloat y_in, GLfloat z_in)
{
    std::list<ptrRenObj>::iterator i;
    GLfloat least_distance=-1.0;
    ptrRenObj closest_obj;
    glm::vec3 difference(1.0);
    GLfloat distance=0.0;
    GLfloat local_origin[3];
    bool retval=false;
    this->mtxSections.lock();
    for(i=this->sections[this->current_section].objects.begin(); i!=this->sections[this->current_section].objects.end(); i++)
    {
        (*i)->GetOrigin(local_origin);
        difference=glm::vec3(local_origin[0]-x_in,local_origin[1]-y_in,local_origin[2]-z_in);
        distance=sqrt(difference.x*difference.x+difference.y*difference.y+difference.z*difference.z);
        if (least_distance==-1.0 || distance<least_distance)
        {
            if(distance<=(*i)->GetSelectionRadius())
            {
                least_distance=distance;
                closest_obj=*i;
            }
        }
    }
    if(closest_obj)
    {
        this->RemoveObject(closest_obj);
        retval=true;
    }
    this->mtxSections.unlock();
    return retval; 
}
bool SectionCollection::RemoveObject(ptrRenObj obj_in)
{
    bool retval=false;
    if(obj_in->GetOccupiedSection()!=(GLuint)_BAD)
    {
        this->mtxSections.lock();
        try
        {
            this->sections[obj_in->GetOccupiedSection()].RemoveObject(obj_in);
            retval=true;
        }
        catch(int e)
        {
        }
        this->mtxSections.unlock();
    }
    return retval;
}
void SectionCollection::Draw(GLint & id_in, OGLUniforms & uniforms_in, OGLMatrices & matrices_in, const bool & textures_en_in, const bool & bumpmap_en_in)
{
    Section * section_ptr;
	std::list<ptrRenObj>::iterator i;
    std::vector<GLuint>::iterator j;
    this->mtxSections.lock();
    for(j=this->adjacent_sections.begin(); j!=this->adjacent_sections.end(); j++)
    {
        section_ptr=&this->sections[*j];
        bool already=false;
	    for(i=section_ptr->objects.begin(); i!= section_ptr->objects.end(); i++)
	    {
            // ~ Draw
		    (*i)->Draw(id_in,uniforms_in,matrices_in,textures_en_in,bumpmap_en_in);
            id_in=(*i)->GetRenderID();
	    }
    }
    /*
    for(j=this->temp_adjacent_sections.begin(); j!= this->temp_adjacent_sections.end(); j++)
    {
        section_ptr=&this->sections[*j];
        bool already=false;
	    for(i=section_ptr->objects.begin(); i!= section_ptr->objects.end(); i++)
	    {
            // ~ Draw
		    (*i)->Draw(id_in,uniforms_in,matrices_in,textures_en_in,bumpmap_en_in);
            id_in=(*i)->GetRenderID();
	    }
    }
    */
    this->mtxSections.unlock();
}
SectionCollection::~SectionCollection(void)
{
    _Delete(this->sections);
}
#pragma endregion
#pragma region Scene Class
bool Scene::globally_initialized=false;
void Scene::GlobalInitialization(void)
{
    if(Scene::globally_initialized)
        return;
    //
    ShaderProgram::CreateShaderProgram("VertexShader.glsl","FragmentShader.glsl");
    //
    for(int i=0; i<2; i++)
        OGLBuffers::CreateVAO();
    for(int i=0; i<5; i++)
        OGLBuffers::CreateVBO();
    for(int i=0; i<1; i++)
        OGLBuffers::CreateEBO();
    //
    GLfloat fogColor[4]={197.7f/255.0f,169.0f/255.0f,144.0f/255.0f,1.0f};
    glClearColor(fogColor[0],fogColor[1],fogColor[2],fogColor[3]);
    //
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDisable(GL_DITHER);
    glEnable(GL_LINE_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    glLineWidth(2);
    //
    Scene::globally_initialized=true;
    //
    sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
    return;
}
Scene::Scene(void):
    // ~ Rendering Properties
    total_object_count(0),
    current_id(_BAD),
    camera_changed(true),
    // ~ Meta
    exitbool(false),
    // ~ Flags
    islambert(false),
    isphong(false),
    isbumpmap(false),
    global_texture_enable(true),
    wireframe_enabled(false),
    sections(50)
{
    // ~ Init/Configure Graphics Objects
    Scene::GlobalInitialization();

        //
    this->matrices.SetSRTVbo(OGLBuffers::vbo(3));
    this->uniforms=OGLUniforms();
    glUniform1f(this->uniforms.fog_start(),SECTION_SIZE*0.5);
    glUniform1f(this->uniforms.fog_end(),SECTION_SIZE*1.6);
    //glUniform1f(this->uniforms.fog_density(),50.0f);
    this->attributes=OGLAttributes(this->matrices);
    //
    this->sound_engine=irrklang::createIrrKlangDevice(); //derp
    if(this->sound_engine==NULL)
    {
        dprint("Sound engine creation failed.");
        _Init(this->sound_engine);
    }
    //
    InputUpdate update_data=this->in_events.GetInputUpdate();
    update_data.trigger_camera_update=true;
    this->sc_camera.Update(this->matrices.projection,this->matrices.view,this->uniforms.eye_position(),this->uniforms.projection_view_matrix(),update_data,this->sound_engine);
    this->sections.UpdateCurrentSection(this->sc_camera.eye[0],this->sc_camera.eye[1],this->sc_camera.eye[2],this->sc_camera.angle_side);
    // ~ Activate Input Events
    this->in_events.Activate();
    //
    if(EN_GRIDLINES==1)
    {
        for(int i=0; i<3; i++)
        {
            LineStrip lines=this->sections.GetGridlines(i);
            this->AddObject(&lines);
        }
    }
    //
	return;
}
GLint Scene::RemoveObject(ptrRenObj renobj_in)
{
    GLint retval=_BAD;
    if(renobj_in->GetForeground())
    {
        this->foregnd_objs.remove(renobj_in);
        this->total_object_count--;
        retval= _OK;
    }
    else if(!renobj_in->GetInternalClipping())
    {
        this->persistant_objs.remove(renobj_in);
        this->total_object_count--;
        retval= _OK;
    }
    else
    {
        if(this->sections.RemoveObject(renobj_in))
        {
            this->total_object_count--;
            retval= _OK;
        }
    }

    return retval;
}
bool Scene::ToggleLambert(void)
{
    this->islambert=!this->islambert;
    glUniform1i(this->uniforms.diffuse_enabled(),this->islambert);
    return this->islambert;
}
bool Scene::TogglePhong(void)
{
    this->isphong=!this->isphong;
    glUniform1i(this->uniforms.specular_enabled(),this->isphong);
    return this->isphong;
}
bool Scene::ToggleBumpMapping(void)
{
    this->isbumpmap=!this->isbumpmap;
    return this->isbumpmap;
}
bool Scene::ToggleTextures(void)
{
    this->global_texture_enable=!this->global_texture_enable;
    return this->global_texture_enable;
}
bool Scene::ToggleWireframe(void)
{
    if(this->wireframe_enabled)
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    return this->wireframe_enabled=!this->wireframe_enabled;
}
bool Scene::GlobalTextureEn(void)
{
    return this->global_texture_enable;
}
bool Scene::CameraQuery(void)
{
    if(this->camera_changed)
    {
        this->camera_changed=false;
        return true;
    }
    else
        return false;
}
bool Scene::BumpMapEn(void)
{
    return this->isbumpmap;
}
GLint Scene::AddLight(GLfloat * light_position, GLfloat * light_color, GLfloat * ambient_color, BlenderObj* renobj_in)
{
    glm::mat4 mvmat=this->matrices.translation_rotation_scale;
    GLfloat ambient_out[4];
    memcpy(ambient_out,ambient_color,sizeof(GLfloat)*4);
    this->islambert=true; this->isphong=true;  this->isbumpmap=true;
    glUniform1i(this->uniforms.bumpmapping_enabled(),true);
    glUniform1i(this->uniforms.diffuse_enabled(),true);
    glUniform1i(this->uniforms.specular_enabled(),true);
    if(renobj_in!=nullptr)
    {
        renobj_in->SetOrigin(light_position[0],light_position[1],light_position[2]);
        this->AddObject(renobj_in);
    }
    
    std::string block_name_str="SourceLighting";
    glBindBuffer(GL_UNIFORM_BUFFER,this->uniforms.GetUniformBlockBuffer(block_name_str));
    std::string uniform_name_str="lightposn";
    //glBufferSubData(GL_UNIFORM_BUFFER,this->uniforms.GetOffset(block_name_str,uniform_name_str),this->uniforms.GetSize(block_name_str,uniform_name_str),position_out);
    glBufferSubData(GL_UNIFORM_BUFFER,this->uniforms.GetOffset(block_name_str,uniform_name_str),this->uniforms.GetSize(block_name_str,uniform_name_str),light_position);
    uniform_name_str="lightcolor";
    glBufferSubData(GL_UNIFORM_BUFFER,this->uniforms.GetOffset(block_name_str,uniform_name_str),this->uniforms.GetSize(block_name_str,uniform_name_str),light_color);
    uniform_name_str="ambient";
    glBufferSubData(GL_UNIFORM_BUFFER,this->uniforms.GetOffset(block_name_str,uniform_name_str),this->uniforms.GetSize(block_name_str,uniform_name_str),ambient_out);
    
    return _OK;
}
void Scene::UpdateLightPosition(GLfloat * light_position)
{
    glm::mat4 mvmat=this->matrices.translation_rotation_scale;
    std::string block_name_str="SourceLighting";
    glBindBuffer(GL_UNIFORM_BUFFER,this->uniforms.GetUniformBlockBuffer(block_name_str));
    std::string uniform_name_str="lightposn";
    //glBufferSubData(GL_UNIFORM_BUFFER,this->uniforms.GetOffset(block_name_str,uniform_name_str),this->uniforms.GetSize(block_name_str,uniform_name_str),position_out); 
    glBufferSubData(GL_UNIFORM_BUFFER,this->uniforms.GetOffset(block_name_str,uniform_name_str),this->uniforms.GetSize(block_name_str,uniform_name_str),light_position);
    
    return;
}
GLint Scene::Draw(void)
{
    GLint retval=_OK;
    // ~ Clear Screen
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	// ~ Reset Matrices
	//--- Draw Objects ---//
    glBindVertexArray(OGLBuffers::vao(0));
    // ~ Draw 3D Objects
    this->sections.Draw(this->current_id,this->uniforms,this->matrices,this->global_texture_enable,this->isbumpmap);
	std::list<ptrRenObj>::iterator i;
    for(i=this->persistant_objs.begin(); i!= this->persistant_objs.end(); i++)
    {
        // ~ Draw
		(*i)->Draw(this->current_id,this->uniforms,this->matrices,this->global_texture_enable,this->isbumpmap);
        this->current_id=(*i)->GetRenderID();
    }
    //--- Draw Foreground Objects ---//
    // ~ Reset SRT Matrices
	this->matrices.scale=glm::mat4(1.0);
	this->matrices.rotation=glm::mat4(1.0);
    // ~ Update View Matrices for 2D
    this->matrices.projection=glm::ortho(0.0f, (GLfloat)winsize[0],(GLfloat)winsize[1],0.0f, -1.0f, 1.0f);
    this->matrices.view=glm::mat4(1.0);
    this->matrices.projection_view=this->matrices.projection*this->matrices.view;
    glUniformMatrix4fv(this->uniforms.projection_view_matrix(),1,GL_FALSE,glm::value_ptr(this->matrices.projection_view));
    // ~ Iterate Through Foreground Objects
    for(i=foregnd_objs.begin(); i!= foregnd_objs.end(); i++)
    {
        // ~ Draw
        (*i)->Draw(this->current_id,this->uniforms,this->matrices,this->global_texture_enable,this->isbumpmap);
        this->current_id=(*i)->GetRenderID();
    }
    // ~ Reset ID (for next render cycle)
    this->current_id=_BAD;
    glBindVertexArray(0);
    //
    this->HandleInput();
    if(!this->exitbool)
        glfwSwapBuffers();
    else
        retval=_BAD;
	return retval;
}
void Scene::HandleInput(void)
{
    InputUpdate update_data=this->in_events.GetInputUpdate();
    this->exitbool=update_data.exitbool;
    if(this->exitbool)
    {
        glfwCloseWindow();
        return;
    }
    if(update_data.pending_placement)
        this->AddDerivedObj(this->placement_obj);
    if(update_data.pending_removal)
        this->RemoveNearObject();
    if(update_data.toggle_bumpmapping)
        this->ToggleBumpMapping();
    if(update_data.toggle_diffuse)
        this->ToggleLambert();
    if(update_data.toggle_specular)
        this->TogglePhong();
    if(update_data.toggle_texture)
        this->ToggleTextures();
    if(update_data.toggle_wireframe)
        this->ToggleWireframe();
    if(update_data.trigger_camera_update)
        this->camera_changed=true;
    // ~ Update Adjacent Sections (if applicable)                                                                                                                                                                     
    if(this->sc_camera.Update(this->matrices.projection,this->matrices.view,this->uniforms.eye_position(),this->uniforms.projection_view_matrix(),update_data,this->sound_engine)== _OK)
    {
        this->sections.UpdateCurrentSection(this->sc_camera.eye[0],this->sc_camera.eye[1],this->sc_camera.eye[2],this->sc_camera.angle_side);

    }
}
Scene::~Scene(void)
{
    ShaderProgram::FreeShaders();
    std::list<ptrRenObj>::iterator i;
    RenObj * ptr_out;
    _Init(ptr_out);
    while(this->persistant_objs.size()!=0)
    {
        this->persistant_objs.pop_front();
    }
    while(this->foregnd_objs.size()!=0)
    {
        this->foregnd_objs.pop_front();
    }
    std::vector<irrklang::ISound*>::iterator j;
    for(j=this->sound_objs.begin(); j!= this->sound_objs.end(); j++)
        (*j)->drop();
    this->sound_engine->drop();
    TextureMaster::FreeTextures();
    this->uniforms.ReleaseBuffers();
    OGLBuffers::Cleanup();
    return;
}
GLuint Scene::GetOccupiedSectionIndex(void)
{
    return this->sections.GetCurrentSection();
}
void Scene::GetOccupiedSectionIndices(GLint indices_out[3])
{
    this->sections.GetOccupiedSectionIndices(indices_out);
}
long int Scene::GetObjCount(void)
{
    return this->total_object_count;
}
long int Scene::IncrementTotal(void)
{
    return ++(this->total_object_count);
}
GLint Scene::RemoveNearObject(void)
{
    GLint retval=_BAD;
    if(!this->sections.RemoveNearObject(this->sc_camera.center[0],this->sc_camera.center[1],this->sc_camera.center[2]))
    {
        std::list<ptrRenObj>::iterator i;
        glm::vec3 camera(this->sc_camera.center[0],this->sc_camera.center[1],this->sc_camera.center[2]);
        GLfloat least_distance=-1.0;
        ptrRenObj closest_obj;
        glm::vec3 difference(1.0);
        GLfloat distance=0.0;
        GLfloat local_origin[3];
        for(i=this->persistant_objs.begin(); i!=this->persistant_objs.end(); i++)
        {
            (*i)->GetOrigin(local_origin);
            difference=glm::vec3(local_origin[0]-camera.x,local_origin[1]-camera.y,local_origin[2]-camera.z);
            distance=sqrt(difference.x*difference.x+difference.y*difference.y+difference.z*difference.z);
            if (least_distance==-1.0 || distance<least_distance)
            {
                if(distance<=(*i)->GetSelectionRadius())
                {
                    least_distance=distance;
                    closest_obj=*i;
                }
            }
        }
        if(closest_obj)
        {
            this->RemoveObject(closest_obj);
            retval= _OK;
        }
    }
    return retval;
}
bool Scene::ExitQuery(void)
{
    return this->exitbool;
}
void Scene::AddDerivedObj(ptrRenObj obj_in)
{
    obj_in.get()->SetOrigin(this->sc_camera.center[0],this->sc_camera.center[1],this->sc_camera.center[2]);
    ptrRenObj local_reference;
    local_reference=std::dynamic_pointer_cast<Group>(obj_in);
    if (local_reference.get()!=nullptr)
    {
        this->AddObject((Group*)local_reference.get());
        return;
    }  
    local_reference=std::dynamic_pointer_cast<BlenderObj>(obj_in);
    if (local_reference.get()!=nullptr)
    {
        this->AddObject((BlenderObj*)local_reference.get());
        return;
    }  
    local_reference=std::dynamic_pointer_cast<Cube>(obj_in);
    if (local_reference.get()!=nullptr)
    {
        this->AddObject((Cube*)local_reference.get());
        return;
    }
    local_reference=std::dynamic_pointer_cast<Floor>(obj_in);
    if (local_reference.get()!=nullptr)
    {
        this->AddObject((Floor*)local_reference.get());
        return;
    }
    local_reference=std::dynamic_pointer_cast<TextOverlay>(obj_in);
    if (local_reference.get()!=nullptr)
    {
        this->AddObject((TextOverlay*)local_reference.get());
        return;
    }
    local_reference=std::dynamic_pointer_cast<BlockArray>(obj_in);
    if (local_reference.get()!=nullptr)
    {
        this->AddObject((BlockArray*)local_reference.get());
        return;
    }
    return;
}
irrklang::ISound* Scene::AddSound(std::string file_in,glm::vec3 location_in,GLfloat min_in)
{
    if(!_Valid(this->sound_engine))
    {
        return (irrklang::ISound*)_BAD;
    }
    irrklang::vec3df location_out(location_in.x,location_in.y,location_in.z);
    irrklang::ISound* bkgnd_music = this->sound_engine->play3D(file_in.c_str(),location_out, true, false, true);
    bkgnd_music->setMinDistance(irrklang::ik_f32(min_in));
    bkgnd_music->setMaxDistance(irrklang::ik_f32(min_in*1000.0f));
    this->sound_objs.push_back(bkgnd_music);
    return bkgnd_music;
}

#pragma endregion
};