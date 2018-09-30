
#include "PhysicMaterial.h"

PhysicMaterial::PhysicMaterial()
{
	_albedo = new osg::Uniform("albedo", osg::Vec3f(1.0, 1.0, 1.0));
	_metallic = new osg::Uniform("metallic", 0.5f);
	_roughness = new osg::Uniform("roughness", 0.5f);
	_ao = new osg::Uniform("ao", 1.0f);

	addUniform(_albedo);
	addUniform(_roughness);
	addUniform(_metallic);
	addUniform(_ao);

    addUniform(new osg::Uniform("albedoMap", 0));
    addUniform(new osg::Uniform("normalMap", 1));
    addUniform(new osg::Uniform("roughnessMap", 2));
    addUniform(new osg::Uniform("metallicMap", 3));
    addUniform(new osg::Uniform("aoMap", 4));
	addUniform(new osg::Uniform("irradianceMap", 5));
}

PhysicMaterial::PhysicMaterial(const PhysicMaterial& rhs, const osg::CopyOp& copyop) :osg::StateSet(rhs, copyop)
{
	_albedoMap    = rhs.getAlbedoMap();
	_normalMap    = rhs.getNormalMap();
	_metallicMap  = rhs.getMetallicMap();
	_aoMap        = rhs.getAoMap();
	_roughnessMap = rhs.getRoughnessMap();

	_albedo = dynamic_cast<osg::Uniform*> (rhs._albedo->clone(osg::CopyOp::DEEP_COPY_ALL));
	_metallic = dynamic_cast<osg::Uniform*> (rhs._metallic->clone(osg::CopyOp::DEEP_COPY_ALL));
	_roughness = dynamic_cast<osg::Uniform*> (rhs._roughness->clone(osg::CopyOp::DEEP_COPY_ALL));
	_ao = dynamic_cast<osg::Uniform*> (rhs._ao->clone(osg::CopyOp::DEEP_COPY_ALL));

	addUniform(_albedo);
	addUniform(_roughness);
	addUniform(_metallic);
	addUniform(_ao);

    addUniform(new osg::Uniform("albedoMap", 0));
    addUniform(new osg::Uniform("normalMap", 1));
    addUniform(new osg::Uniform("roughnessMap", 2));
    addUniform(new osg::Uniform("metallicMap", 3));
    addUniform(new osg::Uniform("aoMap", 4));
	addUniform(new osg::Uniform("irradianceMap", 5));
}

osg::ref_ptr<osg::Texture2D> PhysicMaterial::createTexture2D(const std::string& fileName)
{
	osg::ref_ptr<osg::Image> image = osgDB::readRefImageFile(fileName);

	if (image)
	{
		return createTexture2D(image);
	}

	return 0;
}

osg::ref_ptr<osg::Texture2D> PhysicMaterial::createTexture2D(osg::Image* image)
{
	osg::ref_ptr<osg::Texture2D> texture;

	if (image)
	{
		texture = new osg::Texture2D;
		texture->setImage(image);
		texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
		texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
		texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
		texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
		texture->setMaxAnisotropy(16.0f);
	}

	return texture;
}

osg::ref_ptr<osg::Texture2D> PhysicMaterial::getAlbedoMap() const
{
	return _albedoMap.get();
}

void PhysicMaterial::setAlbedoMap(osg::Texture2D* texture)
{
    if (texture)
    {
        setDefine("ALBEDOMAP");
        setTextureAttribute(0, texture, osg::StateAttribute::ON);
    }
    else
    {
        removeDefine("ALBEDOMAP");
        removeTextureAttribute(0, _albedoMap);
    }

	_albedoMap = texture;
}

void PhysicMaterial::setAlbedoMap(const std::string& fileName)
{
	osg::ref_ptr<osg::Image> image = osgDB::readRefImageFile(fileName);
	setAlbedoMap(image);
}

void PhysicMaterial::setAlbedoMap(osg::Image* image)
{
	osg::ref_ptr<osg::Texture2D> texture = createTexture2D(image);
	setAlbedoMap(texture);
}

osg::ref_ptr<osg::Texture2D> PhysicMaterial::getNormalMap() const
{
	return _normalMap.get();
}

void PhysicMaterial::setNormalMap(osg::Texture2D* texture)
{
    if (texture)
    {
        setDefine("NORMALMAP");
        setTextureAttribute(1, texture, osg::StateAttribute::ON);
    }
    else
    {
        removeDefine("NORMALMAP");
        removeTextureAttribute(1, _normalMap);
    }

    _normalMap = texture;
}

