#include "common/scummsys.h"
#include "graphics/surface.h"
#include "graphics/colormasks.h"
#include "graphics/palette.h"
#include "audio/mixer_intern.h"
#include "backends/mutex/mutex.h"
#include "backends/base-backend.h"
#include "backends/graphics/graphics.h"
#include "common/rect.h"
#include "common/queue.h"
#include "base/main.h"

class OSystemSwitch : public EventsBaseBackend, public PaletteManager {
public:
    OSystemSwitch();

    ~OSystemSwitch();

private:
    bool pollEvent(Common::Event &event);

    void setPalette(const byte *colors, uint start, uint num);

    void grabPalette(byte *colors, uint start, uint num) const;

    const GraphicsMode *getSupportedGraphicsModes() const;

    bool setGraphicsMode(int mode);

    int getDefaultGraphicsMode() const;

    int getGraphicsMode() const;

    Graphics::PixelFormat getScreenFormat() const;

    Common::List<Graphics::PixelFormat> getSupportedFormats() const;

    void initSize(uint width, uint height, const Graphics::PixelFormat *format = NULL);

    int16 getHeight();

    int16 getWidth();

    PaletteManager *getPaletteManager();

    void copyRectToScreen(const void *buf, int pitch, int x, int y, int w, int h);

    Graphics::Surface *lockScreen();

    void unlockScreen();

    void updateScreen();

    void setShakePos(int shakeOffset);

    void showOverlay();

    void hideOverlay();

    Graphics::PixelFormat getOverlayFormat() const;

    void clearOverlay();

    void grabOverlay(void *buf, int pitch);

    void copyRectToOverlay(const void *buf, int pitch, int x, int y, int w, int h);

    int16 getOverlayHeight();

    int16 getOverlayWidth();

    bool showMouse(bool visible);

    void warpMouse(int x, int y);

    void setMouseCursor(const void *buf, uint w, uint h, int hotspotX, int hotspotY, uint32 keycolor, bool dontScale, const Graphics::PixelFormat *format);

    uint32 getMillis(bool skipRecord = false);

    void delayMillis(uint msecs);

    void getTimeAndDate(TimeDate &t) const;

    MutexRef createMutex();

    void lockMutex(MutexRef mutex);

    void unlockMutex(MutexRef mutex);

    void deleteMutex(MutexRef mutex);

    Audio::Mixer *getMixer();

    void quit();

    void logMessage(LogMessageType::Type type, const char *message);

};
