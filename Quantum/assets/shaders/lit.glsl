#type vertex
#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;

out vec3 f_Normal;
out vec3 f_FragPos;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
    f_Normal = a_Normal;

    gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0f);
}

#type fragment
#version 330 core

in vec3 f_Normal;
in vec3 f_FragPos;

out vec4 FragColor;

uniform vec3 u_Albedo;
uniform float u_Metallic;
uniform float u_Smoothness;
uniform vec3 u_CameraPos;

uniform vec3 u_LightDir;
uniform vec3 u_LightColor;

vec3 fresnelSchlick(float cosTheta, vec3 F0) 
{
    return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}

void main()
{
    vec3 normal = normalize(f_Normal);
    vec3 viewDir = normalize(u_CameraPos - f_FragPos);
    vec3 lightDir = normalize(-u_LightDir);

    // Calculate reflection vector for specular highlight
    vec3 reflectDir = reflect(-lightDir, normal);

    // Ambient lighting (can be replaced by ambient lighting calculation or textures)
    vec3 ambient = 0.1f * u_Albedo;

    // Diffuse component (Lambertian)
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = diff * u_Albedo * u_LightColor;

    // Specular component (Blinn-Phong or Cook-Torrance)
    float spec = 0.0f;
    vec3 F0 = mix(vec3(0.04f), u_Albedo, u_Metallic);
    vec3 fresnel = fresnelSchlick(max(dot(viewDir, reflectDir), 0.0f), F0);
    float roughness = 1.0f - u_Smoothness;
    spec = pow(max(dot(viewDir, reflectDir), 0.0f), (1.0f / roughness) * 128.0f);

    vec3 specular = fresnel * spec * u_LightColor;

    // Combine the components
    vec3 color = ambient + diffuse + specular;

    FragColor = vec4(color, 1.0f);
}
