attribute vec4 a_position;
attribute vec2 a_texCoord;

uniform mediump float texelWidthOffset;
uniform mediump float texelHeightOffset;

varying vec2 blurCoordinates[8];
varying vec2 blurCoordinates2[7];

void main()
{
    gl_Position =  CC_MVPMatrix * a_position;
    
    mediump vec2 singleStepOffset = vec2(texelWidthOffset, texelHeightOffset);
    blurCoordinates[0] = a_texCoord.xy;
    blurCoordinates[1] = a_texCoord.xy + singleStepOffset * 1.497396;
    blurCoordinates[2] = a_texCoord.xy - singleStepOffset * 1.497396;
    blurCoordinates[3] = a_texCoord.xy + singleStepOffset * 3.493924;
    blurCoordinates[4] = a_texCoord.xy - singleStepOffset * 3.493924;
    blurCoordinates[5] = a_texCoord.xy + singleStepOffset * 5.490452;
    blurCoordinates[6] = a_texCoord.xy - singleStepOffset * 5.490452;
    blurCoordinates[7] = a_texCoord.xy + singleStepOffset * 7.486982;
    blurCoordinates2[0] = a_texCoord.xy - singleStepOffset * 7.486982;
    blurCoordinates2[1] = a_texCoord.xy + singleStepOffset * 9.483513;
    blurCoordinates2[2] = a_texCoord.xy - singleStepOffset * 9.483513;
    blurCoordinates2[3] = a_texCoord.xy + singleStepOffset * 11.480045;
    blurCoordinates2[4] = a_texCoord.xy - singleStepOffset * 11.480045;
    blurCoordinates2[5] = a_texCoord.xy + singleStepOffset * 13.476581;
    blurCoordinates2[6] = a_texCoord.xy - singleStepOffset * 13.476581;
}