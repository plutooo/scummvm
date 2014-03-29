/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef MADS_NEBULAR_SCENES2_H
#define MADS_NEBULAR_SCENES2_H

#include "common/scummsys.h"
#include "mads/game.h"
#include "mads/scene.h"
#include "mads/nebular/nebular_scenes.h"

namespace MADS {

namespace Nebular {

class Scene2xx : public NebularScene {
protected:
	/**
	 * Plays an appropriate sound when entering a scene
	 */
	void setAAName();

	/**
	 * Updates the prefix used for getting player sprites for the scene
	 */
	void setPlayerSpritesPrefix();

	void sceneEntrySound();
public:
	Scene2xx(MADSEngine *vm) : NebularScene(vm) {}
};

class Scene201: public Scene2xx {
	bool _pterodactylFlag;
public:
	Scene201(MADSEngine *vm) : Scene2xx(vm), _pterodactylFlag(false) {}
	virtual void setup();
	virtual void enter();
	virtual void step();
	virtual void preActions() {};
	virtual void actions();
	virtual void postActions() {};
};

class Scene202: public Scene2xx {
private:
	bool _activeMsgFl, _ladderTopFl, _waitingMeteoFl, _toStationFl, _toTeleportFl;
	int _ladderHotspotId, _lastRoute, _stationCounter, _meteoFrame;
	uint32 _meteoClock1, _meteoClock2, _startTime;

	int subStep1(int randVal);
	int subStep2(int randVal);
	int subStep3(int randVal);
	int subStep4(int randVal);

public:
	Scene202(MADSEngine *vm) : Scene2xx(vm), _activeMsgFl(false), _ladderTopFl(false), _waitingMeteoFl(false),
		_ladderHotspotId(-1), _meteoClock1(0), _meteoClock2(0), _toStationFl(false), _toTeleportFl(false),
		_lastRoute(0), _stationCounter(0), _meteoFrame(0) {}
	virtual void setup();
	virtual void enter();
	virtual void step();
	virtual void preActions();
	virtual void actions();
	virtual void postActions() {};

	void setRandomKernelMessage();
};

class Scene203: public Scene2xx {
private:
	bool _rhotundaEat2Fl, _rhotundaEatFl;
public:
	Scene203(MADSEngine *vm) : Scene2xx(vm), _rhotundaEat2Fl(false), _rhotundaEatFl(false) {}
	virtual void setup();
	virtual void enter();
	virtual void step();
	virtual void preActions();
	virtual void actions();
	virtual void postActions() {};
};

class Scene205: public Scene2xx {
	uint32 _lastFishTime, _chickenTime;
public:
	Scene205(MADSEngine *vm) : Scene2xx(vm), _lastFishTime(0), _chickenTime(0) {}

	virtual void setup();
	virtual void enter();
	virtual void step();
	virtual void preActions() {};
	virtual void actions();
	virtual void postActions() {};
};

class Scene207: public Scene2xx {
	bool _vultureFl, _spiderFl, _eyeFl;
	int _spiderHotspotId, _vultureHotspotId;
	int32 _spiderTime, _vultureTime;

	void moveVulture();
	void moveSpider();

public:
	Scene207(MADSEngine *vm) : Scene2xx(vm), _vultureFl(false), _spiderFl(false), _spiderHotspotId(-1), _vultureHotspotId(-1), _spiderTime(0), _vultureTime(0) {}
	virtual void setup();
	virtual void enter();
	virtual void step();
	virtual void preActions();
	virtual void actions();
	virtual void postActions() {};
};

class Scene208: public Scene2xx {
public:
	Scene208(MADSEngine *vm) : Scene2xx(vm) {}
	virtual void setup();
	virtual void enter();
	virtual void step();
	virtual void preActions();
	virtual void actions();
	virtual void postActions() {};
};

class Scene209: public Scene2xx {
public:
	Scene209(MADSEngine *vm) : Scene2xx(vm) {}
	virtual void setup();
	virtual void enter();
	virtual void step();
	virtual void preActions();
	virtual void actions();
	virtual void postActions() {};
};

} // End of namespace Nebular
} // End of namespace MADS

#endif /* MADS_NEBULAR_SCENES2_H */
