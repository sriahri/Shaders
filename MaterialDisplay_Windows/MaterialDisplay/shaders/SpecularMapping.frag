#version 410 core

const int LIGHT_COUNT = 3;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light lights[LIGHT_COUNT];

in vec3 interp_LightPositions[LIGHT_COUNT];
in vec3 interp_VertexPosition;
in vec3 interp_Normal;
in vec2 interp_Texcoord;

out vec4 fragColor;

vec4 ComputeLighting(int light_index, vec4 diffuse_sample, vec4 normal_sample, vec4 specular_sample) {
    vec3 lightDir = normalize(interp_LightPositions[light_index] - interp_VertexPosition);
    vec3 normal = normalize(normal_sample.rgb * 2.0 - 1.0);
    vec3 viewDir = normalize(-interp_VertexPosition);
    vec3 reflectDir = reflect(-lightDir, normal);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lights[light_index].ambient;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lights[light_index].diffuse * diffuse_sample.rgb;

    float specStrength = 0.5;
    float shininess = 16.0;
    float spec = pow(max(dot(reflect(-viewDir, normal), reflectDir), 0.0), shininess);
    vec3 specular = specStrength * spec * lights[light_index].specular * specular_sample.rgb;

    vec3 result = (ambient + diffuse + specular) * diffuse_sample.rgb;
    return vec4(result, diffuse_sample.a);
}

void main(void) {
    vec4 diffuse_sample = texture(diffuseTexture, interp_Texcoord);
    vec4 normal_sample = texture(normalTexture, interp_Texcoord);
    vec4 specular_sample = texture(specularTexture, interp_Texcoord);

    fragColor = vec4(0.0f);
    for (int i = 0; i < LIGHT_COUNT; i++)
        fragColor += ComputeLighting(i, diffuse_sample, normal_sample, specular_sample);
}
