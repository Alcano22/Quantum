#type vertex
#version 330 core

layout (location = 1) in vec3 a_Pos;
layout (location = 2) in vec3 a_Normal;

out vec3 f_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    f_Normal = a_Normal;

    gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0f);
}

#type fragment
#version 330 core

in vec3 f_Normal;

out vec4 color;

uniform float u_Time;

void main()
{
    color = vec4(f_Normal * abs(sin(u_Time)), 1.0f);
}
