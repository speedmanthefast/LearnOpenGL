#version 330 core

out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
   
   // Just use texture
   FragColor = texture(ourTexture, TexCoord);

   // Mix color with texture
   // FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
};