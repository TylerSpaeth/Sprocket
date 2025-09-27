#version 460 core
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : require

in vec2 v_TexCoord;
in vec4 v_Color;
flat in uint64_t v_TextureID;

out vec4 FragColor;

void main()
{
  if(v_TextureID != 0ul) {
    sampler2D tex = sampler2D(v_TextureID);
    vec4 texColor = texture(tex, v_TexCoord);
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
  