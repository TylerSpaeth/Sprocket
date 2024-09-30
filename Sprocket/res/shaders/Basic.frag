#version 460 core

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TextureID;

out vec4 FragColor;

uniform sampler2D u_Texture[80];

void main()
{
  if(v_TextureID != 0.0) {
    vec4 texColor = texture(u_Texture[int(v_TextureID-1)], v_TexCoord);
    FragColor = texColor;
  }
  else {
    FragColor = v_Color;
  }
}
  