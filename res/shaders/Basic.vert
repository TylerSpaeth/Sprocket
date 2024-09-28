#version 460 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 rgba;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in float textureID;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TextureID;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
// As of right now the position vector is being adjusted according to the modelmatrix on the cpu side
//uniform mat4 u_ModelMatrix;

void main()
{
  //gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * position;
  gl_Position = u_ProjectionMatrix * u_ViewMatrix * position;
  v_TexCoord = texCoord;
  v_Color = rgba;
  v_TextureID = textureID;
}