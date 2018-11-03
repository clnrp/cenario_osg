/*
 * main.h
 *
 *  Created on: 13/03/2011
 *      Author: cleoner
 */

#ifndef MAIN_H_
#define MAIN_H_

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

#include <osgParticle/ExplosionEffect>
#include <osgParticle/ExplosionDebrisEffect>
#include <osgParticle/SmokeEffect>
#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/FireEffect>

#include <osgAudio/FileStream.h>
#include <osgAudio/SoundNode.h>
#include <osgAudio/SoundRoot.h>
#include <osgAudio/SoundManager.h>
#include <osgAudio/SoundState.h>
#include <osgAudio/Version.h>

#include <boost/algorithm/string.hpp>

#include "objects.h"

using namespace std;
using namespace boost;
//using namespace osgAudio;

#define SENS_ROT	5.0
#define SENS_OBS	15.0
#define SENS_TRANSL	30.0
#define PI 3.14159265359

GLfloat angle, fAspect;
GLfloat rotX, rotZ, rotX_ini, rotZ_ini;
GLfloat obsX, obsY, obsZ, obsX_ini, obsY_ini, obsZ_ini;
int x_ini,y_ini,bot;
int x_bk=0, y_bk=0;

void get_serial_data(const char buffer[]);

#endif /* MAIN_H_ */
