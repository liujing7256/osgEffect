#include <osg/io_utils>
#include <osg/LightModel>
#include <osg/Depth>
#include <osg/TexEnv>
#include <osg/TexGen>
#include <osg/TexMat>
#include <osg/TextureCubeMap>
#include <osg/ShapeDrawable>
#include <osg/ClearNode>
#include <osg/LightSource>
#include <osgDB/ReadFile>
#include <osgGA/StateSetManipulator>
#include <osgUtil/CullVisitor>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ViewDependentShadowMap>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#include <osg/AutoTransform>

#include "SkyBox.h"
#include "EffectCompositor.h"
#include "osgDB/FileUtils"
#include "osg/CullFace"

#include "PhysicMaterial.h"

#include <iostream>

using namespace std;

extern void configureViewerForMode( osgViewer::Viewer& viewer, osgFX::EffectCompositor* compositor,
                                    osg::Node* model, int displayMode );

#if defined(USE_SILVERLINING) && defined(HAVE_SILVERLINING)

/* The SilverLining skybox */
#include "SilverLiningNode.h"

class MySilverLiningNode : public SilverLiningNode
{
public:
    MySilverLiningNode( const char* licenseUser, const char* licenseKey )
    : SilverLiningNode(licenseUser, licenseKey) {}
    
    virtual void createAtmosphereData( osg::RenderInfo& renderInfo )
    {
        // Set our location (change this to your own latitude and longitude)
        SilverLining::Location loc;
        loc.SetAltitude( 0.0 );
        loc.SetLatitude( 45.0 );
        loc.SetLongitude( -122.0 );
        _atmosphere->GetConditions()->SetLocation( loc );
        
        // Set the time to noon in PST
        SilverLining::LocalTime t;
        t.SetFromSystemTime();
        t.SetHour( 15.0 );
        t.SetTimeZone( PST );
        _atmosphere->GetConditions()->SetTime( t );
        
        // Create cloud layers
        SilverLining::CloudLayer* cumulusCongestusLayer =
            SilverLining::CloudLayerFactory::Create( CUMULUS_CONGESTUS );
        cumulusCongestusLayer->SetIsInfinite( true );
        cumulusCongestusLayer->SetBaseAltitude( 500 );
        cumulusCongestusLayer->SetThickness( 200 );
        cumulusCongestusLayer->SetBaseLength( 40000 );
        cumulusCongestusLayer->SetBaseWidth( 40000 );
        cumulusCongestusLayer->SetDensity( 0.3 );
        
        // Note, we pass in X and -Y since this accepts "east" and "south" coordinates.
        cumulusCongestusLayer->SetLayerPosition( _cameraPos.x(), -_cameraPos.y() );
        cumulusCongestusLayer->SeedClouds( *_atmosphere );
        cumulusCongestusLayer->GenerateShadowMaps( false );
        atmosphere()->GetConditions()->AddCloudLayer( cumulusCongestusLayer );
    }
};

osg::Node* createSkyBox( float radius )
{
    return new MySilverLiningNode( "Your user name", "Your license code" );
}

#else

osg::Node* createSkyBox( float radius )
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(), radius)) );
    geode->setCullingActive( false );
    
    osg::ref_ptr<SkyBox> skybox = new SkyBox;
    skybox->getOrCreateStateSet()->setTextureAttributeAndModes( 0, new osg::TexGen );
    skybox->setEnvironmentMap( 0,
        osgDB::readImageFile("Cubemap_snow/posx.jpg"), osgDB::readImageFile("Cubemap_snow/negx.jpg"),
        osgDB::readImageFile("Cubemap_snow/posy.jpg"), osgDB::readImageFile("Cubemap_snow/negy.jpg"),
        osgDB::readImageFile("Cubemap_snow/posz.jpg"), osgDB::readImageFile("Cubemap_snow/negz.jpg") );
    skybox->addChild( geode.get() );
    return skybox.release();
}

#endif

/* Shadowed scene */

