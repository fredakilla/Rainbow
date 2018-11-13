QMAKE_CLEAN += $$DESTDIR/$$TARGET

TEMPLATE = subdirs

CONFIG = ordered

SUBDIRS += \
    rainbow/rainbow.pro \
    rainbow-app/rainbow-app.pro \
    rainbow-editor/rainbow-editor.pro

