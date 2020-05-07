#version 330 core
precision mediump float;
precision highp sampler2DArray;

in vec3 v_planetNormal; // , v_tangent;
in vec3 v_normal; // , v_tangent;
in vec2 v_texCoords;
in float v_y;
// in vec3 v_sunDirTanSpace;
// in vec3 v_toCamera;
// in float v_shadowOpacity;
// in vec4 shadowMapCoords;

out vec4 color;

// uniform vec3 sunDir;
// uniform sampler2DArray terrainTextures;
uniform sampler2D grassTexture;
uniform sampler2D groundTexture;
uniform sampler2D rockTexture;
uniform sampler2D sandTexture;
uniform sampler2D snowTexture;

// uniform lowp sampler2DShadow shadowBuffer;
const float SLOPE_THRESHOLD = 0.5;
const float MIX_SLOPE_THRESHOLD = 0.1;

const float WATER_HEIGHT = 0.0,
            WATER_HEIGHT_DEEP = -5.,
            SAND_HEIGHT = 0.1,
            GRASS_HEIGHT = 0.5,
            ROCK_HEIGHT = 9.,
            SNOW_HEIGHT = 25.;

// const int   SAND_TEX = 0,
//             ROCK_TEX = 2,
//             GRASS_TEX = 4,
//             DEAD_GRASS_TEX = 5,
//             ROCK2_TEX = 6;

#define M_PI 3.1415926535897932384626433832795

const float GRASS_TRANSITION = 0.2; // SAND_HEIGHT + (1.0f/1.5f) * (GRASS_HEIGHT - SAND_HEIGHT);

vec3 createBaseColor() 
{
    // Texture colors
    vec3 GRASS_COLOR = texture(grassTexture, v_texCoords).rgb;
    vec3 GROUND_COLOR = texture(groundTexture, v_texCoords).rgb;
   
    vec3 WATER_COLOR_DEEP = vec3(28.5f / 255.f,48.0f/ 255.f,78.0f/ 255.f);
    vec3 SAND_COLOR = texture(sandTexture, v_texCoords).rgb;
   
    // Solid color
    if(v_y <= WATER_HEIGHT){
        return SAND_COLOR;
    } else if (v_y > WATER_HEIGHT && v_y <= SAND_HEIGHT){
        return SAND_COLOR;
    } else if (v_y > SAND_HEIGHT && v_y <= GRASS_HEIGHT){
        float mixCoeff = 1.0f;
        
        if(v_y < GRASS_TRANSITION){
            mixCoeff = (v_y-SAND_HEIGHT) / (GRASS_TRANSITION-SAND_HEIGHT);
        }
        
        return mix(SAND_COLOR, GRASS_COLOR, mixCoeff);
    } else {
        return GRASS_COLOR;
    } 
}

void applySteepRocks(inout vec3 color) {
    vec3 ROCK_COLOR = texture(rockTexture, v_texCoords).rgb;

    float slope = acos(dot(v_planetNormal, v_normal)) / M_PI;

    // Determine which texture to use based on height.
    if((slope < 0.2) && slope >= 0.15 )
    {
        float blendAmount = (slope - (0.15f)) * (1.0f / (0.2f - (0.15f)));
        color = mix(color, ROCK_COLOR, blendAmount);
    } else if (slope > 0.2f) {
        color = ROCK_COLOR;
    }

    vec3 SNOW_COLOR = texture(snowTexture, v_texCoords).rgb;

    if (v_y > ROCK_HEIGHT){
        color = mix(ROCK_COLOR, SNOW_COLOR, (v_y-ROCK_HEIGHT) / (SNOW_HEIGHT-ROCK_HEIGHT));
    }
}

    // color = vec3(slope / M_PI, 0, 0);

// void applyTerrainSlope(inout vec3 color) {
//     float slope = dot(gridNormal, vert);

