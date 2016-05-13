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

#include "titanic/true_talk/tt_adj.h"

namespace Titanic {

bool TTadj::_staticFlag;

TTadj::TTadj(TTString &str, int val1, int val2, int val3, int val4) :
		TTmajorWord(str, val1, val2, val3) {
	if (val4 >= 0 && val4 <= 9) {
		_field30 = val4;
	} else {
		_field30 = 0;
		_status = SS_5;
	}
}

TTadj::TTadj(TTadj *src) : TTmajorWord(src) {
	if (src->getStatus()) {
		_field30 = 0;
		_status = SS_5;
	} else {
		_field30 = src->_field30;
	}
}

int TTadj::load(SimpleFile *file) {
	int val;

	if (!TTword::load(file, 8) && file->scanf("%d", &val)) {
		_field30 = val;
		return 0;
	} else {
		return 8;
	}
}

TTword *TTadj::copy() {
	TTadj *returnWordP = new TTadj(this);
	returnWordP->_status = _status;
	if (!_status) {
		_staticFlag = false;
		return returnWordP;
	} else if (_status == SS_13 && !_staticFlag) {
		_staticFlag = true;
		delete returnWordP;
		return copy();
	} else {
		delete returnWordP;
		return nullptr;
	}
}

} // End of namespace Titanic
