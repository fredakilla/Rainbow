QMAKE_CLEAN += $$DESTDIR/$$TARGET

TEMPLATE = subdirs

CONFIG = ordered

SUBDIRS += \
    01-triangle/01-triangle.pro \
    02-texturedQuad/02-texturedQuad.pro \
    03-pipelines/03-pipelines.pro \
    04-descriptorSets/04-descriptorSets.pro \