//     if(abs(slope) < SLOPE_THRESHOLD && vheight_F > WATER_HEIGHT){
//         float x = 1.0f - (abs(slope)/SLOPE_THRESHOLD - MIX_SLOPE_THRESHOLD);// triangle centered in 0,
//                                                     //maximum at 0 at y = 1, corners at -1 and 1
//         if(slope < -MIX_SLOPE_THRESHOLD){
//             heightCol = mix(heightCol, ROCK_COLOR, smoothstep(-SLOPE_THRESHOLD, -MIX_SLOPE_THRESHOLD, x));
//         }else if(slope > MIX_SLOPE_THRESHOLD){
//             heightCol = mix(heightCol, ROCK_COLOR, smoothstep(MIX_SLOPE_THRESHOLD, SLOPE_THRESHOLD, x));
//         }else{
//             heightCol = ROCK_COLOR;
//         }
//     } 
// }

// void layer(int i, inout vec3 normal, inout vec4 color, inout float remainingA, inout float specular)
// {
//     float a = i == -1 ? remainingA : v_texBlend[i];
//     if (a == 0.) return;
//     float layer = i == -1 ? float(backgroundTerrainLayer) : terrainLayers[i];
//     vec2 coord = v_texCoords * vec2(i == -1 ? 1. : textureScale[i]);
//     vec4 rgbAndHeight = texture(terrainTextures, vec3(coord, layer));
//     vec3 texNormal = (i == -1 || hasNormal[i] == 1) ? texture(terrainTextures, vec3(coord, layer + 1.)).xyz : vec3(.5, .5, 1);

//     if (i >= 0)
//     {
//         if (fadeBlend[i] == 0)
//         {
//             float l = rgbAndHeight.a * a + .002;
//             a = a * a >= l ? 1. : pow(min(1., a + l), 2.);
//         }
//         a = min(remainingA, a);
//     }
//     color.rgb += rgbAndHeight.rgb * a;
//     normal += texNormal * a;
//     remainingA -= a;
//     if (i >= 0) specular += a * specularity[i];
// }

void main() {
//    discard;

    // if (v_y < -.4) discard;

    // float remainingA = 1.;

    //float wetSand = min(1., max(0., .8 - v_y * 2.));
    //float specularA = wetSand * .1;
    vec3 baseColor = createBaseColor();
    // vec3 normal = vec3(0);

    applySteepRocks(baseColor);
    // applySnowCaps(baseColor);
    // // normal *= 2.;
    // // normal -= 1.;

    // // normal = normal * (1. - wetSand) + vec3(0, 0, 1) * wetSand;
    // // color.rgb *= 1. - wetSand * .15;

    // normal *= 2.;
    // normal -= 1.;

    // normal = normal * (1. - wetSand) + vec3(0, 0, 1) * wetSand;
    // color.rgb *= 1. - wetSand * .15;

    // light

//     float lightEffect = max(.35, min(1., dot(vec3(0, 0, 1), v_sunDirTanSpace) * .4 + .3));
// //    color.rgb = vec3(lightEffect);
// //    return;


//     float diffuseLight = dot(normal, v_sunDirTanSpace) * lightEffect + (1. - lightEffect);
//     color.rgb *= diffuseLight;

//     if (shadowMapCoords.x >= 0. && shadowMapCoords.x <= 1. && shadowMapCoords.y >= 0. && shadowMapCoords.y <= 1.)
//     {
//         float shadow = texture(shadowBuffer, shadowMapCoords.xyz);

//         float shadowOpacity = v_shadowOpacity * .3;

//         color.rgb *= shadow * shadowOpacity + (1. - shadowOpacity);

//         specularA *= shadow * shadowOpacity + (1. - shadowOpacity);
//     }

//     vec3 reflectDir = reflect(v_sunDirTanSpace, normal);
//     float specular = dot(reflectDir, normalize(v_toCamera));
//     specular = min(1., max(0., specular));
//     float dampedSpec = pow(specular, 10.);
//     color.rgb += dampedSpec * specularA;

//     // color.rgb = vec3(v_dayLight);

    color = vec4(clamp(baseColor, vec3(0.0f), vec3(1.0f)), 1.0f);
}

