#version 120

varying vec2 texCoord0;

uniform sampler2D sampler;

void main(){
    gl_FragColor = vec4(1.0,1.0,1.0,1.0);
    //gl_FragColor = texture2D(sampler, texCoord0);
}
