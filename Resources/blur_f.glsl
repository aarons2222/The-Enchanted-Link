uniform mediump float texelWidthOffset;
uniform mediump float texelHeightOffset;

varying highp vec2 blurCoordinates[8];
varying highp vec2 blurCoordinates2[7];

void main()
{
    lowp vec4 sum = vec4(0.0);
    sum += texture2D(CC_Texture0, blurCoordinates[0]) * 0.034671;
    sum += texture2D(CC_Texture0, blurCoordinates[1]) * 0.068744;
    sum += texture2D(CC_Texture0, blurCoordinates[2]) * 0.068744;
    sum += texture2D(CC_Texture0, blurCoordinates[3]) * 0.066402;
    sum += texture2D(CC_Texture0, blurCoordinates[4]) * 0.066402;
    sum += texture2D(CC_Texture0, blurCoordinates[5]) * 0.062385;
    sum += texture2D(CC_Texture0, blurCoordinates[6]) * 0.062385;
    sum += texture2D(CC_Texture0, blurCoordinates[7]) * 0.057009;
    sum += texture2D(CC_Texture0, blurCoordinates2[0]) * 0.057009;
    sum += texture2D(CC_Texture0, blurCoordinates2[1]) * 0.050671;
    sum += texture2D(CC_Texture0, blurCoordinates2[2]) * 0.050671;
    sum += texture2D(CC_Texture0, blurCoordinates2[3]) * 0.043806;
    sum += texture2D(CC_Texture0, blurCoordinates2[4]) * 0.043806;
    sum += texture2D(CC_Texture0, blurCoordinates2[5]) * 0.036836;
    sum += texture2D(CC_Texture0, blurCoordinates2[6]) * 0.036836;
    highp vec2 singleStepOffset = vec2(texelWidthOffset, texelHeightOffset);
    sum += texture2D(CC_Texture0, blurCoordinates[0] + singleStepOffset * 15.473117) * 0.030127;
    sum += texture2D(CC_Texture0, blurCoordinates[0] - singleStepOffset * 15.473117) * 0.030127;
    sum += texture2D(CC_Texture0, blurCoordinates[0] + singleStepOffset * 17.469654) * 0.023967;
    sum += texture2D(CC_Texture0, blurCoordinates[0] - singleStepOffset * 17.469654) * 0.023967;
    sum += texture2D(CC_Texture0, blurCoordinates[0] + singleStepOffset * 19.466198) * 0.018544;
    sum += texture2D(CC_Texture0, blurCoordinates[0] - singleStepOffset * 19.466198) * 0.018544;
    sum += texture2D(CC_Texture0, blurCoordinates[0] + singleStepOffset * 21.462742) * 0.013956;
    sum += texture2D(CC_Texture0, blurCoordinates[0] - singleStepOffset * 21.462742) * 0.013956;
    sum += texture2D(CC_Texture0, blurCoordinates[0] + singleStepOffset * 23.459293) * 0.010216;
    sum += texture2D(CC_Texture0, blurCoordinates[0] - singleStepOffset * 23.459293) * 0.010216;
    gl_FragColor = sum;
}