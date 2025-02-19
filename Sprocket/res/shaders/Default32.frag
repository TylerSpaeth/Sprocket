#version 460 core

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TextureID;

out vec4 FragColor;

// This can be changed depending on the system, but is set to 16 since
// that is the minimum mandated by OpenGL 4.6
uniform sampler2D u_Texture[16];

void main()
{
  if(v_TextureID != 0.0) {
    vec4 texColor = texture(u_Texture[int(v_TextureID-1)], v_TexCoord);
    FragColor = texColor;
    // Do not render the fragment if the alpha is really low
    if(FragColor.a < 0.01) {
      discard;
    }
  }
  else {
    FragColor = v_Color;
  }
}
  
