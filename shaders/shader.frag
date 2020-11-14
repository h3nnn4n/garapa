#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture_1;

void main()
{
  FragColor = texture(texture_1, TexCoord);
}
