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

#include "titanic/game/restaurant_cylinder_holder.h"

namespace Titanic {

BEGIN_MESSAGE_MAP(CRestaurantCylinderHolder, CDropTarget)
	ON_MESSAGE(EjectCylinderMsg)
	ON_MESSAGE(EnterViewMsg)
	ON_MESSAGE(MovieEndMsg)
	ON_MESSAGE(QueryCylinderHolderMsg)
	ON_MESSAGE(QueryCylinderNameMsg)
	ON_MESSAGE(MouseDragStartMsg)
END_MESSAGE_MAP()

CRestaurantCylinderHolder::CRestaurantCylinderHolder() : CDropTarget(),
	_field118(0), _field11C(0), _field12C(0), _field130(0),
	_ejectSoundName("z#61.wav"), _defaultCursorId(CURSOR_ARROW) {
}

void CRestaurantCylinderHolder::save(SimpleFile *file, int indent) {
	file->writeNumberLine(1, indent);
	file->writeNumberLine(_field118, indent);
	file->writeNumberLine(_field11C, indent);
	file->writeQuotedLine(_target, indent);
	file->writeNumberLine(_field12C, indent);
	file->writeNumberLine(_field130, indent);
	file->writeQuotedLine(_ejectSoundName, indent);
	file->writeNumberLine(_defaultCursorId, indent);

	CDropTarget::save(file, indent);
}

void CRestaurantCylinderHolder::load(SimpleFile *file) {
	file->readNumber();
	_field118 = file->readNumber();
	_field11C = file->readNumber();
	_target = file->readString();
	_field12C = file->readNumber();
	_field130 = file->readNumber();
	_ejectSoundName = file->readString();
	_defaultCursorId = (CursorId)file->readNumber();

	CDropTarget::load(file);
}

bool CRestaurantCylinderHolder::EjectCylinderMsg(CEjectCylinderMsg *msg) {
	_field11C = true;
	bool hasCylinder = findByName("Phonograph Cylinder") != nullptr;

	if (_field118) {
		playClip(hasCylinder ? "CloseHolder_Full" : "CloseHolder_Empty",
			MOVIE_NOTIFY_OBJECT | MOVIE_GAMESTATE);
		_fieldF4 = 1;
	} else {
		playClip(hasCylinder ? "OpenHolder_Full" : "OpenHolder_Empty",
			MOVIE_NOTIFY_OBJECT | MOVIE_GAMESTATE);
	}

	playSound(_ejectSoundName, 50);
	return true;
}

bool CRestaurantCylinderHolder::EnterViewMsg(CEnterViewMsg *msg) {
	if (_field118) {
		CTreeItem *cylinder = findByName("Phonograph Cylinder", true);
		if (cylinder) {
			loadFrame(_dropFrame);
			_cursorId = _dropCursorId;
		} else {
			loadFrame(_dragFrame);
			_cursorId = _dragCursorId;
		}
	} else {
		loadFrame(_field130);
		_cursorId = _defaultCursorId;
	}

	return true;
}

bool CRestaurantCylinderHolder::MovieEndMsg(CMovieEndMsg *msg) {
	_field11C = false;
	if (_field118) {
		_field118 = false;
		_cursorId = _defaultCursorId;

		CPhonographReadyToPlayMsg readyMsg;
		readyMsg.execute(_target);
	} else {
		_field118 = true;
		_fieldF4 = false;
		_cursorId = findByName("Phonograph Cylinder") ? _dropCursorId : _dragCursorId;
	}

	CCylinderHolderReadyMsg holderMsg;
	holderMsg.execute(_target);
	return true;
}

bool CRestaurantCylinderHolder::QueryCylinderHolderMsg(CQueryCylinderHolderMsg *msg) {
	CNamedItem *cylinder = findByName("Phonograph Cylinder", true);

	msg->_value1 = _field118;	
	if (cylinder) {
		msg->_value2 = 1;
		msg->_target = cylinder;
	}

	return true;
}

bool CRestaurantCylinderHolder::QueryCylinderNameMsg(CQueryCylinderNameMsg *msg) {
	CNamedItem *cylinder = findByName("Phonograph Cylinder", true);

	if (cylinder) {
		CQueryCylinderMsg queryMsg;
		queryMsg.execute(cylinder);
		msg->_name = queryMsg._name;
	}

	return true;
}

bool CRestaurantCylinderHolder::MouseDragStartMsg(CMouseDragStartMsg *msg) {
	if (_field118)
		return CDropTarget::MouseDragStartMsg(msg);
	else
		return true;
}

} // End of namespace Titanic
