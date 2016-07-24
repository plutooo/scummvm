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

/*
* Based on the Reverse Engineering work of Christophe Fontanel,
* maintainer of the Dungeon Master Encyclopaedia (http://dmweb.free.fr/)
*/

#include "champion.h"
#include "dungeonman.h"
#include "eventman.h"
#include "menus.h"
#include "inventory.h"
#include "objectman.h"
#include "text.h"
#include "timeline.h"
#include "projexpl.h"
#include "group.h"
#include "movesens.h"


namespace DM {

uint16 g38_slotMasks[38] = { // @ G0038_ai_Graphic562_SlotMasks
	/* 30 for champion inventory, 8 for chest */
	0xFFFF,   /* Ready Hand       Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Action Hand      Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0x0002,   /* Head             Head */
	0x0008,   /* Torso            Torso */
	0x0010,   /* Legs             Legs */
	0x0020,   /* Feet             Feet */
	0x0100,   /* Pouch 2          Pouch */
	0x0080,   /* Quiver Line2 1   Quiver 2 */
	0x0080,   /* Quiver Line1 2   Quiver 2 */
	0x0080,   /* Quiver Line2 2   Quiver 2 */
	0x0004,   /* Neck             Neck */
	0x0100,   /* Pouch 1          Pouch */
	0x0040,   /* Quiver Line1 1   Quiver 1 */
	0xFFFF,   /* Backpack Line1 1 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line2 2 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line2 3 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line2 4 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line2 5 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line2 6 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line2 7 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line2 8 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line2 9 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line1 2 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line1 3 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line1 4 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line1 5 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line1 6 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line1 7 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line1 8 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line1 9 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0x0400,   /* Chest 1          Chest */
	0x0400,   /* Chest 2          Chest */
	0x0400,   /* Chest 3          Chest */
	0x0400,   /* Chest 4          Chest */
	0x0400,   /* Chest 5          Chest */
	0x0400,   /* Chest 6          Chest */
	0x0400,   /* Chest 7          Chest */
	0x0400}; /* Chest 8          Chest */

const char *g417_baseSkillName[4] = {"FIGHTER", "NINJA", "PRIEST", "WIZARD"};

Box gBoxMouth = Box(55, 72, 12, 29); // @ G0048_s_Graphic562_Box_Mouth 
Box gBoxEye = Box(11, 28, 12, 29); // @ G0049_s_Graphic562_Box_Eye 
Box g54_BoxChampionIcons[4] = {
	Box(281, 299,  0, 13),
	Box(301, 319,  0, 13),
	Box(301, 319, 15, 28),
	Box(281, 299, 15, 28)};
Color g46_ChampionColor[4] = {(Color)7, (Color)11, (Color)8, (Color)14};

int16 g39_LightPowerToLightAmount[16] = {0, 5, 12, 24, 33, 40, 46, 51, 59, 68, 76, 82, 89, 94, 97, 100};

uint16 gSlotMasks[38] = {  // @ G0038_ai_Graphic562_SlotMasks
						   /* 30 for champion inventory, 8 for chest */
	0xFFFF,   /* Ready Hand       Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Action Hand      Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0x0002,   /* Head             Head */
	0x0008,   /* Torso            Torso */
	0x0010,   /* Legs             Legs */
	0x0020,   /* Feet             Feet */
	0x0100,   /* Pouch 2          Pouch */
	0x0080,   /* Quiver Line2 1   Quiver 2 */
	0x0080,   /* Quiver Line1 2   Quiver 2 */
	0x0080,   /* Quiver Line2 2   Quiver 2 */
	0x0004,   /* Neck             Neck */
	0x0100,   /* Pouch 1          Pouch */
	0x0040,   /* Quiver Line1 1   Quiver 1 */
	0xFFFF,   /* Backpack Line1 1 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line2 2 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line2 3 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line2 4 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line2 5 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line2 6 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line2 7 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line2 8 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line2 9 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line1 2 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line1 3 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line1 4 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line1 5 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line1 6 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line1 7 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line1 8 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0xFFFF,   /* Backpack Line1 9 Mouth/Head/Neck/Torso/Legs/Feet/Quiver 1/Quiver 2/Pouch/Hands/Chest */
	0x0400,   /* Chest 1          Chest */
	0x0400,   /* Chest 2          Chest */
	0x0400,   /* Chest 3          Chest */
	0x0400,   /* Chest 4          Chest */
	0x0400,   /* Chest 5          Chest */
	0x0400,   /* Chest 6          Chest */
	0x0400,   /* Chest 7          Chest */
	0x0400}; /* Chest 8          Chest */

Box gBoxChampionPortrait = Box(0, 31, 0, 28); // @ G0047_s_Graphic562_Box_ChampionPortrait 

ChampionMan::ChampionMan(DMEngine *vm) : _vm(vm) {
	for (uint16 i = 0; i < 4; ++i) {
		_g409_championPendingDamage[i] = 0;
		_g410_championPendingWounds[i] = 0;
		_gK71_champions[i].resetToZero();
	}
	_g305_partyChampionCount = 0;
	_g303_partyDead = false;
	_g414_leaderHandObject = Thing(0);
	_g411_leaderIndex = kM1_ChampionNone;
	_g300_partyIsSleeping = false;
	_g506_actingChampionOrdinal = 0;
	_g413_leaderHandObjectIconIndex = (IconIndice)0;
	_g415_leaderEmptyHanded = false;
	_g407_party.resetToZero();
	_g514_magicCasterChampionIndex = kM1_ChampionNone;
	_g420_mousePointerHiddenToDrawChangedObjIconOnScreen = false;
}

bool ChampionMan::f329_isLeaderHandObjectThrown(int16 side) {
	if (_g411_leaderIndex == kM1_ChampionNone) {
		return false;
	}
	return f328_isObjectThrown(_g411_leaderIndex, kM1_ChampionSlotLeaderHand, side);
}

bool ChampionMan::f328_isObjectThrown(uint16 champIndex, int16 slotIndex, int16 side) {
	bool throwingLeaderHandObjectFl = false;
	Thing curThing;
	Champion *curChampion = nullptr;
	Thing actionHandThing;

	if (slotIndex < 0) { /* Throw object in leader hand, which is temporarily placed in action hand */
		if (_g415_leaderEmptyHanded)
			return false;

		curThing = f298_getObjectRemovedFromLeaderHand();
		curChampion = &_gK71_champions[champIndex];
		actionHandThing = curChampion->getSlot(k1_ChampionSlotActionHand);
		curChampion->setSlot(k1_ChampionSlotActionHand, curThing);
		slotIndex = k1_ChampionSlotActionHand;
		throwingLeaderHandObjectFl = true;
	}

	int16 kineticEnergy = f312_getStrength(champIndex, slotIndex);
	if (throwingLeaderHandObjectFl) {
		// In this case, curChampion and actionHandThing are set.
		curChampion->setSlot((ChampionSlot)slotIndex, actionHandThing);
	} else {
		curThing = f300_getObjectRemovedFromSlot(champIndex, slotIndex);
		if (curThing == Thing::_none)
			return false;
	}

	_vm->f064_SOUND_RequestPlay_CPSD(k16_soundCOMBAT_ATTACK_SKELETON_ANIMATED_ARMOUR_DETH_KNIGHT, _vm->_dungeonMan->_g306_partyMapX, _vm->_dungeonMan->_g307_partyMapY, k1_soundModePlayIfPrioritized);
	f325_decrementStamina(champIndex, f305_getThrowingStaminaCost(curThing));
	f330_disableAction(champIndex, 4);
	int16 experience = 8;
	int16 weaponKineticEnergy = 1;
	if (curThing.getType() == k5_WeaponThingType) {
		experience += 4;
		WeaponInfo *curWeapon = _vm->_dungeonMan->f158_getWeaponInfo(curThing);
		if (curWeapon->_class <= k12_WeaponClassPoisinDart) {
			weaponKineticEnergy = curWeapon->_kineticEnergy;
			experience += weaponKineticEnergy >> 2;
		}
	}
	f304_addSkillExperience(champIndex, k10_ChampionSkillThrow, experience);
	kineticEnergy += weaponKineticEnergy;
	int16 skillLevel = f303_getSkillLevel((ChampionIndex)champIndex, k10_ChampionSkillThrow);
	kineticEnergy += _vm->_rnd->getRandomNumber(16) + (kineticEnergy >> 1) + skillLevel;
	int16 attack = f26_getBoundedValue((uint16)40, (uint16)((skillLevel << 3) + _vm->_rnd->getRandomNumber(31)), (uint16)200);
	int16 stepEnergy = MAX(5, 11 - skillLevel);
	_vm->_projexpl->f212_projectileCreate(curThing, _vm->_dungeonMan->_g306_partyMapX, _vm->_dungeonMan->_g307_partyMapY,
										  M21_normalizeModulo4(_vm->_dungeonMan->_g308_partyDir + side),
										  _vm->_dungeonMan->_g308_partyDir, kineticEnergy, attack, stepEnergy);
	_vm->_g311_projectileDisableMovementTicks = 4;
	_vm->_g312_lastProjectileDisabledMovementDirection = _vm->_dungeonMan->_g308_partyDir;
	f292_drawChampionState((ChampionIndex)champIndex);
	return true;
}

uint16 ChampionMan::M27_getChampionPortraitX(uint16 index) {
	return ((index) & 0x7) << 5;
}

uint16 ChampionMan::M28_getChampionPortraitY(uint16 index) {
	return ((index) >> 3) * 29;
}

int16 ChampionMan::f279_getDecodedValue(char *string, uint16 characterCount) {
	int val = 0;
	for (uint16 i = 0; i < characterCount; ++i) {
		val = (val << 4) + (string[i] - 'A');
	}
	return val;
}

void ChampionMan::f289_drawHealthOrStaminaOrManaValue(int16 posY, int16 currVal, int16 maxVal) {
	Common::String tmp = f288_getStringFromInteger(currVal, true, 3);
	_vm->_textMan->f52_printToViewport(55, posY, k13_ColorLightestGray, tmp.c_str());
	_vm->_textMan->f52_printToViewport(73, posY, k13_ColorLightestGray, "/");
	tmp = f288_getStringFromInteger(maxVal, true, 3);
	_vm->_textMan->f52_printToViewport(79, posY, k13_ColorLightestGray, tmp.c_str());
}

uint16 ChampionMan::M70_handSlotIndex(uint16 slotBoxIndex) {
	return slotBoxIndex & 0x1;
}

Common::String ChampionMan::f288_getStringFromInteger(uint16 val, bool padding, uint16 paddingCharCount) {
	Common::String valToStr = Common::String::format("%d", val);
	Common::String result;

	if (padding) {
		for (int16 i = 0, end = paddingCharCount - valToStr.size(); i < end; ++i)
			result += ' ';
	}

	return result += valToStr;
}

void ChampionMan::f299_applyModifiersToStatistics(Champion *champ, int16 slotIndex, int16 iconIndex, int16 modifierFactor, Thing thing) {
	int16 statIndex = k0_ChampionStatLuck;
	int16 modifier = 0;
	ThingType thingType = thing.getType();

	bool cursed = false;
	if (   ((thingType == k5_WeaponThingType) || (thingType == k6_ArmourThingType))
		&& (slotIndex >= k0_ChampionSlotReadyHand) && (slotIndex <= k12_ChampionSlotQuiverLine_1_1)) {
		if (thingType == k5_WeaponThingType) {
			Weapon *weapon = (Weapon *)_vm->_dungeonMan->f156_getThingData(thing);
			cursed = weapon->getCursed();
		} else {
			// k6_ArmourThingType
			Armour *armour = (Armour *)_vm->_dungeonMan->f156_getThingData(thing);
			cursed = armour->getCursed();
		}

		if (cursed) {
			statIndex = k0_ChampionStatLuck;
			modifier = -3;
		}
	}

	if (!cursed) {
		statIndex = (ChampionStatisticType)thingType; // variable sharing

		if ((iconIndex == k137_IconIndiceJunkRabbitsFoot) && (slotIndex < k30_ChampionSlotChest_1)) {
			statIndex = k0_ChampionStatLuck;
			modifier = 10;
		} else if (slotIndex == k1_ChampionSlotActionHand) {
			if (iconIndex == k45_IconIndiceWeaponMaceOfOrder) {
				statIndex = k1_ChampionStatStrength;
				modifier = 5;
			} else {
				statIndex = k8_ChampionStatMana;
				if ((iconIndex >= k20_IconIndiceWeaponStaffOfClawsEmpty) && (iconIndex <= k22_IconIndiceWeaponStaffOfClawsFull)) {
					modifier = 4;
				} else {
					switch (iconIndex) {
					case k38_IconIndiceWeaponDeltaSideSplitter:
						modifier = 1;
						break;
					case k41_IconIndiceWeaponTheInquisitorDragonFang:
						modifier = 2;
						break;
					case k40_IconIndiceWeaponVorpalBlade:
						modifier = 4;
						break;
					case k58_IconIndiceWeaponStaff:
						modifier = 2;
						break;
					case k59_IconIndiceWeaponWand:
						modifier = 1;
						break;
					case k60_IconIndiceWeaponTeowand:
						modifier = 6;
						break;
					case k61_IconIndiceWeaponYewStaff:
						modifier = 4;
						break;
					case k62_IconIndiceWeaponStaffOfManarStaffOfIrra:
						modifier = 10;
						break;
					case k63_IconIndiceWeaponSnakeStaffCrossOfNeta:
						modifier = 8;
						break;
					case k64_IconIndiceWeaponTheConduitSerpentStaff:
						modifier = 16;
						break;
					case k65_IconIndiceWeaponDragonSpit:
						modifier = 7;
						break;
					case k66_IconIndiceWeaponSceptreOfLyf:
						modifier = 5;
						break;
					default:
						break;
					}
				}
			}
		} else if (slotIndex == k4_ChampionSlotLegs) {
			if (iconIndex == k142_IconIndiceArmourPowertowers) {
				statIndex = k1_ChampionStatStrength;
				modifier = 10;
			}
		} else if (slotIndex == k2_ChampionSlotHead) {
			switch (iconIndex) {
			case k104_IconIndiceArmourCrownOfNerra:
				statIndex = k3_ChampionStatWisdom;
				modifier = 10;
				break;
			case k140_IconIndiceArmourDexhelm:
				statIndex = k2_ChampionStatDexterity;
				modifier = 10;
				break;
			default:
				break;
			}
		} else if (slotIndex == k3_ChampionSlotTorso) {
			switch (iconIndex) {
			case k141_IconIndiceArmourFlamebain:
				statIndex = k6_ChampionStatAntifire;
				modifier = 12;
				break;
			case k81_IconIndiceArmourCloakOfNight:
				statIndex = k2_ChampionStatDexterity;
				modifier = 8;
				break;
			default:
				break;
			}
		} else if (slotIndex == k10_ChampionSlotNeck) {
			switch (iconIndex) {
			case k10_IconIndiceJunkJewelSymalUnequipped:
			case k11_IconIndiceJunkJewelSymalEquipped:
				statIndex = k5_ChampionStatAntimagic;
				modifier = 15;
				break;
			case k81_IconIndiceArmourCloakOfNight:
				statIndex = k2_ChampionStatDexterity;
				modifier = 8;
				break;
			case k122_IconIndiceJunkMoonstone:
				statIndex = k8_ChampionStatMana;
				modifier = 3;
				break;
			default:
				break;
			}
		}
	}

	if (modifier) {
		modifier *= modifierFactor;
		//statIndex is set when modifier is set
		if (statIndex == k8_ChampionStatMana) {
			champ->_maxMana += modifier;
		} else if (statIndex < k6_ChampionStatAntifire + 1) {
			for (uint16 statValIndex = k0_ChampionStatMaximum; statValIndex <= k2_ChampionStatMinimum; ++statValIndex) {
				champ->getStatistic((ChampionStatisticType)statIndex, (ChampionStatisticValue)statValIndex) += modifier;
			}
		}
	}
}

bool ChampionMan::f295_hasObjectIconInSlotBoxChanged(int16 slotBoxIndex, Thing thing) {
	ObjectMan &objMan = *_vm->_objectMan;

	IconIndice currIconIndex = objMan.f39_getIconIndexInSlotBox(slotBoxIndex);
	if (((currIconIndex < k32_IconIndiceWeaponDagger) && (currIconIndex >= k0_IconIndiceJunkCompassNorth))
		|| ((currIconIndex >= k148_IconIndicePotionMaPotionMonPotion) && (currIconIndex <= k163_IconIndicePotionWaterFlask))
		|| (currIconIndex == k195_IconIndicePotionEmptyFlask)) {
		IconIndice newIconIndex = objMan.f33_getIconIndex(thing);
		if (newIconIndex != currIconIndex) {
			if ((slotBoxIndex < k8_SlotBoxInventoryFirstSlot) && !_g420_mousePointerHiddenToDrawChangedObjIconOnScreen) {
				_g420_mousePointerHiddenToDrawChangedObjIconOnScreen = true;
				_vm->_eventMan->f77_hideMouse();
			}
			objMan.f38_drawIconInSlotBox(slotBoxIndex, newIconIndex);
			return true;
		}
	}

	return false;
}

void ChampionMan::f296_drawChangedObjectIcons() {
	InventoryMan &invMan = *_vm->_inventoryMan;
	ObjectMan &objMan = *_vm->_objectMan;
	MenuMan &menuMan = *_vm->_menuMan;

	uint16 invChampOrdinal = invMan._g432_inventoryChampionOrdinal;
	if (_g299_candidateChampionOrdinal && !invChampOrdinal)
		return;

	_g420_mousePointerHiddenToDrawChangedObjIconOnScreen = false;
	IconIndice leaderHandObjIconIndex = _g413_leaderHandObjectIconIndex;

	if (((leaderHandObjIconIndex < k32_IconIndiceWeaponDagger) && (leaderHandObjIconIndex >= k0_IconIndiceJunkCompassNorth))	// < instead of <= is correct
		|| ((leaderHandObjIconIndex >= k148_IconIndicePotionMaPotionMonPotion) && (leaderHandObjIconIndex <= k163_IconIndicePotionWaterFlask))
		|| (leaderHandObjIconIndex == k195_IconIndicePotionEmptyFlask)) {
		IconIndice iconIndex = objMan.f33_getIconIndex(_g414_leaderHandObject);
		if (iconIndex != leaderHandObjIconIndex) {
			_g420_mousePointerHiddenToDrawChangedObjIconOnScreen = true;
			_vm->_eventMan->f77_hideMouse();
			objMan.f36_extractIconFromBitmap(iconIndex, objMan._g412_objectIconForMousePointer);
			_vm->_eventMan->f68_setPointerToObject(_vm->_objectMan->_g412_objectIconForMousePointer);
			_g413_leaderHandObjectIconIndex = iconIndex;
			objMan.f34_drawLeaderObjectName(_g414_leaderHandObject);
		}
	}

	for (uint16 slotBoxIndex = 0; slotBoxIndex < (_g305_partyChampionCount * 2); ++slotBoxIndex) {
		int16 champIndex = slotBoxIndex >> 1;
		if (invChampOrdinal == _vm->M0_indexToOrdinal(champIndex))
			continue;

		if (f295_hasObjectIconInSlotBoxChanged(slotBoxIndex, _gK71_champions[champIndex].getSlot((ChampionSlot)M70_handSlotIndex(slotBoxIndex)))
			&& (M70_handSlotIndex(slotBoxIndex) == k1_ChampionSlotActionHand)) {

			menuMan.f386_drawActionIcon((ChampionIndex)champIndex);
		}
	}

	if (invChampOrdinal) {
		Champion *champ = &_gK71_champions[_vm->M1_ordinalToIndex(invChampOrdinal)];
		Thing *thing = &champ->getSlot(k0_ChampionSlotReadyHand);
		uint16 drawViewport = 0;

		for (uint16 slotIndex = k0_ChampionSlotReadyHand; slotIndex < k30_ChampionSlotChest_1; slotIndex++, thing++) {
			uint16 objIconChanged = f295_hasObjectIconInSlotBoxChanged(slotIndex + k8_SlotBoxInventoryFirstSlot, *thing) ? 1 : 0;
			drawViewport |= objIconChanged;
			if (objIconChanged && (slotIndex == k1_ChampionSlotActionHand)) {
				menuMan.f386_drawActionIcon((ChampionIndex)_vm->M1_ordinalToIndex(invChampOrdinal));
			}
		}

		if (invMan._g424_panelContent = k4_PanelContentChest) {
			thing = invMan._g425_chestSlots;
			for (int16 slotIndex = 0; slotIndex < 8; ++slotIndex, thing++) {
				drawViewport |= (f295_hasObjectIconInSlotBoxChanged(slotIndex + k38_SlotBoxChestFirstSlot, *thing) ? 1 : 0);
			}
		}

		if (drawViewport) {
			champ->setAttributeFlag(k0x4000_ChampionAttributeViewport, true);
			f292_drawChampionState((ChampionIndex)_vm->M1_ordinalToIndex(invChampOrdinal));
		}
	}

	if (_g420_mousePointerHiddenToDrawChangedObjIconOnScreen)
		_vm->_eventMan->f78_showMouse();
}

void ChampionMan::f301_addObjectInSlot(ChampionIndex champIndex, Thing thing, ChampionSlot slotIndex) {
	InventoryMan &invMan = *_vm->_inventoryMan;
	DungeonMan &dunMan = *_vm->_dungeonMan;
	ObjectMan &objMan = *_vm->_objectMan;
	MenuMan &menuMan = *_vm->_menuMan;

	if (thing == Thing::_none)
		return;

	Champion *champ = &_gK71_champions[champIndex];

	if (slotIndex >= k30_ChampionSlotChest_1) {
		invMan._g425_chestSlots[slotIndex - k30_ChampionSlotChest_1] = thing;
	} else {
		champ->setSlot(slotIndex, thing);
	}

	champ->_load += dunMan.f140_getObjectWeight(thing);
	champ->setAttributeFlag(k0x0200_ChampionAttributeLoad, true);
	IconIndice iconIndex = objMan.f33_getIconIndex(thing);
	bool isInventoryChampion = (_vm->M0_indexToOrdinal(champIndex) == invMan._g432_inventoryChampionOrdinal);
	f299_applyModifiersToStatistics(champ, slotIndex, iconIndex, 1, thing);
	uint16 *rawObjPtr = dunMan.f156_getThingData(thing);

	if (slotIndex < k2_ChampionSlotHead) {
		if (slotIndex == k1_ChampionSlotActionHand) {
			champ->setAttributeFlag(k0x8000_ChampionAttributeActionHand, true);
			if (_g506_actingChampionOrdinal == _vm->M0_indexToOrdinal(champIndex))
				menuMan.f388_clearActingChampion();

			if ((iconIndex >= k30_IconIndiceScrollOpen) && (iconIndex <= k31_IconIndiceScrollClosed)) {
				((Scroll *)rawObjPtr)->setClosed(false);
				f296_drawChangedObjectIcons();
			}
		}

		if (iconIndex = k4_IconIndiceWeaponTorchUnlit) {
			((Weapon *)rawObjPtr)->setLit(true);
			_vm->_inventoryMan->f337_setDungeonViewPalette();
			f296_drawChangedObjectIcons();
		} else if (isInventoryChampion && (slotIndex == k1_ChampionSlotActionHand) &&
			((iconIndex == k144_IconIndiceContainerChestClosed) || ((iconIndex >= k30_IconIndiceScrollOpen) && (iconIndex <= k31_IconIndiceScrollClosed)))) {
			champ->setAttributeFlag(k0x0800_ChampionAttributePanel, true);
		}
	} else if (slotIndex == k10_ChampionSlotNeck) {
		if ((iconIndex >= k12_IconIndiceJunkIllumuletUnequipped) && (iconIndex <= k13_IconIndiceJunkIllumuletEquipped)) {
			((Junk *)rawObjPtr)->setChargeCount(1);
			_g407_party._magicalLightAmount += g39_LightPowerToLightAmount[2];
			_vm->_inventoryMan->f337_setDungeonViewPalette();
			iconIndex = (IconIndice)(iconIndex + 1);
		} else if ((iconIndex >= k10_IconIndiceJunkJewelSymalUnequipped) && (iconIndex <= k11_IconIndiceJunkJewelSymalEquipped)) {
			((Junk *)rawObjPtr)->setChargeCount(1);
			iconIndex = (IconIndice)(iconIndex + 1);
		}
	}

	f291_drawSlot(champIndex, slotIndex);
	if (isInventoryChampion)
		champ->setAttributeFlag(k0x4000_ChampionAttributeViewport, true);
}

int16 ChampionMan::f315_getScentOrdinal(int16 mapX, int16 mapY) {
	int16 scentIndex = _g407_party._scentCount;

	if (scentIndex) {
		Scent searchedScent;
		searchedScent.setMapX(mapX);
		searchedScent.setMapY(mapY);
		searchedScent.setMapIndex(_vm->_dungeonMan->_g272_currMapIndex);
		uint16 searchedScentRedEagle = searchedScent.toUint16();
		Scent *scent = &_g407_party._scents[scentIndex--];
		do {
			if ((*(--scent)).toUint16() == searchedScentRedEagle) {
				return _vm->M0_indexToOrdinal(scentIndex);
			}
		} while (scentIndex--);
	}
	return 0;
}

Thing ChampionMan::f298_getObjectRemovedFromLeaderHand() {
	_g415_leaderEmptyHanded = true;
	Thing leaderHandObject = _g414_leaderHandObject;

	if (leaderHandObject != Thing::_none) {
		_g414_leaderHandObject = Thing::_none;
		_g413_leaderHandObjectIconIndex = kM1_IconIndiceNone;
		_vm->_eventMan->f78_showMouse();
		_vm->_objectMan->f35_clearLeaderObjectName();
		_vm->_eventMan->f69_setMousePointer();
		_vm->_eventMan->f77_hideMouse();
		if (_g411_leaderIndex != kM1_ChampionNone) {
			_gK71_champions[_g411_leaderIndex]._load -= _vm->_dungeonMan->f140_getObjectWeight(leaderHandObject);
			setFlag(_gK71_champions[_g411_leaderIndex]._attributes, k0x0200_ChampionAttributeLoad);
			f292_drawChampionState(_g411_leaderIndex);
		}
	}
	return leaderHandObject;
}

uint16 ChampionMan::f312_getStrength(int16 champIndex, int16 slotIndex) {
	Champion *curChampion = &_gK71_champions[champIndex];
	int16 strength = _vm->_rnd->getRandomNumber(15) + curChampion->_statistics[k1_ChampionStatStrength][k1_ChampionStatCurrent];
	Thing curThing = curChampion->_slots[slotIndex];
	uint16 objectWeight = _vm->_dungeonMan->f140_getObjectWeight(curThing);
	uint16 oneSixteenthMaximumLoad = f309_getMaximumLoad(curChampion) >> 4;

	if (objectWeight <= oneSixteenthMaximumLoad) {
		strength += objectWeight - 12;
	} else {
		int16 loadThreshold = oneSixteenthMaximumLoad + ((oneSixteenthMaximumLoad - 12) >> 1);
		if (objectWeight <= loadThreshold) {
			strength += (objectWeight - oneSixteenthMaximumLoad) >> 1;
		} else {
			strength -= (objectWeight - loadThreshold) << 1;
		}
	}
	if (curThing.getType() == k5_WeaponThingType) {
		WeaponInfo *weaponInfo = _vm->_dungeonMan->f158_getWeaponInfo(curThing);
		strength += weaponInfo->_strength;
		uint16 skillLevel = 0;
		uint16 weaponClass = weaponInfo->_class;
		if ((weaponClass == k0_WeaponClassSwingWeapon) || (weaponClass == k2_WeaponClassDaggerAndAxes)) {
			skillLevel = f303_getSkillLevel(champIndex, k4_ChampionSkillSwing);
		}
		if ((weaponClass != k0_WeaponClassSwingWeapon) && (weaponClass < k16_WeaponClassFirstBow)) {
			skillLevel += f303_getSkillLevel(champIndex, k10_ChampionSkillThrow);
		}
		if ((weaponClass >= k16_WeaponClassFirstBow) && (weaponClass < k112_WeaponClassFirstMagicWeapon)) {
			skillLevel += f303_getSkillLevel(champIndex, k11_ChampionSkillShoot);
		}
		strength += skillLevel << 1;
	}
	strength = f306_getStaminaAdjustedValue(curChampion, strength);
	if (getFlag(curChampion->_wounds, (slotIndex == k0_ChampionSlotReadyHand) ? k0x0001_ChampionWoundReadHand : k0x0002_ChampionWoundActionHand)) {
		strength >>= 1;
	}
	return f26_getBoundedValue(0, strength >> 1, 100);
}

Thing ChampionMan::f300_getObjectRemovedFromSlot(uint16 champIndex, uint16 slotIndex) {
	Champion *curChampion = &_gK71_champions[champIndex];
	Thing curThing;

	if (slotIndex >= k30_ChampionSlotChest_1) {
		curThing = _vm->_inventoryMan->_g425_chestSlots[slotIndex - k30_ChampionSlotChest_1];
		_vm->_inventoryMan->_g425_chestSlots[slotIndex - k30_ChampionSlotChest_1] = Thing::_none;
	} else {
		curThing = curChampion->_slots[slotIndex];
		curChampion->_slots[slotIndex] = Thing::_none;
	}

	if (curThing == Thing::_none)
		return Thing::_none;

	bool isInventoryChampion = (_vm->M0_indexToOrdinal(champIndex) == _vm->_inventoryMan->_g432_inventoryChampionOrdinal);
	int16 curIconIndex = _vm->_objectMan->f33_getIconIndex(curThing);
	// Remove object modifiers
	f299_applyModifiersToStatistics(curChampion, slotIndex, curIconIndex, -1, curThing);

	Weapon *curWeapon = (Weapon *)_vm->_dungeonMan->f156_getThingData(curThing);
	if (slotIndex == k10_ChampionSlotNeck) {
		if ((curIconIndex >= k12_IconIndiceJunkIllumuletUnequipped) && (curIconIndex <= k13_IconIndiceJunkIllumuletEquipped)) {
			((Junk *)curWeapon)->setChargeCount(0);
			_g407_party._magicalLightAmount -= g39_LightPowerToLightAmount[2];
			_vm->_inventoryMan->f337_setDungeonViewPalette();
		} else if ((curIconIndex >= k10_IconIndiceJunkJewelSymalUnequipped) && (curIconIndex <= k11_IconIndiceJunkJewelSymalEquipped)) {
			((Junk *)curWeapon)->setChargeCount(0);
		}
	}

	f291_drawSlot(champIndex, slotIndex);
	if (isInventoryChampion)
		setFlag(curChampion->_attributes, k0x4000_ChampionAttributeViewport);

	if (slotIndex < k2_ChampionSlotHead) {
		if (slotIndex == k1_ChampionSlotActionHand) {
			setFlag(curChampion->_attributes, k0x8000_ChampionAttributeActionHand);
			if (_g506_actingChampionOrdinal == _vm->M0_indexToOrdinal(champIndex))
				_vm->_menuMan->f388_clearActingChampion();

			if ((curIconIndex >= k30_IconIndiceScrollOpen) && (curIconIndex <= k31_IconIndiceScrollClosed)) {
				((Scroll *)curWeapon)->setClosed(true);
				f296_drawChangedObjectIcons();
			}
		}

		if ((curIconIndex >= k4_IconIndiceWeaponTorchUnlit) && (curIconIndex <= k7_IconIndiceWeaponTorchLit)) {
			curWeapon->setLit(false);
			_vm->_inventoryMan->f337_setDungeonViewPalette();
			f296_drawChangedObjectIcons();
		}

		if (isInventoryChampion && (slotIndex == k1_ChampionSlotActionHand)) {
			switch (curIconIndex) {
			case k144_IconIndiceContainerChestClosed:
				_vm->_inventoryMan->f334_closeChest();
			// No break on purpose
			case k30_IconIndiceScrollOpen:
			case k31_IconIndiceScrollClosed:
				setFlag(curChampion->_attributes, k0x0800_ChampionAttributePanel);
				break;
			default:
				break;
			}
		}
	}
	curChampion->_load -= _vm->_dungeonMan->f140_getObjectWeight(curThing);
	setFlag(curChampion->_attributes, k0x0200_ChampionAttributeLoad);
	return curThing;
}

void ChampionMan::f325_decrementStamina(int16 championIndex, int16 decrement) {
	if (championIndex == kM1_ChampionNone)
		return;

	Champion *curChampion = &_gK71_champions[championIndex];
	curChampion->_currStamina -= decrement;

	int16 stamina = curChampion->_currStamina;
	if (stamina <= 0) {
		curChampion->_currStamina = 0;
		f321_addPendingDamageAndWounds_getDamage(championIndex, (-stamina) >> 1, k0x0000_ChampionWoundNone, k0_attackType_NORMAL);
	} else if (stamina > curChampion->_maxStamina) {
		curChampion->_currStamina = curChampion->_maxStamina;
	}

	setFlag(curChampion->_attributes, k0x0200_ChampionAttributeLoad | k0x0100_ChampionAttributeStatistics);
}

int16 ChampionMan::f321_addPendingDamageAndWounds_getDamage(int16 champIndex, int16 attack, int16 allowedWounds, uint16 attackType) {
	if (attack <= 0)
		return 0;

	Champion *curChampion = &_gK71_champions[champIndex];
	if (!curChampion->_currHealth)
		return 0;

	if (attackType != k0_attackType_NORMAL) {
		uint16 defense = 0;
		uint16 woundCount = 0;
		for (int16 woundIndex = k0_ChampionSlotReadyHand; woundIndex <= k5_ChampionSlotFeet; woundIndex++) {
			if (allowedWounds & (1 << woundIndex)) {
				woundCount++;
				defense += f313_getWoundDefense(champIndex, woundIndex | ((attackType == k4_attackType_SHARP) ? k0x8000_maskUseSharpDefense : k0x0000_maskDoNotUseSharpDefense));
			}
		}
		if (woundCount)
			defense /= woundCount;

		switch (attackType) {
		case k6_attackType_PSYCHIC: 
			{
				int16 wisdomFactor = 115 - curChampion->_statistics[k3_ChampionStatWisdom][k1_ChampionStatCurrent];
				if (wisdomFactor <= 0) {
					attack = 0;
				} else {
					attack = _vm->f30_getScaledProduct(attack, 6, wisdomFactor);
				}
			}
			goto T0321024;
		case k5_attackType_MAGIC:
			attack = f307_getStatisticAdjustedAttack(curChampion, k5_ChampionStatAntimagic, attack);
			attack -= _g407_party._spellShieldDefense;
			goto T0321024;
		case k1_attackType_FIRE:
			attack = f307_getStatisticAdjustedAttack(curChampion, k6_ChampionStatAntifire, attack);
			attack -= _g407_party._fireShieldDefense;
			break;
		case k2_attackType_SELF:
			defense >>= 1;
			break;
		default:
			break;
		}

		if (attack <= 0)
			return 0;

		attack = _vm->f30_getScaledProduct(attack, 6, 130 - defense);
		/* BUG0_44 
			A champion may take much more damage than expected after a Black Flame attack or an impact
			with a Fireball projectile. If the party has a fire shield defense value higher than the fire
			attack value then the resulting intermediary attack value is negative and damage should be 0. 
			However, the negative value is still used for further computations and the result may be a very
			high positive attack value which may kill a champion. This can occur only for k1_attackType_FIRE
			and if attack is negative before calling F0030_MAIN_GetScaledProduct
		*/
T0321024:
		if (attack <= 0)
			return 0;

		int16 adjustedAttack = f307_getStatisticAdjustedAttack(curChampion, k4_ChampionStatVitality, _vm->_rnd->getRandomNumber(127) + 10);
		if (attack > adjustedAttack) {
		/* BUG0_45
			This bug is not perceptible because of BUG0_41 that ignores Vitality while determining the
			probability of being wounded. However if it was fixed, the behavior would be the opposite
			of what it should: the higher the vitality of a champion, the lower the result of
			F0307_CHAMPION_GetStatisticAdjustedAttack and the more likely the champion could get
			wounded (because of more iterations in the loop below)
		*/
			do {
				setFlag(*(uint16 *)&_g410_championPendingWounds[champIndex], (1 << _vm->_rnd->getRandomNumber(7)) & allowedWounds);
			} while ((attack > (adjustedAttack <<= 1)) && adjustedAttack);
		}

		if (_g300_partyIsSleeping)
			f314_wakeUp();
	}
	_g409_championPendingDamage[champIndex] += attack;
	return attack;
}

int16 ChampionMan::f313_getWoundDefense(int16 champIndex, uint16 woundIndex) {
	static const byte g50_woundDefenseFactor[6] = {5, 5, 4, 6, 3, 1}; // @ G0050_auc_Graphic562_WoundDefenseFactor

	int16 L0942_i_Multiple;
#define AL0942_i_SlotIndex    L0942_i_Multiple
#define AL0942_i_WoundDefense L0942_i_Multiple
	uint16 L0943_ui_ArmourShieldDefense;
	bool L0944_B_UseSharpDefense;
	Thing L0945_T_Thing;
	Champion* L0946_ps_Champion;
	ArmourInfo* L0947_ps_ArmourInfo;


	L0946_ps_Champion = &_gK71_champions[champIndex];
	if (L0944_B_UseSharpDefense = getFlag(woundIndex, k0x8000_maskUseSharpDefense)) {
		clearFlag(woundIndex, k0x8000_maskUseSharpDefense);
	}
	for (L0943_ui_ArmourShieldDefense = 0, AL0942_i_SlotIndex = k0_ChampionSlotReadyHand; AL0942_i_SlotIndex <= k1_ChampionSlotActionHand; AL0942_i_SlotIndex++) {
		L0945_T_Thing = L0946_ps_Champion->_slots[AL0942_i_SlotIndex];
		if (L0945_T_Thing.getType() == k6_ArmourThingType) {
			L0947_ps_ArmourInfo = (ArmourInfo *)_vm->_dungeonMan->f156_getThingData(L0945_T_Thing);
			L0947_ps_ArmourInfo = &g239_ArmourInfo[((Armour *)L0947_ps_ArmourInfo)->getType()];
			if (getFlag(L0947_ps_ArmourInfo->_attributes, k0x0080_ArmourAttributeIsAShield)) {
				L0943_ui_ArmourShieldDefense += ((f312_getStrength(champIndex, AL0942_i_SlotIndex) + _vm->_dungeonMan->f143_getArmourDefense(L0947_ps_ArmourInfo, L0944_B_UseSharpDefense)) * g50_woundDefenseFactor[woundIndex]) >> ((AL0942_i_SlotIndex == woundIndex) ? 4 : 5);
			}
		}
	}
	AL0942_i_WoundDefense = _vm->getRandomNumber((L0946_ps_Champion->_statistics[k4_ChampionStatVitality][k1_ChampionStatCurrent] >> 3) + 1);
	if (L0944_B_UseSharpDefense) {
		AL0942_i_WoundDefense >>= 1;
	}
	AL0942_i_WoundDefense += L0946_ps_Champion->_actionDefense + L0946_ps_Champion->_shieldDefense + _g407_party._shieldDefense + L0943_ui_ArmourShieldDefense;
	if ((woundIndex > k1_ChampionSlotActionHand) && ((L0945_T_Thing = L0946_ps_Champion->_slots[woundIndex]).getType() == k6_ArmourThingType)) {
		L0947_ps_ArmourInfo = (ArmourInfo *)_vm->_dungeonMan->f156_getThingData(L0945_T_Thing);
		AL0942_i_WoundDefense += _vm->_dungeonMan->f143_getArmourDefense(&g239_ArmourInfo[((Armour *)L0947_ps_ArmourInfo)->getType()], L0944_B_UseSharpDefense);
	}
	if (getFlag(L0946_ps_Champion->_wounds, 1 << woundIndex)) {
		AL0942_i_WoundDefense -= 8 + _vm->getRandomNumber(4);
	}
	if (_g300_partyIsSleeping) {
		AL0942_i_WoundDefense >>= 1;
	}
	return f26_getBoundedValue(0, AL0942_i_WoundDefense >> 1, 100);
}

uint16 ChampionMan::f307_getStatisticAdjustedAttack(Champion* champ, uint16 statIndex, uint16 attack) {
	int16 L0927_i_Factor;

	if ((L0927_i_Factor = 170 - champ->_statistics[statIndex][k1_ChampionStatCurrent]) < 16) { /* BUG0_41 The Antifire and Antimagic statistics are completely ignored. The Vitality statistic is ignored against poison and to determine the probability of being wounded. Vitality is still used normally to compute the defense against wounds and the speed of health regeneration. A bug in the Megamax C compiler produces wrong machine code for this statement. It always returns 0 for the current statistic value so that L0927_i_Factor = 170 in all cases */
		return attack >> 3;
	}
	return _vm->f30_getScaledProduct(attack, 7, L0927_i_Factor);
}

void ChampionMan::f314_wakeUp() {
	_vm->_g321_stopWaitingForPlayerInput = true;
	_g300_partyIsSleeping = false;
	_vm->_g318_waitForInputMaxVerticalBlankCount = 10;
	_vm->f22_delay(10);
	_vm->_displayMan->f98_drawFloorAndCeiling();
	_vm->_eventMan->_g441_primaryMouseInput = g447_PrimaryMouseInput_Interface;
	_vm->_eventMan->_g442_secondaryMouseInput = g448_SecondaryMouseInput_Movement;
	_vm->_eventMan->_g443_primaryKeyboardInput = g458_primaryKeyboardInput_interface;
	_vm->_eventMan->_g444_secondaryKeyboardInput = g459_secondaryKeyboardInput_movement;
	_vm->_eventMan->f357_discardAllInput();
	_vm->_menuMan->f457_drawEnabledMenus();
}

int16 ChampionMan::f305_getThrowingStaminaCost(Thing thing) {
	int16 L0923_i_Weight;
	int16 L0924_i_StaminaCost;


	L0924_i_StaminaCost = f26_getBoundedValue((int16)1, L0923_i_Weight = _vm->_dungeonMan->f140_getObjectWeight(thing) >> 1, (int16)10);
	while ((L0923_i_Weight -= 10) > 0) {
		L0924_i_StaminaCost += L0923_i_Weight >> 1;
	}
	return L0924_i_StaminaCost;
}

void ChampionMan::f330_disableAction(uint16 champIndex, uint16 ticks) {
	int32 L1001_l_UpdatedEnableActionEventTime;
	int32 L1002_l_CurrentEnableActionEventTime;
	int16 L1003_i_EventIndex;
	Champion* L1004_ps_Champion;
	TimelineEvent L1005_s_Event;


	L1004_ps_Champion = &_gK71_champions[champIndex];
	L1001_l_UpdatedEnableActionEventTime = _vm->_g313_gameTime + ticks;
	L1005_s_Event._type = k11_TMEventTypeEnableChampionAction;
	L1005_s_Event._priority = champIndex;
	L1005_s_Event._B._slotOrdinal = 0;
	if ((L1003_i_EventIndex = L1004_ps_Champion->_enableActionEventIndex) >= 0) {
		L1002_l_CurrentEnableActionEventTime = M30_time(_vm->_timeline->_g370_events[L1003_i_EventIndex]._mapTime);
		if (L1001_l_UpdatedEnableActionEventTime >= L1002_l_CurrentEnableActionEventTime) {
			L1001_l_UpdatedEnableActionEventTime += (L1002_l_CurrentEnableActionEventTime - _vm->_g313_gameTime) >> 1;
		} else {
			L1001_l_UpdatedEnableActionEventTime = L1002_l_CurrentEnableActionEventTime + (ticks >> 1);
		}
		_vm->_timeline->f237_deleteEvent(L1003_i_EventIndex);
	} else {
		setFlag(L1004_ps_Champion->_attributes, k0x8000_ChampionAttributeActionHand | k0x0008_ChampionAttributeDisableAction);
		f292_drawChampionState((ChampionIndex)champIndex);
	}
	M33_setMapAndTime(L1005_s_Event._mapTime, _vm->_dungeonMan->_g309_partyMapIndex, L1001_l_UpdatedEnableActionEventTime);
	L1004_ps_Champion->_enableActionEventIndex = _vm->_timeline->f238_addEventGetEventIndex(&L1005_s_Event);
}

void ChampionMan::f304_addSkillExperience(uint16 champIndex, uint16 skillIndex, uint16 exp) {
#define AP0638_ui_SkillLevelAfter exp
#define AP0638_ui_ChampionColor   exp
	uint16 L0915_ui_Multiple;
#define AL0915_ui_MapDifficulty    L0915_ui_Multiple
#define AL0915_ui_SkillLevelBefore L0915_ui_Multiple
#define AL0915_ui_VitalityAmount   L0915_ui_Multiple
#define AL0915_ui_StaminaAmount    L0915_ui_Multiple
	uint16 L0916_ui_BaseSkillIndex;
	Skill* L0918_ps_Skill;
	Champion* L0919_ps_Champion;
	int16 L0920_i_MinorStatisticIncrease;
	int16 L0921_i_MajorStatisticIncrease;
	int16 L0922_i_BaseSkillLevel;


	warning(false, "potaneitally dangerous cast of uint32 below");
	if ((skillIndex >= k4_ChampionSkillSwing) && (skillIndex <= k11_ChampionSkillShoot) && ((uint32)_vm->_projexpl->_g361_lastCreatureAttackTime < (_vm->_g313_gameTime - 150))) {
		exp >>= 1;
	}
	if (exp) {
		if (AL0915_ui_MapDifficulty = _vm->_dungeonMan->_g269_currMap->_difficulty) {
			exp *= AL0915_ui_MapDifficulty;
		}
		L0919_ps_Champion = &_gK71_champions[champIndex];
		if (skillIndex >= k4_ChampionSkillSwing) {
			L0916_ui_BaseSkillIndex = (skillIndex - k4_ChampionSkillSwing) >> 2;
		} else {
			L0916_ui_BaseSkillIndex = skillIndex;
		}
		AL0915_ui_SkillLevelBefore = f303_getSkillLevel(champIndex, L0916_ui_BaseSkillIndex | (k0x4000_IgnoreObjectModifiers | k0x8000_IgnoreTemporaryExperience));
		warning(false, "potentially dangerous cast of uint32 below");
		if ((skillIndex >= k4_ChampionSkillSwing) && ((uint32)_vm->_projexpl->_g361_lastCreatureAttackTime > (_vm->_g313_gameTime - 25))) {
			exp <<= 1;
		}
		L0918_ps_Skill = &L0919_ps_Champion->_skills[skillIndex];
		L0918_ps_Skill->_experience += exp;
		if (L0918_ps_Skill->_temporaryExperience < 32000) {
			L0918_ps_Skill->_temporaryExperience += f26_getBoundedValue(1, exp >> 3, 100);
		}
		L0918_ps_Skill = &L0919_ps_Champion->_skills[L0916_ui_BaseSkillIndex];
		if (skillIndex >= k4_ChampionSkillSwing) {
			L0918_ps_Skill->_experience += exp;
		}
		AP0638_ui_SkillLevelAfter = f303_getSkillLevel(champIndex, L0916_ui_BaseSkillIndex | (k0x4000_IgnoreObjectModifiers | k0x8000_IgnoreTemporaryExperience));
		if (AP0638_ui_SkillLevelAfter > AL0915_ui_SkillLevelBefore) {
			L0922_i_BaseSkillLevel = AP0638_ui_SkillLevelAfter;
			L0920_i_MinorStatisticIncrease = _vm->getRandomNumber(2);
			L0921_i_MajorStatisticIncrease = 1 + _vm->getRandomNumber(2);
			AL0915_ui_VitalityAmount = _vm->getRandomNumber(2); /* For Priest skill, the amount is 0 or 1 for all skill levels */
			if (L0916_ui_BaseSkillIndex != k2_ChampionSkillPriest) {
				AL0915_ui_VitalityAmount &= AP0638_ui_SkillLevelAfter; /* For non Priest skills the amount is 0 for even skill levels. The amount is 0 or 1 for odd skill levels */
			}
			L0919_ps_Champion->_statistics[k4_ChampionStatVitality][k0_ChampionStatMaximum] += AL0915_ui_VitalityAmount;
			AL0915_ui_StaminaAmount = L0919_ps_Champion->_maxStamina;
			L0919_ps_Champion->_statistics[k6_ChampionStatAntifire][k0_ChampionStatMaximum] += _vm->getRandomNumber(2) & ~AP0638_ui_SkillLevelAfter; /* The amount is 0 for odd skill levels. The amount is 0 or 1 for even skill levels */
			switch (L0916_ui_BaseSkillIndex) {
			case k0_ChampionSkillFighter:
				AL0915_ui_StaminaAmount >>= 4;
				AP0638_ui_SkillLevelAfter *= 3;
				L0919_ps_Champion->_statistics[k1_ChampionStatStrength][k0_ChampionStatMaximum] += L0921_i_MajorStatisticIncrease;
				L0919_ps_Champion->_statistics[k2_ChampionStatDexterity][k0_ChampionStatMaximum] += L0920_i_MinorStatisticIncrease;
				break;
			case k1_ChampionSkillNinja:
				AL0915_ui_StaminaAmount /= 21;
				AP0638_ui_SkillLevelAfter <<= 1;
				L0919_ps_Champion->_statistics[k1_ChampionStatStrength][k0_ChampionStatMaximum] += L0920_i_MinorStatisticIncrease;
				L0919_ps_Champion->_statistics[k2_ChampionStatDexterity][k0_ChampionStatMaximum] += L0921_i_MajorStatisticIncrease;
				break;
			case k3_ChampionSkillWizard:
				AL0915_ui_StaminaAmount >>= 5;
				L0919_ps_Champion->_maxMana += AP0638_ui_SkillLevelAfter + (AP0638_ui_SkillLevelAfter >> 1);
				L0919_ps_Champion->_statistics[k3_ChampionStatWisdom][k0_ChampionStatMaximum] += L0921_i_MajorStatisticIncrease;
				goto T0304016;
			case k2_ChampionSkillPriest:
				AL0915_ui_StaminaAmount /= 25;
				L0919_ps_Champion->_maxMana += AP0638_ui_SkillLevelAfter;
				AP0638_ui_SkillLevelAfter += (AP0638_ui_SkillLevelAfter + 1) >> 1;
				L0919_ps_Champion->_statistics[k3_ChampionStatWisdom][k0_ChampionStatMaximum] += L0920_i_MinorStatisticIncrease;
T0304016:
				if ((L0919_ps_Champion->_maxMana += MIN(_vm->getRandomNumber(4), (uint16)(L0922_i_BaseSkillLevel - 1))) > 900) {
					L0919_ps_Champion->_maxMana = 900;
				}
				L0919_ps_Champion->_statistics[k5_ChampionStatAntimagic][k0_ChampionStatMaximum] += _vm->getRandomNumber(3);
			}
			if ((L0919_ps_Champion->_maxHealth += AP0638_ui_SkillLevelAfter + _vm->getRandomNumber((AP0638_ui_SkillLevelAfter >> 1) + 1)) > 999) {
				L0919_ps_Champion->_maxHealth = 999;
			}
			if ((L0919_ps_Champion->_maxStamina += AL0915_ui_StaminaAmount + _vm->getRandomNumber((AL0915_ui_StaminaAmount >> 1) + 1)) > 9999) {
				L0919_ps_Champion->_maxStamina = 9999;
			}
			setFlag(L0919_ps_Champion->_attributes, k0x0100_ChampionAttributeStatistics);
			f292_drawChampionState((ChampionIndex)champIndex);
			_vm->_textMan->f51_messageAreaPrintLineFeed();
			_vm->_textMan->f47_messageAreaPrintMessage((Color)(AP0638_ui_ChampionColor = g46_ChampionColor[champIndex]), L0919_ps_Champion->_name);
			// TODO: localization
			_vm->_textMan->f47_messageAreaPrintMessage((Color)AP0638_ui_ChampionColor, " JUST GAINED A ");
			_vm->_textMan->f47_messageAreaPrintMessage((Color)AP0638_ui_ChampionColor, g417_baseSkillName[L0916_ui_BaseSkillIndex]);
			_vm->_textMan->f47_messageAreaPrintMessage((Color)AP0638_ui_ChampionColor, " LEVEL!");
		}
	}
}

int16 ChampionMan::f324_damageAll_getDamagedChampionCount(uint16 attack, int16 wounds, int16 attackType) {
	int16 L0984_i_ChampionIndex;
	int16 L0985_i_RandomAttack;
	int16 L0986_i_DamagedChampionCount;

	attack -= (L0985_i_RandomAttack = (attack >> 3) + 1);
	L0985_i_RandomAttack <<= 1;
	for (L0986_i_DamagedChampionCount = 0, L0984_i_ChampionIndex = k0_ChampionFirst; L0984_i_ChampionIndex < _g305_partyChampionCount; L0984_i_ChampionIndex++) {
		if (f321_addPendingDamageAndWounds_getDamage(L0984_i_ChampionIndex, MAX(1, attack + _vm->getRandomNumber(L0985_i_RandomAttack)), wounds, attackType)) { /* Actual attack is attack +/- (attack / 8) */
			L0986_i_DamagedChampionCount++;
		}
	}
	return L0986_i_DamagedChampionCount;
}

int16 ChampionMan::f286_getTargetChampionIndex(int16 mapX, int16 mapY, uint16 cell) {
	if (_g305_partyChampionCount && (M38_distance(mapX, mapY, _vm->_dungeonMan->_g306_partyMapX, _vm->_dungeonMan->_g307_partyMapY) <= 1)) {
		signed char L0840_auc_OrderedCellsToAttack[4];
		_vm->_groupMan->f229_setOrderedCellsToAttack(L0840_auc_OrderedCellsToAttack, _vm->_dungeonMan->_g306_partyMapX, _vm->_dungeonMan->_g307_partyMapY, mapX, mapY, cell);
		for (uint16 L0838_ui_Counter = 0; L0838_ui_Counter < 4; L0838_ui_Counter++) {
			int16 L0839_i_ChampionIndex = f285_getIndexInCell(L0840_auc_OrderedCellsToAttack[L0838_ui_Counter]);
			if (L0839_i_ChampionIndex >= 0)
				return L0839_i_ChampionIndex;
		}
	}
	return kM1_ChampionNone;
}

int16 ChampionMan::f311_getDexterity(Champion* champ) {
	int16 L0934_i_Dexterity = _vm->getRandomNumber(8) + champ->_statistics[k2_ChampionStatDexterity][k1_ChampionStatCurrent];
	L0934_i_Dexterity -= ((int32)(L0934_i_Dexterity >> 1) * (int32)champ->_load) / f309_getMaximumLoad(champ);
	if (_g300_partyIsSleeping) {
		L0934_i_Dexterity >>= 1;
	}
	return f26_getBoundedValue(1 + _vm->getRandomNumber(8), L0934_i_Dexterity >> 1, 100 - _vm->getRandomNumber(8));
}

bool ChampionMan::f308_isLucky(Champion* champ, uint16 percentage) {
#define AP0646_ui_IsLucky percentage

	if (_vm->getRandomNumber(2) && (_vm->getRandomNumber(100) > percentage)) {
		return true;
	}
	unsigned char* L0928_puc_Statistic = champ->_statistics[k0_ChampionStatLuck];
	AP0646_ui_IsLucky = (_vm->getRandomNumber(L0928_puc_Statistic[k1_ChampionStatCurrent]) > percentage);
	L0928_puc_Statistic[k1_ChampionStatCurrent] = f26_getBoundedValue((int32)L0928_puc_Statistic[k2_ChampionStatMinimum], (int32)L0928_puc_Statistic[k1_ChampionStatCurrent] + (AP0646_ui_IsLucky ? -2 : 2), (int32)L0928_puc_Statistic[k0_ChampionStatMaximum]);
	return AP0646_ui_IsLucky;
}

void ChampionMan::f322_championPoison(int16 champIndex, uint16 attack) {
	if ((champIndex == kM1_ChampionNone) || (_vm->M0_indexToOrdinal(champIndex) == _g299_candidateChampionOrdinal))
		return;

	Champion* L0981_ps_Champion = &_gK71_champions[champIndex];
	f321_addPendingDamageAndWounds_getDamage(champIndex, MAX(1, attack >> 6), k0x0000_ChampionWoundNone, k0_attackType_NORMAL);
	setFlag(L0981_ps_Champion->_attributes, k0x0100_ChampionAttributeStatistics);
	if ((_vm->M0_indexToOrdinal(champIndex) == _vm->_inventoryMan->_g432_inventoryChampionOrdinal) && (_vm->_inventoryMan->_g424_panelContent == k0_PanelContentFoodWaterPoisoned)) {
		setFlag(L0981_ps_Champion->_attributes, k0x0800_ChampionAttributePanel);
	}
	if (--attack) {
		L0981_ps_Champion->_poisonEventCount++;
		TimelineEvent L0980_s_Event;
		L0980_s_Event._type = k75_TMEventTypePoisonChampion;
		L0980_s_Event._priority = champIndex;
		M33_setMapAndTime(L0980_s_Event._mapTime, _vm->_dungeonMan->_g309_partyMapIndex, _vm->_g313_gameTime + 36);
		L0980_s_Event._B._attack = attack;
		_vm->_timeline->f238_addEventGetEventIndex(&L0980_s_Event);
	}
	f292_drawChampionState((ChampionIndex)champIndex);
}

void ChampionMan::f284_setPartyDirection(int16 dir) {
	int16 L0833_i_ChampionIndex;
	int16 L0834_i_Delta;
	Champion* L0835_ps_Champion;


	if (dir == _vm->_dungeonMan->_g308_partyDir) {
		return;
	}
	if ((L0834_i_Delta = dir - _vm->_dungeonMan->_g308_partyDir) < 0) {
		L0834_i_Delta += 4;
	}
	L0835_ps_Champion = _gK71_champions;
	for (L0833_i_ChampionIndex = k0_ChampionFirst; L0833_i_ChampionIndex < _g305_partyChampionCount; L0833_i_ChampionIndex++) {
		L0835_ps_Champion->_cell = (ViewCell)M21_normalizeModulo4(L0835_ps_Champion->_cell + L0834_i_Delta);
		L0835_ps_Champion->_dir = (direction)M21_normalizeModulo4(L0835_ps_Champion->_dir + L0834_i_Delta);
		L0835_ps_Champion++;
	}
	_vm->_dungeonMan->_g308_partyDir = (direction)dir;
	f296_drawChangedObjectIcons();
}

void ChampionMan::f316_deleteScent(uint16 scentIndex) {
	uint16 L0953_ui_Count;

	if (L0953_ui_Count = --_g407_party._scentCount - scentIndex) {
		for (uint16 i = 0; i < L0953_ui_Count; ++i) {
			_g407_party._scents[scentIndex + i] = _g407_party._scents[scentIndex + i + 1];
			_g407_party._scentStrengths[scentIndex + i] = _g407_party._scentStrengths[scentIndex + i + 1];
		}
	}
	if (scentIndex < _g407_party._firstScentIndex) {
		_g407_party._firstScentIndex--;
	}
	if (scentIndex < _g407_party._lastScentIndex) {
		_g407_party._lastScentIndex--;
	}
}

void ChampionMan::f317_addScentStrength(int16 mapX, int16 mapY, int32 cycleCount) {
	int16 L0954_i_ScentIndex = _vm->_championMan->_g407_party._scentCount;
	if (L0954_i_ScentIndex) {
		bool L0955_B_Merge = getFlag(cycleCount, k0x8000_mergeCycles);
		if (L0955_B_Merge) {
			clearFlag(cycleCount, k0x8000_mergeCycles);
		}
		Scent L0958_s_Scent; /* BUG0_00 Useless code */
		L0958_s_Scent.setMapX(mapX); /* BUG0_00 Useless code */
		L0958_s_Scent.setMapY(mapY); /* BUG0_00 Useless code */
		L0958_s_Scent.setMapIndex(_vm->_dungeonMan->_g272_currMapIndex); /* BUG0_00 Useless code */
		Scent* L0957_ps_Scent = _vm->_championMan->_g407_party._scents; /* BUG0_00 Useless code */
		bool L0956_B_CycleCountDefined = false;
		while (L0954_i_ScentIndex--) {
			if (&*L0957_ps_Scent++ == &L0958_s_Scent) {
				if (!L0956_B_CycleCountDefined) {
					L0956_B_CycleCountDefined = true;
					if (L0955_B_Merge) {
						cycleCount = MAX((int32)_g407_party._scentStrengths[L0954_i_ScentIndex], cycleCount);
					} else {
						cycleCount = MIN(80, _g407_party._scentStrengths[L0954_i_ScentIndex] + cycleCount);
					}
				}
				_g407_party._scentStrengths[L0954_i_ScentIndex] = cycleCount;
			}
		}
	}
}

void ChampionMan::f297_putObjectInLeaderHand(Thing thing, bool setMousePointer) {
	if (thing == Thing::_none) {
		return;
	}
	_g415_leaderEmptyHanded = false;
	_vm->_objectMan->f36_extractIconFromBitmap(_g413_leaderHandObjectIconIndex = _vm->_objectMan->f33_getIconIndex(_g414_leaderHandObject = thing), _vm->_objectMan->_g412_objectIconForMousePointer);
	_vm->_eventMan->f78_showMouse();
	_vm->_objectMan->f34_drawLeaderObjectName(thing);
	if (setMousePointer) {
		_vm->_g325_setMousePointerToObjectInMainLoop = true;
	} else {
		_vm->_eventMan->f68_setPointerToObject(_vm->_objectMan->_g412_objectIconForMousePointer);
	}
	_vm->_eventMan->f77_hideMouse();
	if (_g411_leaderIndex != kM1_ChampionNone) {
		_gK71_champions[_g411_leaderIndex]._load += _vm->_dungeonMan->f140_getObjectWeight(thing);
		setFlag(_gK71_champions[_g411_leaderIndex]._attributes, k0x0200_ChampionAttributeLoad);
		f292_drawChampionState(_g411_leaderIndex);
	}
}

int16 ChampionMan::f310_getMovementTicks(Champion* champ) {
	uint16 L0931_ui_Multiple;
#define AL0931_ui_Load       L0931_ui_Multiple
#define AL0931_ui_WoundTicks L0931_ui_Multiple
	uint16 L0932_ui_MaximumLoad;
	int16 L0933_i_Ticks;


	if ((L0932_ui_MaximumLoad = _vm->_championMan->f309_getMaximumLoad(champ)) > (AL0931_ui_Load = champ->_load)) { /* BUG0_72 The party moves very slowly even though no champion 'Load' value is drawn in red. When the Load of a champion has exactly the maximum value he can carry then the Load is drawn in yellow but the speed is the same as when the champion is overloaded (when the Load is drawn in red). The comparison operator should be >= instead of > */
		L0933_i_Ticks = 2;
		if (((int32)AL0931_ui_Load << 3) > ((int32)L0932_ui_MaximumLoad * 5)) {
			L0933_i_Ticks++;
		}
		AL0931_ui_WoundTicks = 1;
	} else {
		L0933_i_Ticks = 4 + (((AL0931_ui_Load - L0932_ui_MaximumLoad) << 2) / L0932_ui_MaximumLoad);
		AL0931_ui_WoundTicks = 2;
	}
	if (getFlag(champ->_wounds, k0x0020_ChampionWoundFeet)) {
		L0933_i_Ticks += AL0931_ui_WoundTicks;
	}
	if (_vm->_objectMan->f33_getIconIndex(champ->_slots[k5_ChampionSlotFeet]) == k194_IconIndiceArmourBootOfSpeed) {
		L0933_i_Ticks--;
	}
	return L0933_i_Ticks;
}

bool ChampionMan::f294_isAmmunitionCompatibleWithWeapon(uint16 champIndex, uint16 weaponSlotIndex, uint16 ammunitionSlotIndex) {
	Champion* L0874_ps_Champion;
	WeaponInfo* L0875_ps_WeaponInfo;
	Thing L0878_T_Thing;
	int16 L0879_i_WeaponClass;

	L0874_ps_Champion = &_vm->_championMan->_gK71_champions[champIndex];
	L0878_T_Thing = L0874_ps_Champion->_slots[weaponSlotIndex];
	if (L0878_T_Thing.getType() != k5_WeaponThingType) {
		return false;
	}
	L0875_ps_WeaponInfo = _vm->_dungeonMan->f158_getWeaponInfo(L0878_T_Thing);
	if ((L0875_ps_WeaponInfo->_class >= k16_WeaponClassFirstBow) && (L0875_ps_WeaponInfo->_class <= k31_WeaponClassLastBow)) {
		L0879_i_WeaponClass = k10_WeaponClassBowAmmunition;
	} else {
		if ((L0875_ps_WeaponInfo->_class >= k32_WeaponClassFirstSling) && (L0875_ps_WeaponInfo->_class <= k47_WeaponClassLastSling)) {
			L0879_i_WeaponClass = k11_WeaponClassSlingAmmunition;
		} else {
			return false;
		}
	}
	L0878_T_Thing = L0874_ps_Champion->_slots[ammunitionSlotIndex];
	L0875_ps_WeaponInfo = _vm->_dungeonMan->f158_getWeaponInfo(L0878_T_Thing);
	return ((L0878_T_Thing.getType() == k5_WeaponThingType) && (L0875_ps_WeaponInfo->_class == L0879_i_WeaponClass));
}

void ChampionMan::f293_drawAllChampionStates() {
	int16 L0873_i_ChampionIndex;
	for (L0873_i_ChampionIndex = k0_ChampionFirst; L0873_i_ChampionIndex < _vm->_championMan->_g305_partyChampionCount; L0873_i_ChampionIndex++) {
		_vm->_championMan->f292_drawChampionState((ChampionIndex)L0873_i_ChampionIndex);
	}
}

void ChampionMan::f283_viAltarRebirth(uint16 champIndex) {
	uint16 L0831_ui_Multiple;
#define AL0831_ui_Cell          L0831_ui_Multiple
#define AL0831_ui_MaximumHealth L0831_ui_Multiple
	Champion* L0832_ps_Champion;

	L0832_ps_Champion = &_vm->_championMan->_gK71_champions[champIndex];
	if (_vm->_championMan->f285_getIndexInCell(L0832_ps_Champion->_cell) != kM1_ChampionNone) {
		AL0831_ui_Cell = k0_CellNorthWest;
		while (_vm->_championMan->f285_getIndexInCell(AL0831_ui_Cell) != kM1_ChampionNone) {
			AL0831_ui_Cell++;
		}
		L0832_ps_Champion->_cell = (ViewCell)AL0831_ui_Cell;
	}
	AL0831_ui_MaximumHealth = L0832_ps_Champion->_maxHealth;
	L0832_ps_Champion->_currHealth = (L0832_ps_Champion->_maxHealth = MAX(25, AL0831_ui_MaximumHealth - (AL0831_ui_MaximumHealth >> 6) - 1)) >> 1;
	_vm->_menuMan->f393_drawSpellAreaControls(_vm->_championMan->_g514_magicCasterChampionIndex);
	L0832_ps_Champion->_dir = _vm->_dungeonMan->_g308_partyDir;
	setFlag(L0832_ps_Champion->_attributes, k0x8000_ChampionAttributeActionHand | k0x1000_ChampionAttributeStatusBox | k0x0400_ChampionAttributeIcon);
	_vm->_championMan->f292_drawChampionState((ChampionIndex)champIndex);
}

void ChampionMan::f302_processCommands28to65_clickOnSlotBox(uint16 slotBoxIndex) {
	uint16 champIndex;
	uint16 slotIndex;

	if (slotBoxIndex < k8_SlotBoxInventoryFirstSlot) {
		if (_g299_candidateChampionOrdinal) {
			return;
		}
		champIndex = slotBoxIndex >> 1;
		if ((champIndex >= _g305_partyChampionCount) || (_vm->M0_indexToOrdinal(champIndex) == _vm->_inventoryMan->_g432_inventoryChampionOrdinal) || !_gK71_champions[champIndex]._currHealth) {
			return;
		}
		slotIndex = M70_handSlotIndex(slotBoxIndex);
	} else {
		champIndex = _vm->M1_ordinalToIndex(_vm->_inventoryMan->_g432_inventoryChampionOrdinal);
		slotIndex = slotBoxIndex - k8_SlotBoxInventoryFirstSlot;
	}
	Thing leaderHandObject = _g414_leaderHandObject;
	Thing slotThing;
	if (slotIndex >= k30_ChampionSlotChest_1) {
		slotThing = _vm->_inventoryMan->_g425_chestSlots[slotIndex - k30_ChampionSlotChest_1];
	} else {
		slotThing = _gK71_champions[champIndex]._slots[slotIndex];
	}
	if ((slotThing == Thing::_none) && (leaderHandObject == Thing::_none)) {
		return;
	}
	if ((leaderHandObject != Thing::_none) && (!(g237_ObjectInfo[_vm->_dungeonMan->f141_getObjectInfoIndex(leaderHandObject)]._allowedSlots & g38_slotMasks[slotIndex]))) {
		return;
	}
	_vm->_eventMan->f78_showMouse();
	if (leaderHandObject != Thing::_none) {
		f298_getObjectRemovedFromLeaderHand();
	}
	if (slotThing != Thing::_none) {
		f300_getObjectRemovedFromSlot(champIndex, slotIndex);
		f297_putObjectInLeaderHand(slotThing, false);
	}
	if (leaderHandObject != Thing::_none) {
		f301_addObjectInSlot((ChampionIndex)champIndex, leaderHandObject, (ChampionSlot)slotIndex);
	}
	f292_drawChampionState((ChampionIndex)champIndex);
	_vm->_eventMan->f77_hideMouse();
}

bool ChampionMan::f327_isProjectileSpellCast(uint16 champIndex, Thing thing, int16 kineticEnergy, uint16 requiredManaAmount) {
	int16 L0991_i_StepEnergy;
	Champion* L0992_ps_Champion;

	L0992_ps_Champion = &_vm->_championMan->_gK71_champions[champIndex];
	if (L0992_ps_Champion->_currMana < requiredManaAmount) {
		return false;
	}
	L0992_ps_Champion->_currMana -= requiredManaAmount;
	setFlag(L0992_ps_Champion->_attributes, k0x0100_ChampionAttributeStatistics);
	L0991_i_StepEnergy = 10 - MIN(8, L0992_ps_Champion->_maxMana >> 3);
	if (kineticEnergy < (L0991_i_StepEnergy << 2)) {
		kineticEnergy += 3;
		L0991_i_StepEnergy--;
	}
	f326_championShootProjectile(L0992_ps_Champion, thing, kineticEnergy, 90, L0991_i_StepEnergy);
	return true; // fix BUG_01
}

void ChampionMan::f326_championShootProjectile(Champion* champ, Thing thing, int16 kineticEnergy, int16 attack, int16 stepEnergy) {
	uint16 L0990_ui_Direction;

	L0990_ui_Direction = champ->_dir;
	_vm->_projexpl->f212_projectileCreate(thing, _vm->_dungeonMan->_g306_partyMapX, _vm->_dungeonMan->_g307_partyMapY, M21_normalizeModulo4((((champ->_cell - L0990_ui_Direction + 1) & 0x0002) >> 1) + L0990_ui_Direction), (direction)L0990_ui_Direction, kineticEnergy, attack, stepEnergy);
	_vm->_g311_projectileDisableMovementTicks = 4;
	_vm->_g312_lastProjectileDisabledMovementDirection = L0990_ui_Direction;
}

void ChampionMan::f320_applyAndDrawPendingDamageAndWounds() {
	uint16 L0967_ui_ChampionIndex;
	uint16 L0968_ui_PendingDamage;
	int16 L0969_i_Multiple;
#define AL0969_i_Health     L0969_i_Multiple
#define AL0969_i_X          L0969_i_Multiple
#define AL0969_i_EventIndex L0969_i_Multiple
	int16 L0970_i_PendingWounds;
	Champion* L0971_ps_Champion;
	TimelineEvent* L0972_ps_Event;
	int16 L0973_i_Y;
	TimelineEvent L0974_s_Event;
	Box L0975_s_Box;


	L0971_ps_Champion = _vm->_championMan->_gK71_champions;
	for (L0967_ui_ChampionIndex = k0_ChampionFirst; L0967_ui_ChampionIndex < _vm->_championMan->_g305_partyChampionCount; L0967_ui_ChampionIndex++, L0971_ps_Champion++) {
		setFlag(L0971_ps_Champion->_wounds, L0970_i_PendingWounds = _g410_championPendingWounds[L0967_ui_ChampionIndex]);
		_g410_championPendingWounds[L0967_ui_ChampionIndex] = 0;
		if (!(L0968_ui_PendingDamage = _g409_championPendingDamage[L0967_ui_ChampionIndex]))
			continue;
		_g409_championPendingDamage[L0967_ui_ChampionIndex] = 0;
		if (!(AL0969_i_Health = L0971_ps_Champion->_currHealth))
			continue;
		if ((AL0969_i_Health = AL0969_i_Health - L0968_ui_PendingDamage) <= 0) {
			_vm->_championMan->f319_championKill(L0967_ui_ChampionIndex);
		} else {
			L0971_ps_Champion->_currHealth = AL0969_i_Health;
			setFlag(L0971_ps_Champion->_attributes, k0x0100_ChampionAttributeStatistics);
			if (L0970_i_PendingWounds) {
				setFlag(L0971_ps_Champion->_attributes, k0x2000_ChampionAttributeWounds);
			}
			AL0969_i_X = L0967_ui_ChampionIndex * k69_ChampionStatusBoxSpacing;
			L0975_s_Box._y1 = 0;
			_vm->_eventMan->f78_showMouse();
			if (_vm->M0_indexToOrdinal(L0967_ui_ChampionIndex) == _vm->_inventoryMan->_g432_inventoryChampionOrdinal) {
				L0975_s_Box._y2 = 28;
				L0975_s_Box._x2 = (L0975_s_Box._x1 = AL0969_i_X + 7) + 31; /* Box is over the champion portrait in the status box */
				_vm->_displayMan->f21_blitToScreen(_vm->_displayMan->f489_getNativeBitmapOrGraphic(k16_damageToChampionBig), &L0975_s_Box, k16_byteWidth, k10_ColorFlesh, 29);
				if (L0968_ui_PendingDamage < 10) { /* 1 digit */
					AL0969_i_X += 21;
				} else {
					if (L0968_ui_PendingDamage < 100) { /* 2 digits */
						AL0969_i_X += 18;
					} else { /* 3 digits */
						AL0969_i_X += 15;
					}
				}
				L0973_i_Y = 16;
			} else {
				L0975_s_Box._y2 = 6;
				L0975_s_Box._x2 = (L0975_s_Box._x1 = AL0969_i_X) + 47; /* Box is over the champion name in the status box */
				_vm->_displayMan->f21_blitToScreen(_vm->_displayMan->f489_getNativeBitmapOrGraphic(k15_damageToChampionSmallIndice), &L0975_s_Box, k24_byteWidth, k10_ColorFlesh, 7);
				if (L0968_ui_PendingDamage < 10) { /* 1 digit */
					AL0969_i_X += 19;
				} else {
					if (L0968_ui_PendingDamage < 100) { /* 2 digits */
						AL0969_i_X += 16;
					} else { /* 3 digits */
						AL0969_i_X += 13;
					}
				}
				L0973_i_Y = 5;
			}
			_vm->_textMan->f53_printToLogicalScreen(AL0969_i_X, L0973_i_Y, k15_ColorWhite, k8_ColorRed, _vm->_championMan->f288_getStringFromInteger(L0968_ui_PendingDamage, false, 3).c_str());
			if ((AL0969_i_EventIndex = L0971_ps_Champion->_hideDamageReceivedIndex) == -1) {
				L0974_s_Event._type = k12_TMEventTypeHideDamageReceived;
				M33_setMapAndTime(L0974_s_Event._mapTime, _vm->_dungeonMan->_g309_partyMapIndex, _vm->_g313_gameTime + 5);
				L0974_s_Event._priority = L0967_ui_ChampionIndex;
				L0971_ps_Champion->_hideDamageReceivedIndex = _vm->_timeline->f238_addEventGetEventIndex(&L0974_s_Event);
			} else {
				L0972_ps_Event = &_vm->_timeline->_g370_events[AL0969_i_EventIndex];
				M33_setMapAndTime(L0972_ps_Event->_mapTime, _vm->_dungeonMan->_g309_partyMapIndex, _vm->_g313_gameTime + 5);
				_vm->_timeline->f236_fixChronology(_vm->_timeline->f235_getIndex(AL0969_i_EventIndex));
			}
			_vm->_championMan->f292_drawChampionState((ChampionIndex)L0967_ui_ChampionIndex);
			_vm->_eventMan->f77_hideMouse();
		}
	}
}

void ChampionMan::f319_championKill(uint16 champIndex) {
	uint16 L0962_ui_Multiple = 0;
#define AL0962_ui_Cell              L0962_ui_Multiple
#define AL0962_ui_ChampionIconIndex L0962_ui_Multiple
	int16 L0963_i_AliveChampionIndex;
	Thing L0964_T_Thing;
	Champion* L0965_ps_Champion;

	L0965_ps_Champion = &_vm->_championMan->_gK71_champions[champIndex];
	L0965_ps_Champion->_currHealth = 0;
	setFlag(L0965_ps_Champion->_attributes, k0x1000_ChampionAttributeStatusBox);
	if (_vm->M0_indexToOrdinal(champIndex) == _vm->_inventoryMan->_g432_inventoryChampionOrdinal) {
		if (_vm->_g331_pressingEye) {
			_vm->_g331_pressingEye = false;
			_vm->_eventMan->_g597_ignoreMouseMovements = false;
			if (!_vm->_championMan->_g415_leaderEmptyHanded) {
				_vm->_objectMan->f34_drawLeaderObjectName(_vm->_championMan->_g414_leaderHandObject);
			}
			_vm->_eventMan->_g587_hideMousePointerRequestCount = 1;
			_vm->_eventMan->f77_hideMouse();
		} else {
			if (_vm->_g333_pressingMouth) {
				_vm->_g333_pressingMouth = false;
				_vm->_eventMan->_g597_ignoreMouseMovements = false;
				_vm->_eventMan->_g587_hideMousePointerRequestCount = 1;
				_vm->_eventMan->f77_hideMouse();
			}
		}
		_vm->_inventoryMan->f355_toggleInventory(k4_ChampionCloseInventory);
	}
	f318_dropAllObjects(champIndex);
	L0964_T_Thing = _vm->_dungeonMan->f166_getUnusedThing(k0x8000_championBones | k10_JunkThingType);
	if (L0964_T_Thing == Thing::_none) {
	} else {
		Junk* L0966_ps_Junk = (Junk*)_vm->_dungeonMan->f156_getThingData(L0964_T_Thing);
		L0966_ps_Junk->setType(k5_JunkTypeBones);
		L0966_ps_Junk->setDoNotDiscard(true);
		L0966_ps_Junk->setChargeCount(champIndex);
		AL0962_ui_Cell = L0965_ps_Champion->_cell;
		_vm->_movsens->f267_getMoveResult(M15_thingWithNewCell(L0964_T_Thing, AL0962_ui_Cell), kM1_MapXNotOnASquare, 0, _vm->_dungeonMan->_g306_partyMapX, _vm->_dungeonMan->_g307_partyMapY);
	}
	L0965_ps_Champion->_symbolStep = 0;
	L0965_ps_Champion->_symbols[0] = '\0';
	L0965_ps_Champion->_dir = _vm->_dungeonMan->_g308_partyDir;
	L0965_ps_Champion->_maximumDamageReceived = 0;
	AL0962_ui_ChampionIconIndex = _vm->_championMan->M26_championIconIndex(AL0962_ui_Cell, _vm->_dungeonMan->_g308_partyDir);
	if (_vm->M0_indexToOrdinal(AL0962_ui_ChampionIconIndex) == _vm->_eventMan->_g599_useChampionIconOrdinalAsMousePointerBitmap) {
		_vm->_eventMan->_g598_mousePointerBitmapUpdated = true;
		_vm->_eventMan->_g599_useChampionIconOrdinalAsMousePointerBitmap = _vm->M0_indexToOrdinal(kM1_ChampionNone);
		warning(false, "IGNORED CODE:G0592_B_BuildMousePointerScreenAreaRequested = true");
	}
	if (L0965_ps_Champion->_poisonEventCount) {
		f323_unpoison(champIndex);
	}
	_vm->_displayMan->_g578_useByteBoxCoordinates = false;
	_vm->_displayMan->D24_fillScreenBox(g54_BoxChampionIcons[AL0962_ui_ChampionIconIndex << 2], k0_ColorBlack);
	_vm->_championMan->f292_drawChampionState((ChampionIndex)champIndex);
	for (L0963_i_AliveChampionIndex = k0_ChampionFirst, L0965_ps_Champion = _vm->_championMan->_gK71_champions; L0963_i_AliveChampionIndex < _vm->_championMan->_g305_partyChampionCount; L0963_i_AliveChampionIndex++, L0965_ps_Champion++) {
		if (L0965_ps_Champion->_currHealth)
			break;
	}
	if (L0963_i_AliveChampionIndex == _vm->_championMan->_g305_partyChampionCount) { /* BUG0_43 The game does not end if the last living champion in the party is killed while looking at a candidate champion in a portrait. The condition to end the game when the whole party is killed is not true because the code considers the candidate champion as alive (in the loop above) */
		_vm->_championMan->_g303_partyDead = true;
		return;
	}
	if (champIndex == _vm->_championMan->_g411_leaderIndex) {
		_vm->_eventMan->f368_commandSetLeader((ChampionIndex)L0963_i_AliveChampionIndex);
	}
	if (champIndex == _vm->_championMan->_g514_magicCasterChampionIndex) {
		_vm->_menuMan->f394_setMagicCasterAndDrawSpellArea(L0963_i_AliveChampionIndex);
	} else {
		_vm->_menuMan->f393_drawSpellAreaControls(_vm->_championMan->_g514_magicCasterChampionIndex);
	}
}

void ChampionMan::f318_dropAllObjects(uint16 champIndex) {
	static int16 G0057_ai_Graphic562_SlotDropOrder[30] = {
		k5_ChampionSlotFeet,
		k4_ChampionSlotLegs,
		k9_ChampionSlotQuiverLine_2_2,
		k8_ChampionSlotQuiverLine_1_2,
		k7_ChampionSlotQuiverLine_2_1,
		k12_ChampionSlotQuiverLine_1_1,
		k6_ChampionSlotPouch_2,
		k11_ChampionSlotPouch_1,
		k3_ChampionSlotTorso,
		k13_ChampionSlotBackpackLine_1_1,
		k14_ChampionSlotBackpackLine_2_2,
		k15_ChampionSlotBackpackLine_2_3,
		k16_ChampionSlotBackpackLine_2_4,
		k17_ChampionSlotBackpackLine_2_5,
		k18_ChampionSlotBackpackLine_2_6,
		k19_ChampionSlotBackpackLine_2_7,
		k20_ChampionSlotBackpackLine_2_8,
		k21_ChampionSlotBackpackLine_2_9,
		k22_ChampionSlotBackpackLine_1_2,
		k23_ChampionSlotBackpackLine_1_3,
		k24_ChampionSlotBackpackLine_1_4,
		k25_ChampionSlotBackpackLine_1_5,
		k26_ChampionSlotBackpackLine_1_6,
		k27_ChampionSlotBackpackLine_1_7,
		k28_ChampionSlotBackpackLine_1_8,
		k29_ChampionSlotBackpackLine_1_9,
		k10_ChampionSlotNeck,
		k2_ChampionSlotHead,
		k0_ChampionSlotReadyHand,
		k1_ChampionSlotActionHand};

	uint16 L0959_ui_Cell;
	Thing L0960_T_Thing;
	uint16 L0961_ui_SlotIndex;

	L0959_ui_Cell = _vm->_championMan->_gK71_champions[champIndex]._cell;
	for (L0961_ui_SlotIndex = k0_ChampionSlotReadyHand; L0961_ui_SlotIndex < k30_ChampionSlotChest_1; L0961_ui_SlotIndex++) {
		if ((L0960_T_Thing = f300_getObjectRemovedFromSlot(champIndex, G0057_ai_Graphic562_SlotDropOrder[L0961_ui_SlotIndex])) != Thing::_none) {
			_vm->_movsens->f267_getMoveResult(M15_thingWithNewCell(L0960_T_Thing, L0959_ui_Cell), kM1_MapXNotOnASquare, 0, _vm->_dungeonMan->_g306_partyMapX, _vm->_dungeonMan->_g307_partyMapY);
		}
	}
}

void ChampionMan::f323_unpoison(int16 champIndex) {
	int16 L0982_i_EventIndex;
	TimelineEvent* L0983_ps_Event;

	if (champIndex == kM1_ChampionNone) {
		return;
	}
	for (L0982_i_EventIndex = 0, L0983_ps_Event = _vm->_timeline->_g370_events; L0982_i_EventIndex < _vm->_timeline->_g369_eventMaxCount; L0983_ps_Event++, L0982_i_EventIndex++) {
		if ((L0983_ps_Event->_type == k75_TMEventTypePoisonChampion) && (L0983_ps_Event->_priority == champIndex)) {
			_vm->_timeline->f237_deleteEvent(L0982_i_EventIndex);
		}
	}
	_vm->_championMan->_gK71_champions[champIndex]._poisonEventCount = 0;
}

void ChampionMan::f331_applyTimeEffects() {
	uint16 L1006_ui_Multiple;
#define AL1006_ui_GameTime      L1006_ui_Multiple
#define AL1006_ui_ChampionIndex L1006_ui_Multiple
	uint16 L1007_ui_Multiple;
#define AL1007_ui_ScentIndex            L1007_ui_Multiple
#define AL1007_ui_ManaGain              L1007_ui_Multiple
#define AL1007_ui_StaminaGainCycleCount L1007_ui_Multiple
#define AL1007_ui_StatisticIndex        L1007_ui_Multiple
	uint16 L1008_ui_Multiple;
#define AL1008_ui_WizardSkillLevel L1008_ui_Multiple
#define AL1008_ui_Delay            L1008_ui_Multiple
#define AL1008_ui_StaminaAboveHalf L1008_ui_Multiple
#define AL1008_ui_StatisticMaximum L1008_ui_Multiple
	int16 L1009_i_Multiple;
#define AL1009_i_SkillIndex       L1009_i_Multiple
#define AL1009_i_StaminaMagnitude L1009_i_Multiple
#define AL1009_i_StaminaLoss      L1009_i_Multiple
	Champion* L1010_ps_Champion;
	unsigned char* L1011_puc_Statistic;
	uint16 L1012_ui_TimeCriteria;
	int16 L1013_i_Multiple;
#define AL1013_i_StaminaAmount L1013_i_Multiple
#define AL1013_i_HealthGain    L1013_i_Multiple
	Scent L1014_s_Scent;


	if (!_vm->_championMan->_g305_partyChampionCount) {
		return;
	}
	L1014_s_Scent.setMapX(_vm->_dungeonMan->_g306_partyMapX);
	L1014_s_Scent.setMapY(_vm->_dungeonMan->_g307_partyMapY);
	L1014_s_Scent.setMapIndex(_vm->_dungeonMan->_g309_partyMapIndex);
	AL1007_ui_ScentIndex = 0;
	while ((int16)AL1007_ui_ScentIndex < (int16)(_vm->_championMan->_g407_party._scentCount - 1)) {
		if (&_vm->_championMan->_g407_party._scents[AL1007_ui_ScentIndex] != &L1014_s_Scent) {
			if (!(_vm->_championMan->_g407_party._scentStrengths[AL1007_ui_ScentIndex] = MAX(0, _vm->_championMan->_g407_party._scentStrengths[AL1007_ui_ScentIndex] - 1)) && !AL1007_ui_ScentIndex) {
				f316_deleteScent(0);
				continue;
			}
		}
		AL1007_ui_ScentIndex++;
	}
	AL1006_ui_GameTime = _vm->_g313_gameTime;
	L1012_ui_TimeCriteria = (((AL1006_ui_GameTime & 0x0080) + ((AL1006_ui_GameTime & 0x0100) >> 2)) + ((AL1006_ui_GameTime & 0x0040) << 2)) >> 2;
	for (AL1006_ui_ChampionIndex = k0_ChampionFirst, L1010_ps_Champion = _vm->_championMan->_gK71_champions; AL1006_ui_ChampionIndex < _vm->_championMan->_g305_partyChampionCount; AL1006_ui_ChampionIndex++, L1010_ps_Champion++) {
		if (L1010_ps_Champion->_currHealth && (_vm->M0_indexToOrdinal(AL1006_ui_ChampionIndex) != _vm->_championMan->_g299_candidateChampionOrdinal)) {
			if ((L1010_ps_Champion->_currMana < L1010_ps_Champion->_maxMana) && (L1012_ui_TimeCriteria < (L1010_ps_Champion->_statistics[k3_ChampionStatWisdom][k1_ChampionStatCurrent] + (AL1008_ui_WizardSkillLevel = _vm->_championMan->f303_getSkillLevel(AL1006_ui_ChampionIndex, k3_ChampionSkillWizard) + _vm->_championMan->f303_getSkillLevel(AL1006_ui_ChampionIndex, k2_ChampionSkillPriest))))) {
				AL1007_ui_ManaGain = L1010_ps_Champion->_maxMana / 40;
				if (_vm->_championMan->_g300_partyIsSleeping) {
					AL1007_ui_ManaGain = AL1007_ui_ManaGain << 1;
				}
				AL1007_ui_ManaGain++;
				f325_decrementStamina(AL1006_ui_ChampionIndex, AL1007_ui_ManaGain * MAX(7, 16 - AL1008_ui_WizardSkillLevel));
				L1010_ps_Champion->_currMana += MIN(AL1007_ui_ManaGain, (uint16)(L1010_ps_Champion->_maxMana - L1010_ps_Champion->_currMana));
			} else {
				if (L1010_ps_Champion->_currMana > L1010_ps_Champion->_maxMana) {
					L1010_ps_Champion->_currMana--;
				}
			}
			for (AL1009_i_SkillIndex = k19_ChampionSkillWater; AL1009_i_SkillIndex >= k0_ChampionSkillFighter; AL1009_i_SkillIndex--) {
				if (L1010_ps_Champion->_skills[AL1009_i_SkillIndex]._temporaryExperience > 0) {
					L1010_ps_Champion->_skills[AL1009_i_SkillIndex]._temporaryExperience--;
				}
			}
			AL1007_ui_StaminaGainCycleCount = 4;
			AL1009_i_StaminaMagnitude = L1010_ps_Champion->_maxStamina;
			while (L1010_ps_Champion->_currStamina < (AL1009_i_StaminaMagnitude >>= 1)) {
				AL1007_ui_StaminaGainCycleCount += 2;
			}
			AL1009_i_StaminaLoss = 0;
			AL1013_i_StaminaAmount = f26_getBoundedValue(1, (L1010_ps_Champion->_maxStamina >> 8) - 1, 6);
			if (_vm->_championMan->_g300_partyIsSleeping) {
				AL1013_i_StaminaAmount <<= 1;
			}
			if ((AL1008_ui_Delay = (_vm->_g313_gameTime - _vm->_projexpl->_g362_lastPartyMovementTime)) > 80) {
				AL1013_i_StaminaAmount++;
				if (AL1008_ui_Delay > 250) {
					AL1013_i_StaminaAmount++;
				}
			}
			do {
				AL1008_ui_StaminaAboveHalf = (AL1007_ui_StaminaGainCycleCount <= 4);
				if (L1010_ps_Champion->_food < -512) {
					if (AL1008_ui_StaminaAboveHalf) {
						AL1009_i_StaminaLoss += AL1013_i_StaminaAmount;
						L1010_ps_Champion->_food -= 2;
					}
				} else {
					if (L1010_ps_Champion->_food >= 0) {
						AL1009_i_StaminaLoss -= AL1013_i_StaminaAmount;
					}
					L1010_ps_Champion->_food -= AL1008_ui_StaminaAboveHalf ? 2 : AL1007_ui_StaminaGainCycleCount >> 1;
				}
				if (L1010_ps_Champion->_water < -512) {
					if (AL1008_ui_StaminaAboveHalf) {
						AL1009_i_StaminaLoss += AL1013_i_StaminaAmount;
						L1010_ps_Champion->_water -= 1;
					}
				} else {
					if (L1010_ps_Champion->_water >= 0) {
						AL1009_i_StaminaLoss -= AL1013_i_StaminaAmount;
					}
					L1010_ps_Champion->_water -= AL1008_ui_StaminaAboveHalf ? 1 : AL1007_ui_StaminaGainCycleCount >> 2;
				}
			} while (--AL1007_ui_StaminaGainCycleCount && ((L1010_ps_Champion->_currStamina - AL1009_i_StaminaLoss) < L1010_ps_Champion->_maxStamina));
			f325_decrementStamina(AL1006_ui_ChampionIndex, AL1009_i_StaminaLoss);
			if (L1010_ps_Champion->_food < -1024) {
				L1010_ps_Champion->_food = -1024;
			}
			if (L1010_ps_Champion->_water < -1024) {
				L1010_ps_Champion->_water = -1024;
			}
			if ((L1010_ps_Champion->_currHealth < L1010_ps_Champion->_maxHealth) && (L1010_ps_Champion->_currStamina >= (L1010_ps_Champion->_maxStamina >> 2)) && (L1012_ui_TimeCriteria < (L1010_ps_Champion->_statistics[k4_ChampionStatVitality][k1_ChampionStatCurrent] + 12))) {
				AL1013_i_HealthGain = (L1010_ps_Champion->_maxHealth >> 7) + 1;
				if (_vm->_championMan->_g300_partyIsSleeping) {
					AL1013_i_HealthGain <<= 1;
				}
				if (_vm->_objectMan->f33_getIconIndex(L1010_ps_Champion->_slots[k10_ChampionSlotNeck]) == k121_IconIndiceJunkEkkhardCross) {
					AL1013_i_HealthGain += (AL1013_i_HealthGain >> 1) + 1;
				}
				L1010_ps_Champion->_currHealth += MIN(AL1013_i_HealthGain, (int16)(L1010_ps_Champion->_maxHealth - L1010_ps_Champion->_currHealth));
			}
			if (!((int)_vm->_g313_gameTime & (_vm->_championMan->_g300_partyIsSleeping ? 63 : 255))) {
				for (AL1007_ui_StatisticIndex = k0_ChampionStatLuck; AL1007_ui_StatisticIndex <= k6_ChampionStatAntifire; AL1007_ui_StatisticIndex++) {
					L1011_puc_Statistic = L1010_ps_Champion->_statistics[AL1007_ui_StatisticIndex];
					AL1008_ui_StatisticMaximum = L1011_puc_Statistic[k0_ChampionStatMaximum];
					if (L1011_puc_Statistic[k1_ChampionStatCurrent] < AL1008_ui_StatisticMaximum) {
						L1011_puc_Statistic[k1_ChampionStatCurrent]++;
					} else {
						if (L1011_puc_Statistic[k1_ChampionStatCurrent] > AL1008_ui_StatisticMaximum) {
							L1011_puc_Statistic[k1_ChampionStatCurrent] -= L1011_puc_Statistic[k1_ChampionStatCurrent] / AL1008_ui_StatisticMaximum;
						}
					}
				}
			}
			if (!_vm->_championMan->_g300_partyIsSleeping && (L1010_ps_Champion->_dir != _vm->_dungeonMan->_g308_partyDir) && (_vm->_projexpl->_g361_lastCreatureAttackTime + 60 < (int32)_vm->_g313_gameTime)) {
				L1010_ps_Champion->_dir = _vm->_dungeonMan->_g308_partyDir;
				L1010_ps_Champion->_maximumDamageReceived = 0;
				setFlag(L1010_ps_Champion->_attributes, k0x0400_ChampionAttributeIcon);
			}
			setFlag(L1010_ps_Champion->_attributes, k0x0100_ChampionAttributeStatistics);
			if (_vm->M0_indexToOrdinal(AL1006_ui_ChampionIndex) == _vm->_inventoryMan->_g432_inventoryChampionOrdinal) {
				if (_vm->_g333_pressingMouth || _vm->_g331_pressingEye || (_vm->_inventoryMan->_g424_panelContent == k0_PanelContentFoodWaterPoisoned)) {
					setFlag(L1010_ps_Champion->_attributes, k0x0800_ChampionAttributePanel);
				}
			}
		}
	}
	f293_drawAllChampionStates();
}

void ChampionMan::save2_PartyPart(Common::OutSaveFile* file) {
	for (uint16 i = 0; i < 4; ++i) {
		Champion *champ = &_gK71_champions[i];
		file->writeUint16BE(champ->_attributes);
		file->writeUint16BE(champ->_wounds);
		for (uint16 y = 0; y < 7; ++y)
			for (uint16 x = 0; x < 3; ++x)
				file->writeByte(champ->_statistics[y][x]);
		for (uint16 j = 0; j < 30; ++j)
			file->writeUint16BE(champ->_slots[j].toUint16());
		for (uint16 j = 0; j < 20; ++j) {
			file->writeSint16BE(champ->_skills[j]._temporaryExperience);
			file->writeSint32BE(champ->_skills[j]._experience);
		}
		for (uint16 j = 0; j < 8; ++j)
			file->writeByte(champ->_name[j]);
		for (uint16 j = 0; j < 20; ++j)
			file->writeByte(champ->_title[j]);
		file->writeUint16BE(champ->_dir);
		file->writeUint16BE(champ->_cell);
		file->writeUint16BE(champ->_actionIndex);
		file->writeUint16BE(champ->_symbolStep);
		for (uint16 j = 0; j < 5; ++j)
			file->writeByte(champ->_symbols[j]);
		file->writeUint16BE(champ->_directionMaximumDamageReceived);
		file->writeUint16BE(champ->_maximumDamageReceived);
		file->writeUint16BE(champ->_poisonEventCount);
		file->writeSint16BE(champ->_enableActionEventIndex);
		file->writeSint16BE(champ->_hideDamageReceivedIndex);
		file->writeSint16BE(champ->_currHealth);
		file->writeSint16BE(champ->_maxHealth);
		file->writeSint16BE(champ->_currStamina);
		file->writeSint16BE(champ->_maxStamina);
		file->writeSint16BE(champ->_currMana);
		file->writeSint16BE(champ->_maxMana);
		file->writeSint16BE(champ->_actionDefense);
		file->writeSint16BE(champ->_food);
		file->writeSint16BE(champ->_water);
		file->writeUint16BE(champ->_load);
		file->writeSint16BE(champ->_shieldDefense);
		for (uint16 j = 0; j < 928; ++j)
			file->writeByte(champ->_portrait[j]);
	}

	Party &party = _g407_party;
	file->writeSint16BE(party._magicalLightAmount);
	file->writeByte(party._event73Count_ThievesEye);
	file->writeByte(party._event79Count_Footprints);
	file->writeSint16BE(party._shieldDefense);
	file->writeSint16BE(party._fireShieldDefense);
	file->writeSint16BE(party._spellShieldDefense);
	file->writeByte(party._scentCount);
	file->writeByte(party._freezeLifeTicks);
	file->writeByte(party._firstScentIndex);
	file->writeByte(party._lastScentIndex);
	for (uint16 i = 0; i < 24; ++i)
		file->writeUint16BE(party._scents[i].toUint16());
	for (uint16 i = 0; i < 24; ++i)
		file->writeByte(party._scentStrengths[i]);
	file->writeByte(party._event71Count_Invisibility);
}

void ChampionMan::load2_PartyPart(Common::InSaveFile* file) {
	for (uint16 i = 0; i < 4; ++i) {
		Champion *champ = &_gK71_champions[i];
		champ->_attributes = file->readUint16BE();
		champ->_wounds = file->readUint16BE();
		for (uint16 y = 0; y < 7; ++y)
			for (uint16 x = 0; x < 3; ++x)
				champ->_statistics[y][x] = file->readByte();
		for (uint16 j = 0; j < 30; ++j)
			champ->_slots[j] = Thing(file->readUint16BE());
			for (uint16 j = 0; j < 20; ++j) {
				champ->_skills[j]._temporaryExperience = file->readSint16BE();
				champ->_skills[j]._experience = file->readSint32BE();
			}
		for (uint16 j = 0; j < 8; ++j)
			champ->_name[j] = file->readByte();
		for (uint16 j = 0; j < 20; ++j)
			champ->_title[j] = file->readByte();
		champ->_dir = (direction)file->readUint16BE();
		champ->_cell = (ViewCell)file->readUint16BE();
		champ->_actionIndex = (ChampionAction)file->readUint16BE();
		champ->_symbolStep = file->readUint16BE();
		for (uint16 j = 0; j < 5; ++j)
			champ->_symbols[j] = file->readByte();
		champ->_directionMaximumDamageReceived = file->readUint16BE();
		champ->_maximumDamageReceived = file->readUint16BE();
		champ->_poisonEventCount = file->readUint16BE();
		champ->_enableActionEventIndex = file->readSint16BE();
		champ->_hideDamageReceivedIndex = file->readSint16BE();
		champ->_currHealth = file->readSint16BE();
		champ->_maxHealth = file->readSint16BE();
		champ->_currStamina = file->readSint16BE();
		champ->_maxStamina = file->readSint16BE();
		champ->_currMana = file->readSint16BE();
		champ->_maxMana = file->readSint16BE();
		champ->_actionDefense = file->readSint16BE();
		champ->_food = file->readSint16BE();
		champ->_water = file->readSint16BE();
		champ->_load = file->readUint16BE();
		champ->_shieldDefense = file->readSint16BE();
		for (uint16 j = 0; j < 928; ++j)
			champ->_portrait[j] = file->readByte();
	}

	Party &party = _g407_party;
	party._magicalLightAmount = file->readSint16BE();
	party._event73Count_ThievesEye = file->readByte();
	party._event79Count_Footprints = file->readByte();
	party._shieldDefense = file->readSint16BE();
	party._fireShieldDefense = file->readSint16BE();
	party._spellShieldDefense = file->readSint16BE();
	party._scentCount = file->readByte();
	party._freezeLifeTicks = file->readByte();
	party._firstScentIndex = file->readByte();
	party._lastScentIndex = file->readByte();
	for (uint16 i = 0; i < 24; ++i)
		party._scents[i] = Scent(file->readUint16BE());
	for (uint16 i = 0; i < 24; ++i)
		party._scentStrengths[i] = file->readByte();
	party._event71Count_Invisibility = file->readByte();
}

ChampionIndex ChampionMan::f285_getIndexInCell(int16 cell) {
	for (uint16 i = 0; i < _g305_partyChampionCount; ++i) {
		if ((_gK71_champions[i]._cell == cell) && _gK71_champions[i]._currHealth)
			return (ChampionIndex)i;
	}

	return kM1_ChampionNone;
}

void ChampionMan::f278_resetDataToStartGame() {
	if (!_vm->_g298_newGame) {
		warning(false, "MISSING CODE: stuff for resetting for loaded games");
		assert(false);
	}

	_g414_leaderHandObject = Thing::_none;
	_g413_leaderHandObjectIconIndex = kM1_IconIndiceNone;
	_g415_leaderEmptyHanded = true;
}


void ChampionMan::f280_addCandidateChampionToParty(uint16 championPortraitIndex) {
	Thing L0793_T_Thing;
	uint16 L0794_ui_Multiple;
#define AL0794_ui_ViewCell       L0794_ui_Multiple
#define AL0794_ui_SlotIndex      L0794_ui_Multiple
#define AL0794_ui_CharacterIndex L0794_ui_Multiple
#define AL0794_ui_StatisticIndex L0794_ui_Multiple
#define AL0794_ui_SkillIndex     L0794_ui_Multiple
	int16 L0795_i_HiddenSkillIndex;
	uint16 L0796_ui_Multiple;
#define AL0796_ui_Character  L0796_ui_Multiple
#define AL0796_ui_SkillValue L0796_ui_Multiple
#define AL0796_ui_ThingType  L0796_ui_Multiple
	Champion* L0797_ps_Champion;
	char* L0798_pc_Character;
	uint16 L0799_ui_PreviousPartyChampionCount;
	uint16 L0800_ui_Multiple;
#define AL0800_B_ChampionTitleCopied L0800_ui_Multiple
#define AL0800_ui_HiddenSkillCounter L0800_ui_Multiple
	uint16 L0801_ui_SlotIndex;
	int16 L0802_i_MapX;
	int16 L0803_i_MapY;
	uint16 L0804_ui_ChampionObjectsCell;
	int16 L0805_i_ObjectAllowedSlots;
	int32 L0806_l_BaseSkillExperience;
	char L0807_ac_DecodedChampionText[77];

	if (!_vm->_championMan->_g415_leaderEmptyHanded) {
		return;
	}
	if (_vm->_championMan->_g305_partyChampionCount == 4) {
		return;
	}
	L0797_ps_Champion = &_vm->_championMan->_gK71_champions[L0799_ui_PreviousPartyChampionCount = _vm->_championMan->_g305_partyChampionCount];
	L0797_ps_Champion->resetToZero();
	// Strangerke - TODO: Check if the new code is possible to run on the older version (example: the portaits could be missing in the data)
	_vm->_displayMan->_g578_useByteBoxCoordinates = true;
	_vm->_displayMan->f132_blitToBitmap(_vm->_displayMan->f489_getNativeBitmapOrGraphic(k26_ChampionPortraitsIndice), L0797_ps_Champion->_portrait, gBoxChampionPortrait, _vm->_championMan->M27_getChampionPortraitX(championPortraitIndex), _vm->_championMan->M28_getChampionPortraitY(championPortraitIndex), k128_byteWidth, k16_byteWidth, kM1_ColorNoTransparency);
	L0797_ps_Champion->_actionIndex = k255_ChampionActionNone;
	L0797_ps_Champion->_enableActionEventIndex = -1;
	L0797_ps_Champion->_hideDamageReceivedIndex = -1;
	L0797_ps_Champion->_dir = _vm->_dungeonMan->_g308_partyDir;
	AL0794_ui_ViewCell = k0_ViewCellFronLeft;
	while (_vm->_championMan->f285_getIndexInCell(M21_normalizeModulo4(AL0794_ui_ViewCell + _vm->_dungeonMan->_g308_partyDir)) != kM1_ChampionNone) {
		AL0794_ui_ViewCell++;
	}
	L0797_ps_Champion->_cell = (ViewCell)M21_normalizeModulo4(AL0794_ui_ViewCell + _vm->_dungeonMan->_g308_partyDir);
	L0797_ps_Champion->_attributes = k0x0400_ChampionAttributeIcon;
	L0797_ps_Champion->_directionMaximumDamageReceived = _vm->_dungeonMan->_g308_partyDir;
	L0797_ps_Champion->_food = 1500 + _vm->getRandomNumber(256);
	L0797_ps_Champion->_water = 1500 + _vm->getRandomNumber(256);
	for (AL0794_ui_SlotIndex = k0_ChampionSlotReadyHand; AL0794_ui_SlotIndex < k30_ChampionSlotChest_1; AL0794_ui_SlotIndex++) {
		L0797_ps_Champion->_slots[AL0794_ui_SlotIndex] = Thing::_none;
	}
	L0793_T_Thing = _vm->_dungeonMan->f161_getSquareFirstThing(_vm->_dungeonMan->_g306_partyMapX, _vm->_dungeonMan->_g307_partyMapY);
	while ((L0793_T_Thing.getType()) != k2_TextstringType) {
		L0793_T_Thing = _vm->_dungeonMan->f159_getNextThing(L0793_T_Thing);
	}
	_vm->_dungeonMan->f168_decodeText(L0798_pc_Character = L0807_ac_DecodedChampionText, L0793_T_Thing, (TextType)(k2_TextTypeScroll | k0x8000_DecodeEvenIfInvisible));
	AL0794_ui_CharacterIndex = 0;
	while ((AL0796_ui_Character = *L0798_pc_Character++) != '\n') { /* New line */
		L0797_ps_Champion->_name[AL0794_ui_CharacterIndex++] = AL0796_ui_Character;
	}
	L0797_ps_Champion->_name[AL0794_ui_CharacterIndex] = '\0';
	AL0794_ui_CharacterIndex = 0;
	AL0800_B_ChampionTitleCopied = false;
	for (;;) { /*_Infinite loop_*/
		AL0796_ui_Character = *L0798_pc_Character++;
		if (AL0796_ui_Character == '\n') { /* New line */
			if (AL0800_B_ChampionTitleCopied)
				break;
			AL0800_B_ChampionTitleCopied = true;
		} else {
			L0797_ps_Champion->_title[AL0794_ui_CharacterIndex++] = AL0796_ui_Character;
		}
	}
	L0797_ps_Champion->_title[AL0794_ui_CharacterIndex] = '\0';
	if (*L0798_pc_Character++ == 'M') {
		setFlag(L0797_ps_Champion->_attributes, k0x0010_ChampionAttributeMale);
	}
	L0798_pc_Character++;
	L0797_ps_Champion->_currHealth = L0797_ps_Champion->_maxHealth = _vm->_championMan->f279_getDecodedValue(L0798_pc_Character, 4);
	L0798_pc_Character += 4;
	L0797_ps_Champion->_currStamina = L0797_ps_Champion->_maxStamina = _vm->_championMan->f279_getDecodedValue(L0798_pc_Character, 4);
	L0798_pc_Character += 4;
	L0797_ps_Champion->_currMana = L0797_ps_Champion->_maxMana = _vm->_championMan->f279_getDecodedValue(L0798_pc_Character, 4);
	L0798_pc_Character += 4;
	L0798_pc_Character++;
	for (AL0794_ui_StatisticIndex = k0_ChampionStatLuck; AL0794_ui_StatisticIndex <= k6_ChampionStatAntifire; AL0794_ui_StatisticIndex++) {
		L0797_ps_Champion->_statistics[AL0794_ui_StatisticIndex][k2_ChampionStatMinimum] = 30;
		L0797_ps_Champion->_statistics[AL0794_ui_StatisticIndex][k1_ChampionStatCurrent] = L0797_ps_Champion->_statistics[AL0794_ui_StatisticIndex][k0_ChampionStatMaximum] = _vm->_championMan->f279_getDecodedValue(L0798_pc_Character, 2);
		L0798_pc_Character += 2;
	}
	L0797_ps_Champion->_statistics[k0_ChampionStatLuck][k2_ChampionStatMinimum] = 10;
	L0798_pc_Character++;
	for (AL0794_ui_SkillIndex = k4_ChampionSkillSwing; AL0794_ui_SkillIndex <= k19_ChampionSkillWater; AL0794_ui_SkillIndex++) {
		if ((AL0796_ui_SkillValue = *L0798_pc_Character++ - 'A') > 0) {
			L0797_ps_Champion->_skills[AL0794_ui_SkillIndex]._experience = 125L << AL0796_ui_SkillValue;
		}
	}
	for (AL0794_ui_SkillIndex = k0_ChampionSkillFighter; AL0794_ui_SkillIndex <= k3_ChampionSkillWizard; AL0794_ui_SkillIndex++) {
		L0806_l_BaseSkillExperience = 0;
		L0795_i_HiddenSkillIndex = (AL0794_ui_SkillIndex + 1) << 2;
		for (AL0800_ui_HiddenSkillCounter = 0; AL0800_ui_HiddenSkillCounter < 4; AL0800_ui_HiddenSkillCounter++) {
			L0806_l_BaseSkillExperience += L0797_ps_Champion->_skills[L0795_i_HiddenSkillIndex + AL0800_ui_HiddenSkillCounter]._experience;
		}
		L0797_ps_Champion->_skills[AL0794_ui_SkillIndex]._experience = L0806_l_BaseSkillExperience;
	}
	_vm->_championMan->_g299_candidateChampionOrdinal = L0799_ui_PreviousPartyChampionCount + 1;
	if (++_vm->_championMan->_g305_partyChampionCount == 1) {
		_vm->_eventMan->f368_commandSetLeader(k0_ChampionFirst);
		_vm->_menuMan->_g508_refreshActionArea = true;
	} else {
		_vm->_menuMan->f388_clearActingChampion();
		_vm->_menuMan->f386_drawActionIcon((ChampionIndex)(_vm->_championMan->_g305_partyChampionCount - 1));
	}
	L0802_i_MapX = _vm->_dungeonMan->_g306_partyMapX;
	L0803_i_MapY = _vm->_dungeonMan->_g307_partyMapY;
	L0804_ui_ChampionObjectsCell = returnOppositeDir(_vm->_dungeonMan->_g308_partyDir);
	L0802_i_MapX += _vm->_dirIntoStepCountEast[_vm->_dungeonMan->_g308_partyDir], L0803_i_MapY += _vm->_dirIntoStepCountNorth[_vm->_dungeonMan->_g308_partyDir];
	L0793_T_Thing = _vm->_dungeonMan->f161_getSquareFirstThing(L0802_i_MapX, L0803_i_MapY);
	AL0794_ui_SlotIndex = k13_ChampionSlotBackpackLine_1_1;
	while (L0793_T_Thing != Thing::_endOfList) {
		if (((AL0796_ui_ThingType = (L0793_T_Thing.getType())) > k3_SensorThingType) && ((L0793_T_Thing.getCell()) == L0804_ui_ChampionObjectsCell)) {
			L0805_i_ObjectAllowedSlots = g237_ObjectInfo[_vm->_dungeonMan->f141_getObjectInfoIndex(L0793_T_Thing)]._allowedSlots;
			switch (AL0796_ui_ThingType) {
			case k6_ArmourThingType:
				for (L0801_ui_SlotIndex = k2_ChampionSlotHead; L0801_ui_SlotIndex <= k5_ChampionSlotFeet; L0801_ui_SlotIndex++) {
					if (L0805_i_ObjectAllowedSlots & gSlotMasks[L0801_ui_SlotIndex])
						goto T0280048;
				}
				if ((L0805_i_ObjectAllowedSlots & gSlotMasks[k10_ChampionSlotNeck]) && (L0797_ps_Champion->_slots[k10_ChampionSlotNeck] == Thing::_none)) {
					L0801_ui_SlotIndex = k10_ChampionSlotNeck;
				} else {
					goto T0280046;
				}
				break;
			case k5_WeaponThingType:
				if (L0797_ps_Champion->_slots[k1_ChampionSlotActionHand] == Thing::_none) {
					L0801_ui_SlotIndex = k1_ChampionSlotActionHand;
				} else {
					goto T0280046;
				}
				break;
			case k7_ScrollThingType:
			case k8_PotionThingType:
				if (L0797_ps_Champion->_slots[k11_ChampionSlotPouch_1] == Thing::_none) {
					L0801_ui_SlotIndex = k11_ChampionSlotPouch_1;
				} else {
					if (L0797_ps_Champion->_slots[k6_ChampionSlotPouch_2] == Thing::_none) {
						L0801_ui_SlotIndex = k6_ChampionSlotPouch_2;
					} else {
						goto T0280046;
					}
				}
				break;
			case k9_ContainerThingType:
			case k10_JunkThingType:
T0280046:
				if ((L0805_i_ObjectAllowedSlots & gSlotMasks[k10_ChampionSlotNeck]) && (L0797_ps_Champion->_slots[k10_ChampionSlotNeck] == Thing::_none)) {
					L0801_ui_SlotIndex = k10_ChampionSlotNeck;
				} else {
					L0801_ui_SlotIndex = AL0794_ui_SlotIndex++;
				}
			}
T0280048:
			if (L0797_ps_Champion->_slots[L0801_ui_SlotIndex] != Thing::_none) {
				goto T0280046;
			}
			_vm->_championMan->f301_addObjectInSlot((ChampionIndex)L0799_ui_PreviousPartyChampionCount, L0793_T_Thing, (ChampionSlot)L0801_ui_SlotIndex);
		}
		L0793_T_Thing = _vm->_dungeonMan->f159_getNextThing(L0793_T_Thing);
	}
	_vm->_inventoryMan->f355_toggleInventory((ChampionIndex)L0799_ui_PreviousPartyChampionCount);
	_vm->_menuMan->f456_drawDisabledMenu();;
}

void ChampionMan::f287_drawChampionBarGraphs(ChampionIndex champIndex) {
	int16 barGraphHeights[3];
	Champion *champ = &_vm->_championMan->_gK71_champions[champIndex];
	int16 AL0842_i_BarGraphIndex = 0;
	if (champ->_currHealth > 0) {
		int32 barGraphHeight = (((int32)champ->_currHealth << 10) * 25) / champ->_maxHealth;
		if (barGraphHeight & 0x000003FF) {
			barGraphHeights[AL0842_i_BarGraphIndex++] = (barGraphHeight >> 10) + 1;
		} else {
			barGraphHeights[AL0842_i_BarGraphIndex++] = (barGraphHeight >> 10);
		}
	} else {
		barGraphHeights[AL0842_i_BarGraphIndex++] = 0;
	}
	if (champ->_currStamina > 0) {
		int32 barGraphHeight = (((int32)champ->_currStamina << 10) * 25) / champ->_maxStamina;
		if (barGraphHeight & 0x000003FF) {
			barGraphHeights[AL0842_i_BarGraphIndex++] = (barGraphHeight >> 10) + 1;
		} else {
			barGraphHeights[AL0842_i_BarGraphIndex++] = (barGraphHeight >> 10);
		}
	} else {
		barGraphHeights[AL0842_i_BarGraphIndex++] = 0;
	}
	if (champ->_currMana > 0) {
		if (champ->_currMana > champ->_maxMana) {
			barGraphHeights[AL0842_i_BarGraphIndex] = 25;
		} else {
			int32 barGraphHeight = (((int32)champ->_currMana << 10) * 25) / champ->_maxMana;
			if (barGraphHeight & 0x000003FF) {
				barGraphHeights[AL0842_i_BarGraphIndex] = (barGraphHeight >> 10) + 1;
			} else {
				barGraphHeights[AL0842_i_BarGraphIndex] = (barGraphHeight >> 10);
			}
		}
	} else {
		barGraphHeights[AL0842_i_BarGraphIndex] = 0;
	}
	_vm->_eventMan->f78_showMouse();

	// Strangerke - TO CHECK: if portraits, maybe the old (assembly) code is required for older versions
	Box box;
	box._x1 = champIndex * k69_ChampionStatusBoxSpacing + 46;
	box._x2 = box._x1 + 3;
	box._y1 = 2;
	box._y2 = 26;
	for (int16 barGraphIndex = 0; barGraphIndex < 3; barGraphIndex++) {
		int16 barGraphHeight = barGraphHeights[barGraphIndex];
		if (barGraphHeight < 25) {
			box._y1 = 2;
			box._y2 = 27 - barGraphHeight;
			_vm->_displayMan->D24_fillScreenBox(box, k12_ColorDarkestGray);
		}
		if (barGraphHeight) {
			box._y1 = 27 - barGraphHeight;
			box._y2 = 26;
			_vm->_displayMan->D24_fillScreenBox(box, g46_ChampionColor[champIndex]);
		}
		box._x1 += 7;
		box._x2 += 7;
	}
	_vm->_eventMan->f77_hideMouse();
}


uint16 ChampionMan::f306_getStaminaAdjustedValue(Champion *champ, int16 val) {
	int16 currStamina = champ->_currStamina;
	int16 halfMaxStamina = champ->_maxStamina / 2;
	if (currStamina < halfMaxStamina) {
		warning(false, "Possible undefined behavior in the original code");
		val /= 2;
		return val + ((uint32)val * (uint32)currStamina) / halfMaxStamina;
	}
	return val;
}

uint16 ChampionMan::f309_getMaximumLoad(Champion *champ) {
	uint16 maximumLoad = champ->getStatistic(k1_ChampionStatStrength, k1_ChampionStatCurrent) * 8 + 100;
	maximumLoad = f306_getStaminaAdjustedValue(champ, maximumLoad);
	int16 wounds = champ->getWounds();
	if (wounds) {
		maximumLoad -= maximumLoad >> (champ->getWoundsFlag(k0x0010_ChampionWoundLegs) ? 2 : 3);
	}
	if (_vm->_objectMan->f33_getIconIndex(champ->getSlot(k5_ChampionSlotFeet)) == k119_IconIndiceArmourElvenBoots) {
		maximumLoad += maximumLoad * 16;
	}
	maximumLoad += 9;
	maximumLoad -= maximumLoad % 10;
	return maximumLoad;
}

void ChampionMan::f292_drawChampionState(ChampionIndex champIndex) {
	uint16 L0862_ui_ChampionAttributes;
	bool L0863_B_IsInventoryChampion;
	int16 L0864_i_Multiple;
#define AL0864_i_BorderCount       L0864_i_Multiple
#define AL0864_i_ColorIndex        L0864_i_Multiple
#define AL0864_i_Load              L0864_i_Multiple
#define AL0864_i_ChampionIconIndex L0864_i_Multiple
#define AL0864_i_StatisticIndex    L0864_i_Multiple
#define AL0864_i_SlotIndex         L0864_i_Multiple
	Champion* L0865_ps_Champion;
	char* L0866_pc_ChampionName;
	char L0867_c_ChampionTitleFirstCharacter;
	int16 L0868_i_ChampionStatusBoxX;
	int16 L0869_i_ChampionTitleX;
	int16 L0870_i_Multiple;
#define AL0870_i_NativeBitmapIndex L0870_i_Multiple
#define AL0870_i_Color             L0870_i_Multiple
	Box L0871_s_Box;
	int16 L0872_ai_NativeBitmapIndices[3];


	L0868_i_ChampionStatusBoxX = champIndex * k69_ChampionStatusBoxSpacing;
	L0865_ps_Champion = &_gK71_champions[champIndex];
	L0862_ui_ChampionAttributes = L0865_ps_Champion->_attributes;
	if (!getFlag(L0862_ui_ChampionAttributes, k0x0080_ChampionAttributeNameTitle | k0x0100_ChampionAttributeStatistics | k0x0200_ChampionAttributeLoad | k0x0400_ChampionAttributeIcon | k0x0800_ChampionAttributePanel | k0x1000_ChampionAttributeStatusBox | k0x2000_ChampionAttributeWounds | k0x4000_ChampionAttributeViewport | k0x8000_ChampionAttributeActionHand)) {
		return;
	}
	L0863_B_IsInventoryChampion = (_vm->M0_indexToOrdinal(champIndex) == _vm->_inventoryMan->_g432_inventoryChampionOrdinal);
	_vm->_displayMan->_g578_useByteBoxCoordinates = false;
	_vm->_eventMan->f78_showMouse();
	if (getFlag(L0862_ui_ChampionAttributes, k0x1000_ChampionAttributeStatusBox)) {
		L0871_s_Box._y1 = 0;
		L0871_s_Box._y2 = 28;
		L0871_s_Box._x2 = (L0871_s_Box._x1 = L0868_i_ChampionStatusBoxX) + 66;
		if (L0865_ps_Champion->_currHealth) {
			_vm->_displayMan->D24_fillScreenBox(L0871_s_Box, k12_ColorDarkestGray);
			for (uint16 i = 0; i < 3; ++i)
				L0872_ai_NativeBitmapIndices[i] = 0;
			AL0864_i_BorderCount = 0;
			if (_g407_party._fireShieldDefense > 0) {
				L0872_ai_NativeBitmapIndices[AL0864_i_BorderCount++] = k38_BorderPartyFireshieldIndice;
			}
			if (_g407_party._spellShieldDefense > 0) {
				L0872_ai_NativeBitmapIndices[AL0864_i_BorderCount++] = k39_BorderPartySpellshieldIndice;
			}
			if ((_g407_party._shieldDefense > 0) || L0865_ps_Champion->_shieldDefense) {
				L0872_ai_NativeBitmapIndices[AL0864_i_BorderCount++] = k37_BorderPartyShieldIndice;
			}
			while (AL0864_i_BorderCount--) {
				_vm->_displayMan->f21_blitToScreen(_vm->_displayMan->f489_getNativeBitmapOrGraphic(L0872_ai_NativeBitmapIndices[AL0864_i_BorderCount]), &L0871_s_Box, k40_byteWidth, k10_ColorFlesh, 29);
			}
			if (L0863_B_IsInventoryChampion) {
				_vm->_inventoryMan->f354_drawStatusBoxPortrait(champIndex);
				setFlag(L0862_ui_ChampionAttributes, k0x0100_ChampionAttributeStatistics);
			} else {
				setFlag(L0862_ui_ChampionAttributes, k0x0080_ChampionAttributeNameTitle | k0x0100_ChampionAttributeStatistics | k0x2000_ChampionAttributeWounds | k0x8000_ChampionAttributeActionHand);
			}
		} else {
			_vm->_displayMan->f21_blitToScreen(_vm->_displayMan->f489_getNativeBitmapOrGraphic(k8_StatusBoxDeadChampion), &L0871_s_Box, k40_byteWidth, kM1_ColorNoTransparency, 29);
			_vm->_textMan->f53_printToLogicalScreen(L0868_i_ChampionStatusBoxX + 1, 5, k13_ColorLightestGray, k1_ColorDarkGary, L0865_ps_Champion->_name);
			_vm->_menuMan->f386_drawActionIcon(champIndex);
			goto T0292042;
		}
	}
	if (!(L0865_ps_Champion->_currHealth))
		goto T0292042;
	if (getFlag(L0862_ui_ChampionAttributes, k0x0080_ChampionAttributeNameTitle)) {
		AL0864_i_ColorIndex = (champIndex == _g411_leaderIndex) ? k9_ColorGold : k13_ColorLightestGray;
		if (L0863_B_IsInventoryChampion) {
			_vm->_textMan->f52_printToViewport(3, 7, (Color)AL0864_i_ColorIndex, L0866_pc_ChampionName = L0865_ps_Champion->_name);
			L0869_i_ChampionTitleX = 6 * strlen(L0866_pc_ChampionName) + 3;
			L0867_c_ChampionTitleFirstCharacter = L0865_ps_Champion->_title[0];
			if ((L0867_c_ChampionTitleFirstCharacter != ',') && (L0867_c_ChampionTitleFirstCharacter != ';') && (L0867_c_ChampionTitleFirstCharacter != '-')) {
				L0869_i_ChampionTitleX += 6;
			}
			_vm->_textMan->f52_printToViewport(L0869_i_ChampionTitleX, 7, (Color)AL0864_i_ColorIndex, L0865_ps_Champion->_title);
			setFlag(L0862_ui_ChampionAttributes, k0x4000_ChampionAttributeViewport);
		} else {
			L0871_s_Box._y1 = 0;
			L0871_s_Box._y2 = 6;
			L0871_s_Box._x2 = (L0871_s_Box._x1 = L0868_i_ChampionStatusBoxX) + 42;
			_vm->_displayMan->D24_fillScreenBox(L0871_s_Box, k1_ColorDarkGary);
			_vm->_textMan->f53_printToLogicalScreen(L0868_i_ChampionStatusBoxX + 1, 5, (Color)AL0864_i_ColorIndex, k1_ColorDarkGary, L0865_ps_Champion->_name);
		}
	}
	if (getFlag(L0862_ui_ChampionAttributes, k0x0100_ChampionAttributeStatistics)) {
		f287_drawChampionBarGraphs(champIndex);
		if (L0863_B_IsInventoryChampion) {
			f290_drawHealthStaminaManaValues(L0865_ps_Champion);
			if ((L0865_ps_Champion->_food < 0) || (L0865_ps_Champion->_water < 0) || (L0865_ps_Champion->_poisonEventCount)) {
				AL0870_i_NativeBitmapIndex = k34_SlotBoxWoundedIndice;
			} else {
				AL0870_i_NativeBitmapIndex = k33_SlotBoxNormalIndice;
			}
			_vm->_displayMan->f20_blitToViewport(_vm->_displayMan->f489_getNativeBitmapOrGraphic(AL0870_i_NativeBitmapIndex), gBoxMouth, k16_byteWidth, k12_ColorDarkestGray, 18);
			AL0870_i_NativeBitmapIndex = k33_SlotBoxNormalIndice;
			for (AL0864_i_StatisticIndex = k1_ChampionStatStrength; AL0864_i_StatisticIndex <= k6_ChampionStatAntifire; AL0864_i_StatisticIndex++) {
				if ((L0865_ps_Champion->_statistics[AL0864_i_StatisticIndex][k1_ChampionStatCurrent] < L0865_ps_Champion->_statistics[AL0864_i_StatisticIndex][k0_ChampionStatMaximum])) {
					AL0870_i_NativeBitmapIndex = k34_SlotBoxWoundedIndice;
					break;
				}
			}
			_vm->_displayMan->f20_blitToViewport(_vm->_displayMan->f489_getNativeBitmapOrGraphic(AL0870_i_NativeBitmapIndex), gBoxEye, k16_byteWidth, k12_ColorDarkestGray, 18);
			setFlag(L0862_ui_ChampionAttributes, k0x4000_ChampionAttributeViewport);
		}
	}
	if (getFlag(L0862_ui_ChampionAttributes, k0x2000_ChampionAttributeWounds)) {
		for (AL0864_i_SlotIndex = L0863_B_IsInventoryChampion ? k5_ChampionSlotFeet : k1_ChampionSlotActionHand; AL0864_i_SlotIndex >= k0_ChampionSlotReadyHand; AL0864_i_SlotIndex--) {
			f291_drawSlot(champIndex, AL0864_i_SlotIndex);
		}
		if (L0863_B_IsInventoryChampion) {
			setFlag(L0862_ui_ChampionAttributes, k0x4000_ChampionAttributeViewport);
		}
	}
	if (getFlag(L0862_ui_ChampionAttributes, k0x0200_ChampionAttributeLoad) && L0863_B_IsInventoryChampion) {
		if (L0865_ps_Champion->_load > (AL0864_i_Load = f309_getMaximumLoad(L0865_ps_Champion))) {
			AL0870_i_Color = k8_ColorRed;
		} else {
			if (((long)L0865_ps_Champion->_load << 3) > ((long)AL0864_i_Load * 5)) {
				AL0870_i_Color = k11_ColorYellow;
			} else {
				AL0870_i_Color = k13_ColorLightestGray;
			}
		}
		_vm->_textMan->f52_printToViewport(104, 132, (Color)AL0870_i_Color, "LOAD ");
		AL0864_i_Load = L0865_ps_Champion->_load / 10;
		strcpy(_vm->_g353_stringBuildBuffer, f288_getStringFromInteger(AL0864_i_Load, true, 3).c_str());
		strcat(_vm->_g353_stringBuildBuffer, ".");
		AL0864_i_Load = L0865_ps_Champion->_load - (AL0864_i_Load * 10);
		strcat(_vm->_g353_stringBuildBuffer, f288_getStringFromInteger(AL0864_i_Load, false, 1).c_str());
		strcat(_vm->_g353_stringBuildBuffer, "/");
		AL0864_i_Load = (f309_getMaximumLoad(L0865_ps_Champion) + 5) / 10;
		strcat(_vm->_g353_stringBuildBuffer, f288_getStringFromInteger(AL0864_i_Load, true, 3).c_str());
		strcat(_vm->_g353_stringBuildBuffer, " KG");
		_vm->_textMan->f52_printToViewport(148, 132, (Color)AL0870_i_Color, _vm->_g353_stringBuildBuffer);
		setFlag(L0862_ui_ChampionAttributes, k0x4000_ChampionAttributeViewport);
	}
	AL0864_i_ChampionIconIndex = M26_championIconIndex(L0865_ps_Champion->_cell, _vm->_dungeonMan->_g308_partyDir);
	if (getFlag(L0862_ui_ChampionAttributes, k0x0400_ChampionAttributeIcon) && (_vm->_eventMan->_g599_useChampionIconOrdinalAsMousePointerBitmap != _vm->M0_indexToOrdinal(AL0864_i_ChampionIconIndex))) {
		_vm->_displayMan->D24_fillScreenBox(g54_BoxChampionIcons[AL0864_i_ChampionIconIndex], g46_ChampionColor[champIndex]);
		_vm->_displayMan->f132_blitToBitmap(_vm->_displayMan->f489_getNativeBitmapOrGraphic(k28_ChampionIcons), _vm->_displayMan->_g348_bitmapScreen, g54_BoxChampionIcons[AL0864_i_ChampionIconIndex], _vm->_championMan->M26_championIconIndex(L0865_ps_Champion->_dir, _vm->_dungeonMan->_g308_partyDir) * 19, 0, k40_byteWidth, k160_byteWidthScreen, k12_ColorDarkestGray, 14, k200_heightScreen);
	}
	if (getFlag(L0862_ui_ChampionAttributes, k0x0800_ChampionAttributePanel) && L0863_B_IsInventoryChampion) {
		if (_vm->_g333_pressingMouth) {
			_vm->_inventoryMan->f345_drawPanelFoodWaterPoisoned();
		} else {
			if (_vm->_g331_pressingEye) {
				if (_g415_leaderEmptyHanded) {
					_vm->_inventoryMan->f351_drawChampionSkillsAndStatistics();
				}
			} else {
				_vm->_inventoryMan->f347_drawPanel();
			}
		}
		setFlag(L0862_ui_ChampionAttributes, k0x4000_ChampionAttributeViewport);
	}
	if (getFlag(L0862_ui_ChampionAttributes, k0x8000_ChampionAttributeActionHand)) {
		f291_drawSlot(champIndex, k1_ChampionSlotActionHand);
		_vm->_menuMan->f386_drawActionIcon(champIndex);
		if (L0863_B_IsInventoryChampion) {
			setFlag(L0862_ui_ChampionAttributes, k0x4000_ChampionAttributeViewport);
		}
	}
	if (getFlag(L0862_ui_ChampionAttributes, k0x4000_ChampionAttributeViewport)) {
		_vm->_displayMan->f97_drawViewport(k0_viewportNotDungeonView);
	}
T0292042:
	clearFlag(L0865_ps_Champion->_attributes, k0x0080_ChampionAttributeNameTitle | k0x0100_ChampionAttributeStatistics | k0x0200_ChampionAttributeLoad | k0x0400_ChampionAttributeIcon | k0x0800_ChampionAttributePanel | k0x1000_ChampionAttributeStatusBox | k0x2000_ChampionAttributeWounds | k0x4000_ChampionAttributeViewport | k0x8000_ChampionAttributeActionHand);
	_vm->_eventMan->f77_hideMouse();
}

uint16 ChampionMan::M26_championIconIndex(int16 val, direction dir) {
	return ((val + 4 - dir) & 0x3);
}

void ChampionMan::f290_drawHealthStaminaManaValues(Champion* champ) {
	f289_drawHealthOrStaminaOrManaValue(116, champ->_currHealth, champ->_maxHealth);
	f289_drawHealthOrStaminaOrManaValue(124, champ->_currStamina, champ->_maxStamina);
	f289_drawHealthOrStaminaOrManaValue(132, champ->_currMana, champ->_maxMana);
}

void ChampionMan::f291_drawSlot(uint16 champIndex, int16 slotIndex) {
	int16 nativeBitmapIndex = -1;
	Champion *champ = &_gK71_champions[champIndex];
	bool isInventoryChamp = (_vm->_inventoryMan->_g432_inventoryChampionOrdinal == _vm->M0_indexToOrdinal(champIndex));

	uint16 slotBoxIndex;
	if (!isInventoryChamp) {  /* If drawing a slot for a champion other than the champion whose inventory is open */
		if ((slotIndex > k1_ChampionSlotActionHand) || (_g299_candidateChampionOrdinal == _vm->M0_indexToOrdinal(champIndex))) {
			return;
		}
		slotBoxIndex = (champIndex << 1) + slotIndex;
	} else {
		slotBoxIndex = k8_SlotBoxInventoryFirstSlot + slotIndex;
	}

	Thing thing;
	if (slotIndex >= k30_ChampionSlotChest_1) {
		thing = _vm->_inventoryMan->_g425_chestSlots[slotIndex - k30_ChampionSlotChest_1];
	} else {
		thing = champ->getSlot((ChampionSlot)slotIndex);
	}

	SlotBox *slotBox = &_vm->_objectMan->_g30_slotBoxes[slotBoxIndex];
	Box box;
	box._x1 = slotBox->_x - 1;
	box._y1 = slotBox->_y - 1;
	box._x2 = box._x1 + 17;
	box._y2 = box._y1 + 17;


	if (!isInventoryChamp) {
		_vm->_eventMan->f77_hideMouse();
	}

	int16 iconIndex;
	if (thing == Thing::_none) {
		if (slotIndex <= k5_ChampionSlotFeet) {
			iconIndex = k212_IconIndiceReadyHand + (slotIndex << 1);
			if (champ->getWoundsFlag((ChampionWound)(1 << slotIndex))) {
				iconIndex++;
				nativeBitmapIndex = k34_SlotBoxWoundedIndice;
			} else {
				nativeBitmapIndex = k33_SlotBoxNormalIndice;
			}
		} else {
			if ((slotIndex >= k10_ChampionSlotNeck) && (slotIndex <= k13_ChampionSlotBackpackLine_1_1)) {
				iconIndex = k208_IconIndiceNeck + (slotIndex - k10_ChampionSlotNeck);
			} else {
				iconIndex = k204_IconIndiceEmptyBox;
			}
		}
	} else {
		iconIndex = _vm->_objectMan->f33_getIconIndex(thing); // BUG0_35
		if (isInventoryChamp && (slotIndex == k1_ChampionSlotActionHand) && ((iconIndex == k144_IconIndiceContainerChestClosed) || (iconIndex == k30_IconIndiceScrollOpen))) {
			iconIndex++;
		} // BUG2_00
		if (slotIndex <= k5_ChampionSlotFeet) {
			if (champ->getWoundsFlag((ChampionWound)(1 << slotIndex))) {
				nativeBitmapIndex = k34_SlotBoxWoundedIndice;
			} else {
				nativeBitmapIndex = k33_SlotBoxNormalIndice;
			}
		}
	}

	if ((slotIndex == k1_ChampionSlotActionHand) && (_vm->M0_indexToOrdinal(champIndex) == _g506_actingChampionOrdinal)) {
		nativeBitmapIndex = k35_SlotBoxActingHandIndice;
	}

	if (nativeBitmapIndex != -1) {
		_vm->_displayMan->_g578_useByteBoxCoordinates = false;
		if (isInventoryChamp) {
			_vm->_displayMan->f132_blitToBitmap(_vm->_displayMan->f489_getNativeBitmapOrGraphic(nativeBitmapIndex),
												_vm->_displayMan->_g296_bitmapViewport, box, 0, 0, 16, k112_byteWidthViewport, k12_ColorDarkestGray);
		} else {
			_vm->_displayMan->f132_blitToBitmap(_vm->_displayMan->f489_getNativeBitmapOrGraphic(nativeBitmapIndex),
												_vm->_displayMan->_g348_bitmapScreen, box, 0, 0, 16, k160_byteWidthScreen, k12_ColorDarkestGray);
		}
	}

	_vm->_objectMan->f38_drawIconInSlotBox(slotBoxIndex, iconIndex);

	if (!isInventoryChamp) {
		_vm->_eventMan->f78_showMouse();
	}
}

void ChampionMan::f281_renameChampion(Champion* champ) {
	warning(false, "STUB METHOD: Champion::renameChampion, F0281_CHAMPION_Rename");

	DisplayMan &dispMan = *_vm->_displayMan;
	TextMan &textMan = *_vm->_textMan;

	Box box;
	box._y1 = 3;
	box._y2 = 8;
	box._x1 = 3;
	box._x2 = box._x1 + 167;

	dispMan.f135_fillBoxBitmap(dispMan._g296_bitmapViewport, box, k12_ColorDarkestGray, k112_byteWidthViewport, k136_heightViewport);
	dispMan.f132_blitToBitmap(dispMan.f489_getNativeBitmapOrGraphic(k27_PanelRenameChampionIndice), dispMan._g296_bitmapViewport, g32_BoxPanel,
							  0, 0, 72, k112_byteWidthViewport, k4_ColorCyan);
	textMan.f52_printToViewport(177, 58, k13_ColorLightestGray, "_______");
	textMan.f52_printToViewport(105, 76, k13_ColorLightestGray, "___________________");
	Common::Point clickPos;
	static Box okButtonBox(197, 215, 147, 155);
	for (;;) {
		_vm->_eventMan->processInput();
		if (_vm->_eventMan->f360_hasPendingClick(clickPos, k1_LeftMouseButton) && okButtonBox.isPointInside(clickPos)) {
			return;
		}
		dispMan.f97_drawViewport(k0_viewportNotDungeonView);
		dispMan.updateScreen();
	}
}

uint16 ChampionMan::f303_getSkillLevel(int16 champIndex, uint16 skillIndex) {
	if (_vm->_championMan->_g300_partyIsSleeping) {
		return 1;
	}
	bool ignoreTmpExp = getFlag(skillIndex, k0x8000_IgnoreTemporaryExperience);
	bool ignoreObjModifiers = getFlag(skillIndex, k0x4000_IgnoreObjectModifiers);
	clearFlag(skillIndex, k0x8000_IgnoreTemporaryExperience | k0x4000_IgnoreObjectModifiers);
	Champion *champ = &_vm->_championMan->_gK71_champions[champIndex];
	Skill *skill = &champ->_skills[skillIndex];
	int32 exp = skill->_experience;
	if (!ignoreTmpExp) {
		exp += skill->_temporaryExperience;
	}
	if (skillIndex > k3_ChampionSkillWizard) { /* Hidden skill */
		skill = &champ->_skills[(skillIndex - k4_ChampionSkillSwing) >> 2];
		exp += skill->_experience; /* Add experience in the base skill */
		if (!ignoreTmpExp) {
			exp += skill->_temporaryExperience;
		}
		exp >>= 1; /* Halve experience to get average of base skill + hidden skill experience */
	}
	int16 skillLevel = 1;
	while (exp >= 500) {
		exp >>= 1;
		skillLevel++;
	}
	if (!ignoreObjModifiers) {
		int16 actionHandIconIndex;
		if ((actionHandIconIndex = _vm->_objectMan->f33_getIconIndex(champ->_slots[k1_ChampionSlotActionHand])) == k27_IconIndiceWeaponTheFirestaff) {
			skillLevel++;
		} else {
			if (actionHandIconIndex == k28_IconIndiceWeaponTheFirestaffComplete) {
				skillLevel += 2;
			}
		}
		int16 neckIconIndex = _vm->_objectMan->f33_getIconIndex(champ->_slots[k10_ChampionSlotNeck]);
		switch (skillIndex) {
		case k3_ChampionSkillWizard:
			if (neckIconIndex == k124_IconIndiceJunkPendantFeral) {
				skillLevel += 1;
			}
			break;
		case k15_ChampionSkillDefend:
			if (neckIconIndex == k121_IconIndiceJunkEkkhardCross) {
				skillLevel += 1;
			}
			break;
		case k13_ChampionSkillHeal:
			if ((neckIconIndex == k120_IconIndiceJunkGemOfAges) || (actionHandIconIndex == k66_IconIndiceWeaponSceptreOfLyf)) { /* The skill modifiers of these two objects are not cumulative */
				skillLevel += 1;
			}
			break;
		case k14_ChampionSkillInfluence:
			if (neckIconIndex == k122_IconIndiceJunkMoonstone) {
				skillLevel += 1;
			}
		}
	}
	return skillLevel;
}

}


