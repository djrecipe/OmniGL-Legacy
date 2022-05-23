//--- Pragmas ---//
#pragma once
//--- Includes ---//
#include "OGLEngine.h"
using namespace OmniOGL;
//--- Global Variables ---//
GLfloat initial_time=0.0;
// --- Function Defintions --- //
int render(void)
{
    // ~ Object Instantiation
    Scene scene;
    BlockArray block_array;
    BlockArray * ba = scene.AddObject(&block_array);
    GLfloat orig_out[3]={-(GLfloat)SECTION_SIZE/2.0f,(GLfloat)SECTION_SIZE/2.0f,14.0f};
    ba->SetOrigin(orig_out[0],orig_out[1],orig_out[2]);
    TextOverlay overlay(10.0);
    overlay.SetTexture("asciiMap.png");
    TextOverlay * bump_mapping_txt=scene.AddObject(&overlay);
    bump_mapping_txt->SetOrigin(10.0f,winsize[1]-185.0f,0.0f);
    TextOverlay * ttl_objs_txt=scene.AddObject(&overlay);
    ttl_objs_txt->SetOrigin(10.0f,winsize[1]-165.0f,0.0f);
    overlay.SetTextSize(27.0f);
    TextOverlay * section_txt=scene.AddObject(&overlay);
    section_txt->SetOrigin(10.0f,winsize[1]-150.0f,0.0f);
    TextOverlay * eye_txt=scene.AddObject(&overlay);
    eye_txt->SetOrigin(10.0f,winsize[1]-100.0f,0.0f);
    TextOverlay * center_txt=scene.AddObject(&overlay);
    center_txt->SetOrigin(10.0f,winsize[1]-50.0f,0.0f);
    //BlenderObj cobj("sphere.obj",0.1f);
    //cobj.SetColor(1.0,1.0,1.0,1.0);
    //BlenderObj lotus("lotus.obj",100.0f);//
    //lotus.SetLightingEnable(true);
    //srand((unsigned)time(0));
    // ~ Add Group of Cubes
    Cube cube(1000.0f);
    cube.SetTexture("box.jpg");
    cube.SetBumpMap("box_NRM.jpg");
    cube.SetOrigin(orig_out[0],orig_out[1],orig_out[2]);
    scene.AddObject(&cube);
    //LineStrip cube_meshes(vfield.GetCubeMeshes());
    //scene.AddObject(&cube_meshes);
    //LineStrip normals(temp_barr.GetNormals());
    //scene.AddObject(&normals);
    // ~ Capture Keyboard/Mouse Events
	scene.SetPlacementObject(&cube);
    // ~ Init Audio
    //scene.AddSound("MemoryLane.wav",glm::vec3(light_pos_out[0],light_pos_out[1],light_pos_out[2]),500.0);
    dprint("\nINITIALIZATION COMPLETE IN %f SECONDS!\n",(GLfloat)clock()/(GLfloat)CLOCKS_PER_SEC-initial_time);
    dprint("\n--------------------------------------\n");
    GLfloat light_pos_out[4]={0.0f,0.0f,0.0f,1.0f};
    GLfloat light_clr_out[4]={0.75f,0.73f,0.71f,0.85f};
    GLfloat ambient_clr_out[4]={197.7f/255.0f,169.0f/255.0f,144.0f/255.0f,0.3f};
    GLint section_indices[3]={-1,-1,-1};
    scene.AddLight(light_pos_out,light_clr_out,ambient_clr_out);
    //--- Main Program Loop ---//
    while (scene.Draw()!=_BAD)
    {
        ttl_objs_txt->SetText("# Objs: %li",scene.GetObjCount());
        if(scene.CameraQuery())
        {
            scene.GetOccupiedSectionIndices(section_indices);
            section_txt->SetText("Section: %i,%i,%i",section_indices[0],section_indices[1],section_indices[2]);
            eye_txt->SetText("Eye: %2.1f,%2.1f,%2.1f",scene.sc_camera.eye[0],scene.sc_camera.eye[1],scene.sc_camera.eye[2]);
            center_txt->SetText("Center: %2.1f,%2.1f,%2.1f",scene.sc_camera.center[0],scene.sc_camera.center[1],scene.sc_camera.center[2]);
            memcpy(light_pos_out,scene.sc_camera.eye,3*sizeof(GLfloat));
            scene.UpdateLightPosition(light_pos_out);
        }
        if(scene.BumpMapEn())
            bump_mapping_txt->SetText("Bump Mapping Enabled");
        else
            bump_mapping_txt->SetText("Bump Mapping Disabled");
    }
    
    return _OK;
}
int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,_In_ LPSTR lpCmdLine,_In_ int nCmdShow)
{
    initial_time=(GLfloat)clock()/(GLfloat)CLOCKS_PER_SEC;

    if(MakeWindow("OGL Engine")==_BAD)
        return 0;
    if(MyGLEWInit()==_BAD)
        return 0;

    render();
    return 0;
}
/*
GENERAL NOTES
===============+====================================================================================================================================================================================
03/10/2013     |    * TODO :: When attributes such as the color or normals for a given object are not relevant and
               |           they are not being used in the rendering equation then they should NOT be sent to the
               |           graphics card.
               |    * TODO :: Is it possible for triplanar texturing data to be computed before-hand / only upon
               |           voxel modification so-as to reduce the amount of fragment shader overhead?
---------------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/