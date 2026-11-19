#version 460 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;   /// 每个片段的世界位置

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    /// 获取光的方向
    vec3 lightDir = normalize(lightPos - FragPos);

    /// 光照方向 与 法向量的点乘 = 光线方向与法线的夹角
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    /// 镜面强度(Specular Intensity)变量
    float specularStrength = 0.5;
    /// 视线方向
    vec3 viewDir = normalize(viewPos - FragPos);

    /// 光线反射方向
    vec3 reflectDir = reflect(-lightDir, norm);

    ///  32是高光的反光度
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;


    vec3 result = (ambient + diffuse + specular) * objectColor;    
    FragColor = vec4(result, 1.0);
}
