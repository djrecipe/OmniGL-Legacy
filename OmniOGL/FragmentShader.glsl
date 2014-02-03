#version 330

in vec4 Color;
in vec2 Texcoord;
in vec4 myvertex;
in vec3 mynormal;
in mat4 modelview;
in float fogFactor;
in vec3 eyepos_in;

out vec4 outColor;

uniform sampler2D tex0;
uniform sampler2D tex1;


uniform bool isblack;
uniform bool istex;      // flag: use texture?
uniform bool islight;    // flag: use lighting?
uniform bool islambert;
uniform bool isphong;
uniform bool isvoxel;
uniform bool isbumpmap;

// ~ Light Properties
uniform SourceLighting {
    uniform vec4 lightposn ; 
    uniform vec4 lightcolor ;  
    uniform vec4 ambient ;
};
// ~ Material Properties
uniform MaterialLighting {
    uniform vec4 diffuse ; 
    uniform vec4 specular ; 
    uniform float shininess ; 
};

vec4 ComputeLight (const in vec3 direction, const in vec4 lightcolorin, const in vec3 normal, const in vec3 halfvec, const in vec4 mydiffuse, const in vec4 myspecular, const in float myshininess, const in float dist_in)
{

        vec4 lambert=vec4(0.0,0.0,0.0,0.0);
        vec4 phong=vec4(0.0,0.0,0.0,0.0);
        if(islambert)
        {
            float nDotL = dot(normal, direction)  ;         
            lambert = mydiffuse * lightcolorin * max (nDotL, 0.0) ;
        }
        if(isphong)
        {
            float nDotH = dot(normal, halfvec) ; 
            phong = myspecular * lightcolorin * pow (max(nDotH, 0.0), myshininess) ;
        }

        return (lambert + phong) * max(min(1.0-dist_in/5000.0,1.0),0.0);
} 

void main()
{
    vec4 texColor;
    vec3 blended_bump_vec;
    vec4 _mypos = modelview * myvertex ; 
    vec3 mypos = _mypos.xyz / _mypos.w ; // Dehomogenize current location
    vec3 blend_weights;
    vec4 output_color;
    if(!isvoxel)
    {
        if(istex)
        {
            texColor = texture2D(tex0,Texcoord);
        }
        if(isbumpmap)
        {
            blended_bump_vec=2.0*texture2D(tex1,Texcoord).rgb-1.0;
        }
    }
    else
    {

        float divisor=6000.0;
        blend_weights=max((abs(mynormal.xyz)- 0.2) *7,0);
        blend_weights /= (blend_weights.x + blend_weights.y + blend_weights.z);
        if(istex)
        { 
            vec4 sampleX = texture2D(tex0, mypos.yz/divisor); // Project along x axis
            vec4 sampleY = texture2D(tex0, mypos.zx/divisor); // Project along y axis
            vec4 sampleZ = texture2D(tex0, mypos.xy/divisor); // Project along z axis
            texColor=sampleX*blend_weights.x+sampleY*blend_weights.y+sampleZ*blend_weights.z;
        }
        if(isbumpmap)
        {
            vec2 bumpFetch1 = texture2D(tex1, mypos.yz/divisor).xy*2.0-1.0;  
            vec2 bumpFetch2 = texture2D(tex1, mypos.zx/divisor).xy*2.0-1.0;  
            vec2 bumpFetch3 = texture2D(tex1, mypos.xy/divisor).xy*2.0-1.0;  
            blended_bump_vec=vec3(vec3(0, bumpFetch1.x, bumpFetch1.y) * blend_weights.xxx + vec3(bumpFetch2.y, 0, bumpFetch2.x) * blend_weights.yyy +  vec3(bumpFetch3.x, bumpFetch3.y,0) * blend_weights.zzz);
        }

    }
    if(!islight)
    {
        if(istex)
        {
            if(!isblack)
                output_color = texColor;
            else
                output_color = vec4(0.0,0.0,0.0,texColor.w);
        }
        else
        {
            if(!isblack)
                output_color = vec4(Color);
            else
                output_color = vec4(0.0,0.0,0.0,Color.w);
        }
    }
    else { 
        vec3 eyedirn = normalize(eyepos_in - mypos) ; 
        mat4 inversetranspose=transpose(inverse(modelview));
        vec3 normal = normalize((inversetranspose*vec4(mynormal,0.0)).xyz);
        if(isbumpmap)
        {
            normal=normalize(normal+blended_bump_vec);
        }
        vec3 direction = normalize (lightposn.xyz/lightposn.w - mypos) ; // no attenuation 
        vec3 half = normalize (direction + eyedirn) ;
        float ldistance = distance(mypos,lightposn.xyz/lightposn.w); 
        vec4 col = ComputeLight(direction, lightcolor, normal, half, diffuse, specular, shininess, ldistance) ;
        vec4 final_color=col+ambient*ambient.w;
        if(istex)
        {
            //outColor=mix(final_color,texColor,0.5);
            output_color=texColor+final_color;
        }
        else
            output_color =vec4(Color)+final_color;
    }
    outColor=mix(output_color,ambient,fogFactor);
}