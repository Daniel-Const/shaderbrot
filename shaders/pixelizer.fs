#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;

// Output fragment color
out vec4 finalColor;

uniform vec2 u_render_size;
uniform float u_time;

void main()
{
    float pixelSize = 2 * sqrt(abs(sin(u_time))) + 3;
    float dx = pixelSize*(1.0/u_render_size[0]);
    float dy = pixelSize*(1.0/u_render_size[1]);

    vec2 coord = vec2(dx*floor(fragTexCoord.x/dx), dy*floor(fragTexCoord.y/dy));
    
    vec3 tc = texture(texture0, coord).rgb;

    finalColor = vec4(tc, 1.0);
}