void PhysicMaterial::setNormalMap(osg::Image* image)
{
	osg::ref_ptr<osg::Texture2D> texture = createTexture2D(image);
	setNormalMap(texture);
}

void PhysicMaterial::setNormalMap(const std::string& fileName)
{
	osg::ref_ptr<osg::Image> image = osgDB::readRefImageFile(fileName);
	setNormalMap(image);
}

osg::ref_ptr<osg::Texture2D> PhysicMaterial::getRoughnessMap() const
{
	return _roughnessMap.get();
}

void PhysicMaterial::setRoughnessMap(osg::Texture2D* texture)
{
	if (texture)
	{
		setDefine("ROUGHNESSMAP");
		setTextureAttribute(2, texture, osg::StateAttribute::ON);
	}
	else
	{
		removeDefine("ROUGHNESSMAP");
		removeTextureAttribute(2, _roughnessMap);
	}

	_roughnessMap = texture;
}

void PhysicMaterial::setRoughnessMap(osg::Image* image)
{
	osg::ref_ptr<osg::Texture2D> texture = createTexture2D(image);
	setRoughnessMap(texture);
}

void PhysicMaterial::setRoughnessMap(const std::string& fileName)
{
	osg::ref_ptr<osg::Image> image = osgDB::readRefImageFile(fileName);
	setRoughnessMap(image);
}


osg::ref_ptr<osg::Texture2D> PhysicMaterial::getMetallicMap() const
{
	return _metallicMap.get();
}

void PhysicMaterial::setMetallicMap(osg::Texture2D* texture)
{
    if (texture)
    {
        setDefine("METALLICMAP");
        setTextureAttribute(3, texture, osg::StateAttribute::ON);
    }
    else
    {
        removeDefine("METALLICMAP");
        removeTextureAttribute(3, _metallicMap);
    }

    _metallicMap = texture;
}

void PhysicMaterial::setMetallicMap(osg::Image* image)
{
	osg::ref_ptr<osg::Texture2D> texture = createTexture2D(image);
	setMetallicMap(texture);
}

void PhysicMaterial::setMetallicMap(const std::string& fileName)
{
	osg::ref_ptr<osg::Image> image = osgDB::readRefImageFile(fileName);
	setMetallicMap(image);
}

osg::ref_ptr<osg::Texture2D> PhysicMaterial::getAoMap() const
{
	return _aoMap.get();
}

void PhysicMaterial::setAoMap(osg::Texture2D* texture)
{
    if (texture)
    {
        setDefine("AOMAP");
        setTextureAttribute(4, texture, osg::StateAttribute::ON);
    }
    else
    {
        removeDefine("AOMAP");
        removeTextureAttribute(4, _aoMap);
    }

    _aoMap = texture;
}

void PhysicMaterial::setAoMap(const std::string& fileName)
{
	osg::ref_ptr<osg::Image> image = osgDB::readRefImageFile(fileName);
	setAoMap(image);
}

void PhysicMaterial::setAoMap(osg::Image* image)
{
	osg::ref_ptr<osg::Texture2D> texture = createTexture2D(image);
	setAoMap(texture);
}

void PhysicMaterial::setIrradianceMap(osg::TextureCubeMap* texture)
{
	if (texture)
	{
		setDefine("IRRADIANCEMAP");
		setTextureAttribute(5, texture, osg::StateAttribute::ON);
	}
	else
	{
		removeDefine("IRRADIANCEMAP");
		removeTextureAttribute(5, _irradianceMap);
	}

	_irradianceMap = texture;
}

void PhysicMaterial::setIrradianceMap(osg::Image* posX, osg::Image* negX, osg::Image* posY, osg::Image* negY, osg::Image* posZ, osg::Image* negZ)
{
	osg::ref_ptr<osg::TextureCubeMap> cubemap;
	if (posX && negX && posY && negY && posZ && negZ)
	{

		cubemap = new osg::TextureCubeMap;
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_X, posX);
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_X, negX);
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_Y, posY);
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Y, negY);
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_Z, posZ);
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Z, negZ);

		cubemap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
		cubemap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
		cubemap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);

		cubemap->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
		cubemap->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

	}

	setIrradianceMap(cubemap);
}

