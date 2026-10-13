#version 330 core

out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;


void main()
{
   
   // Just use texture
   FragColor = mix(texture(texture1, TexCoord),
                  texture(texture2, TexCoord * 2), 0.4);
                  // Since the texture wrap parameter is set to repeat, sampling outside the standard 0->1 range will cause the sampler to wrap to the beginning of the image when interpolating

};