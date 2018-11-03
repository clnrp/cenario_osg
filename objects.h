#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <iostream>
#include <algorithm>

#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/StateSet>

#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>

#include <osgAudio/FileStream.h>
#include <osgAudio/SoundNode.h>
#include <osgAudio/SoundRoot.h>
#include <osgAudio/SoundManager.h>
#include <osgAudio/SoundState.h>
#include <osgAudio/Version.h>

#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;
using namespace osgAudio;

namespace objects {

class Object{

protected:
    int number;
	std::string *name;
	osg::ref_ptr<osg::Node> node;
	osg::ref_ptr<osg::Group> group;
	osg::ref_ptr<osg::MatrixTransform> matrixTransform;
	osg::ref_ptr<osg::PositionAttitudeTransform> positionAttitudeTransform;
	osg::ref_ptr<osg::AnimationPath> animationPath;

	osg::ref_ptr<osgAudio::SoundNode> soundNode;
	osg::ref_ptr<osgAudio::Sample> sample;
	osg::ref_ptr<osgAudio::SoundState> soundState;

	void createSound(const std::string& file, bool repeat);

	void incNumber(){
		static int number = 0;
		number++;
	}

public:
	osg::ref_ptr<osg::Group> getObject();
	void addObject(osg::ref_ptr<osg::Group> object);
	int getNumber();
	void setName(string name);
	string getName();

	Object(){
		incNumber();
		group = new osg::Group();
	}

	~Object(){

	}
};

int Object::getNumber(){
	return this->number;
}

void Object::setName(string name){
	*this->name = name;
}

string Object::getName(){
	return *this->name;
}

void Object::createSound(const std::string& file, bool repeat){
	sample = new osgAudio::Sample(file.c_str());
	soundState = new osgAudio::SoundState("g");

	soundState->setSample(sample);

	// Set its gain (volume) to 0.9
	soundState->setGain(0.4f);

	// Set its pitch to 1 (normal speed)
	soundState->setPitch(1);

	soundState->setPlay(true);

	soundState->setLooping(repeat);

	soundState->allocateSource(10, false);

	soundState->setReferenceDistance(4.9);
	soundState->setRolloffFactor(4);
	soundState->apply();

	osgAudio::SoundManager::instance()->addSoundState(soundState.get());

	soundNode = new osgAudio::SoundNode;
	soundNode->setSoundState(soundState.get());
}

osg::ref_ptr<osg::Group> Object::getObject(){
	return this->group;
}

void Object::addObject(osg::ref_ptr<osg::Group> object){
	this->group->addChild(object);
}

class Geometry: public Object{

private:
	osg::ref_ptr<osg::Geode> geode;
	osg::ref_ptr<osg::Geometry> geometry;

public:
	Geometry(string imageFile, osg::ref_ptr<osg::Vec3Array> vertices, osg::ref_ptr<osg::Vec3Array> normals, osg::ref_ptr<osg::Vec4Array> colors, osg::ref_ptr<osg::Vec2Array> texCoords, bool repeatImage){
		incNumber();

		group = new osg::Group();
		geode = new osg::Geode();

		// vertices do objeto
		geometry = new osg::Geometry();
		geometry->setVertexArray(vertices.get());

		// vetor normal
		geometry->setNormalArray(normals.get());
		geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

		// cores
		geometry->setColorArray(colors.get());
		geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

		// textura
		osg::ref_ptr<osg::Image> image(osgDB::readImageFile(imageFile.c_str()));
		if (image.get() == 0)
		{
			std::cerr << "Erro ao carregar:" << imageFile << "\n";
		}

		osg::ref_ptr<osg::Texture2D> texture(new osg::Texture2D);
		texture->setDataVariance(osg::Object::DYNAMIC);
		texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
		texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
		texture->setImage(image.get());

		osg::ref_ptr<osg::StateSet> stateSet(geode->getOrCreateStateSet());
		stateSet->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON |osg::StateAttribute::OVERRIDE);
		stateSet->setMode(GL_LIGHTING, osg::StateAttribute::ON);
		stateSet->setMode(GL_LIGHT0, osg::StateAttribute::ON);

		//stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);

		geometry->setTexCoordArray(0, texCoords.get());
		geometry->setTexCoordArray(1, texCoords.get());
		geometry->addPrimitiveSet(new osg::DrawArrays (osg::PrimitiveSet::QUADS, 0, vertices->size()));
		geode->addDrawable(geometry.get());
		group->addChild(geode);
	}

