
#ifndef __H_PHYSICMATERIAL__
#define __H_PHYSICMATERIAL__

#include "osg/StateSet"
#include "osg/Texture2D"
#include "osg/TextureCubeMap"
#include "osg/Program"
#include "osg/Shader"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgDB/FileUtils"

class PhysicMaterial : public osg::StateSet
{
public:
	PhysicMaterial();
	PhysicMaterial(const PhysicMaterial&, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);

	virtual Object* cloneType() const { return new StateSet(); }
	virtual Object* clone(const osg::CopyOp& copyop) const { return new PhysicMaterial(*this, copyop); }
	virtual bool isSameKindAs(const osg::Object* obj) const { return dynamic_cast<const PhysicMaterial*>(obj) != NULL; }
	virtual const char* libraryName() const { return "osg"; }
	virtual const char* className() const { return "PhysicMaterial"; }

	osg::ref_ptr<osg::Texture2D> getAlbedoMap() const;
	void setAlbedoMap(osg::Texture2D* texture);
	void setAlbedoMap(osg::Image* image);
	void setAlbedoMap(const std::string& fileName);

	osg::ref_ptr<osg::Texture2D> getNormalMap() const;
	void setNormalMap(osg::Texture2D* texture);
	void setNormalMap(osg::Image* image);
	void setNormalMap(const std::string& fileName);

	osg::ref_ptr<osg::Texture2D> getMetallicMap() const;
	void setMetallicMap(osg::Texture2D* texture);
	void setMetallicMap(osg::Image* image);
	void setMetallicMap(const std::string& fileName);

	osg::ref_ptr<osg::Texture2D> getRoughnessMap() const;
	void setRoughnessMap(osg::Texture2D* texture);
	void setRoughnessMap(osg::Image* image);
	void setRoughnessMap(const std::string& fileName);

	osg::ref_ptr<osg::Texture2D> getAoMap() const;
	void setAoMap(osg::Texture2D* texture);
	void setAoMap(osg::Image* image);
	void setAoMap(const std::string& fileName);

	osg::ref_ptr<osg::TextureCubeMap> getIrradianceMap() const;
	void setIrradianceMap(osg::TextureCubeMap* texture);
	void setIrradianceMap(osg::Image* posX, osg::Image* negX, osg::Image* posY, osg::Image* negY, osg::Image* posZ, osg::Image* negZ);
	void loadIrradianceMap(const std::string& filePath);

	void setAlbedo(osg::Vec3f albedo);
	osg::Vec3f getAlbedo() const;

	void setMetallic(float metallic);
	float getMetallic() const;

	void setRoughness(float roughness);
	float getRoughness() const;

	void setAo(float ao);
	float getAo() const;

protected:
	osg::ref_ptr<osg::Texture2D> createTexture2D(const std::string& fileName);
	osg::ref_ptr<osg::Texture2D> createTexture2D(osg::Image* image);

protected:
	osg::ref_ptr<osg::Texture2D>  _albedoMap;
	osg::ref_ptr<osg::Texture2D>  _normalMap;
	osg::ref_ptr<osg::Texture2D>  _metallicMap;
	osg::ref_ptr<osg::Texture2D>  _roughnessMap;
	osg::ref_ptr<osg::Texture2D>  _aoMap;
	osg::ref_ptr<osg::TextureCubeMap> _irradianceMap;

	osg::ref_ptr<osg::Uniform> _albedo;
	osg::ref_ptr<osg::Uniform> _metallic;
	osg::ref_ptr<osg::Uniform> _roughness;
	osg::ref_ptr<osg::Uniform> _ao;

protected:
    virtual ~PhysicMaterial() {}
};

#endif // __H_PHYSICMATERIAL__
