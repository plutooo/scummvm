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

#include "sherlock/inventory.h"
#include "sherlock/sherlock.h"

namespace Sherlock {

InventoryItem::InventoryItem(int requiredFlag, const Common::String &name,
		const Common::String &description, const Common::String &examine) : 
		_requiredFlag(requiredFlag), _name(name), _description(description), 
		_examine(examine), _lookFlag(0) {
}

/*----------------------------------------------------------------*/

Inventory::Inventory(SherlockEngine *vm) : Common::Array<InventoryItem>(), _vm(vm) {
	Common::fill(&_invShapes[0], &_invShapes[MAX_VISIBLE_INVENTORY], (ImageFile *)nullptr);
	_invGraphicsLoaded = false;
	_invIndex = 0;
	_holdings = 0;
	_oldFlag = 0;
	_invFlag = 0;
	_invMode = INVMODE_EXIT;
}

Inventory::~Inventory() {
	freeGraphics();
}

void Inventory::freeInv() {
	freeGraphics();

	_names.clear();
	_invGraphicsLoaded = false;
}

/**
 * Free any loaded inventory graphics
 */
void Inventory::freeGraphics() {
	for (uint idx = 0; idx < MAX_VISIBLE_INVENTORY; ++idx)
		delete _invShapes[idx];
	
	Common::fill(&_invShapes[0], &_invShapes[MAX_VISIBLE_INVENTORY], (ImageFile *)nullptr);
	_invGraphicsLoaded = false;
}

/** Load the list of names the inventory items correspond to.
 */
void Inventory::loadInv() {
	// Exit if the inventory names are already loaded
	if (_names.size() > 0)
		return;

	// Load the inventory names
	Common::SeekableReadStream *stream = _vm->_res->load("invent.txt");
	_names.clear();

	while (stream->pos() < stream->size()) {
		Common::String name;
		char c;
		while ((c = stream->readByte()) != 0)
			name += c;

		_names.push_back(name);
	}
	
	delete stream;

	loadGraphics();
}

/**
 * Load the list of names of graphics for the inventory
 */
void Inventory::loadGraphics() {
	if (_invGraphicsLoaded)
		return;

	// Default all inventory slots to empty
	Common::fill(&_invShapes[0], &_invShapes[MAX_VISIBLE_INVENTORY], (ImageFile *)nullptr);

	for (int idx = _invIndex; (idx < _holdings) && (idx - _invIndex) < MAX_VISIBLE_INVENTORY; ++idx) {
		// Get the name of the item to be dispalyed, figure out it's accompanying
		// .VGS file with it's picture, and then load it
		int invNum = findInv((*this)[idx]._name);
		Common::String fName = Common::String::format("item%02d.vgs", invNum + 1);

		_invShapes[idx] = new ImageFile(fName);
	}

	_invGraphicsLoaded = true;
}

/**
 * Searches through the list of names that correspond to the inventory items
 * and returns the numer that matches the passed name
 */
int Inventory::findInv(const Common::String &name) {
	for (int idx = 0; idx < (int)size(); ++idx) {
		if (scumm_stricmp(name.c_str(), _names[idx].c_str()) == 0)
			return idx;
	}

	return 1;
}

/**
 * Display the character's inventory. The slamIt parameter specifies:
 * 0 = Draw it on the back buffer, and don't display it
 * 1 = Draw it on the back buffer, and then display it
 * 2 = Draw it on the secondary back buffer, and don't display it
 */
void Inventory::putInv(int slamIt) {
	Screen &screen = *_vm->_screen;
	UserInterface &ui = *_vm->_ui;

	// If an inventory item has disappeared (due to using it or giving it),
	// a blank space slot may haave appeared. If so, adjust the inventory
	if (_invIndex > 0 && _invIndex > (_holdings - 6)) {
		--_invIndex;
		freeGraphics();
		loadGraphics();
	}

	if (slamIt != 2) {
		screen.makePanel(Common::Rect(6, 163, 54, 197));
		screen.makePanel(Common::Rect(58, 163, 106, 197));
		screen.makePanel(Common::Rect(110, 163, 158, 197));
		screen.makePanel(Common::Rect(162, 163, 210, 197));
		screen.makePanel(Common::Rect(214, 163, 262, 197));
		screen.makePanel(Common::Rect(266, 163, 314, 197));
	}

	// Iterate through displaying up to 6 objects at a time
	for (int idx = _invIndex; idx < _holdings && (idx - _invIndex) < MAX_VISIBLE_INVENTORY; ++idx) {
		int itemNum = idx - _invIndex;
		Surface &bb = slamIt == 2 ? screen._backBuffer2 : screen._backBuffer1;
		Common::Rect r(8 + itemNum * 52, 165, 51 + itemNum * 52, 194);

		// Draw the background
		if (idx == ui._selector) {
			bb.fillRect(r, 235);
		} else if (slamIt == 2) {
			bb.fillRect(r, BUTTON_MIDDLE);
		}

		// Draw the item image
		Graphics::Surface &img = (*_invShapes[itemNum])[0]._frame;
		bb.transBlitFrom(img, Common::Point(6 + itemNum * 52 + ((47 - img.w) / 2), 
			163 + ((33 - img.h) / 2)));
	}

	if (slamIt == 1)
		screen.slamArea(6, 163, 308, 34);

	if (slamIt != 2)
		ui.clearInfo();

	if (slamIt == 0) {
		invCommands(0);
	} else if (slamIt == 2) {
		screen._backBuffer = &screen._backBuffer2;
		invCommands(0);
		screen._backBuffer = &screen._backBuffer1;
	}
}

/**
 * Put the game into inventory mode and open the interface window.
 * The flag parameter specifies the mode:
 * 0   = plain inventory mode
 * 2   = use inventory mode
 * 3   = give inventory mode
 * 128 = Draw window in the back buffer, but don't display it 
 */
void Inventory::drawInventory(int flag) {
	Screen &screen = *_vm->_screen;
	UserInterface &ui = *_vm->_ui;
	int tempFlag = flag;

	_oldFlag = 7;
	loadInv();

	if (flag == 128) {
		screen._backBuffer = &screen._backBuffer2;
	}

	// Draw the window background
	Surface &bb = *screen._backBuffer;
	bb.fillRect(Common::Rect(0, CONTROLS_Y1, SHERLOCK_SCREEN_WIDTH, CONTROLS_Y1 + 10), BORDER_COLOR);
	bb.fillRect(Common::Rect(0, CONTROLS_Y1 + 10, 2, SHERLOCK_SCREEN_HEIGHT), BORDER_COLOR);
	bb.fillRect(Common::Rect(SHERLOCK_SCREEN_WIDTH - 2, CONTROLS_Y1 + 10, 
		SHERLOCK_SCREEN_WIDTH, SHERLOCK_SCREEN_HEIGHT), BORDER_COLOR);
	bb.fillRect(Common::Rect(0, SHERLOCK_SCREEN_HEIGHT - 2, SHERLOCK_SCREEN_WIDTH,
		SHERLOCK_SCREEN_HEIGHT), BORDER_COLOR);
	bb.fillRect(Common::Rect(2, CONTROLS_Y1 + 10, SHERLOCK_SCREEN_WIDTH - 2, SHERLOCK_SCREEN_HEIGHT - 2),
		INV_BACKGROUND);

	// Draw the buttons
	screen.makeButton(Common::Rect(INVENTORY_POINTS[0][0], CONTROLS_Y1, INVENTORY_POINTS[0][1], 
		CONTROLS_Y1 + 10), INVENTORY_POINTS[0][2] - screen.stringWidth("Exit") / 2, "Exit");
	screen.makeButton(Common::Rect(INVENTORY_POINTS[1][0], CONTROLS_Y1, INVENTORY_POINTS[1][1],
		CONTROLS_Y1 + 10), INVENTORY_POINTS[1][2] - screen.stringWidth("Look") / 2, "Look");
	screen.makeButton(Common::Rect(INVENTORY_POINTS[2][0], CONTROLS_Y1, INVENTORY_POINTS[2][1], 
		CONTROLS_Y1 + 10), INVENTORY_POINTS[2][2] - screen.stringWidth("Use") / 2, "Use");
	screen.makeButton(Common::Rect(INVENTORY_POINTS[3][0], CONTROLS_Y1, INVENTORY_POINTS[3][1], 
		CONTROLS_Y1 + 10), INVENTORY_POINTS[3][2] - screen.stringWidth("Give") / 2, "Give");
	screen.makeButton(Common::Rect(INVENTORY_POINTS[4][0], CONTROLS_Y1, INVENTORY_POINTS[4][1], 
		CONTROLS_Y1 + 10), INVENTORY_POINTS[4][2], "^^");
	screen.makeButton(Common::Rect(INVENTORY_POINTS[5][0], CONTROLS_Y1, INVENTORY_POINTS[5][1], 
		CONTROLS_Y1 + 10), INVENTORY_POINTS[5][2], "^");
	screen.makeButton(Common::Rect(INVENTORY_POINTS[6][0], CONTROLS_Y1, INVENTORY_POINTS[6][1], 
		CONTROLS_Y1 + 10), INVENTORY_POINTS[6][2], "_");
	screen.makeButton(Common::Rect(INVENTORY_POINTS[7][0], CONTROLS_Y1, INVENTORY_POINTS[7][1], 
		CONTROLS_Y1 + 10), INVENTORY_POINTS[7][2], "__");

	if (tempFlag == 128)
		flag = 1;
	_invMode = (InvMode)flag;

	if (flag) {
		ui._oldKey = INVENTORY_COMMANDS[flag];
		_oldFlag = flag;
	} else {
		ui._oldKey = -1;
		_invFlag = 6;
	}

	invCommands(0);
	putInv(0);

	if (tempFlag != 128) {
		if (!ui._windowStyle) {
			screen.slamRect(Common::Rect(0, CONTROLS_Y1, SHERLOCK_SCREEN_WIDTH, SHERLOCK_SCREEN_HEIGHT));
		} else {
			ui.summonWindow(false, CONTROLS_Y1);
		}

		ui._windowOpen = true;
	} else {
		// Reset the screen back buffer to the first buffer now that drawing is done
		screen._backBuffer = &screen._backBuffer1;
	}

	ui._oldUse = -1;
}

/**
 * Prints the line of inventory commands at the top of an inventory window with
 * the correct highlighting
 */
void Inventory::invCommands(bool slamIt) {
	Screen &screen = *_vm->_screen;
	UserInterface &ui = *_vm->_ui;

	if (slamIt) {
		screen.buttonPrint(Common::Point(INVENTORY_POINTS[0][2], CONTROLS_Y1), 
			_invMode == 0 ? COMMAND_HIGHLIGHTED :COMMAND_FOREGROUND,
			true, "Exit");
		screen.buttonPrint(Common::Point(INVENTORY_POINTS[1][2], CONTROLS_Y1), 
			_invMode == 1 ? COMMAND_HIGHLIGHTED :COMMAND_FOREGROUND,
			true, "Look");
		screen.buttonPrint(Common::Point(INVENTORY_POINTS[2][2], CONTROLS_Y1), 
			_invMode == 2 ? COMMAND_HIGHLIGHTED : COMMAND_FOREGROUND,
			true, "Use");
		screen.buttonPrint(Common::Point(INVENTORY_POINTS[3][2], CONTROLS_Y1), 
			_invMode == 3 ? COMMAND_HIGHLIGHTED : COMMAND_FOREGROUND,
			true, "Give");
		screen.print(Common::Point(INVENTORY_POINTS[4][2], CONTROLS_Y1 + 1), 
			_invMode == 0 ? COMMAND_NULL : COMMAND_FOREGROUND,
			"^^");
		screen.print(Common::Point(INVENTORY_POINTS[5][2], CONTROLS_Y1 + 1), 
			_invMode == 0 ? COMMAND_NULL : COMMAND_FOREGROUND,
			"^");
		screen.print(Common::Point(INVENTORY_POINTS[6][2], CONTROLS_Y1 + 1), 
			(_holdings - _invIndex <= 6) ? COMMAND_NULL : COMMAND_FOREGROUND,
			"_");
		screen.print(Common::Point(INVENTORY_POINTS[7][2], CONTROLS_Y1 + 1), 
			(_holdings - _invIndex <= 6) ? COMMAND_NULL : COMMAND_FOREGROUND,
			"__");
		if (_invMode != 1)
			ui.clearInfo();
	} else {
		screen.buttonPrint(Common::Point(INVENTORY_POINTS[0][2], CONTROLS_Y1), 
			_invMode == 0 ? COMMAND_HIGHLIGHTED : COMMAND_FOREGROUND,
			false, "Exit");
		screen.buttonPrint(Common::Point(INVENTORY_POINTS[1][2], CONTROLS_Y1), 
			_invMode == 1 ? COMMAND_HIGHLIGHTED : COMMAND_FOREGROUND,
			false, "Look");
		screen.buttonPrint(Common::Point(INVENTORY_POINTS[2][2], CONTROLS_Y1), 
			_invMode == 2 ? COMMAND_HIGHLIGHTED : COMMAND_FOREGROUND,
			false, "Use");
		screen.buttonPrint(Common::Point(INVENTORY_POINTS[3][2], CONTROLS_Y1), 
			_invMode == 3 ? COMMAND_HIGHLIGHTED : COMMAND_FOREGROUND,
			false, "Give");
		screen.gPrint(Common::Point(INVENTORY_POINTS[4][2], CONTROLS_Y1),
			_invIndex == 0 ? COMMAND_NULL : COMMAND_FOREGROUND,
			"^^");
		screen.gPrint(Common::Point(INVENTORY_POINTS[5][2], CONTROLS_Y1),
			_invIndex == 0 ? COMMAND_NULL : COMMAND_FOREGROUND,
			"^");
		screen.gPrint(Common::Point(INVENTORY_POINTS[6][2], CONTROLS_Y1),
			(_holdings - _invIndex < 7) ? COMMAND_NULL : COMMAND_FOREGROUND,
			"_");
		screen.gPrint(Common::Point(INVENTORY_POINTS[7][2], CONTROLS_Y1),
			(_holdings - _invIndex < 7) ? COMMAND_NULL : COMMAND_FOREGROUND,
			"__");
	}
}

/**
 * Set the highlighting color of a given inventory item
 */
void Inventory::highlight(int index, byte color) {
	Screen &screen = *_vm->_screen;
	Surface &bb = *screen._backBuffer;
	int slot = index - _invIndex;
	Graphics::Surface &img = (*_invShapes[slot])[0]._frame;

	bb.fillRect(Common::Rect(8 + slot * 52, 165, (slot + 1) * 52, 194), color);
	bb.transBlitFrom(img, Common::Point(6 + slot * 52 + ((47 - img.w) / 2),
		163 + ((33 - img.h) / 2)));
	screen.slamArea(8 + slot * 52, 165, 44, 30);
}

void Inventory::doInvJF() {
	Screen &screen = *_vm->_screen;
	Talk &talk = *_vm->_talk;
	UserInterface &ui = *_vm->_ui;

	ui._invLookFlag = true;
	freeInv();

	ui._infoFlag = true;
	ui.clearInfo();

	screen._backBuffer2.blitFrom(screen._backBuffer1, Common::Point(0, CONTROLS_Y),
		Common::Rect(0, CONTROLS_Y, SHERLOCK_SCREEN_WIDTH, SHERLOCK_SCREEN_HEIGHT));
	ui.examine();

	if (!talk._talkToAbort) {
		screen._backBuffer2.blitFrom((*ui._controlPanel)[0]._frame,
			Common::Point(0, CONTROLS_Y));
		loadInv();
	}
}

} // End of namespace Sherlock
