/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

/*
 * This code is based on original Sfinx source code
 * Copyright (c) 1994-1997 Janus B. Wisniewski and L.K. Avalon
 */

#ifndef CGE2_HERO_H
#define CGE2_HERO_H

#include "cge2/cge2.h"
#include "cge2/vga13h.h"
#include "cge2/snail.h"

namespace CGE2 {

#define kMaxTry 400

class Hero;

struct HeroTab {
	Hero *_ptr;
	Sprite *_face;
	Sprite *_pocket[kPocketMax + 1];
	int _pocPtr;
	V2D *_posTab[kCaveMax];
	HeroTab(CGE2Engine *vm) {
		for (int i = 0; i < kCaveMax; i++)
			_posTab[i] = new V2D(vm);
	}
	~HeroTab() {
		for (int i = 0; i < kCaveMax; i++)
			delete _posTab[i];
	}
};

class Hero : public Sprite {
	int _hig[kDimMax];
	Sprite *_contact;
public:
	BitmapPtr _dim[kDimMax];
	V3D _trace[kWayMax];
	enum Dir { kNoDir = -1, kSS, kWW, kNN, kEE } _dir;
	int _curDim;
	int _tracePtr;
	int _reachStart, _reachCycle, _sayStart, _funStart;
	int _funDel0, _funDel;
	int _maxDist;
	bool _ignoreMap;
	Hero(CGE2Engine *vm);
	void tick(void);
	Sprite *expand(void);
	Sprite *contract(void) { return this; }
	Sprite *setContact(void);
	int stepSize(void) { return _ext->_seq[7]._dx; }
	int distance(V3D pos);
	int distance(Sprite * spr);
	void turn(Dir d);
	void park(void);
	static uint32 len(V2D v);
	bool findWay(void);
	static int snap(int p, int q, int grid);
	void walkTo(V3D pos);
	void walkTo(V2D pos) { walkTo(screenToGround(pos)); }
	V3D screenToGround(V2D pos);
	void walkTo(Sprite *spr);
	void say(void) { step(_sayStart); }
	void fun(void);
	void resetFun(void) { _funDel = _funDel0; }
	void hStep(void);
	bool lower(Sprite * spr);
	int cross(const V2D &a, const V2D &b);
	int mapCross(const V2D &a, const V2D &b);
	int mapCross(const V3D &a, const V3D &b);
	Hero *other(void) { return _vm->_heroTab[!(_ref & 1)]->_ptr;}
	Action action(void) { return (Action)(_ref % 10); }
	void reach(int mode);
	void setCurrent(void);
	void setCave(int c);
	void operator ++ (void);
	void operator -- (void);
};

} // End of namespace CGE2

#endif // CGE2_HERO_H