#define SHADOW_RECEIVE_MASK 0x1
#define SHADOW_CAST_MASK 0x2
osg::Group* createShadowedScene( osg::Node* scene )
{
    osg::ref_ptr<osgShadow::ShadowSettings> settings = new osgShadow::ShadowSettings;
    settings->setShaderHint( osgShadow::ShadowSettings::PROVIDE_FRAGMENT_SHADER );
	settings->setTextureSize( osg::Vec2s(2048, 2048) );
#if 0
    settings->setShadowMapTechniqueHints( osgShadow::ShadowSettings::PARALLEL_SPLIT|osgShadow::ShadowSettings::PERSPECTIVE );
    settings->setNumShadowMapsPerLight( 3 );
#endif
    
    osg::ref_ptr<osgShadow::ShadowedScene> shadowedScene = new osgShadow::ShadowedScene;
	shadowedScene->setReceivesShadowTraversalMask( SHADOW_RECEIVE_MASK );
	shadowedScene->setCastsShadowTraversalMask( SHADOW_CAST_MASK );
	shadowedScene->setShadowTechnique( new osgShadow::ViewDependentShadowMap );
	shadowedScene->setShadowSettings( settings.get() );
    shadowedScene->addChild( scene );
    
    osg::ref_ptr<osg::LightModel> lm = new osg::LightModel;
    lm->setAmbientIntensity( osg::Vec4(0.55f, 0.6f, 0.71f, 1.0f) );
    shadowedScene->getOrCreateStateSet()->setAttributeAndModes( lm.get() );
    
    osg::ref_ptr<osg::LightSource> ls = new osg::LightSource;
    ls->getLight()->setLightNum( 0 );
	ls->getLight()->setPosition( osg::Vec4(0.5f, 0.5f, 0.5f, 0.0f) );
	ls->getLight()->setAmbient( osg::Vec4(0.2f, 0.2f, 0.2f, 1.0f) );
	ls->getLight()->setDiffuse( osg::Vec4(0.49f, 0.465f, 0.494f, 1.0f) );
    ls->getLight()->setSpecular( osg::Vec4(1.0f, 0.98f, 0.95f, 1.0f) );
    shadowedScene->addChild( ls.get() );
    return shadowedScene.release();
}


osg::Geode* createLightMark(const osg::Vec4& pos)
{
	osg::Geode* light = new osg::Geode();

	light->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3f(pos.x(), pos.y(), pos.z()),.2)));

	osg::Program* program = new osg::Program();
	osg::ref_ptr<osg::Shader> vertexShader = osgDB::readRefShaderFile("lamp.vs");
	vertexShader->setType(osg::Shader::VERTEX);

	osg::ref_ptr<osg::Shader> fragmentShader = osgDB::readRefShaderFile("lamp.fs");
	fragmentShader->setType(osg::Shader::FRAGMENT);

	program->addShader(vertexShader);
	program->addShader(fragmentShader);

	light->getOrCreateStateSet()->setAttribute(program, osg::StateAttribute::PROTECTED|osg::StateAttribute::ON);

	return light;
}


osg::Texture2D *createTexture(const std::string &fileName)
{
	osg::ref_ptr<osg::Texture2D> texture;

	osg::ref_ptr<osg::Image> image = osgDB::readRefImageFile(fileName);
	if (image.valid())
	{
		texture = new osg::Texture2D;
		texture->setImage(osgDB::readRefImageFile(fileName));
		texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
		texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
		texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
		texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
		texture->setMaxAnisotropy(16.0f);
	}

    return texture.release();
}


