QT -= core gui
TARGET = rainbow-app
TEMPLATE = app
CONFIG += c++11
CONFIG -= qt
CONFIG(debug, debug|release): DEFINES += _DEBUG

BUILD_DIR = $$PWD/../../build
EXTERNAL_DEPS_DIR = $$PWD/../../external-deps

DESTDIR = $$BUILD_DIR/bin

SOURCES += src/sample.cpp

HEADERS += src/sample.h

INCLUDEPATH += ../../rainbow/src
INCLUDEPATH += $$EXTERNAL_DEPS_DIR/include

win32 {
    DEFINES += _WINDOWS WIN32
    DEFINES += VK_USE_PLATFORM_WIN32_KHR
    SOURCES += src/main-windows.cpp
    INCLUDEPATH += $$(VULKAN_SDK)/Include
    CONFIG(debug, debug|release): LIBS += -L$$PWD/../build/lib/ -lrainbow_d
    CONFIG(release, debug|release): LIBS += -L$$PWD/../build/lib/ -lrainbow
    CONFIG(debug, debug|release): LIBS += -L$$PWD/../external-deps/lib/windows/x86_64/Debug/ -lrainbow-deps
    CONFIG(release, debug|release): LIBS += -L$$PWD/../external-deps/lib/windows/x86_64/Release/ -lrainbow-deps
    LIBS += -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32 -limm32 -limagehlp -lversion -lwinmm -lxinput
    LIBS += -L$$(VULKAN_SDK)/Lib -lvulkan-1
    QMAKE_CXXFLAGS_WARN_ON -= -w34100
    QMAKE_CXXFLAGS_WARN_ON -= -w34189
    QMAKE_CXXFLAGS_WARN_ON -= -w4302
    QMAKE_CXXFLAGS_WARN_ON -= -w4311
    QMAKE_CXXFLAGS_WARN_ON -= -w4244
    RC_FILE = rainbow-app.rc
}

linux {
    CONFIG(debug, debug|release): PRE_TARGETDEPS += $$BUILD_DIR/lib/librainbow_d.a
    CONFIG(release, debug|release):PRE_TARGETDEPS += $$BUILD_DIR/lib/librainbow.a
    DEFINES += SDL_VIDEO_DRIVER_X11
    DEFINES += VK_USE_PLATFORM_XLIB_KHR
    SOURCES += src/main-linux.cpp
    INCLUDEPATH += $$(VULKAN_SDK)/include
    INCLUDEPATH += /usr/include/gtk-2.0
    INCLUDEPATH += /usr/lib/x86_64-linux-gnu/gtk-2.0/include
    INCLUDEPATH += /usr/include/atk-1.0
    INCLUDEPATH += /usr/include/cairo
    INCLUDEPATH += /usr/include/gdk-pixbuf-2.0
    INCLUDEPATH += /usr/include/pango-1.0
    INCLUDEPATH += /usr/include/gio-unix-2.0
    INCLUDEPATH += /usr/include/freetype2
    INCLUDEPATH += /usr/include/glib-2.0
    INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
    INCLUDEPATH += /usr/include/pixman-1
    INCLUDEPATH += /usr/include/libpng12
    INCLUDEPATH += /usr/include/harfbuzz
    CONFIG(debug, debug|release): LIBS += -L$$BUILD_DIR/lib/ -lrainbow_d
    CONFIG(release, debug|release): LIBS += -L$$BUILD_DIR/lib/ -lrainbow
    LIBS += -L$$EXTERNAL_DEPS_DIR/lib/linux/x86_64/ -lrainbow-deps
    LIBS += -lm -lrt -ldl -lX11 -lpthread -lglib-2.0 -lgobject-2.0 -lxcb -lsndio
    LIBS += -L$$(VULKAN_SDK)/lib/ -lvulkan
    QMAKE_CXXFLAGS += -lstdc++ -pthread -w
}

macx {
    DEFINES += VK_USE_PLATFORM_MACOS_MVK
    INCLUDEPATH += $$(HOME)/vulkansdk-macos-1.0.69.0/macOS/include
    SOURCES += src/main-macos.mm
    CONFIG(debug, debug|release): LIBS += -L$$PWD/../build/lib/ -lrainbow_d
    CONFIG(release, debug|release):LIBS += -L$$PWD/../build/lib/ -lrainbow
    LIBS += -L$$PWD/../external-deps/lib/macos/x86_64/ -lrainbow-deps
    LIBS += -L/usr/lib -liconv
    LIBS += -F$$(HOME)/vulkansdk-macos-1.0.69.0/MoltenVK/macOS -framework MoltenVK
    LIBS += -F/System/Library/Frameworks -framework Metal
    LIBS += -F/System/Library/Frameworks -framework MetalKit
    LIBS += -F/System/Library/Frameworks -framework GameKit 
    LIBS += -F/System/Library/Frameworks -framework IOKit
    LIBS += -F/System/Library/Frameworks -framework IOSurface
    LIBS += -F/System/Library/Frameworks -framework ForceFeedback
    LIBS += -F/System/Library/Frameworks -framework OpenAL
    LIBS += -F/System/Library/Frameworks -framework CoreAudio
    LIBS += -F/System/Library/Frameworks -framework AudioToolbox
    LIBS += -F/System/Library/Frameworks -framework QuartzCore
    LIBS += -F/System/Library/Frameworks -framework Carbon
    LIBS += -F/System/Library/Frameworks -framework Cocoa
    LIBS += -F/System/Library/Frameworks -framework Foundation
    QMAKE_MACOSX_DEPLOYMENT_TARGET=10.13
    QMAKE_CXXFLAGS += -x c++ -x objective-c++ -stdlib=libc++ -w
    QMAKE_INFO_PLIST = rainbow-app.plist

    icon.files = rainbow-app.icns
    icon.path = Contents/Resources
    QMAKE_BUNDLE_DATA += icon

    gameconfig.files = game.config
    gameconfig.path = Contents/Resources
    QMAKE_BUNDLE_DATA += gameconfig
}

DISTFILES += \
    game.config
