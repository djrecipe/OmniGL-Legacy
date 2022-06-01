#include "Blocks.h"
#include <glm/gtc/noise.hpp>
namespace OmniOGL
{
GLushort Voxel::EdgesByAxis[3][4]={{1,3,5,7},
                                        {8,9,10,11},
                                        {0,2,4,6}};
GLushort Voxel::EdgesByRegion[3][2][4]={{{0,4,8,9},{2,6,10,11}},{{0,1,2,3},{4,5,6,7}},{{3,7,8,11},{1,5,9,10}}};
GLushort Voxel::CornersOfEdge[12][2]={{0,1},{1,2},{3,2},{0,3},
                                            {4,5},{5,6},{7,6},{4,7},
                                            {0,4},{1,5},{2,6},{3,7}};
#pragma region Voxel::VerticeCount
GLchar Voxel::VerticeCount[]={0,3,3,6,3,6,6,9,3,6,6,9,6,9,9,6,3,6,6,9,6,9,9,12,6,9,9,12,9,12,12,9,3,6,6,9,6,9,9,12,6,9,9,12,9,12,12,
9,6,9,9,6,9,12,12,9,9,12,12,9,12,15,15,6,3,6,6,9,6,9,9,12,6,9,9,12,9,12,12,9,6,9,9,12,9,12,12,15,9,12,12,15,12,15,15,12,6,9,9,12,9,12,6,9,9,12,
12,15,12,15,9,6,9,12,12,9,12,15,9,6,12,15,15,12,15,6,12,3,3,6,6,9,6,9,9,12,6,9,9,12,9,12,12,9,6,9,9,12,9,12,12,15,9,6,12,9,12,9,15,6,6,9,9,12,
9,12,12,15,9,12,12,15,12,15,15,12,9,12,12,9,12,15,15,12,12,9,15,6,15,12,6,3,6,9,9,12,9,12,12,15,9,12,12,15,6,9,9,6,9,12,12,15,12,15,15,6,12,9,
15,12,9,6,12,3,9,12,12,15,12,15,9,12,12,15,15,6,9,12,6,3,6,9,9,6,9,12,6,3,9,6,12,3,6,3,3,0};
#pragma endregion
#pragma region Voxel::EdgesOfIndex
GLchar Voxel::EdgesOfIndex[256][15]={
{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{3,0,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{1,9,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{3,9,8,3,1,9,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{10,1,2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{10,1,2,0,8,3,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{10,0,2,10,9,0,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{2,8,3,2,10,8,10,9,8,-1,-1,-1,-1,-1,-1},
{11,2,3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{2,8,11,2,0,8,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{11,2,3,1,9,0,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{1,11,2,1,9,11,9,8,11,-1,-1,-1,-1,-1,-1},
{11,1,3,11,10,1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{0,10,1,0,8,10,8,11,10,-1,-1,-1,-1,-1,-1},
{3,9,0,3,11,9,11,10,9,-1,-1,-1,-1,-1,-1},
{9,8,10,10,8,11,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{8,4,7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{0,7,3,0,4,7,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{1,9,0,4,7,8,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{4,1,9,4,7,1,7,3,1,-1,-1,-1,-1,-1,-1},
{8,4,7,10,1,2,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{7,0,4,7,3,0,2,10,1,-1,-1,-1,-1,-1,-1},
{10,0,2,10,9,0,4,7,8,-1,-1,-1,-1,-1,-1},
{9,4,10,10,4,7,10,7,2,2,7,3,-1,-1,-1},
{4,7,8,11,2,3,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{11,4,7,11,2,4,2,0,4,-1,-1,-1,-1,-1,-1},
{9,0,1,3,11,2,6,8,4,-1,-1,-1,-1,-1,-1},
{2,1,11,11,1,9,11,9,7,7,9,4,-1,-1,-1},
{1,11,10,1,3,11,8,4,7,-1,-1,-1,-1,-1,-1},
{10,7,11,10,4,7,10,1,4,1,0,4,-1,-1,-1},
{3,9,0,3,11,9,11,10,9,0,7,8,-1,-1,-1},
{11,10,9,11,9,4,7,11,4,-1,-1,-1,-1,-1,-1},
{9,5,4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{3,0,8,9,5,4,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{1,4,0,1,5,4,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{8,5,4,8,3,5,3,1,5,-1,-1,-1,-1,-1,-1},
{4,9,5,1,2,10,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{3,0,8,9,5,4,6,1,2,-1,-1,-1,-1,-1,-1},
{5,2,10,5,4,2,4,0,2,-1,-1,-1,-1,-1,-1},
{4,8,5,5,8,3,5,3,10,10,3,2,-1,-1,-1},
{11,2,3,9,5,4,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{2,8,11,2,0,8,9,5,4,-1,-1,-1,-1,-1,-1},
{4,1,5,4,0,1,3,11,2,-1,-1,-1,-1,-1,-1},
{5,2,1,5,11,2,5,4,11,4,8,11,-1,-1,-1},
{11,1,3,11,10,1,5,4,9,-1,-1,-1,-1,-1,-1},
{0,10,1,0,8,10,8,11,10,1,4,9,-1,-1,-1},
{0,3,4,4,3,11,4,11,5,5,11,10,-1,-1,-1},
{8,11,10,8,10,5,4,8,5,-1,-1,-1,-1,-1,-1},
{8,5,7,8,9,5,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{9,3,0,9,5,3,5,7,3,-1,-1,-1,-1,-1,-1},
{0,7,8,0,1,7,1,5,7,-1,-1,-1,-1,-1,-1},
{5,7,1,1,7,3,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{8,5,7,8,9,5,1,2,10,-1,-1,-1,-1,-1,-1},
{9,3,0,9,5,3,5,7,3,0,10,1,-1,-1,-1},
{7,10,5,7,2,10,7,8,2,8,0,2,-1,-1,-1},
{5,7,3,5,3,2,10,5,2,-1,-1,-1,-1,-1,-1},
{5,8,9,5,7,8,11,2,3,-1,-1,-1,-1,-1,-1},
{0,9,2,2,9,5,2,5,11,11,5,7,-1,-1,-1},
{0,7,8,0,1,7,1,5,7,8,2,3,-1,-1,-1},
{1,5,7,1,7,11,2,1,11,-1,-1,-1,-1,-1,-1},
{8,5,7,8,9,5,3,11,10,3,10,1,-1,-1,-1},
{9,8,0,0,8,7,0,7,11,0,11,10,0,10,1},
{3,1,0,0,1,10,0,10,5,0,5,7,0,7,8},
{10,5,7,11,10,7,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{5,10,6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{3,0,8,5,10,6,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{0,1,9,10,6,5,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{3,9,8,3,1,9,10,6,5,-1,-1,-1,-1,-1,-1},
{5,2,6,5,1,2,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{5,2,6,5,1,2,0,8,3,-1,-1,-1,-1,-1,-1},
{9,6,5,9,0,6,0,2,6,-1,-1,-1,-1,-1,-1},
{8,5,9,8,6,5,8,3,6,3,2,6,-1,-1,-1},
{5,10,6,2,3,11,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{8,2,0,8,11,2,6,5,10,-1,-1,-1,-1,-1,-1},
{11,2,3,1,9,0,4,10,6,-1,-1,-1,-1,-1,-1},
{1,11,2,1,9,11,9,8,11,2,5,10,-1,-1,-1},
{6,3,11,6,5,3,5,1,3,-1,-1,-1,-1,-1,-1},
{1,0,5,5,0,8,5,8,6,6,8,11,-1,-1,-1},
{0,3,9,9,3,11,9,11,5,5,11,6,-1,-1,-1},
{9,8,11,9,11,6,5,9,6,-1,-1,-1,-1,-1,-1},
{8,4,7,5,10,6,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{0,7,3,0,4,7,5,10,6,-1,-1,-1,-1,-1,-1},
{8,4,7,5,10,6,2,9,0,-1,-1,-1,-1,-1,-1},
{4,1,9,4,7,1,7,3,1,9,6,5,-1,-1,-1},
{2,5,1,2,6,5,7,8,4,-1,-1,-1,-1,-1,-1},
{0,7,3,0,4,7,1,2,6,1,6,5,-1,-1,-1},
{9,6,5,9,0,6,0,2,6,5,8,4,-1,-1,-1},
{4,0,9,9,0,3,9,3,2,9,2,6,9,6,5},
{10,6,5,7,8,4,0,11,2,-1,-1,-1,-1,-1,-1},
{11,4,7,11,2,4,2,0,4,7,10,6,-1,-1,-1},
{8,4,7,5,10,6,1,9,0,2,3,11,-1,-1,-1},
{10,6,5,2,1,11,11,1,9,11,9,7,7,9,4},
{6,3,11,6,5,3,5,1,3,11,4,7,-1,-1,-1},
{6,2,11,11,2,1,11,1,0,11,0,4,11,4,7},
{8,4,7,0,3,9,9,3,11,9,11,5,5,11,6},
{6,2,11,2,9,11,9,4,11,11,4,7,-1,-1,-1},
{9,6,4,9,10,6,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{6,9,10,6,4,9,8,3,0,-1,-1,-1,-1,-1,-1},
{10,0,1,10,6,0,6,4,0,-1,-1,-1,-1,-1,-1},
{4,8,6,6,8,3,6,3,10,10,3,1,-1,-1,-1},
{1,4,9,1,2,4,2,6,4,-1,-1,-1,-1,-1,-1},
{1,4,9,1,2,4,2,6,4,9,3,0,-1,-1,-1},
{6,4,2,2,4,0,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{3,2,8,2,4,8,2,6,4,-1,-1,-1,-1,-1,-1},                          // 103 - REVERSED
{9,6,4,9,10,6,2,3,11,-1,-1,-1,-1,-1,-1},
{2,8,11,2,0,8,10,6,4,10,4,9,-1,-1,-1},
{10,0,1,10,6,0,6,4,0,1,11,2,-1,-1,-1},
{10,9,1,1,9,4,1,4,8,1,8,11,1,11,2},
{3,9,1,3,4,9,3,11,4,11,6,4,-1,-1,-1},
{0,2,1,1,2,11,1,11,6,1,6,4,1,4,9},
{6,4,0,6,0,3,11,6,3,-1,-1,-1,-1,-1,-1},
{11,6,4,8,11,4,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{7,10,6,7,8,10,8,9,10,-1,-1,-1,-1,-1,-1},
{10,0,9,10,3,0,10,6,3,6,7,3,-1,-1,-1},
{6,7,10,10,7,8,10,8,1,1,8,0,-1,-1,-1},
{7,3,1,7,1,10,6,7,10,-1,-1,-1,-1,-1,-1},
{6,7,2,2,7,8,2,8,1,1,8,9,-1,-1,-1},
{1,5,9,9,5,6,9,6,7,9,7,3,9,3,0},
{0,2,6,0,6,7,8,0,7,-1,-1,-1,-1,-1,-1},
{3,2,6,7,3,6,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{7,10,6,7,8,10,8,9,10,6,3,11,-1,-1,-1},
{11,8,7,7,8,0,7,0,9,7,9,10,7,10,6},
{11,2,3,6,7,10,10,7,8,10,8,1,1,8,0},
{10,9,1,9,7,1,7,11,1,1,11,2,-1,-1,-1},
{7,5,6,6,5,9,6,9,1,6,1,3,6,3,11},
{0,9,1,11,6,7,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{3,1,0,1,6,0,6,7,0,0,7,8,-1,-1,-1},
{11,6,7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{7,6,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{6,11,7,3,0,8,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{7,6,11,1,9,0,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{9,3,1,9,8,3,7,6,11,-1,-1,-1,-1,-1,-1},
{7,6,11,10,1,2,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{1,2,10,11,7,6,4,3,0,-1,-1,-1,-1,-1,-1},
{0,10,9,0,2,10,11,7,6,-1,-1,-1,-1,-1,-1},
{2,8,3,2,10,8,10,9,8,3,6,11,-1,-1,-1},
{6,3,7,6,2,3,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{7,0,8,7,6,0,6,2,0,-1,-1,-1,-1,-1,-1},
{6,3,7,6,2,3,1,9,0,-1,-1,-1,-1,-1,-1},
{2,1,6,6,1,9,6,9,7,7,9,8,-1,-1,-1},
{10,7,6,10,1,7,1,3,7,-1,-1,-1,-1,-1,-1},
{6,10,7,7,10,1,7,1,8,8,1,0,-1,-1,-1},
{9,6,10,9,7,6,9,0,7,0,3,7,-1,-1,-1},
{10,9,8,10,8,7,6,10,7,-1,-1,-1,-1,-1,-1},
{4,11,8,4,6,11,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{3,6,11,3,0,6,0,4,6,-1,-1,-1,-1,-1,-1},
{11,4,6,11,8,4,0,1,9,-1,-1,-1,-1,-1,-1},
{1,11,3,1,6,11,1,9,6,9,4,6,-1,-1,-1},
{4,11,8,4,6,11,10,1,2,-1,-1,-1,-1,-1,-1},
{3,6,11,3,0,6,0,4,6,11,1,2,-1,-1,-1},
{4,11,8,4,6,11,9,0,2,9,2,10,-1,-1,-1},
{2,0,3,3,0,9,3,9,4,3,4,6,3,6,11},
{8,2,3,8,4,2,4,6,2,-1,-1,-1,-1,-1,-1},
{4,6,0,0,6,2,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{8,2,3,8,4,2,4,6,2,3,9,0,-1,-1,-1},
{4,6,2,4,2,1,9,4,1,-1,-1,-1,-1,-1,-1},
{6,10,4,4,10,1,4,1,8,8,1,3,-1,-1,-1},
{0,4,6,0,6,10,1,0,10,-1,-1,-1,-1,-1,-1},
{8,11,3,3,11,6,3,6,10,3,10,9,3,9,0},
{6,10,9,4,6,9,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{9,5,4,6,11,7,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{5,4,9,8,3,0,2,7,6,-1,-1,-1,-1,-1,-1},
{1,4,0,1,5,4,6,11,7,-1,-1,-1,-1,-1,-1},
{8,5,4,8,3,5,3,1,5,4,11,7,-1,-1,-1},
{7,6,11,10,1,2,0,5,4,-1,-1,-1,-1,-1,-1},
{3,0,8,9,5,4,10,1,2,6,11,7,-1,-1,-1},
{5,2,10,5,4,2,4,0,2,10,7,6,-1,-1,-1},
{7,6,11,4,8,5,5,8,3,5,3,10,10,3,2},
{3,6,2,3,7,6,4,9,5,-1,-1,-1,-1,-1,-1},
{7,0,8,7,6,0,6,2,0,8,5,4,-1,-1,-1},
{6,3,7,6,2,3,5,4,0,5,0,1,-1,-1,-1},
{7,3,8,8,3,2,8,2,1,8,1,5,8,5,4},
{10,7,6,10,1,7,1,3,7,6,9,5,-1,-1,-1},
{5,4,9,6,10,7,7,10,1,7,1,8,8,1,0},
{5,1,10,10,1,0,10,0,3,10,3,7,10,7,6},
{5,1,10,1,8,10,8,7,10,10,7,6,-1,-1,-1},
{6,9,5,6,11,9,11,8,9,-1,-1,-1,-1,-1,-1},
{0,9,3,3,9,5,3,5,11,11,5,6,-1,-1,-1},
{5,6,1,1,6,11,1,11,0,0,11,8,-1,-1,-1},
{3,1,5,3,5,6,11,3,6,-1,-1,-1,-1,-1,-1},
{6,9,5,6,11,9,11,8,9,5,2,10,-1,-1,-1},
{1,2,10,0,9,3,3,9,5,3,5,11,11,5,6},
{6,4,5,5,4,8,5,8,0,5,0,2,5,2,10},
{2,0,3,0,5,3,5,6,3,3,6,11,-1,-1,-1},
{9,3,8,9,2,3,9,5,2,5,6,2,-1,-1,-1},
{6,2,0,6,0,9,5,6,9,-1,-1,-1,-1,-1,-1},
{0,4,8,8,4,5,8,5,6,8,6,2,8,2,3},
{2,1,5,6,2,5,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{10,11,6,6,11,3,6,3,8,6,8,9,6,9,5},
{10,11,6,11,0,6,0,9,6,6,9,5,-1,-1,-1},
{6,10,5,8,0,3,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{6,10,5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{7,10,11,7,5,10,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{10,7,5,10,11,7,3,0,8,-1,-1,-1,-1,-1,-1},
{7,10,11,7,5,10,9,0,1,-1,-1,-1,-1,-1,-1},
{3,9,8,3,1,9,11,7,5,11,5,10,-1,-1,-1},
{11,1,2,11,7,1,7,5,1,-1,-1,-1,-1,-1,-1},
{11,1,2,11,7,1,7,5,1,2,8,3,-1,-1,-1},
{2,11,0,0,11,7,0,7,9,9,7,5,-1,-1,-1},
{11,10,2,2,10,5,2,5,9,2,9,8,2,8,3},
{2,5,10,2,3,5,3,7,5,-1,-1,-1,-1,-1,-1},
{5,8,7,5,0,8,5,10,0,10,2,0,-1,-1,-1},
{2,5,10,2,3,5,3,7,5,10,0,1,-1,-1,-1},
{1,3,2,2,3,8,2,8,7,2,7,5,2,5,10},
{1,3,5,5,3,7,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{7,5,1,7,1,0,8,7,0,-1,-1,-1,-1,-1,-1},
{3,7,5,3,5,9,0,3,9,-1,-1,-1,-1,-1,-1},
{5,9,8,7,5,8,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{5,8,4,5,10,8,10,11,8,-1,-1,-1,-1,-1,-1},
{4,5,0,0,5,10,0,10,3,3,10,11,-1,-1,-1},
{5,8,4,5,10,8,10,11,8,4,1,9,-1,-1,-1},
{5,7,4,4,7,11,4,11,3,4,3,1,4,1,9},
{1,4,5,1,8,4,1,2,8,2,11,8,-1,-1,-1},
{3,7,11,11,7,4,11,4,5,11,5,1,11,1,2},
{9,10,5,5,10,2,5,2,11,5,11,8,5,8,4},
{4,5,9,3,2,11,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{4,5,8,8,5,10,8,10,3,3,10,2,-1,-1,-1},
{2,0,4,2,4,5,10,2,5,-1,-1,-1,-1,-1,-1},
{9,0,1,4,5,8,8,5,10,8,10,3,3,10,2},
{5,7,4,7,2,4,2,1,4,4,1,9,-1,-1,-1},
{5,1,3,5,3,8,4,5,8,-1,-1,-1,-1,-1,-1},
{4,5,1,0,4,1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{9,10,5,10,3,5,3,8,5,5,8,4,-1,-1,-1},
{4,5,9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{4,11,7,4,9,11,9,10,11,-1,-1,-1,-1,-1,-1},
{4,11,7,4,9,11,9,10,11,7,0,8,-1,-1,-1},
{11,1,10,11,0,1,11,7,0,7,4,0,-1,-1,-1},
{8,9,4,4,9,1,4,1,10,4,10,11,4,11,7},
{2,11,1,1,11,7,1,7,9,9,7,4,-1,-1,-1},
{3,0,8,2,11,1,1,11,7,1,7,9,9,7,4},
{4,0,2,4,2,11,7,4,11,-1,-1,-1,-1,-1,-1},
{8,9,4,9,2,4,2,11,4,4,11,7,-1,-1,-1},
{10,2,9,9,2,3,9,3,4,4,3,7,-1,-1,-1},
{4,6,7,7,6,10,7,10,2,7,2,0,7,0,8},
{2,6,10,10,6,7,10,7,4,10,4,0,10,0,1},
{7,4,8,2,1,10,-1,-1,-1,-1,-1,-1,-1,-1,-1},                      // 235 - REVERSED
{1,3,7,1,7,4,9,1,4,-1,-1,-1,-1,-1,-1},
{4,6,7,6,1,7,1,0,7,7,0,8,-1,-1,-1},
{3,7,0,7,4,0,-1,-1,-1,-1,-1,-1,-1,-1,-1},                       // 238 - REVERSED
{7,4,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},                    // 239 - REVERSED
{10,11,9,9,11,8,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{9,10,11,9,11,3,0,9,3,-1,-1,-1,-1,-1,-1},
{10,11,8,10,8,0,1,10,0,-1,-1,-1,-1,-1,-1},
{1,10,11,3,1,11,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{11,8,9,11,9,1,2,11,1,-1,-1,-1,-1,-1,-1},
{1,5,9,5,11,9,11,3,9,9,3,0,-1,-1,-1},
{8,0,2,11,8,2,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{3,2,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{8,9,10,8,10,2,3,8,2,-1,-1,-1,-1,-1,-1},                        // 248
{0,9,10,2,0,10,-1,-1,-1,-1,-1,-1,-1,-1,-1},                     // 249
{0,4,8,4,10,8,10,2,8,8,2,3,-1,-1,-1},                           // 250
{2,1,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},                   // 251
{9,1,3,8,9,3,-1,-1,-1,-1,-1,-1,-1,-1,-1},                       // 252
{0,9,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},                    // 253
{8,0,3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},                    // 254
{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};                // 255
#pragma endregion
GLuchar Voxel::InvertedIndices[]={31,47,55,59,63,79,103,110,111,118,119,125,127,143,155,157,159,179,185,187,190,191,205,206,
                                207,215,217,220,221,223,230,235,236,238,239,241,242,243,244,246,247,248,249,251,252,253,254};
Voxel::Voxel(GLfloat x_in, GLfloat y_in, GLfloat z_in, GLfloat radius_in)
{
    //
    this->origin[0]=x_in; this->origin[1]=y_in; this->origin[2]=z_in;
    this->radius=radius_in;
    this->mc_index=_BAD;
    // ~ Corners
    // 0
    GLfloat this_corner[3]={x_in-radius_in,y_in-radius_in,z_in-radius_in};
    memcpy(this->corners[0],this_corner,sizeof(GLfloat)*3);
    // 1
    this_corner[0]=x_in-radius_in; this_corner[1]=y_in-radius_in; this_corner[2]=z_in+radius_in;
    memcpy(this->corners[1],this_corner,sizeof(GLfloat)*3);
    // 2
    this_corner[0]=x_in+radius_in; this_corner[1]=y_in-radius_in; this_corner[2]=z_in+radius_in;
    memcpy(this->corners[2],this_corner,sizeof(GLfloat)*3);
    // 3
    this_corner[0]=x_in+radius_in; this_corner[1]=y_in-radius_in; this_corner[2]=z_in-radius_in;
    memcpy(this->corners[3],this_corner,sizeof(GLfloat)*3);
    // 4
    this_corner[0]=x_in-radius_in; this_corner[1]=y_in+radius_in; this_corner[2]=z_in-radius_in;
    memcpy(this->corners[4],this_corner,sizeof(GLfloat)*3);
    // 5
    this_corner[0]=x_in-radius_in; this_corner[1]=y_in+radius_in; this_corner[2]=z_in+radius_in;
    memcpy(this->corners[5],this_corner,sizeof(GLfloat)*3);
    // 6
    this_corner[0]=x_in+radius_in; this_corner[1]=y_in+radius_in; this_corner[2]=z_in+radius_in;
    memcpy(this->corners[6],this_corner,sizeof(GLfloat)*3);
    // 7
    this_corner[0]=x_in+radius_in; this_corner[1]=y_in+radius_in; this_corner[2]=z_in-radius_in;
    memcpy(this->corners[7],this_corner,sizeof(GLfloat)*3);
    this->num_vertices=0;
    return;
}  
Voxel & Voxel::operator=(const Voxel & voxel_in)
{
    for (GLuint i=0; i<3; i++)
    {
        this->origin[i]=voxel_in.origin[i];
        for(GLuint j=0; j<8; j++)
        {
            this->corners[j][i]=voxel_in.corners[j][i];
        }
    }
    this->radius=voxel_in.radius;
    this->mc_index=voxel_in.mc_index;
    return *this;
}
GLuint Voxel::SetMarchingCubesParameters(GLchar corner_vals[8],SurfaceNet & net_in, bool limit[3][2])
{
    this->mc_index=0;
    for(GLint i=0; i<8; i++)
    {
        if(corner_vals[i]<0)
            this->mc_index+=(GLshort)pow(2.0,i);
    }
    if(this->mc_index==0 || this->mc_index==255)
        return _OK;
    GLchar these_edges[15];
    memcpy(these_edges,Voxel::EdgesOfIndex[this->mc_index],sizeof(GLchar)*15);
    GLuint num_verts=3*(GLuint)Voxel::VerticeCount[this->mc_index];         
    GLfloat these_vertices[45];
    int neighbor_indices[3];
    assert(num_verts<=45);
    for(GLint i=0; i<15; i++)
    {
        bool limit_node=false;
        if (these_edges[i]==_BAD)
            break;
        GLshort this_corner=Voxel::CornersOfEdge[these_edges[i]][0];
        GLfloat this_ratio=((GLfloat)abs(corner_vals[this_corner]))/((GLfloat)(abs(corner_vals[this_corner])+abs(corner_vals[Voxel::CornersOfEdge[these_edges[i]][1]])));
        GLint this_axis;
        this_axis=_BAD;
        for(GLint axis_i=0; axis_i<3; axis_i++)
        {
            for(GLint axis_j=0; axis_j<4; axis_j++)
            {
                if (Voxel::EdgesByAxis[axis_i][axis_j]==these_edges[i])
                {
                    this_axis=axis_i;
                }
                if(this_axis!=(GLuint)_BAD)
                    break;
            }
            if(this_axis!=(GLuint)_BAD)
                break;
        }
        for(GLint j=0; j<3 && !limit_node; j++)
        {
            for (GLint k=0; k<2 && !limit_node; k++)
            {
                for(GLint m=0; m<4 && !limit_node; m++)
                {
                    if(these_edges[i]==Voxel::EdgesByRegion[j][k][m] && limit[j][k])
                        limit_node=true;
                }
            }
        }
        float node_vals[3];
        for(GLint j=0; j<3; j++)
        {
            node_vals[j]=this->corners[this_corner][j];
            if(this_axis==j)
                node_vals[j]+=(GLfloat)(2.0*this_ratio*this->radius);
            these_vertices[i*3+j]=node_vals[j];
            //dprint("v%i: %f\n",j+i*3,temp_vertices[j+i*j]);
        }
        neighbor_indices[i%3]=net_in.AddNode(glm::vec3(node_vals[0],node_vals[1],node_vals[2]),limit_node);
        if((i+1)%3==0)
        {
            for(int j=0; j<3; j++)
            {
                GLuint my_neighbors[2];
                GLuint my_neighbor_counter=0;
                for(int k=0; k<3; k++)
                {
                    if(neighbor_indices[k]!=neighbor_indices[j])
                        my_neighbors[my_neighbor_counter++]=neighbor_indices[k];

                }
                if(my_neighbor_counter>0)
                    net_in.AddNeighbors(neighbor_indices[j],my_neighbors,my_neighbor_counter);
            }
        }

    }
    this->num_vertices=(GLchar)num_verts;
    return this->num_vertices;
}
GLint Voxel::AddCubeMesh(LineStrip & line_in)
{
    for(GLint i=0; i<12; i++)
        line_in.AddInstance(this->corners[Voxel::CornersOfEdge[i][0]],this->corners[Voxel::CornersOfEdge[i][1]]);
    return _OK;
}
Voxel::~Voxel(void)
{
    return;
}
VoxelField::VoxelField(GLuint width_in, GLfloat each_vox_size, GLint index_x_in, GLint index_y_in,GLint index_z_in)
{
    this->width=width_in;
    this->widthplus=width_in+1;
    this->widthpluscubed=this->widthplus*this->widthplus*this->widthplus;
    this->width_squared=this->width*this->width;
    this->width_cubed=this->width_squared*this->width;
    this->v_width=this->width+1;
    this->v_w_squared=this->v_width*this->v_width;
    this->vox_size=each_vox_size;
    this->origin[0]=(GLfloat)index_x_in*(GLfloat)this->width*this->vox_size; this->origin[1]=(GLfloat)index_y_in*(GLfloat)this->width*this->vox_size; this->origin[2]=(GLfloat)index_z_in*(GLfloat)this->width*this->vox_size;
    this->index_offsets[0]=index_x_in*this->width; this->index_offsets[1]=index_y_in*this->width; this->index_offsets[2]=index_z_in*this->width;
    this->center[0]=this->origin[0]+(GLfloat)(index_x_in+1)*(GLfloat)this->width*this->vox_size;
    this->center[1]=this->origin[1]+(GLfloat)(index_y_in+1)*(GLfloat)this->width*this->vox_size;
    this->center[2]=this->origin[2]+(GLfloat)(index_z_in+1)*(GLfloat)this->width*this->vox_size;
    //dprint("Origin: %f,%f,%f || Center: %f,%f,%f",this->origin[0],this->origin[1],this->origin[2],this->center[0],this->center[1],this->center[2]);
    return;
}
GLint VoxelField::FillWithRandom(BlockArray & barr_in, sqlite3_stmt * stmt_in)
{
    this->vox.clear();
    this->vox.reserve(this->widthpluscubed);
    for(GLuint i=0; i<this->widthpluscubed; i++)         // generate a value for each voxel
    {
        GLint z_index=i/(this->v_w_squared);                                                   // the voxel's z index
        GLint y_index=(i-z_index*(this->v_w_squared))/this->v_width;                           // the voxel's y index
        GLint x_index=(i-z_index*(this->v_w_squared)-y_index*this->v_width);                   // the voxel's x index
        x_index+=index_offsets[0];
        y_index+=index_offsets[1];
        z_index+=index_offsets[2];
        //this->vox.push_back((GLchar)(glm::perlin(glm::vec3((GLfloat)x_index/130.1,(GLfloat)y_index/99.7,(GLfloat)z_index/5.2),glm::vec3(100.0f))*127.0));

        this->vox.push_back((GLchar)(glm::simplex(glm::vec3((GLdouble)x_index/79.7,(GLdouble)y_index/65.03,(GLdouble)z_index/30.261))*128.0));      // use the voxel's indices to seed a pseudo-random function
    }
    this->voxel_vec.clear();
    this->voxel_vec.reserve(width_cubed);
    SurfaceNet my_surface;
    for(GLuint i=0; i<width_cubed; i++)
    {
		//dprint("filling random %i", i);
        GLuint z_index=i/width_squared;
        GLuint y_index=(i-z_index*width_squared)/this->width;
        GLuint x_index=i-z_index*width_squared-y_index*this->width;
        Voxel temp_vox(this->origin[0]+(GLfloat)(x_index*this->vox_size*2.0),this->origin[1]+(GLfloat)(y_index*this->vox_size*2.0),this->origin[2]+(GLfloat)(z_index*this->vox_size*2.0),this->vox_size);
        GLchar these_params[8]={this->vox[x_index+y_index*v_width+z_index*v_w_squared],
                                this->vox[x_index+y_index*v_width+z_index*v_w_squared+v_w_squared],
                                this->vox[x_index+y_index*v_width+z_index*v_w_squared+v_w_squared+1],
                                this->vox[x_index+y_index*v_width+z_index*v_w_squared+1],
                                this->vox[x_index+y_index*v_width+z_index*v_w_squared+v_width],
                                this->vox[x_index+y_index*v_width+z_index*v_w_squared+v_width+v_w_squared],
                                this->vox[x_index+y_index*v_width+z_index*v_w_squared+v_width+v_w_squared+1],
                                this->vox[x_index+y_index*v_width+z_index*v_w_squared+v_width+1]};
        bool limit_out[3][2]={{x_index==0,x_index>=this->width-1},{y_index==0,y_index>=this->width-1},{z_index==0,z_index>=this->width-1}};
        //bool limit_out[3][2]={{false,false},{false,false},{false,false}};
        temp_vox.SetMarchingCubesParameters(these_params,my_surface,limit_out);
        this->voxel_vec.push_back(temp_vox);
    }
	my_surface.RelaxNodes(50);
    my_surface.BlendNormals(); 
	//my_surface.SimplifyMesh(2);
    //my_surface.BurnData(this->index_offsets,stmt_in);
    if(my_surface.vertices.size()>0)
        barr_in.UpdateVertices(my_surface.vertices,my_surface.normals,my_surface.element_indices,my_surface.colors,this->origin,this->center,this->width*this->vox_size);
    return _OK;
}
/*
Kisses VoxelField::GetKisses(GLint number_of_kisses)
{
    Kisses all_the_kisses;
    for(GLint i=0; i<=number_of_kisses; i++)
    {
        all_the_kisses[i] == Kisses;
    }
    return all_the_kisses;
}
*/
void VoxelField::GetCenter(GLfloat arr_in[3])
{
    memcpy(arr_in,this->center,3*sizeof(GLfloat));
}
LineStrip VoxelField::GetCubeMeshes(void)
{
    std::vector<Voxel>::iterator i;
    GLuint temp_counter=0;
    LineStrip all_cube_meshes;
    for(i=this->voxel_vec.begin(); i!=this->voxel_vec.end(); i++)
    {
        (*i).AddCubeMesh(all_cube_meshes);
        if(temp_counter%1000==0)
            dprint("Processing voxel outline mesh #%u\n",temp_counter);
        temp_counter++;
    }
    return all_cube_meshes;
}
bool VoxelField::LoadData(BlockArray & barr_in, sqlite3_stmt * stmt_in)
{
    SurfaceNet my_surface;
    GLuint indices_out[3]={this->index_offsets[0],this->index_offsets[1],this->index_offsets[2]};
    if(!my_surface.LoadData(indices_out,stmt_in))
        return false;
    else
        barr_in.UpdateVertices(my_surface.vertices,my_surface.normals,my_surface.element_indices,my_surface.colors, this->origin,this->center,this->width*this->vox_size);
    return true;
}
VoxelField::~VoxelField(void)
{
    return;
}
#pragma region NetNode Class
NetNode::NetNode(GLfloat x_in, GLfloat y_in, GLfloat z_in, bool limit_in):
    limited(limit_in),
    original_limit(limit_in),
    simplified(false),
    pending_limit(false),
	constraint(_BAD),
	normal_dev(0.0)
{
    this->vertex[0]=x_in; this->vertex[1]=y_in; this->vertex[2]=z_in;
    memcpy(this->new_vertex,this->vertex,sizeof(GLfloat)*3);
    return;
}
bool NetNode::IsLimited(void)
{
    return this->limited;
}
bool NetNode::IsOriginalLimit(void)
{
    return this->original_limit;
}
bool NetNode::AddNeighbor(GLuint index_in, glm::vec3 vertex_in)
{
	//
	if (vertex_in == this->GetVertex())
		return false;
	for (std::vector<GLuint>::iterator i = this->neighbors.begin(); i != this->neighbors.end(); i++)
    {
		if (index_in == (*i))
            return false;
	}
	this->neighbors.push_back(index_in);
	//
    return true;
}
int NetNode::AddNormal(glm::vec3 normal_in)
{
    this->normals.push_back(normal_in);
    return this->normals.size();
}
std::vector<GLuint> NetNode::GetNeighbors(void)
{
    return this->neighbors;
}
void NetNode::SetSimplified(void)
{
    this->simplified=true;
}
void NetNode::ClearSimplified(void)
{
    this->simplified=false;
}
bool NetNode::IsSimplified(void)
{
    return this->simplified;
}
bool NetNode::operator==(NetNode & node_in)
{
	return (this->GetVertex() == node_in.GetVertex());
}
void NetNode::UpdateNeighbors(GLuint old_in, GLuint new_in)
{
	std::vector<GLuint> delete_us;
	for (int i = 0; i < this->neighbors.size(); i++)
	{
		if (this->neighbors[i] == old_in)
		{
			if (std::find(this->neighbors.begin(), this->neighbors.end(), new_in) != this->neighbors.end())
				delete_us.push_back(i);
			else
				this->neighbors[i] = new_in;
		}
	}
	for (int i = 0; i < delete_us.size(); i++)
	{
		this->neighbors.erase(this->neighbors.begin() + delete_us[i]);
		for (int j = i; j < delete_us.size(); j++)
		{
			if (delete_us[j]>delete_us[i])
				delete_us[j]--;
		}
	}
	return;
}
void NetNode::UpdateNeighbors(GLuint old_in,bool subtract, bool erase)
{
	std::vector<GLuint> delete_us;
	for (int i = 0; i < this->neighbors.size(); i++)
	{
		if (erase && this->neighbors[i] == old_in)
			delete_us.push_back(i);
		if (subtract && this->neighbors[i]>old_in)
			this->neighbors[i]--;
	}
	for (int i = 0; i < delete_us.size(); i++)
	{
		this->neighbors.erase(this->neighbors.begin() + delete_us[i]);
		for (int j = i+1; j < delete_us.size(); j++)
		{
			if (delete_us[j]>delete_us[i])
				delete_us[j]--;
		}
	}
	return;
}
void NetNode::SetConstraint(GLfloat constraint_in)
{
	this->constraint = constraint_in;
}
GLfloat NetNode::GetConstraint(void)
{
	return this->constraint;
}
void NetNode::ClearNormals(void)
{
	this->normals.clear();
	this->blended_normal = glm::vec3(0.0, 0.0, 0.0);
	return;
}
GLfloat NetNode::GetDistance(GLfloat x_in, GLfloat y_in, GLfloat z_in)
{
	return glm::distance(glm::vec3(this->vertex[0], this->vertex[1], this->vertex[2]), glm::vec3(x_in, y_in, z_in)); 
}
void NetNode::SetBlendedNormal(glm::vec3 normal_in)
{
	//this->normals.push_back(normal_in);
    this->blended_normal=normal_in;
}
glm::vec3 NetNode::GetBlendedNormal(bool normalized)
{
    return ((normalized)?(this->blended_normal/glm::length(this->blended_normal)):this->blended_normal);
}
void NetNode::GetVertex(GLfloat * ptr_in)
{
    memcpy(ptr_in,this->vertex,sizeof(GLfloat)*3);
    return;
}				
void NetNode::SetVertex(glm::vec3 vertex_in)
{
	this->vertex[0] = vertex_in.x;
	this->vertex[1] = vertex_in.y;
	this->vertex[2] = vertex_in.z;
	memcpy(this->new_vertex, this->vertex, sizeof(GLfloat)* 3);
	return;
}
glm::vec3 NetNode::GetVertex(void)
{
	return glm::vec3(this->vertex[0], this->vertex[1], this->vertex[2]);
}
void NetNode::SetNormalDeviation(GLfloat deviation_in)
{
	this->normal_dev = deviation_in;
}
GLfloat NetNode::GetNormalDeviation(void)
{
	return this->normal_dev;
}
void NetNode::CommitNewVertex(void)
{
    if(this->limited)
        return;
    memcpy(this->vertex,this->new_vertex,sizeof(GLfloat)*3);
    return;
}

void NetNode::Limit(void)
{
    if(this->pending_limit)
        this->limited=true;
    return;
}
void NetNode::PendingLimit(void)
{
    this->pending_limit=true;
    return;
}
NetNode::~NetNode(void)
{
    return;
}
#pragma endregion
#pragma region SurfaceNet Class
SurfaceNet::SurfaceNet(void)
{
    return;
}
GLuint SurfaceNet::AddNode(glm::vec3 vertex_in, bool limit_in, bool blind_add)
{
    std::vector<glm::vec3>::iterator i;
    GLuint node_index=_BAD;
    GLuint local_counter=0;
    float distances[3]={0.0,0.0,0.0};
    for(i=this->vertices.begin(); i!=this->vertices.end(); i++)
    {
        if(glm::distance((*i),vertex_in)<10.0)
        {
            (*i)=glm::mix((*i),vertex_in,0.5);
            this->field[local_counter]->new_vertex[0]=(*i).x;
            this->field[local_counter]->new_vertex[1]=(*i).y;
            this->field[local_counter]->new_vertex[2]=(*i).z;
            this->field[local_counter]->CommitNewVertex();
            if(limit_in && !this->field[local_counter]->IsLimited())
            {
                this->field[local_counter]->PendingLimit();
                this->field[local_counter]->Limit();
                this->colors[local_counter]=glm::vec4(1.0,0.0,0.0,1.0);
            }
            node_index=local_counter;
            break;
        }
        local_counter++;
    }
    if(node_index==_BAD)
    {
        this->vertices.push_back(vertex_in);
        node_index=this->vertices.size()-1;
        NetNode * new_node=new NetNode(vertex_in.x,vertex_in.y,vertex_in.z,limit_in);
        this->field.push_back(new_node);
        if(blind_add)
            this->colors.push_back(glm::vec4(0.0,0.0,1.0,1.0));
        else if(limit_in)
            this->colors.push_back(glm::vec4(1.0,0.0,0.0,1.0));
        else
            this->colors.push_back(glm::vec4(0.5,0.5,0.5,1.0));
    }
    if(!blind_add)
        this->element_indices.push_back(node_index);
    return node_index;
}
bool SurfaceNet::RelaxNode(GLuint index_in)
{
	NetNode * thisn = this->field[index_in];
	if (thisn->limited)
		return false;
	glm::vec3 diff_vector(0.0, 0.0, 0.0);
	GLuint counter=0;
	for (std::vector<GLuint>::iterator i = thisn->neighbors.begin(); i != thisn->neighbors.end(); i++)
	{
		diff_vector += this->field[*i]->GetVertex() - thisn->GetVertex();
	}
	diff_vector /= thisn->neighbors.size();
	if (glm::length(diff_vector) > thisn->GetConstraint())
	{
		diff_vector *= thisn->GetConstraint() / glm::length(diff_vector);
	}
	glm::vec3 new_vec = thisn->GetVertex() + diff_vector;
	thisn->new_vertex[0] = new_vec.x;
	thisn->new_vertex[1] = new_vec.y;
	thisn->new_vertex[2] = new_vec.z;
	return true;
}
int SurfaceNet::AddNeighbors(GLuint target_index, GLuint * indices_in, GLuint neighbor_count)
{
	assert(target_index < this->field.size());
	if (neighbor_count < 1)
		return _BAD;
    NetNode * this_node=this->field[target_index];
    for(GLuint i=0; i<neighbor_count; i++)
    {
		if (indices_in[i] >= this->field.size())
		{
			dprint("index: %u || size: %u", indices_in[i], this->field.size());
		}
		assert(indices_in[i] < this->field.size());
        this_node->AddNeighbor(indices_in[i],this->field[indices_in[i]]->GetVertex());
    }
    return _OK;
}
int SurfaceNet::AddNormal(GLuint target_index, glm::vec3 normal_in)
{
    this->field[target_index]->AddNormal(normal_in);
    return _OK;
}
glm::vec3 SurfaceNet::CalculateNormal(GLuint indices_in[3], bool normalize)
{
	return OGLMath::Normal(this->field[indices_in[1]]->GetVertex() - this->field[indices_in[0]]->GetVertex(), this->field[indices_in[2]]->GetVertex() - this->field[indices_in[0]]->GetVertex(), normalize);
}
int SurfaceNet::SetBlendedNormal(GLuint target_index, glm::vec3 normal_in)
{
    this->field[target_index]->SetBlendedNormal(normal_in);
    return _OK;
}
void SurfaceNet::BlendNormals()
{
    GLuint these_indices[3];
	for (GLuint i = 0; i < this->field.size(); i++)
	{
		this->field[i]->ClearNormals();
	}
	this->normals.clear();
    for(GLuint i=0; i<this->element_indices.size(); i++)
    {
        these_indices[i%3]=this->element_indices[i];
        if(i%3==2)
        {
            for(int j=0; j<3; j++)
				this->field[these_indices[j]]->AddNormal(this->CalculateNormal(these_indices,false));
        }
    }
    //
    std::vector<glm::vec3>::iterator j;
    this->normals.clear();
    this->normals.reserve(this->field.size());
    for(GLuint i=0; i<this->field.size(); i++)
    {
        std::vector<glm::vec3> curr_normals=this->field[i]->normals;
        glm::vec3 blended_normal=glm::vec3(0.0);
        for(j=curr_normals.begin(); j!=curr_normals.end(); j++)
        {
            blended_normal+=(*j);
        }
        blended_normal/=curr_normals.size();
		blended_normal /= glm::length(blended_normal);
        this->normals.push_back(blended_normal);
        this->field[i]->SetBlendedNormal(blended_normal);
		GLfloat norm_angle = 0.0;
		for (j = curr_normals.begin(); j != curr_normals.end(); j++)
		{
			(*j) /= glm::length(*j);
			norm_angle += OGLMath::Angle(*j, blended_normal);
		}
		norm_angle /= curr_normals.size();
		this->field[i]->SetNormalDeviation(norm_angle);
    }
}
int SurfaceNet::GetNodeCount(void)
{
    return this->field.size();
}
bool SurfaceNet::GetVertices(GLfloat * float_ptr_in, GLuint start_index_in,GLuint count_in)
{
    if((start_index_in+count_in)/3>this->element_indices.size())
        return false;
    GLuint local_counter=0;
    for(GLuint i=start_index_in/3; i<(start_index_in+count_in)/3; i++)
    {
        assert(i<this->element_indices.size());
        assert(this->element_indices[i]<this->vertices.size());
        float_ptr_in[local_counter++]=this->vertices[this->element_indices[i]].x;
        float_ptr_in[local_counter++]=this->vertices[this->element_indices[i]].y;
        float_ptr_in[local_counter++]=this->vertices[this->element_indices[i]].z;
    }
    return true;
}
void SurfaceNet::SafeDeleteNodes(std::vector<GLuint> delete_us, std::vector<GLuint> new_in)
{
	//
	for (GLint iv = 0; iv<delete_us.size(); iv++)
	{
		//
		this->field.erase(this->field.begin() + delete_us[iv]);
		this->vertices.erase(this->vertices.begin() + delete_us[iv]);
		this->normals.erase(this->normals.begin() + delete_us[iv]);
		this->colors.erase(this->colors.begin() + delete_us[iv]);
		//
		for (std::vector<NetNode*>::iterator i = this->field.begin(); i != this->field.end(); i++)
			(*i)->UpdateNeighbors(delete_us[iv],true,false);
		for (std::vector<GLuint>::iterator p = this->element_indices.begin(); p != this->element_indices.end(); p++)
		{
			if ((*p) == delete_us[iv])
				(*p) = new_in[iv];
			if ((*p) > delete_us[iv])
				(*p)--;
		}
		//
		for (GLint ivv = iv + 1; ivv<delete_us.size(); ivv++)
		{
			if (delete_us[ivv]>delete_us[iv])
				delete_us[ivv]--;
			if (new_in[ivv]>delete_us[iv])
				new_in[ivv]--;
		}
		//*/
	}
	return;
}
void SurfaceNet::SafeDeleteNodes(std::vector<GLuint> delete_us)
{
	//
	for (GLint iv = 0; iv<delete_us.size(); iv++)
	{

		std::vector<GLuint> erase_us;
		GLuint pcounter = 0;
		for (std::vector<GLuint>::iterator p = this->element_indices.begin(); p != this->element_indices.end(); p++)
		{
			if (pcounter % 3 == 2)
			{
				if ((*p) == delete_us[iv] || ((*(p - 1)) == delete_us[iv]) || ((*(p - 2)) == delete_us[iv]))
				{
					erase_us.push_back(pcounter - 2);
					erase_us.push_back(pcounter - 1);
					erase_us.push_back(pcounter);
				}
				if ((*(p - 2)) > delete_us[iv])
					(*(p - 2))--;
				if ((*(p - 1)) > delete_us[iv])
					(*(p - 1))--;
				if ((*p) > delete_us[iv])
					(*p)--;
			}
			pcounter++;
		}
		for (int i = 0; i < erase_us.size(); i++)
		{
			this->element_indices.erase(this->element_indices.begin() + erase_us[i]);
			for (GLint ivv = i + 1; ivv<erase_us.size(); ivv++)
			{
				if (erase_us[ivv]>erase_us[i])
					erase_us[ivv]--;
			}
		}
		erase_us.clear();
		//
		this->field.erase(this->field.begin() + delete_us[iv]);
		this->vertices.erase(this->vertices.begin() + delete_us[iv]);
		this->normals.erase(this->normals.begin() + delete_us[iv]);
		this->colors.erase(this->colors.begin() + delete_us[iv]);
		// 
		for (std::vector<NetNode*>::iterator i = this->field.begin(); i != this->field.end(); i++)
			(*i)->UpdateNeighbors(delete_us[iv],true,false);
		//
		for (GLint ivv = iv + 1; ivv<delete_us.size(); ivv++)
		{
			if (delete_us[ivv]>delete_us[iv])
				delete_us[ivv]--;
		}
	}
	return;
}
GLint SurfaceNet::FindPartner(GLuint index_in, glm::vec3& vec_out)
{
	NetNode * thisn = this->field[index_in];
	vec_out = glm::vec3(0.0, 0.0, 0.0);
	if (thisn->limited || thisn->simplified)
		return _BAD;
	//
	std::vector<GLuint>::iterator i, j, k, m;
	std::vector<GLuint> oneighbors, cousins, far_cousins;
	// Iterate Through All Neighbors
	oneighbors = thisn->GetNeighbors();
	bool breakbool = false;
	for (i = oneighbors.begin(); i != oneighbors.end() && !breakbool; i++)
	{
		if (this->field[*i]->IsSimplified() || this->field[*i]->IsLimited())
			continue;
		// ~ Get Neighbors of Neighbor 
		assert(*i < this->field.size());
		cousins = this->field[*i]->GetNeighbors();
		for (j = cousins.begin(); j != cousins.end() && !breakbool; j++)
		{
			if (this->field[*j]->IsLimited() || this->field[*j]->IsSimplified())
				continue;
			// ~ Ensure Neighbor-of-Neighbor is NOT Same as Original Node
			if (*(this->field[*j]) == *thisn)
				continue;
			bool match_found = false;
			// ~ Ensure Neighbor-of-Neighbor is ALSO a Neighbor of Original
			for (k = oneighbors.begin(); k != oneighbors.end() && !match_found; k++)
			{
				if (this->field[*k]->IsSimplified() || this->field[*k]->IsLimited())
					continue;
				if (*(this->field[*k]) == *(this->field[*j]))
				{
					match_found = true;
					break;
				}
			}
			if (!match_found)
				continue;
			// Iterate Through Neighbors of Neighbor-of-Neighbor
			far_cousins = this->field[*j]->GetNeighbors();
			for (k = far_cousins.begin(); k != far_cousins.end() && !breakbool; k++)
			{
				if (this->field[*k]->IsLimited() || this->field[*k]->IsSimplified())
					continue;
				// ~ Ensure Cousin is NOT Same as Original Neighbor
				if (*(this->field[*k]) == *(this->field[*i]))
					continue;
				// Iterate Through Original Set of Neighbors Again
				for (m = oneighbors.begin(); m != oneighbors.end(); m++)
				{
					if (this->field[*m]->IsSimplified() || this->field[*m]->IsLimited())
						continue;
					// ~ Ensure 2nd Neighbor of Original & 3rd Degree Neighbor Are Equal
					if (*(this->field[*m]) == *(this->field[*k]))
					{
						glm::vec3 center = glm::mix(this->field[*j]->GetVertex(), thisn->GetVertex(), 0.5);
						GLfloat normal_angle = glm::degrees(glm::acos(glm::dot(thisn->GetBlendedNormal(), this->field[*j]->GetBlendedNormal()) / (glm::length(thisn->GetBlendedNormal())*glm::length(this->field[*j]->GetBlendedNormal()))));
						if (normal_angle <25.0 && thisn->GetNormalDeviation()<30.0 && this->field[*j]->GetNormalDeviation()<30.0)
						{
							thisn->SetSimplified();
							this->field[*j]->SetSimplified();
							vec_out = center;
							return *j;
						}
						else
							return _BAD;
					}
				}
			}
		}
	}
	return _BAD;
}
bool SurfaceNet::SimplifyNode(GLint index_in)
{
	NetNode * thisn = this->field[index_in];
	if (thisn->IsLimited() || thisn->IsSimplified())
		return false;
	std::vector<GLuint> neighbs = thisn->GetNeighbors();
	if (neighbs.size() < 3)
		return false;
	if (neighbs.size() <= 6 && this->field[index_in]->GetNormalDeviation()<1.0)
	{
		glm::vec3 composite_normal(0.0, 0.0, 0.0);
		for (GLuint i = 0; i < neighbs.size(); i++)
		{
			if (this->field[neighbs[i]]->IsLimited() || this->field[neighbs[i]]->IsSimplified())
				return false;
			composite_normal += this->field[neighbs[i]]->GetBlendedNormal();
		}
		composite_normal /= neighbs.size();
		composite_normal /= glm::length(composite_normal);
		if (OGLMath::Angle(thisn->GetBlendedNormal(), composite_normal) < 30.0)
		{
			GLuint pool_size = 1 + (neighbs.size() - 3);
			GLuint pool[3] = { _BAD, _BAD, _BAD };
			GLuint prevpool[3] = { _BAD, _BAD, _BAD };
			std::vector<GLuint> used, cousins;
			GLuint np1 = _BAD, backup=_BAD;
			for (GLuint i = 0; i < pool_size; i++)
			{
				#pragma region Iteration 1 / 3+
				if (i==0)
				{
					#pragma region Neighbor 1 / 3
					pool[0] = neighbs[0];
					if (neighbs.size() != 3)											// if NOT filling a triangle..
						used.push_back(pool[0]);										// ..remember pool[0] as "used"
					#pragma endregion
					#pragma region Neighbor 2 / 3
					if (neighbs.size() == 3)											// if filling a triangle..
						pool[1] = neighbs[1];											// ..vertex[1]=neighbor[1]
					else																// if NOT filling a triangle..
					{
						GLuint np_counter = 0;											// ..count neighbors	 
						cousins = this->field[pool[0]]->GetNeighbors();					// ..get neighbors of pool[0]
						for (GLuint j = 1; j < neighbs.size() && np_counter < 2; j++)	// ..iterate through neighbors of original node (skip pool[0])
						{
							for (GLuint k = 0; k < cousins.size(); k++)					// ..iterate through neighbors of pool[0]
							{
								if (cousins[k] == neighbs[j])							// ..check for shared neighbor
								{
									if (np_counter++ == 0)
									{
										pool[1] = cousins[k];
										used.push_back(pool[1]);
									}
									else
									{
										assert(np1 == _BAD);
										np1 = cousins[k]; 								// ..unused neighbor of pool[0] which also neighbor original node
										used.push_back(np1);
									}
									break;												// ..break to look for 2nd shared neighbor
								}
							}
						}
					}
					#pragma endregion
					#pragma region Neighbor 3 / 3
					if (neighbs.size() == 3)
						pool[2] = neighbs[2];
					else
					{
						backup = _BAD;
						std::vector<GLuint> p0n = this->field[pool[0]]->GetNeighbors();
						cousins = this->field[pool[1]]->GetNeighbors();
						for (GLuint j = 1; j < neighbs.size() && pool[2] == _BAD; j++)			// iterate through neighbors of original node (skip pool[0])
						{
							if (std::find(used.begin(), used.end(), neighbs[j]) == used.end())
							{
								if(backup==_BAD)
									backup = neighbs[j];
								for (GLuint k = 0; k < cousins.size() && pool[2] == _BAD; k++)
								{
									if (cousins[k] != neighbs[j] && std::find(used.begin(), used.end(), cousins[k]) == used.end())
										pool[2] = cousins[k];
								}
							}
						}
						if (pool[2] == _BAD)
							pool[2] = backup;
					}
					#pragma endregion
				}
				#pragma endregion
				#pragma region Iteration 2 / 3+
				else if (i == 1)
				{
					#pragma region Neighbor 1 / 3
					pool[0] = prevpool[0];
					#pragma endregion
					#pragma region Neighbor 2 / 3
					pool[1] = np1;
					#pragma endregion
					#pragma region Neighbor 3 / 3
					pool[2] = prevpool[2];
					#pragma endregion
				}
				#pragma endregion
				#pragma region Iteration 3 / 3+
				else
				{
					#pragma region Neighbor 1 / 3
					backup = _BAD;
					cousins = this->field[prevpool[2]]->GetNeighbors();						// get neighbors of endpoint
					for (GLuint j = 1; j < neighbs.size(); j++)								// iterate through neighbors of original node (skip pool[0])
					{
						if (std::find(used.begin(), used.end(), neighbs[j]) == used.end())	// find an unused neighbor
						{	
							if(backup==_BAD)												// if no backup has been established yet..
								backup = neighbs[j];										// ..remember unused neighbor as backup
							for (GLuint k = 0; k < cousins.size() && pool[0] == _BAD; k++)	// iterate through neighbors of end point
							{
								if (cousins[k] != neighbs[j])								// check if unused neighbor does NOT neighbor endpoint
								{
									backup = neighbs[j];									// remember as backup (overwrite any prev backup)
									std::vector<GLuint> used_query = this->field[neighbs[j]]->GetNeighbors();	// get neighbors of pool[0] prospect
									for (GLuint m = 0; m < used_query.size() && pool[0] == _BAD; m++)			// iterate through neighbors of prospect
									{
										if (std::find(used.begin(), used.end(), neighbs[j]) != used.end())		// if prospect has a used neighbor..
											pool[0] = cousins[k];
									}
								}
							}
						}
					}
					if (pool[0] == _BAD)
						pool[0] = backup;
					assert(pool[0] != _BAD);
					used.push_back(pool[0]);
					#pragma endregion
					#pragma region Neighbor 2 / 3
					backup = _BAD;
					cousins = this->field[pool[0]]->GetNeighbors();
					for (GLuint j = 0; j < neighbs.size() && pool[1]==_BAD; j++)												// iterate through neighbors of i
					{
						if (neighbs[j] != prevpool[2] && std::find(cousins.begin(), cousins.end(), neighbs[j]) != cousins.end())// i shares a neighbor with pool[0] (which is not pool[2])
						{
							if(backup==_BAD)
								backup = neighbs[j];
							if (std::find(used.begin(), used.end(), neighbs[j]) != used.end())
							{
								pool[1] = neighbs[j];
							}
						}
					}
					if (pool[1] == _BAD)
					{
						pool[1] = backup;
						used.push_back(pool[1]);
					}
					assert(pool[1] != _BAD);
					#pragma endregion
					#pragma region Neighbor 3 / 3
					pool[2] = prevpool[2];
					#pragma endregion
				}
				#pragma endregion
				if (pool[0] == pool[1] || pool[1] == pool[2] || pool[0] == pool[2])
					dprint("%u (%u): %u %u %u",i, neighbs.size(),pool[0], pool[1], pool[2]);
				assert(pool[0] != pool[1] && pool[1] != pool[2] && pool[0]!=pool[2]);
				if (pool[0] == _BAD)
					dprint("%u (%u): %u %u %u", i, neighbs.size(), pool[0], pool[1], pool[2]);
				assert(pool[0] != _BAD);
				if (pool[1] == _BAD)
					dprint("%u (%u): %u %u %u", i, neighbs.size(), pool[0], pool[1], pool[2]);
				assert(pool[1] != _BAD);
				if (pool[2] == _BAD)
					dprint("%u (%u): %u %u %u", i, neighbs.size(), pool[0], pool[1], pool[2]);
				assert(pool[2] != _BAD);
				//
				glm::vec3 new_normal = this->CalculateNormal(pool, true);
				GLfloat angle = OGLMath::Angle(thisn->GetBlendedNormal(true), new_normal);
				//assert(_finite(angle) && !_isnan(angle));
				if (!_finite(angle) || _isnan(angle))
				{
					if (glm::length(thisn->GetBlendedNormal(true) + new_normal)<glm::length(new_normal))
						angle = 180.0;
					else
						angle = 0.0;
				}
				for (GLuint i = 0; i < 3; i++)
				{
					this->element_indices.push_back(pool[(angle>90.0) ? neighbs.size() - i - 1 : i]);
				}
				for (std::vector<NetNode*>::iterator j = this->field.begin(); j != this->field.end(); j++)
					(*j)->UpdateNeighbors(index_in, false, true);
				//
				backup = _BAD;
				memcpy(prevpool, pool, 3 * sizeof(GLuint));
				memset(pool, _BAD, 3 * sizeof(GLuint));

			}
			for (int i = 0; i < neighbs.size(); i++)
			{
				this->field[neighbs[i]]->SetSimplified();
			}
			thisn->SetSimplified();
			return true;

		}
	}
	return false;
}
void SurfaceNet::SimplifyMesh(GLint iterations_in)
{
    std::vector<GLuint> delete_us;
	std::vector<GLuint> new_indices;
    for(int ii=0; ii<iterations_in; ii++)
	{
        // ~ Iterate Through All Currently-Existing Nodes
		for (GLuint i = 0; i<this->field.size(); i++)
        {
			if (this->field[i]->IsLimited() || this->field[i]->IsSimplified())
				continue;
            // ~ Determine if Current Node can be Combined with Another Node
			glm::vec3 new_vertex(0.0, 0.0, 0.0);
			GLint merge0_index = this->FindPartner(i, new_vertex);
            if(merge0_index!=_BAD)
            {       
				// ~ Update Vertex
				this->field[i]->SetVertex(new_vertex);
				this->vertices[i] = new_vertex;
                // ~ Update Normal Data
                glm::vec3 new_normal=glm::normalize(glm::mix(this->field[merge0_index]->GetBlendedNormal(),this->field[i]->GetBlendedNormal(),0.5));
				this->SetBlendedNormal(i, new_normal);
				this->normals[i] = new_normal;
				// ~ Find Zero-Area Triangles
				GLuint eindices[3] = { 0, 0, 0 };
				std::vector<GLuint> erase_me;
				for (GLuint j = 0; j < this->element_indices.size(); j++)
				{
					eindices[j % 3] = this->element_indices[j];
					if (j % 3 == 2)
					{
						GLuint found_counter = 0;
						for (GLuint k = 0; k < 3; k++)
						{
							if (eindices[k] == merge0_index || eindices[k] == i)
								found_counter++;
						}
						if (found_counter >= 2)
						{
							erase_me.push_back(j);
							erase_me.push_back(j - 1);
							erase_me.push_back(j - 2);
						}
					}
				}
				for (GLuint k = 0; k < erase_me.size(); k++)
				{
					this->element_indices.erase(this->element_indices.begin() + erase_me[k]);
					for (GLuint j = k + 1; j < erase_me.size(); j++)
					{
						if (erase_me[j]>erase_me[k])
							erase_me[j]--;
					}
				}
                // ~ Update Neighbors
				std::vector<GLuint> neighbors = this->field[merge0_index]->GetNeighbors();
				this->AddNeighbors(i, neighbors.data(), neighbors.size());
				for (std::vector<NetNode*>::iterator j = this->field.begin(); j != this->field.end(); j++)
					(*j)->UpdateNeighbors(merge0_index, i);
                // ~ Remember Erasures
				delete_us.push_back(merge0_index); new_indices.push_back(i);
            }

		}
		this->SafeDeleteNodes(delete_us, new_indices);
		delete_us.clear(); new_indices.clear();
		for (std::vector<NetNode*>::iterator i = this->field.begin(); i != this->field.end(); i++)
			(*i)->ClearSimplified();
		
		//this->DestroyInnerTriangles();
		
		for (GLint j = 0; j < this->field.size(); j++)
		{
			if (this->field[j]->IsLimited())
				continue;
			std::vector<GLuint> neighbs = this->field[j]->GetNeighbors();
			if (neighbs.size() <= 2)
			{
				bool breakbool = false;
				for (GLint k = 0; k < neighbs.size() && !breakbool; k++)
				{
					if (this->field[neighbs[k]]->IsLimited())
					{
						breakbool = true;
						break;
					}
				}
				if(!breakbool)
					delete_us.push_back(j);
			}
		}
		this->SafeDeleteNodes(delete_us);
		delete_us.clear();
		this->RelaxNodes(20);
		this->BlendNormals();
		for (GLuint i = 0; i < this->field.size(); i++)
		{
			if (this->SimplifyNode(i))
				delete_us.push_back(i);
		}
		this->SafeDeleteNodes(delete_us);
		delete_us.clear();
		for (std::vector<NetNode*>::iterator i = this->field.begin(); i != this->field.end(); i++)
			(*i)->ClearSimplified();
		this->RelaxNodes(20);
	    this->BlendNormals();
    }
}
void SurfaceNet::DestroyInnerTriangles()
{  /*
	std::vector<GLuint> delete_us;
	for (int i = 0; i < this->field.size(); i++)
	{
		if (this->field[i]->IsSimplified() || this->field[i]->IsLimited())
			continue;
		std::vector<GLuint> neighbors = this->field[i]->GetNeighbors();
		if (neighbors.size() == 3)
		{
			glm::vec3 x = this->field[i]->GetVertex();
			glm::vec3 a = this->field[neighbors[0]]->GetVertex();
			glm::vec3 b = this->field[neighbors[1]]->GetVertex();
			glm::vec3 c = this->field[neighbors[2]]->GetVertex();
			GLfloat ab = glm::distance(a, b);
			GLfloat ac = glm::distance(a, c);
			GLfloat bc = glm::distance(b, c);
			GLfloat xa = glm::distance(x, a);
			GLfloat xb = glm::distance(x, b);
			GLfloat xc = glm::distance(x, c);
			// abc
			GLfloat s = (ab + ac + bc) / 2.0;
			GLfloat area_abc = std::sqrt(s*(s-ab)*(s-ac)*(s-bc));
			// abx
			s = (ab + xb + xa) / 2.0;
			GLfloat area_abx = std::sqrt(s*(s - ab)*(s - xa)*(s - xb));
			// bcx
			s = (bc + xb + xc) / 2.0;
			GLfloat area_bcx = std::sqrt(s*(s - bc)*(s - xb)*(s - xc));
			// axc
			s = (ac + xa + xc) / 2.0;
			GLfloat area_axc = std::sqrt(s*(s - ac)*(s - xa)*(s - xc));
			// total
			GLfloat total_area = area_abx + area_bcx + area_axc;
			if (abs(total_area/area_abc-1.0)<0.01)
			{
				this->field[i]->SetSimplified();
				this->field[neighbors[0]]->SetSimplified();
				this->field[neighbors[1]]->SetSimplified();
				this->field[neighbors[2]]->SetSimplified();
				this->element_indices.push_back(neighbors[0]);
				this->element_indices.push_back(neighbors[1]);
				this->element_indices.push_back(neighbors[2]);
				delete_us.push_back(i);

				this->field[neighbors[0]]->UpdateNeighbors(i);
				this->field[neighbors[0]]->AddNeighbor(neighbors[1], this->field[neighbors[1]]->GetVertex());
				this->field[neighbors[0]]->AddNeighbor(neighbors[2], this->field[neighbors[2]]->GetVertex());
				this->field[neighbors[1]]->UpdateNeighbors(i);
				this->field[neighbors[1]]->AddNeighbor(neighbors[0], this->field[neighbors[0]]->GetVertex());
				this->field[neighbors[1]]->AddNeighbor(neighbors[2], this->field[neighbors[2]]->GetVertex());
				this->field[neighbors[2]]->UpdateNeighbors(i);
				this->field[neighbors[2]]->AddNeighbor(neighbors[0], this->field[neighbors[0]]->GetVertex());
				this->field[neighbors[2]]->AddNeighbor(neighbors[1], this->field[neighbors[1]]->GetVertex());
				for (GLint j = 0; j < this->field.size(); j++)
				{
					if (j == neighbors[0] || j == neighbors[1] || j == neighbors[2])
						continue;
					this->field[j]->UpdateNeighbors(i);
				}
				//dprint("Candidate Found @ %f,%f,%f", x.x, x.y, x.z);
			}
		}
	}
	this->SafeDeleteNodes(delete_us);
	for (std::vector<NetNode*>::iterator i = this->field.begin(); i != this->field.end(); i++)
		(*i)->ClearSimplified();  */
}
void SurfaceNet::SetSimplified(GLuint target_index)
{
    this->field[target_index]->SetSimplified();
}
void SurfaceNet::UpdateConstraints(void)
{
	for (std::vector<NetNode*>::iterator n = this->field.begin(); n != this->field.end(); n++)
	{
		(*n)->SetConstraint(-1.0);
		for (std::vector<GLuint>::iterator i = (*n)->neighbors.begin(); i != (*n)->neighbors.end(); i++)
		{
			GLdouble new_distance = glm::distance(this->field[*i]->GetVertex(),(*n)->GetVertex())*0.4;
			if (new_distance < (*n)->GetConstraint() || (*n)->GetConstraint() == -1.0)
				(*n)->SetConstraint(new_distance);
		}
	}

}
bool SurfaceNet::RelaxNodes(GLuint count_in)
{
    for(int c=0; c<count_in; c++)
	{
		this->UpdateConstraints();
		for (GLint i = 0; i < this->field.size(); i++)
			this->RelaxNode(i);
		for (GLint i = 0; i < this->field.size(); i++)
		{
			this->field[i]->CommitNewVertex();
			this->vertices[i] = this->field[i]->GetVertex();
		}
    }
    return true;
}

bool SurfaceNet::SpreadLimitOfNode(GLuint index_in)
{
	NetNode * thisn = this->field[index_in];
	if (!thisn->IsLimited())
		return false;
	for (std::vector<GLuint>::iterator i = thisn->neighbors.begin(); i != thisn->neighbors.end(); i++)
		this->field[*i]->PendingLimit();
	return true;
}

void SurfaceNet::SpreadLimits(GLint count)
{
    for(int j=0; j<count; j++)
    {
        for(int i=0; i<this->field.size(); i++)
            this->SpreadLimits(i);
		for (int i = 0; i<this->field.size(); i++)
            this->field[i]->Limit();
    }

}
void SurfaceNet::BurnData(GLuint indices_in[3], sqlite3_stmt * stmt_in)
{
    char * buffer = new char[500000];
    memset(buffer,'\0',500000*sizeof(char));
    int node_count=this->GetNodeCount();
	memcpy(buffer, &node_count, sizeof(GLint));
	GLuint offset = sizeof(GLint) / sizeof(char);
    std::vector<NetNode*>::iterator i;
    std::vector<GLuint>::iterator j;
    std::vector<glm::vec3>::iterator k;
    for(i=this->field.begin(); i!=this->field.end(); i++)
    {
        float vertex_out[3]={(*i)->vertex[0],(*i)->vertex[1],(*i)->vertex[2]};
        memcpy(&buffer[offset],vertex_out,3*sizeof(GLfloat));
        //dprint("float: %f",*((GLfloat*)&buffer[offset]));
        offset+=3*sizeof(GLfloat)/sizeof(char);
        memcpy(&buffer[offset],&(*i)->constraint,sizeof(GLdouble));
        //dprint("Constraint: %f",*((GLdouble*)&buffer[offset]));
        offset+=sizeof(GLdouble)/sizeof(char);
        memcpy(&buffer[offset],&(*i)->limited,sizeof(bool));
        //if((bool)buffer[offset])
        //    dprint("Limited");
        //else
        //    dprint("Not limited");
        offset+=sizeof(bool)/sizeof(char);
        /*
        GLuint temp=(*i)->neighbor_indices.size();
        memcpy(&buffer[offset],&temp,sizeof(GLuint));
        //dprint("Neighbor Count: %u",*((GLuint*)&buffer[offset]));
        offset+=sizeof(GLuint)/sizeof(char);
        for(j=(*i)->neighbor_indices.begin(); j!=(*i)->neighbor_indices.end(); j++)
        {
            memcpy(&buffer[offset],&(*j),sizeof(GLuint));
            offset+=sizeof(GLuint)/sizeof(char);
        }
        */
        /*
        GLuint normal_count=(*i)->normals.size();
        memcpy(&buffer[offset],&normal_count,sizeof(GLuint));
        offset+=sizeof(GLuint)/sizeof(char);
        for(k=(*i)->normals.begin(); k!=(*i)->normals.end(); k++)
        {
            float temp[3]={(*k).x,(*k).y,(*k).z};
            //dprint("Normal: %f,%f,%f",temp[0],temp[1],temp[2]);
            memcpy(&buffer[offset],temp,3*sizeof(GLfloat));
            offset+=3*sizeof(GLfloat)/sizeof(char);
        }
        */
        glm::vec3 blend_norm_out= (*i)->GetBlendedNormal();
        memcpy(&buffer[offset],&blend_norm_out,sizeof(glm::vec3));
        offset+=sizeof(glm::vec3)/sizeof(char);
    }
    GLuint element_count=this->element_indices.size();
    memcpy(&buffer[offset],&element_count,sizeof(GLuint));
    offset+=sizeof(GLuint)/sizeof(char);
    for(j=this->element_indices.begin(); j!=this->element_indices.end(); j++)
    {
        memcpy(&buffer[offset],&(*j),sizeof(GLuint));
        offset+=sizeof(GLuint)/sizeof(char);
    }

    sqlite3_bind_int(stmt_in,1,indices_in[0]);
    sqlite3_bind_int(stmt_in,2,indices_in[1]);
    sqlite3_bind_int(stmt_in,3,indices_in[2]);
    sqlite3_bind_blob(stmt_in,4,buffer, offset,SQLITE_TRANSIENT);
    GLint step_result=sqlite3_step(stmt_in);
    GLuint loop_count=0;
    while(step_result!=SQLITE_DONE && loop_count<1000000)
    {
        if(loop_count>1)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        step_result=sqlite3_step(stmt_in);
        loop_count++;
    }
    if(step_result!=SQLITE_DONE || loop_count>=1000000) 
    {
        dprint("BURN DATA || SQLITE STEP RESULT CODE: %u\nLOOP COUNT: %u",step_result,loop_count);
        assert(1==2);
    }
    delete[] buffer;
}
bool SurfaceNet::LoadData(GLuint indices_in[3], sqlite3_stmt * stmt_in)
{
    char * buffer;
    _Init(buffer);
    GLint blob_size=0;
    int step_result=SQLITE_DONE;
    GLuint loop_count=0;
    sqlite3_bind_int(stmt_in,1,indices_in[0]);
    sqlite3_bind_int(stmt_in,2,indices_in[1]);
    sqlite3_bind_int(stmt_in,3,indices_in[2]);
    while(step_result!=SQLITE_DONE && loop_count<1000000)
    {
        //if(loop_count>1)
        //    std::this_thread::sleep_for(std::chrono::milliseconds(10));
        step_result=sqlite3_step(stmt_in);
        if(step_result==SQLITE_ROW && loop_count==0)
        {
            blob_size=sqlite3_column_type(stmt_in,3);
            //if(blob_size==SQLITE_BLOB)
                //dprint("Confirmed Blob Recipient!");
            blob_size=sqlite3_column_bytes(stmt_in,3);
            if(blob_size!=0)
            {
                //_Delete(buffer);
                _New(buffer,blob_size);
                memcpy(buffer,sqlite3_column_blob(stmt_in,3),blob_size*sizeof(char));
            } 
        }
        loop_count++;
    }
    if(step_result!=SQLITE_DONE || loop_count>=1000000)
    {
        dprint("LOAD DATA || SQLITE STEP RESULT CODE: %u",step_result);
        assert(1==2);
    }
    sqlite3_reset(stmt_in);
    if(blob_size==0 || loop_count<2)
        return false;
    GLint new_node_count=0;
    memcpy(&new_node_count,buffer,sizeof(GLint));
    std::vector<NetNode*>::iterator ii;
    for(ii=this->field.begin(); ii!=this->field.end(); ii++)
        delete[] (*ii);
    this->field.clear();
    this->vertices.clear();
    this->field.reserve(new_node_count);
    this->vertices.reserve(new_node_count);
    GLuint offset=0;
    offset+=sizeof(GLint)/sizeof(char);
    GLfloat new_vertex[3];
    GLdouble new_constraint;
    bool new_limited;
    GLuint new_neighbor_count;
    GLuint * new_neighbor_indices;
    GLuint new_normal_count;
    glm::vec3 new_blended_normal;
    for(int i=0; i<new_node_count; i++)
    {
        memcpy(new_vertex,&buffer[offset],3*sizeof(GLfloat));
        offset+=3*sizeof(GLfloat)/sizeof(char);
        memcpy(&new_constraint,&buffer[offset],sizeof(GLdouble));
        offset+=sizeof(GLdouble)/sizeof(char);
        memcpy(&new_limited,&buffer[offset],sizeof(bool));
        offset+=sizeof(bool)/sizeof(char);
        GLint new_target_index= this->AddNode(glm::vec3(new_vertex[0],new_vertex[1],new_vertex[2]),new_limited);
        /*
        memcpy(&new_neighbor_count,&buffer[offset],sizeof(GLuint));
        assert(new_neighbor_count<10000);
        offset+=sizeof(GLuint)/sizeof(char);
        new_neighbor_indices=new GLuint[new_neighbor_count];
        for(int j=0; j<new_neighbor_count; j++)             
        {
            memcpy(&new_neighbor_indices[j],&buffer[offset],sizeof(GLuint));
            offset+=sizeof(GLuint)/sizeof(char);
        }
        this->AddNeighbors(new_target_index,new_neighbor_indices,new_neighbor_count);
        delete[] new_neighbor_indices;
        */
        /*
        memcpy(&new_normal_count,&buffer[offset],sizeof(GLuint));
        offset+=sizeof(GLuint)/sizeof(char);
        for(int j=0; j<new_normal_count; j++)
        {
            float temp[3];
            memcpy(temp,&buffer[offset],3*sizeof(GLfloat));
            offset+=3*sizeof(GLfloat)/sizeof(char);
            glm::vec3 temp_vec(temp[0],temp[1],temp[2]);
            this->AddNormal(new_target_index,temp_vec);
        }
        */
        memcpy(&new_blended_normal,&buffer[offset],sizeof(glm::vec3));
        this->field[new_target_index]->SetBlendedNormal(new_blended_normal);
        this->normals.push_back(new_blended_normal);
        offset+=sizeof(glm::vec3)/sizeof(char);
    }
    GLuint element_count;
    memcpy(&element_count,&buffer[offset],sizeof(GLuint));
    offset+=sizeof(GLuint)/sizeof(char);
    this->element_indices.clear();
    this->element_indices.reserve(element_count);
    for(int i=0; i<element_count; i++)
    {
        this->element_indices.push_back(*((GLuint*)(&buffer[offset])));
        offset+=sizeof(GLuint)/sizeof(char);
    }
    delete[] buffer;
    //this->BlendNormals();
    return true;
};
SurfaceNet::~SurfaceNet(void)
{
    std::vector<NetNode*>::iterator i;
    for(i=this->field.begin(); i!=this->field.end(); i++)
    {
        delete (*i);
    }
    return;
}
#pragma endregion
}