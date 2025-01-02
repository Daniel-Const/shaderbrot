#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;

// Output fragment color
out vec4 finalColor;

uniform vec2 renderSize;
uniform float pixelWidth = 5.0;
uniform float pixelHeight = 5.0;

void main()
{
    float dx = pixelWidth*(1.0/renderSize[0]);
    float dy = pixelHeight*(1.0/renderSize[1]);

    vec2 coord = vec2(dx*floor(fragTexCoord.x/dx), dy*floor(fragTexCoord.y/dy));
    
    vec3 tc = texture(texture0, coord).rgb;

    finalColor = vec4(tc, 1.0);
}