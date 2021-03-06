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

#include "bladerunner/scene_objects.h"

#include "bladerunner/bladerunner.h"

#include "bladerunner/obstacles.h"
#include "bladerunner/view.h"

namespace BladeRunner {

SceneObjects::SceneObjects(BladeRunnerEngine *vm, View *view) {
	_vm = vm;
	_view = view;

	_count = 0;

	for (int i = 0; i < kSceneObjectCount; ++i) {
		_sceneObjectsSortedByDistance[i] = -1;
	}
}

SceneObjects::~SceneObjects() {
	_vm = nullptr;
	_view = nullptr;
	_count = 0;
}

void SceneObjects::clear() {
	for (int i = 0; i < kSceneObjectCount; ++i) {
		_sceneObjects[i].id               = -1;
		_sceneObjects[i].type             = kSceneObjectTypeUnknown;
		_sceneObjects[i].distanceToCamera = 0.0f;
		_sceneObjects[i].isPresent        = false;
		_sceneObjects[i].isClickable      = false;
		_sceneObjects[i].isObstacle       = false;
		_sceneObjects[i].unknown1         = 0;
		_sceneObjects[i].isTarget         = false;
		_sceneObjects[i].isMoving         = false;
		_sceneObjects[i].isRetired        = false;

		_sceneObjectsSortedByDistance[i]  = -1;
	}
	_count = 0;
}

bool SceneObjects::addActor(int sceneObjectId, BoundingBox *boundingBox, Common::Rect *screenRectangle, bool isClickable, bool isMoving, bool isTarget, bool isRetired) {
	return addSceneObject(sceneObjectId, kSceneObjectTypeActor, boundingBox, screenRectangle, isClickable, false, 0, isTarget, isMoving, isRetired);
}

bool SceneObjects::addObject(int sceneObjectId, BoundingBox *boundingBox, bool isClickable, bool isObstacle, uint8 unknown1, bool isTarget) {
	Common::Rect rect(-1, -1, -1, -1);
	return addSceneObject(sceneObjectId, kSceneObjectTypeObject, boundingBox, &rect, isClickable, isObstacle, unknown1, isTarget, false, false);
}

bool SceneObjects::addItem(int sceneObjectId, BoundingBox *boundingBox, Common::Rect *screenRectangle, bool isTarget, bool isObstacle) {
	return addSceneObject(sceneObjectId, kSceneObjectTypeItem, boundingBox, screenRectangle, isObstacle, 0, 0, isTarget, 0, 0);
}

bool SceneObjects::remove(int sceneObjectId) {
	int i = findById(sceneObjectId);
	if (i == -1) {
		return false;
	}
	_sceneObjects[i].isPresent = false;
	int j;
	for (j = 0; j < _count; ++j) {
		if (_sceneObjectsSortedByDistance[j] == i) {
			break;
		}
	}
	for (int k = j; k < _count - 1; ++k) {
		_sceneObjectsSortedByDistance[k] = _sceneObjectsSortedByDistance[k + 1];
	}

	--_count;
	return true;
}

int SceneObjects::findByXYZ(bool *isClickable, bool *isObstacle, bool *isTarget, Vector3 &position, bool findClickables, bool findObstacles, bool findTargets) const {
	*isClickable = false;
	*isObstacle  = false;
	*isTarget    = false;

	for (int i = 0; i < _count; ++i) {
		assert(_sceneObjectsSortedByDistance[i] < kSceneObjectCount);

		const SceneObject *sceneObject = &_sceneObjects[_sceneObjectsSortedByDistance[i]];

		if ((findClickables && sceneObject->isClickable) ||
			(findObstacles  && sceneObject->isObstacle) ||
			(findTargets    && sceneObject->isTarget)) {
			BoundingBox boundingBox = *sceneObject->boundingBox;

			if (sceneObject->type == kSceneObjectTypeActor) {
				boundingBox.expand(-4.0, 0.0, -4.0, 4.0, 0.0, 4.0);
			}

			if (boundingBox.inside(position)) {
				*isClickable = sceneObject->isClickable;
				*isObstacle  = sceneObject->isObstacle;
				*isTarget    = sceneObject->isTarget;

				return sceneObject->id;
			}
		}
	}

	return -1;
}

bool SceneObjects::existsOnXZ(int exceptSceneObjectId, float x, float z, bool movingActorIsObstacle, bool standingActorIsObstacle) const {
	float xMin = x - 12.5f;
	float xMax = x + 12.5f;
	float zMin = z - 12.5f;
	float zMax = z + 12.5f;

	int count = _count;

	if (count > 0) {
		for (int i = 0; i < count; i++) {
			const SceneObject *sceneObject = &_sceneObjects[_sceneObjectsSortedByDistance[i]];
			bool isObstacle = false;
			if (sceneObject->type == kSceneObjectTypeActor) {
				if (sceneObject->isRetired) {
					isObstacle = false;
				} else if (sceneObject->isMoving) {
					isObstacle = movingActorIsObstacle != 0;
				} else {
					isObstacle = standingActorIsObstacle != 0;
				}
			} else {
				isObstacle = sceneObject->isObstacle;
			}

			if (isObstacle && sceneObject->id != exceptSceneObjectId) {
				float x1, y1, z1, x2, y2, z2;
				sceneObject->boundingBox->getXYZ(&x1, &y1, &z1, &x2, &y2, &z2);
				if (z1 <= zMax && z2 >= zMin && x1 <= xMax && x2 >= xMin) {
					return true;
				}
			}
		}
	}
	return false;
}

int SceneObjects::findById(int sceneObjectId) const {
	for (int i = 0; i < _count; ++i) {
		int j = this->_sceneObjectsSortedByDistance[i];

		if (_sceneObjects[j].isPresent && _sceneObjects[j].id == sceneObjectId) {
			return j;
		}
	}
	return -1;
}

bool SceneObjects::addSceneObject(int sceneObjectId, SceneObjectType sceneObjectType, BoundingBox *boundingBox, Common::Rect *screenRectangle, bool isClickable, bool isObstacle, uint8 unknown1, bool isTarget, bool isMoving, bool isRetired) {
	int index = findEmpty();
	if (index == -1) {
		return false;
	}

	_sceneObjects[index].id              = sceneObjectId;
	_sceneObjects[index].type            = sceneObjectType;
	_sceneObjects[index].isPresent       = true;
	_sceneObjects[index].boundingBox     = boundingBox;
	_sceneObjects[index].screenRectangle = screenRectangle;
	_sceneObjects[index].isClickable     = isClickable;
	_sceneObjects[index].isObstacle      = isObstacle;
	_sceneObjects[index].unknown1        = unknown1;
	_sceneObjects[index].isTarget        = isTarget;
	_sceneObjects[index].isMoving        = isMoving;
	_sceneObjects[index].isRetired       = isRetired;

	float centerZ = (_sceneObjects[index].boundingBox->getZ0() + _sceneObjects[index].boundingBox->getZ1()) / 2.0;

	float distanceToCamera = fabs(_view->_cameraPosition.z - centerZ);
	_sceneObjects[index].distanceToCamera = distanceToCamera;

	// insert according to distance from camera
	int i;
	for (i = 0; i < _count; ++i) {
		if (distanceToCamera < _sceneObjects[_sceneObjectsSortedByDistance[i]].distanceToCamera) {
			break;
		}
	}
	for (int j = CLIP(_count - 1, 0, kSceneObjectCount - 2); j >= i; --j) {
		_sceneObjectsSortedByDistance[j + 1] = _sceneObjectsSortedByDistance[j];
	}

	_sceneObjectsSortedByDistance[i] = index;
	++_count;
	return true;
}

int SceneObjects::findEmpty() const {
	for (int i = 0; i < kSceneObjectCount; ++i) {
		if (!_sceneObjects[i].isPresent)
			return i;
	}
	return -1;
}

void SceneObjects::setMoving(int sceneObjectId, bool isMoving) {
	int i = findById(sceneObjectId);
	if (i == -1) {
		return;
	}
	_sceneObjects[i].isMoving = isMoving;
}

void SceneObjects::setRetired(int sceneObjectId, bool isRetired) {
	int i = findById(sceneObjectId);
	if (i == -1) {
		return;
	}
	_sceneObjects[i].isRetired = isRetired;
}

bool SceneObjects::isBetween(float sourceX, float sourceZ, float targetX, float targetZ, int sceneObjectId) const {
	int i = findById(sceneObjectId);
	if (i == -1) {
		return false;
	}

	float objectX1, objectY1, objectZ1, objectX2, objectY2, objectZ2;
	_sceneObjects[i].boundingBox->getXYZ(&objectX1, &objectY1, &objectZ1, &objectX2, &objectY2, &objectZ2);

	Vector2 intersection;
	return lineIntersection(Vector2(sourceX, sourceZ), Vector2(targetX, targetZ), Vector2(objectX1, objectZ1), Vector2(objectX2, objectZ1), &intersection)
	    || lineIntersection(Vector2(sourceX, sourceZ), Vector2(targetX, targetZ), Vector2(objectX2, objectZ1), Vector2(objectX2, objectZ2), &intersection)
	    || lineIntersection(Vector2(sourceX, sourceZ), Vector2(targetX, targetZ), Vector2(objectX2, objectZ2), Vector2(objectX1, objectZ2), &intersection)
	    || lineIntersection(Vector2(sourceX, sourceZ), Vector2(targetX, targetZ), Vector2(objectX1, objectZ2), Vector2(objectX1, objectZ1), &intersection);
}

bool SceneObjects::isObstacleBetween(float sourceX, float sourceZ, float targetX, float targetZ, float altitude, int exceptSceneObjectId) const {
	for (int i = 0; i < _count; ++i) {
		const SceneObject *sceneObject = &_sceneObjects[_sceneObjectsSortedByDistance[i]];

		if (sceneObject->type == kSceneObjectTypeActor || !sceneObject->isObstacle || sceneObject->id == exceptSceneObjectId) {
			continue;
		}

		float objectX1, objectY1, objectZ1, objectX2, objectY2, objectZ2;
		_sceneObjects[i].boundingBox->getXYZ(&objectX1, &objectY1, &objectZ1, &objectX2, &objectY2, &objectZ2);

		if (84.0f <= objectY1 - altitude || 72.0f >= objectY2 - altitude) {
			continue;
		}

		float xAdjustement = (objectX2 - objectX1) * 0.1f;
		float zAdjustement = (objectZ2 - objectZ1) * 0.1f;

		objectX1 = objectX1 + xAdjustement;
		objectZ1 = objectZ1 + zAdjustement;
		objectX2 = objectX2 - xAdjustement;
		objectZ2 = objectZ2 - zAdjustement;

		Vector2 intersection;
		if (lineIntersection(Vector2(sourceX, sourceZ), Vector2(targetX, targetZ), Vector2(objectX1, objectZ1), Vector2(objectX2, objectZ1), &intersection)
		 || lineIntersection(Vector2(sourceX, sourceZ), Vector2(targetX, targetZ), Vector2(objectX2, objectZ1), Vector2(objectX2, objectZ2), &intersection)
		 || lineIntersection(Vector2(sourceX, sourceZ), Vector2(targetX, targetZ), Vector2(objectX2, objectZ2), Vector2(objectX1, objectZ2), &intersection)
		 || lineIntersection(Vector2(sourceX, sourceZ), Vector2(targetX, targetZ), Vector2(objectX1, objectZ2), Vector2(objectX1, objectZ1), &intersection)) {
			return true;
		}
	}
	return false;
}

void SceneObjects::setIsClickable(int sceneObjectId, bool isClickable) {
	int i = findById(sceneObjectId);
	if (i == -1) {
		return;
	}
	_sceneObjects[i].isClickable = isClickable;
}

void SceneObjects::setIsObstacle(int sceneObjectId, bool isObstacle) {
	int i = findById(sceneObjectId);
	if (i == -1) {
		return;
	}
	_sceneObjects[i].isObstacle = isObstacle;
}

void SceneObjects::setIsTarget(int sceneObjectId, bool isTarget) {
	int i = findById(sceneObjectId);
	if (i == -1) {
		return;
	}
	_sceneObjects[i].isTarget = isTarget;
}

void SceneObjects::updateObstacles() {
	_vm->_obstacles->clear();
	for (int i = 0; i < _count; ++i) {
		int index = _sceneObjectsSortedByDistance[i];
		const SceneObject *sceneObject = &_sceneObjects[index];
		if (sceneObject->isObstacle) {
			float x0, y0, z0, x1, y1, z1;
			sceneObject->boundingBox->getXYZ(&x0, &y0, &z0, &x1, &y1, &z1);
			_vm->_obstacles->add(x0, z0, x1, z1);
		}
	}
	_vm->_obstacles->backup();
}

} // End of namespace BladeRunner
