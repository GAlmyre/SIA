#include "material.h"

Ward::Ward(const PropertyList &propList)
    : Diffuse(propList.getColor("diffuse",Color3f(0.2)))
{
    m_reflectivity = propList.getColor("reflectivity",Color3f(0.0));
    m_transmissivness = propList.getColor("transmissivness",Color3f(0.0));
    m_etaA = propList.getFloat("etaA",1);
    m_etaB = propList.getFloat("etaB",1);
    m_specularColor = propList.getColor("specular",Color3f(0.9));
    m_alphaX = propList.getFloat("alphaX",0.2);
    m_alphaY = propList.getFloat("alphaY",0.2);

    std::string texturePath = propList.getString("texture","");
    if(texturePath.size()>0){
        filesystem::path filepath = getFileResolver()->resolve(texturePath);
        loadTextureFromFile(filepath.str());
        setTextureScale(propList.getFloat("scale",1));
        setTextureMode(TextureMode(propList.getInteger("mode",0)));
    }
}

Color3f Ward::brdf(const Vector3f& viewDir, const Vector3f& lightDir, const Normal3f& normal, const Vector2f& uv) const
{
    Vector3f y = Vector3f(0,1,0).cross(normal);
    y.normalize();
    Vector3f x = normal.cross(y);
    x.normalize();
    Vector3f h = (viewDir+lightDir).normalized();
    if (viewDir.dot(normal)*lightDir.dot(normal) <= 0) {
      return Color3f(0);
    }
    Color3f rhos = (m_specularColor/(4*M_PI*m_alphaX*m_alphaY*sqrt(viewDir.dot(normal)*lightDir.dot(normal))))*exp(-1*(((h.dot(x))*(h.dot(x))/(m_alphaX*m_alphaX))+(h.dot(y))*(h.dot(y))/(m_alphaY*m_alphaY))/(h.dot(normal)*h.dot(normal)));
    return m_diffuseColor + rhos;
}

Color3f Ward::premultBrdf(const Vector3f& viewDir, const Vector3f& lightDir, const Normal3f& normal, const Vector2f& uv) const {
  if(viewDir.dot(normal)*lightDir.dot(normal) <=0)
    return Color3f(0.);
  Vector3f h = (viewDir+lightDir).normalized();

  return m_diffuseColor + m_specularColor * h.dot(viewDir) * h.dot(normal)*h.dot(normal)*h.dot(normal) * sqrt(lightDir.dot(normal)/viewDir.dot(normal));
}

std::string Ward::toString() const {
    return tfm::format(
        "Ward [\n"
        "  diffuse color = %s\n"
        "  specular color = %s\n"
        "  alphaX = %f  alphaY = %f\n"
        "]", m_diffuseColor.toString(),
             m_specularColor.toString(),
             m_alphaX, m_alphaY);
}

Vector3f Ward::is(const Vector3f& normal, const Vector3f& viewDir) const
{
  Vector3f x = normal.unitOrthogonal();
  Vector3f y = normal.cross(x);

  float u = Eigen::internal::random<float>(0,1);
  float v = Eigen::internal::random<float>(0,1);

  float phih = atan(m_alphaY/m_alphaX*tan(2*M_PI*v));
  float thetah = atan(sqrt(-log(u)/((cos(phih)*cos(phih))/(m_alphaX*m_alphaX)+(sin(phih)*sin(phih))/(m_alphaY*m_alphaY))));

  Vector3f h = Vector3f(sin(thetah)*cos(phih)*x + sin(thetah)*sin(phih)*y + cos(thetah)*normal);

  return 2*(viewDir.dot(h))*h-viewDir;
}
REGISTER_CLASS(Ward, "ward")
