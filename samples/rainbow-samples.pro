QMAKE_CLEAN += $$DESTDIR/$$TARGET

TEMPLATE = subdirs

CONFIG = ordered

SUBDIRS += \
    00-init/sample-init.pro \
    01-triangle/sample-triangle.pro \
    02-uniforms/sample-uniforms.pro


