QMAKE_CLEAN += $$DESTDIR/$$TARGET

TEMPLATE = subdirs

CONFIG = ordered

SUBDIRS += \
    01-triangle/01-triangle.pro \
    02-texturedQuad/02-texturedQuad.pro \


