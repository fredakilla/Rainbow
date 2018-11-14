QT -= core gui
TARGET = rainbow
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11
CONFIG -= qt
CONFIG(debug, debug|release): DEFINES += _DEBUG

CONFIG(debug, debug|release): TARGET = rainbow_d
DESTDIR = $$PWD/../build/lib

#CONFIG += GP_API_VULKAN

#-------------------------------------------------------------------
# files
#-------------------------------------------------------------------

SOURCES += \
    src/Animation/Animation.cpp \
    src/Assets/Asset.cpp \
    src/Assets/AssetLoader.cpp \
    src/Assets/AssetManager.cpp \
    src/Assets/TextureLoader.cpp \
    src/Audio/Audio.cpp \
    src/Audio/AudioClip.cpp \
    src/Audio/AudioListener.cpp \
    src/Audio/AudioSource.cpp \
    src/Core/Activator.cpp \
    src/Core/FileSystem.cpp \
    src/Core/Game.cpp \
    src/Core/Input.cpp \
    src/Core/Logger.cpp \
    src/Core/Platform.cpp \
    src/Core/Serializer.cpp \
    src/Core/SerializerBinary.cpp \
    src/Core/SerializerJson.cpp \
    src/Graphics/Buffer.cpp \
    src/Graphics/CommandBuffer.cpp \
    src/Graphics/DescriptorSet.cpp \
    src/Graphics/Graphics.cpp \
    src/Graphics/Heightfield.cpp \
    src/Graphics/Material.cpp \
    src/Graphics/Mesh.cpp \
    src/Graphics/MeshInstance.cpp \
    src/Graphics/Renderer.cpp \
    src/Graphics/RenderPass.cpp \
    src/Graphics/RenderPipeline.cpp \
    src/Graphics/Sampler.cpp \
    src/Graphics/Semaphore.cpp \
    src/Graphics/Shader.cpp \
    src/Graphics/Sprite.cpp \
    src/Graphics/Text.cpp \
    src/Graphics/Texture.cpp \
    src/Graphics/VertexLayout.cpp \
    src/Math/BoundingBox.cpp \
    src/Math/BoundingSphere.cpp \
    src/Math/Curve.cpp \
    src/Math/Frustum.cpp \
    src/Math/MathUtil.cpp \
    src/Math/Matrix.cpp \
    src/Math/Path.cpp \
    src/Math/Plane.cpp \
    src/Math/Quaternion.cpp \
    src/Math/Ray.cpp \
    src/Math/Rectangle.cpp \
    src/Math/Vector2.cpp \
    src/Math/Vector3.cpp \
    src/Math/Vector4.cpp \
    src/Physics/Physics.cpp \
    src/Physics/PhysicsCharacter.cpp \
    src/Physics/PhysicsCloth.cpp \
    src/Physics/PhysicsCollider.cpp \
    src/Physics/PhysicsJoint.cpp \
    src/Physics/PhysicsJointFixed.cpp \
    src/Physics/PhysicsJointGeneric.cpp \
    src/Physics/PhysicsJointHinge.cpp \
    src/Physics/PhysicsJointSocket.cpp \
    src/Physics/PhysicsJointSpring.cpp \
    src/Physics/PhysicsMaterial.cpp \
    src/Physics/PhysicsRigidBody.cpp \
    src/Physics/PhysicsVehicle.cpp \
    src/Scene/Camera.cpp \
    src/Scene/Component.cpp \
    src/Scene/Light.cpp \
    src/Scene/Scene.cpp \
    src/Scene/SceneLoader.cpp \
    src/Scene/SceneObject.cpp \
    src/Scene/Tileset.cpp \
    src/Script/Script.cpp

HEADERS += \
    src/Animation/Animation.h \
    src/Assets/Asset.h \
    src/Assets/AssetLoader.h \
    src/Assets/AssetManager.h \
    src/Assets/TextureLoader.h \
    src/Audio/Audio.h \
    src/Audio/AudioClip.h \
    src/Audio/AudioListener.h \
    src/Audio/AudioSource.h \
    src/Core/Activator.h \
    src/Core/Base.h \
    src/Core/FileSystem.h \
    src/Core/Game.h \
    src/Core/Input.h \
    src/Core/Logger.h \
    src/Core/Platform.h \
    src/Core/Serializable.h \
    src/Core/Serializer.h \
    src/Core/SerializerBinary.h \
    src/Core/SerializerJson.h \
    src/Core/Stream.h \
    src/Graphics/Buffer.h \
    src/Graphics/ColorBlendState.h \
    src/Graphics/CommandBuffer.h \
    src/Graphics/DepthStencilState.h \
    src/Graphics/DescriptorSet.h \
    src/Graphics/Format.h \
    src/Graphics/Graphics.h \
    src/Graphics/GraphicsUtil.h \
    src/Graphics/Heightfield.h \
    src/Graphics/Material.h \
    src/Graphics/Mesh.h \
    src/Graphics/MeshInstance.h \
    src/Graphics/RasterizerState.h \
    src/Graphics/Renderer.h \
    src/Graphics/RenderPass.h \
    src/Graphics/RenderPipeline.h \
    src/Graphics/Sampler.h \
    src/Graphics/Semaphore.h \
    src/Graphics/Shader.h \
    src/Graphics/Sprite.h \
    src/Graphics/Text.h \
    src/Graphics/Texture.h \
    src/Graphics/VertexLayout.h \
    src/Math/BoundingBox.h \
    src/Math/BoundingSphere.h \
    src/Math/Curve.h \
    src/Math/Frustum.h \
    src/Math/MathUtil.h \
    src/Math/Matrix.h \
    src/Math/Path.h \
    src/Math/Plane.h \
    src/Math/Quaternion.h \
    src/Math/Ray.h \
    src/Math/Rectangle.h \
    src/Math/Vector2.h \
    src/Math/Vector3.h \
    src/Math/Vector4.h \
    src/Physics/Physics.h \
    src/Physics/PhysicsCharacter.h \
    src/Physics/PhysicsCloth.h \
    src/Physics/PhysicsCollider.h \
    src/Physics/PhysicsJoint.h \
    src/Physics/PhysicsJointFixed.h \
    src/Physics/PhysicsJointGeneric.h \
    src/Physics/PhysicsJointHinge.h \
    src/Physics/PhysicsJointSocket.h \
    src/Physics/PhysicsJointSpring.h \
    src/Physics/PhysicsMaterial.h \
    src/Physics/PhysicsRigidBody.h \
    src/Physics/PhysicsVehicle.h \
    src/Scene/Camera.h \
    src/Scene/Component.h \
    src/Scene/Light.h \
    src/Scene/Scene.h \
    src/Scene/SceneLoader.h \
    src/Scene/SceneObject.h \
    src/Scene/Tileset.h \
    src/Script/Script.h \
    src/rainbow.h




