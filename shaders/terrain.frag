#version 330 core
precision mediump float;
precision highp sampler2DArray;

// in vec3 v_normal, v_tangent;
in vec3 v_pos;
in vec2 v_texCoord;
in vec4 v_texBlend;

in vec3 v_sunDirTanSpace;

in vec3 v_toCamera;
in float v_y;

in float v_shadowOpacity;

in vec4 shadowMapCoords;

out vec4 color;

uniform vec3 sunDir;
uniform float time;
uniform sampler2DArray terrainTextures;
// uniform sampler2D grassTexture;
uniform int backgroundTerrainLayer;
uniform vec4 terrainLayers, specularity, textureScale;
uniform ivec4 hasNormal, fadeBlend;

uniform highp sampler2DShadow shadowBuffer;

// Returns a random number based on a vec3 and an int.
float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}


vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

void layer(int i, inout vec3 normal, inout vec4 color, inout float remainingA, inout float specular)
{
    
    float a = i == -1 ? remainingA : v_texBlend[i];
    if (a == 0.) return;
    float layer = i == -1 ? float(backgroundTerrainLayer) : terrainLayers[i];
    vec2 coord = v_texCoord * vec2(i == -1 ? 1. : textureScale[i]);
    vec4 rgbAndHeight = texture(terrainTextures, vec3(coord, layer));

    vec3 texNormal = (i == -1 || hasNormal[i] == 1) ? texture(terrainTextures, vec3(coord, layer + 1.)).xyz : vec3(.5, .5, 1);

    if (i >= 0)
    {
        if (fadeBlend[i] == 0)
        {
            float l = rgbAndHeight.a * a + .002;
            a = a * a >= l ? 1. : pow(min(1., a + l), 2.);
        }
        a = min(remainingA, a);
    }
    color.rgb += rgbAndHeight.rgb * a;
    normal += texNormal * a;
    remainingA -= a;
    if (i >= 0) specular += a * specularity[i];
}

void main() {
//    discard;

    // if (v_y < -.4) discard;

    float remainingA = 1.;

    float wetSand = min(1., max(0., .8 - v_y * 2.));
//    color.rgb = vec3(wetSand);
//    return;
    float specularA = wetSand * .1;

    color = vec4(0, 0, 0, 1);
    vec3 normal = vec3(0);

    layer(3, normal, color, remainingA, specularA);
    layer(2, normal, color, remainingA, specularA);
    layer(1, normal, color, remainingA, specularA);
    layer(0, normal, color, remainingA, specularA);
    layer(-1, normal, color, remainingA, specularA);

    normal *= 2.;
    normal -= 1.;

    normal = normal * (1. - wetSand) + vec3(0, 0, 1) * wetSand;
    color.rgb *= 1. - wetSand * .15;

    // light

    float lightEffect = max(.35, min(1., dot(vec3(0, 0, 1), v_sunDirTanSpace) * .4 + .3));
//    color.rgb = vec3(lightEffect);
//    return;


    float diffuseLight = dot(normal, v_sunDirTanSpace) * lightEffect + (1. - lightEffect);
    color.rgb *= diffuseLight;

    if (shadowMapCoords.x >= 0. && shadowMapCoords.x <= 1. && shadowMapCoords.y >= 0. && shadowMapCoords.y <= 1.)
    {
        float cosTheta = clamp( dot( normal, sunDir ), 0, 1);

        float bias = 0.005*tan(acos(cosTheta));
        bias = clamp(bias, 0,0.01);

        float shadow=1.0;

        for (int i=0;i<4;i++){
            // use either :
            //  - Always the same samples.
            //    Gives a fixed pattern in the shadow, but no noise
            // int index = i;
            //  - A random sample, based on the pixel's position in world space.
            //    The position is rounded to the millimeter to avoid too much aliasing
            int index = int(16.0*random(floor(v_pos.xyz*1000.0), i))%16;
            
            // being fully in the shadow will eat up 4*0.2 = 0.8
            // 0.2 potentially remain, which is quite dark.
            shadow -= 0.2*(1.0-texture( shadowBuffer, vec3(shadowMapCoords.xy + poissonDisk[index]/700.0,  (shadowMapCoords.z-bias)/shadowMapCoords.w) ));
        }

        // float shadow = texture(shadowBuffer, shadowMapCoords.xyz);
        
        // color.rgb *= shadow * vec3(1, 0, 0);
        float shadowOpacity = v_shadowOpacity * .6;

        color.rgb *= shadow * shadowOpacity + (1. - shadowOpacity);

        specularA *= shadow * shadowOpacity + (1. - shadowOpacity);
    }

    vec3 reflectDir = reflect(v_sunDirTanSpace, normal);
    float specular = dot(reflectDir, normalize(v_toCamera));
    specular = min(1., max(0., specular));
    float dampedSpec = pow(specular, 10.);
    color.rgb += dampedSpec * specularA;

    // color.rgb = vec3(v_dayLight);
}