	Geometry(string name, string imageFile, osg::ref_ptr<osg::Vec3Array> vertices, osg::ref_ptr<osg::Vec3Array> normals, osg::ref_ptr<osg::Vec4Array> colors, osg::ref_ptr<osg::Vec2Array> texCoords, bool repeatImage){
		incNumber();
		this->name = new string();
		if(name != ""){
			*this->name=name;
		}else{
			*this->name=imageFile.substr(imageFile.rfind("/"),imageFile.rfind("."));
		}

		group = new osg::Group();
		geode = new osg::Geode();

		// vertices do objeto
		geometry = new osg::Geometry();
		geometry->setVertexArray(vertices.get());

		// vetor normal
		geometry->setNormalArray(normals.get());
		geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

		// cores
		geometry->setColorArray(colors.get());
		geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

		// textura
		osg::ref_ptr<osg::Image> image(osgDB::readImageFile(imageFile.c_str()));
		if (image.get() == 0)
		{
			std::cerr << "Erro ao carregar:" << imageFile << "\n";
		}

		osg::ref_ptr<osg::Texture2D> texture(new osg::Texture2D);
		texture->setDataVariance(osg::Object::DYNAMIC);
		texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
		texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
		texture->setImage(image.get());

		osg::ref_ptr<osg::StateSet> stateSet(geode->getOrCreateStateSet());
		stateSet->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON |osg::StateAttribute::OVERRIDE);
		stateSet->setMode(GL_LIGHTING, osg::StateAttribute::ON);
		stateSet->setMode(GL_LIGHT0, osg::StateAttribute::ON);

		//stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);

		geometry->setTexCoordArray(0, texCoords.get());
		geometry->setTexCoordArray(1, texCoords.get());
		geometry->addPrimitiveSet(new osg::DrawArrays (osg::PrimitiveSet::QUADS, 0, vertices->size()));
		geode->addDrawable(geometry.get());
		group->addChild(geode);
	}

};

class Model: public Object{

public:

	Model(string name, string modelFile, osg::Vec3d *position, string soundFile, bool repeatSound){
		incNumber();
		this->name = new string();
		if(name != ""){
			*this->name=name;
		}else{
			*this->name=modelFile.substr(modelFile.rfind("/"), modelFile.rfind("."));
		}

		group = new osg::Group();

		node = new osg::Node();
		node = osgDB::readNodeFile(modelFile.c_str());
		if(node)
		{
			matrixTransform = new osg::MatrixTransform();
			matrixTransform->setDataVariance(osg::Object::STATIC);
			//matrixTransform->setMatrix(osg::Matrix::scale(50,50,50)*osg::Matrix::translate(0,0,30)*osg::Matrix::rotate(osg::inDegrees(-90.0f),0.0f,0.0f,1.0f));
			matrixTransform->setMatrix(osg::Matrix::scale(1,1,1)*osg::Matrix::translate(0,0,5)*osg::Matrix::rotate(osg::inDegrees(-90.0f),0.0f,0.0f,1.0f));

			matrixTransform->addChild(node);

			if(soundFile != ""){
				this->createSound(soundFile, repeatSound);
				matrixTransform->addChild(soundNode);
			}

			positionAttitudeTransform = new osg::PositionAttitudeTransform();
			positionAttitudeTransform->setPosition(*position);
			positionAttitudeTransform->addChild(matrixTransform);
			group->addChild(positionAttitudeTransform);
		}
	}

	Model(string name, string modelFile, osg::ref_ptr<osg::AnimationPath> animationPath, string soundFile, bool repeatSound){
		incNumber();
		this->name = new string();
		if(name != ""){
			*this->name=name;
		}else{
			*this->name=modelFile.substr(modelFile.rfind("/"), modelFile.rfind("."));
		}

		group = new osg::Group();

		node = new osg::Node();
		node = osgDB::readNodeFile(modelFile.c_str());
		if(node)
		{
			this->animationPath = animationPath;
			matrixTransform = new osg::MatrixTransform();
			matrixTransform->setDataVariance(osg::Object::STATIC);
			matrixTransform->setMatrix(osg::Matrix::scale(1,1,1)*osg::Matrix::translate(0,0,5)*osg::Matrix::rotate(osg::inDegrees(-90.0f),0.0f,0.0f,1.0f));

			matrixTransform->addChild(node);

			if(soundFile != ""){
				this->createSound(soundFile, repeatSound);
				matrixTransform->addChild(soundNode);
			}

			positionAttitudeTransform = new osg::PositionAttitudeTransform();
			positionAttitudeTransform->setUpdateCallback(new osg::AnimationPathCallback(this->animationPath,0.0,0.5));
			positionAttitudeTransform->addChild(matrixTransform);
			group->addChild(positionAttitudeTransform);
		}
	}


};

}

#endif /* OBJECTS_H_ */
