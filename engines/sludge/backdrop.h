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

#ifndef SLUDGE_BACKDROP_H
#define SLUDGE_BACKDROP_H

#if 0
#if !defined(HAVE_GLES2)
#include "GLee.h"
#else
#include <GLES2/gl2.h>
#endif
#endif

#include "variable.h"
#include "graphics.h"

enum {
	LIGHTMAPMODE_NONE       = -1,
	LIGHTMAPMODE_HOTSPOT,
	LIGHTMAPMODE_PIXEL,
	LIGHTMAPMODE_NUM
};

extern unsigned int winWidth, winHeight, sceneWidth, sceneHeight;
extern int lightMapMode;


struct parallaxLayer {
#if 0
	GLubyte *texture;
	GLuint textureName;
#endif
	int width, height, speedX, speedY;
	bool wrapS, wrapT;
	unsigned short fileNum, fractionX, fractionY;
	int cameraX, cameraY;
	parallaxLayer *next;
	parallaxLayer *prev;
};

bool resizeBackdrop(int x, int y);
void killBackDrop();
void loadBackDrop(int fileNum, int x, int y);
void mixBackDrop(int fileNum, int x, int y);
void drawBackDrop();
void blankScreen(int x1, int y1, int x2, int y2);
void darkScreen();
#if ALLOW_FILE
void saveHSI(FILE *writer);
void saveCoreHSI(FILE *writer, GLuint texture, int w, int h);
bool loadHSI(FILE *fp, int, int, bool);
bool mixHSI(FILE *fp, int x = 0, int y = 0);
#endif
void drawHorizontalLine(unsigned int, unsigned int, unsigned int);
void drawVerticalLine(unsigned int, unsigned int, unsigned int);
void hardScroll(int distance);
bool getRGBIntoStack(unsigned int x, unsigned int y, stackHandler *sH);

// Also the light map stuff

void killLightMap();
bool loadLightMap(int v);

#if 0
extern texture lightMap;
#endif

// And background parallax scrolling

void killParallax();
bool loadParallax(unsigned short v, unsigned short fracX, unsigned short fracY);
#if 0
void saveParallaxRecursive(parallaxLayer *me, FILE *fp);
#endif
void reloadParallaxTextures();

void nosnapshot();
bool snapshot();
#if ALLOW_FILE
void saveSnapshot(FILE *fp);
bool restoreSnapshot(FILE *fp);
#endif
#endif