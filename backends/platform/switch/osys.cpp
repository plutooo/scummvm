#include "osys.h"

OSystemSwitch::~OSystemSwitch() {

}

OSystemSwitch::OSystemSwitch() {

}

bool OSystemSwitch::pollEvent(Event &event) {
    return false; // ?
}

void OSystemSwitch::setPalette(const byte *colors, uint start, uint num) {

}

void OSystemSwitch::grabPalette(byte *colors, uint start, uint num) {

}

const GraphicsMode *OSystemSwitch::getSupportedGraphicsModes() {
    return NULL;
}

bool OSystemSwitch::setGraphicsMode(int mode) {
    return false;
}

int OSystemSwitch::getDefaultGraphicsMode() {
    return 0;
}

int OSystemSwitch::getGraphicsMode() {
    return 0;
}

Graphics::PixelFormat OSystemSwitch::getScreenFormat() {

}

Common::List<Graphics::PixelFormat> OSystemSwitch::getSupportedFormats() {

}

void OSystemSwitch::initSize(uint width, uint height, const Graphics::PixelFormat *format = NULL) {

}

int16 OSystemSwitch::getHeight() {
    return 720;
}

int16 OSystemSwitch::getWidth() {
    return 1280;
}

PaletteManager *OSystemSwitch::getPaletteManager() {

}

void OSystemSwitch::copyRectToScreen(const void *buf, int pitch, int x, int y, int w, int h) {

}

Graphics::Surface *OSystemSwitch::lockScreen() {

}

void OSystemSwitch::unlockScreen() {

}

void OSystemSwitch::updateScreen() {

}

void OSystemSwitch::setShakePos(int shakeOffset) {

}

void OSystemSwitch::showOverlay() {

}

void OSystemSwitch::hideOverlay() {

}

Graphics::PixelFormat OSystemSwitch::getOverlayFormat() {

}

void OSystemSwitch::clearOverlay() {

}

void OSystemSwitch::grabOverlay(void *buf, int pitch) {

}

void OSystemSwitch::copyRectToOverlay(const void *buf, int pitch, int x, int y, int w, int h) {

}

int16 OSystemSwitch::getOverlayHeight() {
    return 720;
}

int16 OSystemSwitch::getOverlayWidth() {
    return 1280;
}

bool OSystemSwitch::showMouse(bool visible) {
    return false;
}

void OSystemSwitch::warpMouse(int x, int y) {

}

void OSystemSwitch::setMouseCursor(const void *buf, uint w, uint h, int hotspotX, int hotspotY, uint32 keycolor, bool dontScale, const Graphics::PixelFormat *format) {

}

uint32 OSystemSwitch::getMillis(bool skipRecord = false) {
    return 0;
}

void OSystemSwitch::delayMillis(uint msecs) {

}

void OSystemSwitch::getTimeAndDate(TimeDate &t) {

}

MutexRef OSystemSwitch::createMutex() {

}

void OSystemSwitch::lockMutex(MutexRef mutex) {

}

void OSystemSwitch::unlockMutex(MutexRef mutex) {

}

void OSystemSwitch::deleteMutex(MutexRef mutex) {

}

Audio::Mixer *OSystemSwitch::getMixer() {
    return NULL;
}

void OSystemSwitch::quit() {

}

void OSystemSwitch::logMessage(LogMessageType::Type type, const char *message) {

}
