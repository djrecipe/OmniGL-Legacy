/* File    : SmallOGL.cpp
   Updated : 8/14/2012
   Notes   : This is the primary execution/calling code.
*/
//--- Pragmas ---//
#pragma once
//--- Includes ---//
#include "OGLEngine.h"
using namespace OmniOGL;
//--- Global Variables ---//
float initial_time=0.0;
//#include <math.h>
// --- Function Defintions --- //
int render(GLuint shaderProgram,GLuint* vbo_render,GLuint* vao_render)
{
    // ~ Object Instantiation
    Scene scene(shaderProgram,vbo_render,vao_render);
    Floor floor;
    scene.AddObject(&floor);
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
    ComplexObj cobj("sphere.obj",0.1f);
    //srand((unsigned)time(0));
    // ~ Add Group of Cubes
    GLfloat color_out[4]={1.0f,1.0f,1.0f,1.0f};
    GLfloat orig_out[3]={-(GLfloat)SECTION_SIZE/2.0f,(GLfloat)SECTION_SIZE/2.0f,14.0f};
    Cube cube(30.0f);
    cube.SetTexture("txr_Box0.jpg");
    cube.SetOrigin(orig_out[0],orig_out[1],orig_out[2]);
    scene.AddObject(&cube);
    VoxelField vfield(90,5.0f,0.0f,0.0f,0.0f);
    BlockArray temp_barr;
    vfield.FillWithRandom(&temp_barr);
    temp_barr.SetTexture("darkseamless.jpg",true);
    temp_barr.SetBumpMap("darkseamless_NRM.jpg",true);
    //LineStrip cube_meshes(vfield.GetCubeMeshes());
    //scene.AddObject(&cube_meshes);
    //LineStrip normals(temp_barr.GetNormals());
    //scene.AddObject(&normals);
    scene.AddObject(&temp_barr);
    // ~ Capture Keyboard/Mouse Events
	CaptureInput(&scene);
    SetPlacementObject(&cobj);
    GLfloat light_pos_out[4]={0.0f,0.0f,0.0f,1.0f};
    temp_barr.GetOrigin(light_pos_out);
    GLfloat light_clr_out[4]={0.4f,0.43f,0.41f,1.0f};
    GLfloat ambient_clr_out[4]={0.0f,0.0f,0.0f,0.0f};
    scene.ApplyLighting(light_pos_out,light_clr_out,ambient_clr_out,&cobj);
    // ~ Init Audio
    //scene.AddSound("Kairo_Kingdom-Bounce.mp3",glm::vec3(light_pos_out[0],light_pos_out[1],light_pos_out[2]),250.0);
    scene.StartClock();
    dprint("\nINITIALIZATION COMPLETE IN %f SECONDS!\n",(float)clock()/(float)CLOCKS_PER_SEC-initial_time);
    dprint("\n--------------------------------------\n");
    //--- Main Program Loop ---//
    while (glfwGetWindowParam(GLFW_OPENED) && !ExitQuery())
    {
        ttl_objs_txt->SetText("# Objs: %li",scene.GetObjCount());
        section_txt->SetText("Section: %u",scene.GetOccupiedSectionIndex());
        eye_txt->SetText("Eye: %2.1f,%2.1f,%2.1f",scene.sc_camera.eye[0],scene.sc_camera.eye[1],scene.sc_camera.eye[2]);
        center_txt->SetText("Center: %2.1f,%2.1f,%2.1f",scene.sc_camera.center[0],scene.sc_camera.center[1],scene.sc_camera.center[2]);
        if(scene.BumpMapEn())
            bump_mapping_txt->SetText("Bump Mapping Enabled");
        else
            bump_mapping_txt->SetText("Bump Mapping Disabled");
        scene.UpdateTime();
        scene.Draw();
    }
    return _OK;
}
int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,_In_ LPSTR lpCmdLine,_In_ int nCmdShow)
{
/* Purpose: 
   Paramters: n/a
   Return: 0 = good, -1 = bad
   Notes: 
*/
    initial_time=(float)clock()/(float)CLOCKS_PER_SEC;
    // Shader Program
    GLuint vertshad, fragshad;
    GLuint local_vbo[MAX_VBO];
    GLuint local_vao[MAX_VAO];
    // Elements
    GLuint elementx[]={
        0,1,2,
        2,3,0,
        4,5,6,
        6,7,4
    };
    //--- Initialization ---//
    // ~ Init GLFW
    if(MakeWindow("OGL Engine")==_BAD)
        return 0;
    // ~ Init GLEW
    if(MyGLEWInit()==_BAD)
        return 0;
    // ~ Init/Configure Graphics Objects
    for(int i=0; i<2; i++)
        MakVao(local_vao);
    for(int i=0; i<5; i++)
        MakVbo(local_vbo);
    //MakEbo(elementx,sizeof(elementx)/sizeof(elementx[0]));
    // ~ Init Shader Program
    vertshad=MakVertShad("VertexShader.glsl");
    fragshad=MakFragShad("FragmentShader.glsl");
    GLuint shaderProgram=MakShadProg(vertshad,fragshad);
    //--- Rendering ---//
    render(shaderProgram,local_vbo,local_vao);
    //--- Termination ---//
    Recycle(local_vao,local_vbo,shaderProgram,vertshad,fragshad);
    glfwCloseWindow();
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