#define MATERIAL_NAME std::string("rusted_iron")
osg::Group* createMaterialSpheres()
{
	osg::Group* root = new osg::Group;
	const unsigned int X_SEGMENTS = 6;
	const unsigned int Y_SEGMENTS = 6;

	osg::ref_ptr<osg::Texture2D> albedo = createTexture(MATERIAL_NAME+"\\albedo.png");
	osg::ref_ptr<osg::Texture2D> normal = createTexture(MATERIAL_NAME+"\\normal.png");
	osg::ref_ptr<osg::Texture2D> metallic = createTexture(MATERIAL_NAME+"\\metallic.png");
	osg::ref_ptr<osg::Texture2D> roughness = createTexture(MATERIAL_NAME+"\\roughness.png");
	osg::ref_ptr<osg::Texture2D> ao = createTexture(MATERIAL_NAME+"\\ao.png");
	osg::ref_ptr<osg::TextureCubeMap> irradianceMap = PhysicMaterial::loadCubeMap("\\skybox\\test\\");

	for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
	{
        for (unsigned int x = 0; x < X_SEGMENTS; ++x)
        {
            osg::Geode* shpere = new osg::Geode();
			shpere->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3f(2.5*(x + 1), 0.0, 2.5*(y + 1)), 1.0)));

            PhysicMaterial* material = new PhysicMaterial();

            material->setMetallic((float)y / (float)Y_SEGMENTS);
            material->setRoughness(1.0f - (float)x / (float)X_SEGMENTS);
            material->setAlbedo(osg::Vec3f(1.0, 0.0, 0.0));

            material->setAlbedoMap(albedo);
            material->setNormalMap(normal);
		
            material->setMetallicMap(metallic);
            material->setRoughnessMap(roughness);
            material->setAoMap(ao);
			material->setIrradianceMap(irradianceMap);
            
			shpere->setStateSet(material);
            root->addChild(shpere);
        }
	}
	return root;
}


#define DATA_PATH   "/data/"
#define SHADER_PATH "/data/shaders/"
#define IMAGE_PATH  "/data/images/"
#define MODEL_PATH  "/data/models/"
#define SKYBOX_PATH "/data/images/skybox"

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720

