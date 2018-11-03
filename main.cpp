/*
 * main.cpp
 *
 *  Created on: 13/03/2011
 *      Author: cleoner
 */

#ifndef MAIN_CPP_
#define MAIN_CPP_

#include "main.h"

//#define SERIAL_GLOBAL
#include "serial.h"


class PickHandler : public osgGA::GUIEventHandler
{
public:
	GLint valor;
	float sin_ft,cos_ft,sin_ed,cos_ed;

	PickHandler():
		_mx(0.0),_my(0.0),
		_usePolytopeIntersector(false),
		_useWindowCoordinates(false) {}

	~PickHandler() {}

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		switch(ea.getEventType())
		{
		case(osgGA::GUIEventAdapter::KEYDOWN):
		{
			sin_ft = 5*sin(rotX*PI/180.0);
			cos_ft = 5*cos(rotX*PI/180.0);
			sin_ed = 2*sin((rotX+90)*PI/180.0);
			cos_ed = 2*cos((rotX+90)*PI/180.0);

			if (ea.getKey()=='w')
			{
				obsX = obsX - sin_ed;
				obsY = obsY + cos_ed;
			}
			else if (ea.getKey()=='s')
			{
				obsX = obsX + sin_ed;
				obsY = obsY - cos_ed;
			}
			else if (ea.getKey()=='a')
			{
				obsX = obsX + sin_ft;
				obsY = obsY - cos_ft;
			}
			else if (ea.getKey()=='d')
			{
				obsX = obsX - sin_ft;
				obsY = obsY + cos_ft;
			}
			else if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Delete || ea.getKey()==osgGA::GUIEventAdapter::KEY_BackSpace)
			{

			}
		}
		case(osgGA::GUIEventAdapter::DRAG):
	    {
			_mx = ea.getX();
			_my = ea.getY();
			int deltax = x_ini - _mx;
			int deltaz = y_ini - _my;
			rotX = rotX_ini + deltax/SENS_ROT;
			rotZ = rotZ_ini + deltaz/SENS_ROT;
	    }
		case(osgGA::GUIEventAdapter::PUSH):
		case(osgGA::GUIEventAdapter::MOVE):
		{
			_mx = ea.getX();
			_my = ea.getY();
		}
		case(osgGA::GUIEventAdapter::RELEASE):
        {
        }

		default:
		{
			x_ini = _mx;
			y_ini = _my;
			obsX_ini = obsX;
			obsY_ini = obsY;
			obsZ_ini = obsZ;
			rotX_ini = rotX;
			rotZ_ini = rotZ;
		}

		}
	}

protected:

	float _mx,_my;
	bool _usePolytopeIntersector;
	bool _useWindowCoordinates;
};

