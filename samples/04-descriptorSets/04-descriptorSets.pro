QT -= core gui
TARGET = 04-descriptorSets
TEMPLATE = app
CONFIG += c++11
CONFIG -= qt

include(../common/samples-common.pri)

SOURCES += src/sample.cpp

SHADERS += $$PWD/src/shaders/texture.vert
SHADERS += $$PWD/src/shaders/texture.frag


# build shaders

spirv.output = ${QMAKE_FILE_NAME}.spv
spirv.commands = glslangValidator -V ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
spirv.depends = $$SHADERS
spirv.input = SHADERS
spirv.variable_out = COMPILED_SHADERS
spirv.CONFIG = target_predeps

target.path = build
target.depends = spirv
QMAKE_EXTRA_COMPILERS += spirv

# copy *.spv to out dir
copydata.commands = $(COPY_DIR) $$PWD/src/shaders/*.spv $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