int main( int argc, char** argv )
{
    std::string curDir = osgDB::getCurrentWorkingDirectory();
	osgDB::FilePathList& filePaths = osgDB::getDataFilePathList();
    filePaths.push_back(curDir+DATA_PATH);
    filePaths.push_back(curDir+SHADER_PATH);
    filePaths.push_back(curDir+IMAGE_PATH);
    filePaths.push_back(curDir+MODEL_PATH);
	filePaths.push_back(curDir+SKYBOX_PATH);
		
    osg::ArgumentParser arguments( &argc, argv );
    osgViewer::Viewer viewer;
    
    int displayMode = 1;
    if ( arguments.read("--simple-mode") ) displayMode = 0;
    else if ( arguments.read("--analysis-mode") ) displayMode = 1;
    else if ( arguments.read("--compare-mode") ) displayMode = 2;
    
    std::string effectFile("pbr.xml");
    arguments.read( "--effect", effectFile );
    
    bool useSkyBox = false, shadowed = false;
    if ( arguments.read("--skybox") ) useSkyBox = true;
    if ( arguments.read("--shadowed") ) shadowed = true;
    
    float lodscale = 1.0f;
    arguments.read( "--lod-scale", lodscale );
    
    std::string normalSceneFile;
    arguments.read( "--normal-scene", normalSceneFile );
    
    // Create the scene
    osg::Node* model = osgDB::readNodeFiles( arguments );
    if (!model){ model = createMaterialSpheres();}
	else
	{
		osg::Texture2D* albedo = createTexture(MATERIAL_NAME + "\\albedo.png");
		osg::Texture2D* normal = createTexture(MATERIAL_NAME + "\\normal.png");
		osg::Texture2D* metallic = createTexture(MATERIAL_NAME + "\\metallic.png");
		osg::Texture2D* roughness = createTexture(MATERIAL_NAME + "\\roughness.png");
		osg::Texture2D* ao = createTexture(MATERIAL_NAME + "\\ao.png");
		

		PhysicMaterial* material = new PhysicMaterial();
		material->setAlbedoMap(albedo);
	    material->setNormalMap(normal);

		material->setMetallicMap(metallic);
		material->setRoughnessMap(roughness);
		material->setAoMap(ao);

		material->setIrradianceMap(PhysicMaterial::loadCubeMap("\\skybox\\room\\"));

		model->setStateSet(material);

	}
	
    osg::ref_ptr<osg::AutoTransform> scene = new osg::AutoTransform;
	scene->setScale(1.0);
    if ( useSkyBox ) scene->addChild( createSkyBox( model->getBound().radius() ) );
    scene->addChild( shadowed ? createShadowedScene(model) : model );
    
    // Create the effect compositor from XML file
    osgFX::EffectCompositor* compositor = osgFX::readEffectFile( effectFile );
    if ( !compositor )
    {
        OSG_WARN << "Effect file " << effectFile << " can't be loaded!" << std::endl;
        cin.get();
        return 1;
    }

	// lighting info
	// -------------
	{
		osg::Vec4 lightPositions[] = {
			//osg::Vec4(-10.0f,  10.0f, 10.0f,1.0f),
			//osg::Vec4(10.0f,  10.0f, 10.0f,1.0f),
			//osg::Vec4(-10.0f, -10.0f, 10.0f,1.0f),
			//osg::Vec4(10.0f, -10.0f, 10.0f,1.0f),

			osg::Vec4(0.0f, -10.0f, 0.0f,0.0f),
            osg::Vec4(0.0f, 10.0f, 0.0f,0.0f)

			//osg::Vec4(0.0f, -10.0f, 0.0f,1.0f),
		};
		osg::Vec3 lightColors[] = {
			/*osg::Vec3(300.0f, 300.0f, 300.0f),
			osg::Vec3(300.0f, 300.0f, 300.0f),
			osg::Vec3(300.0f, 300.0f, 300.0f),
			osg::Vec3(300.0f, 300.0f, 300.0f),*/

			osg::Vec3(1.0f, 1.0f, 1.0f),
            osg::Vec3(1.0f, 1.0f, 1.0f)
			//osg::Vec3(150.0f, 150.0f, 150.0f)
		};

		char lightNum[32];
		sprintf(lightNum, "%Id", sizeof(lightPositions) / sizeof(lightPositions[0]));

		compositor->getOrCreateStateSet()->setDefine("NUMBER_LIGHTS", lightNum);
		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			char name[128];
			sprintf(name, "Lights[%d].position", i);
			osg::Uniform* position = new osg::Uniform(name, lightPositions[i]);

			sprintf(name, "Lights[%d].color", i);
			osg::Uniform* color = new osg::Uniform(name, lightColors[i]);

			compositor->getOrCreateStateSet()->addUniform(position);
			compositor->getOrCreateStateSet()->addUniform(color);

			scene->addChild(createLightMark(lightPositions[i]));
		}
	}

    // For the fastest and simplest effect use, this is enough!
    compositor->addChild( scene.get() );

    // Add all to the root node of the viewer
    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild(compositor);

    {
        osg::ref_ptr<osg::Node> triad = osgDB::readNodeFile("axes.osgt");
       if(triad.valid()) root->addChild(triad);
    }

    if ( !normalSceneFile.empty() )
    {
        // FIXME: Disable near/far computing here; otherwise the scene with effects will
        // be clipped by the main camera. Maybe we can find better solutions instead?
        viewer.getCamera()->setComputeNearFarMode( osg::Camera::DO_NOT_COMPUTE_NEAR_FAR );
        
        // To make sure effect compositor can be correctly merged with normal scene,
        // you must force set gl_FragDepth in the last pass which is used for display.
        // This may be later applied automatically with PostDrawCallbacks, but at present
        // it should be done manually (see dof.xml for details).
        root->addChild( osgDB::readNodeFile(normalSceneFile) );
    }
    
    viewer.getCamera()->setLODScale( lodscale );
    viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
    viewer.addEventHandler( new osgViewer::StatsHandler );
    viewer.addEventHandler( new osgViewer::WindowSizeHandler );
    viewer.setSceneData( root.get() );


	viewer.getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);
    
    if ( displayMode>0 )
        configureViewerForMode( viewer, compositor, scene.get(), 0);

    viewer.setUpViewInWindow((1920 - WINDOW_WIDTH) / 2, (1080 - WINDOW_HEIGHT) / 2, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    
    return viewer.run();
}