osg::Node* createLights()
{
	osg::Group* group = new osg::Group;

	osg::Light* light = new osg::Light;
	light->setLightNum(0);
	light->setPosition(osg::Vec4(1.0f,0.0f,30.0f,1.0f));
	light->setAmbient(osg::Vec4(255.0f,255.0f,255.0f,1.0f));
	light->setDiffuse(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
	//light->setSpotCutoff(20.0f);
	//light->setSpotExponent(50.0f);
	//light->setDirection(osg::Vec3(1.0f,1.0f,-1.0f));
	light->setConstantAttenuation(1.0f);
	light->setLinearAttenuation(2.0f/1.0);
	light->setQuadraticAttenuation(2.0f/osg::square(1.0));

	osg::LightSource* lightSource = new osg::LightSource;
	lightSource->setLight(light);
	lightSource->setLocalStateSetModes(osg::StateAttribute::ON);

	//lightSource->setStateSetModes(*stateSet,osg::StateAttribute::ON);

	group->addChild(lightSource);

	return group;
}

osg::AnimationPath* createAnimationPath(const osg::Vec3& center,float radius,double looptime)
{
    // set up the animation path
    osg::AnimationPath* animationPath = new osg::AnimationPath;
    animationPath->setLoopMode(osg::AnimationPath::LOOP);

    int numSamples = 40;
    float yaw = 0.0f;
    float yaw_delta = 2.0f*3.14/((float)numSamples-1.0f);
    float roll = osg::inDegrees(30.0f);

    double time=0.0f;
    double time_delta = looptime/(double)numSamples;
    for(int i=0;i<numSamples;++i)
    {
        osg::Vec3 position(center+osg::Vec3(sinf(yaw)*radius,cosf(yaw)*radius,0.0f));
        osg::Quat rotation(osg::Quat(roll,osg::Vec3(0.0,1.0,0.0))*osg::Quat(-(yaw+osg::inDegrees(90.0f)),osg::Vec3(0.0,0.0,1.0)));

        animationPath->insert(time,osg::AnimationPath::ControlPoint(position,rotation));

        yaw += yaw_delta;
        time += time_delta;

    }
    return animationPath;
}

osg::ref_ptr<osg::Geode> criateCapsule(){
	osg::ref_ptr<osg::Geode> geode(new osg::Geode);

	osg::ref_ptr<osg::Capsule> Capsule(new osg::Capsule(osg::Vec3f(),1,2));
	osg::ref_ptr<osg::ShapeDrawable> capsuledrawable(new osg::ShapeDrawable(Capsule.get()));
	geode->addDrawable(capsuledrawable.get());

	//material
	osg::ref_ptr<osg::StateSet> stateSet(geode->getOrCreateStateSet());
	osg::ref_ptr<osg::Material> material(new osg::Material);
	material->setShininess(osg::Material::FRONT,96.f);
	material->setDiffuse(osg::Material::FRONT,osg::Vec4(.9f,6.9f,.2f,1.0f));
	stateSet->setAttribute(material.get());

	return geode;
}

int main(){
	cSerial *serial = new cSerial();
	serial->initSerial();

	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->x = 200;
	traits->y = 200;
	traits->width = 640;
	traits->height = 480;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
	osg::ref_ptr<osg::GraphicsContext> graphicsContext = osg::GraphicsContext::createGraphicsContext(traits.get());

	osgViewer::Viewer viewer;
	//viewer.setUpViewer(osgProducer::Viewer::STANDARD_SETTINGS);
	viewer.getCamera()->setGraphicsContext(graphicsContext.get());
	viewer.getCamera()->setViewport(new osg::Viewport(0,0, traits->width, traits->height));
	viewer.getCamera()->setProjectionMatrixAsPerspective(45.0, viewer.getCamera()->getViewport()->aspectRatio(), 0.1, 1000.0);

	osgAudio::SoundManager::instance()->init(16);
	osgAudio::SoundManager::instance()->getEnvironment()->setDistanceModel(osgAudio::InverseDistance);
	osgAudio::SoundManager::instance()->getEnvironment()->setDopplerFactor(1);

	//Creating the root node
	osg::ref_ptr<osg::Group> root(new osg::Group);

	// Create background
	osg::ref_ptr<osg::Vec3Array> vertices(new osg::Vec3Array());
	osg::ref_ptr<osg::Vec3Array> normals(new osg::Vec3Array());
	osg::ref_ptr<osg::Vec4Array> colors(new osg::Vec4Array());
	osg::ref_ptr<osg::Vec2Array> texCoords(new osg::Vec2Array());

	vertices->push_back(osg::Vec3(-100.0, 100.0, 0.0));
	vertices->push_back(osg::Vec3( 100.0, 100.0, 0.0));
	vertices->push_back(osg::Vec3( 100.0,-100.0, 0.0));
	vertices->push_back(osg::Vec3(-100.0,-100.0, 0.0));
	normals->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
	colors->push_back(osg::Vec4(255.0f, 255.0f, 255.0f, 1.0f));

	texCoords->push_back(osg::Vec2(-1.0, 1.0));
	texCoords->push_back(osg::Vec2( 1.0, 1.0));
	texCoords->push_back(osg::Vec2( 1.0,-1.0));
	texCoords->push_back(osg::Vec2(-1.0,-1.0));

	objects::Geometry *background = new objects::Geometry("texture/azulejo.jpg", vertices, normals, colors, texCoords, true);
	root->addChild(background->getObject());
    //------------------------------

	const osg::Vec3 *positionEf = new osg::Vec3(0,0,1);
	osg::Vec3 wind(1.0f,0.0f,0.0f);

    osgParticle::SmokeEffect* smoke = new osgParticle::SmokeEffect(*positionEf, 1.0f);
	osgParticle::FireEffect* fire = new osgParticle::FireEffect(*positionEf, 1.0f);
    smoke->setWind(wind);
    fire->setWind(wind);

    root->addChild(smoke);
	root->addChild(fire);

	objects::Model *fly = new objects::Model("", "model/Harrier.x", createAnimationPath(osg::Vec3(0.0f,0.0f,0.0f),10.0f,10.0f), "sound/bee.wav", true);
	//objects::Model *fly = new objects::Model("", "model/Harrier.x",new osg::Vec3d(0,0,5), "sound/bee.wav", true);
	root->addChild(fly->getObject());

	root->addChild(createLights());

	viewer.setSceneData(root.get());
	viewer.addEventHandler(new PickHandler());

	viewer.getCamera()->setClearColor(osg::Vec4(0., 0., 0., 1.));

	obsX = 0;
	obsY = -50;
	obsZ = 2;

	// pela regra da mão direita
	rotX = -90;
	rotZ = 0; // paralelo ao plano xy

	osg::ref_ptr<osgAudio::SoundRoot> soundRoot = new osgAudio::SoundRoot;
    root->addChild(soundRoot.get());

	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer.realize();
	while(!viewer.done()){
		osg::Vec3 position(obsX, obsY, obsZ);
		osg::Vec3 look(obsX-10*cos(rotZ*PI/180.0)*cos(rotX*PI/180.0), obsY-10*cos(rotZ*PI/180.0)*sin(rotX*PI/180.0), obsZ-10*sin(rotZ*PI/180.0));
		viewer.getCamera()->setViewMatrixAsLookAt(position, look, osg::Vec3(0,0,1));
		viewer.frame();
	}
	osgAudio::SoundManager::instance()->shutdown();

	cout << "fim.." << endl;
}

void get_serial_data(const char buffer[]){
	string data;
	data=data.append(buffer);
	vector<string> result;
	algorithm::split(result, data, boost::is_any_of(","));
	int x=(int)atof(result[0].c_str());
	int y=(int)atof(result[1].c_str());
	int z=(int)atof(result[2].c_str());

	if(abs(x)>1000 || abs(y)>1000){
		x_bk=x;
		y_bk=y;
		rotX = rotX_ini - 5.0*((double)x/16000.0);
		rotZ = rotZ_ini + 5.0*((double)y/16000.0);
	}
	printf("%d %d %d\n",x,y,z);
}

#endif /* MAIN_CPP_ */
