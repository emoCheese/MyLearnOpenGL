#version 460 core

/// 此处用 Material 结构体代替 02 小节中硬编码的数值
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;
uniform Light light;
uniform Material material;

uniform vec3 viewPos;
in vec3 Normal;
in vec3 FragPos;   /// 每个片段的世界位置
in vec2 TexCoords;

void main()
{
    /// 移除了环境光材质颜色向量，因为环境光颜色在几乎所有情况下都等于漫反射颜色，所以我们不需要将它们分开储存
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));


    /// =========== 漫反射 ===========
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);  /// 获取光的方向

    /// 光照方向 与 法向量的点乘 = 光线方向与法线的夹角
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));

    /// =========== 镜面光 =========== 
    /// 视线方向
    vec3 viewDir = normalize(viewPos - FragPos);

    /// 光线反射方向
    vec3 reflectDir = reflect(-lightDir, norm);

    ///  32是高光的反光度
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));


    vec3 result = (ambient + diffuse + specular);    
    FragColor = vec4(result, 1.0);
}