#-------------------------------------------------------------------
# graphics api
#-------------------------------------------------------------------

#GP_API_VULKAN {
#
#    message(Building with vulkan)
#
#SOURCES += \
#    src/Graphics/api/VK/Buffer.cpp \
#    src/Graphics/api/VK/CommandBuffer.cpp \
#    src/Graphics/api/VK/DescriptorSet.cpp \
#    src/Graphics/api/VK/Graphics.cpp \
#    src/Graphics/api/VK/RenderPass.cpp \
#    src/Graphics/api/VK/RenderPipeline.cpp \
#    src/Graphics/api/VK/Sampler.cpp \
#    src/Graphics/api/VK/Semaphore.cpp \
#    src/Graphics/api/VK/Shader.cpp \
#    src/Graphics/api/VK/Texture.cpp
#
#HEADERS += \
#    src/Graphics/api/VK/Buffer.h \
#    src/Graphics/api/VK/CommandBuffer.h \
#    src/Graphics/api/VK/DescriptorSet.h \
#    src/Graphics/api/VK/Graphics.h \
#    src/Graphics/api/VK/GraphicsUtil.h \
#    src/Graphics/api/VK/RenderPass.h \
#    src/Graphics/api/VK/RenderPipeline.h \
#    src/Graphics/api/VK/Sampler.h \
#    src/Graphics/api/VK/Semaphore.h \
#    src/Graphics/api/VK/Shader.h \
#    src/Graphics/api/VK/Texture.h
#
#} else {
#
#    message(Building with bgfx)
#
#SOURCES += \
#    src/Graphics/api/BGFX/Buffer.cpp \
#    src/Graphics/api/BGFX/CommandBuffer.cpp \
#    src/Graphics/api/BGFX/DescriptorSet.cpp \
#    src/Graphics/api/BGFX/Graphics.cpp \
#    src/Graphics/api/BGFX/RenderPass.cpp \
#    src/Graphics/api/BGFX/RenderPipeline.cpp \
#    src/Graphics/api/BGFX/Sampler.cpp \
#    src/Graphics/api/BGFX/Semaphore.cpp \
#    src/Graphics/api/BGFX/Shader.cpp \
#    src/Graphics/api/BGFX/Texture.cpp
#
#HEADERS += \
#    src/api/BGFX/Buffer.h \
#    src/Graphics/api/BGFX/CommandBuffer.h \
#    src/Graphics/api/BGFX/DescriptorSet.h \
#    src/Graphics/api/BGFX/Graphics.h \
#    src/Graphics/api/BGFX/GraphicsUtil.h \
#    src/Graphics/api/BGFX/RenderPass.h \
#    src/Graphics/api/BGFX/RenderPipeline.h \
#    src/Graphics/api/BGFX/Sampler.h \
#    src/Graphics/api/BGFX/Semaphore.h \
#    src/Graphics/api/BGFX/Shader.h \
#    src/Graphics/api/BGFX/Texture.h
#}



#-------------------------------------------------------------------
# platform
#-------------------------------------------------------------------

INCLUDEPATH += $$PWD/../rainbow/src
INCLUDEPATH += $$PWD/../external-deps/include

win32 {
    DEFINES += _WINDOWS WIN32 _WIN32
    DEFINES += VK_USE_PLATFORM_WIN32_KHR
    INCLUDEPATH += $$(VULKAN_SDK)/Include
    QMAKE_CXXFLAGS_WARN_ON -= -w34100
    QMAKE_CXXFLAGS_WARN_ON -= -w34189
    QMAKE_CXXFLAGS_WARN_ON -= -w4302
    QMAKE_CXXFLAGS_WARN_ON -= -w4311
    QMAKE_CXXFLAGS_WARN_ON -= -w4244
}

linux {
    DEFINES += SDL_VIDEO_DRIVER_X11
    DEFINES += VK_USE_PLATFORM_XLIB_KHR
    INCLUDEPATH += $$(VULKAN_SDK)/include
    QMAKE_CXXFLAGS += -lstdc++ -pthread -w
}

macx {
    QMAKE_CXXFLAGS += -x c++ -x objective-c++ -stdlib=libc++ -w
    DEFINES += VK_USE_PLATFORM_MACOS_MVK
    INCLUDEPATH += "$$(HOME)/vulkansdk-macos-1.0.69.0/macOS/include"
}