osg::ref_ptr<osg::TextureCubeMap> PhysicMaterial::loadCubeMap(const std::string& filePath)
{
	osg::ref_ptr<osg::TextureCubeMap> cubemap;

	std::string absolutePath = osgDB::findDataFile(filePath);
	std::map<std::string, std::string> imageMap;

	osgDB::DirectoryContents contents = osgDB::getDirectoryContents(absolutePath);
	for (unsigned int i = 0; i < contents.size(); ++i)
	{
		std::string filenameInDir = osgDB::convertToLowerCase(contents[i]);

		if (filenameInDir == "." ||
			filenameInDir == "..")
		{
			continue;
		}


		if (filenameInDir.find("posx") != std::string::npos || filenameInDir.find("right") != std::string::npos)
		{
			imageMap["posx"] = absolutePath + "\\" + contents[i];
		}

		if (filenameInDir.find("negx") != std::string::npos || filenameInDir.find("left") != std::string::npos)
		{
			imageMap["negx"] = absolutePath + "\\" + contents[i];
		}

		if (filenameInDir.find("posy") != std::string::npos || filenameInDir.find("top") != std::string::npos)
		{
			imageMap["posy"] = absolutePath + "\\" + contents[i];
		}

		if (filenameInDir.find("negy") != std::string::npos || filenameInDir.find("bottom") != std::string::npos)
		{
			imageMap["negy"] = absolutePath + "\\" +  contents[i];
		}

		if (filenameInDir.find("posz") != std::string::npos || filenameInDir.find("front") != std::string::npos)
		{
			imageMap["posz"] = absolutePath + "\\"  +contents[i];
		}

		if (filenameInDir.find("negz") != std::string::npos || filenameInDir.find("back") != std::string::npos)
		{
			imageMap["negz"] = absolutePath + "\\"  + contents[i];
		}
	}

	if (imageMap.size() < 6) 
		return cubemap;

	osg::ref_ptr<osg::Image> imagePosX = osgDB::readImageFile(imageMap["posx"]);
	osg::ref_ptr<osg::Image> imageNegX = osgDB::readImageFile(imageMap["negx"]);
	osg::ref_ptr<osg::Image> imagePosY = osgDB::readImageFile(imageMap["posy"]);
	osg::ref_ptr<osg::Image> imageNegY = osgDB::readImageFile(imageMap["negy"]);
	osg::ref_ptr<osg::Image> imagePosZ = osgDB::readImageFile(imageMap["posz"]);
	osg::ref_ptr<osg::Image> imageNegZ = osgDB::readImageFile(imageMap["negz"]);


	if (imagePosX.valid() && imageNegX.valid() && imagePosY.valid() && imageNegY.valid() && imagePosZ.valid() && imageNegZ.valid())
	{
		imagePosX->flipVertical();
		imageNegX->flipVertical();
		imagePosY->flipVertical();
		imageNegY->flipVertical();
		imagePosZ->flipVertical();
		imageNegZ->flipVertical();

		cubemap = new osg::TextureCubeMap;
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_X, imagePosX);
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_X, imageNegX);
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_Y, imagePosY);
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Y, imageNegY);
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_Z, imagePosZ);
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Z, imageNegZ);

		cubemap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
		cubemap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
		cubemap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);

		cubemap->setFilter(osg::Texture::MIN_FILTER, osg::Texture::NEAREST);
		cubemap->setFilter(osg::Texture::MAG_FILTER, osg::Texture::NEAREST);
	}

	return cubemap.get();
}

osg::ref_ptr<osg::TextureCubeMap> PhysicMaterial::getIrradianceMap() const
{
	return _irradianceMap.get();
}

void PhysicMaterial::setAlbedo(osg::Vec3f albedo)
{
	_albedo->set(albedo);
}

osg::Vec3 PhysicMaterial::getAlbedo() const
{
	osg::Vec3 albedo(1.0,1.0,1.0);
	_albedo->get(albedo);
	return albedo;
}

void PhysicMaterial::setMetallic(float metallic)
{
	_metallic->set(metallic);
}

float PhysicMaterial::getMetallic() const
{
	float metallic = 0.5f;
	_metallic->get(metallic);
	return metallic;
}

void PhysicMaterial::setRoughness(float roughness)
{
	_roughness->set(roughness);
}

float PhysicMaterial::getRoughness() const
{
	float roughness = 0.5f;
	_roughness->get(roughness);
	return roughness;
}

void PhysicMaterial::setAo(float ao)
{
	_ao->set(ao);
}

float PhysicMaterial::getAo() const
{
	float ao = 1.0f;
	_ao->get(ao);
	return ao;
}

