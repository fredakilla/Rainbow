#pragma once

// Core
#include "Core/Base.h"
#include "Core/Platform.h"
#include "Core/Logger.h"
#include "Core/Game.h"
#include "Core/FileSystem.h"
#include "Core/Stream.h"
#include "Core/Serializable.h"
#include "Core/Serializer.h"
#include "Core/SerializerBinary.h"
#include "Core/SerializerJson.h"
#include "Core/Activator.h"
#include "Core/Input.h"

// Math
#include "Math/MathUtil.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Quaternion.h"
#include "Math/Matrix.h"
#include "Math/Frustum.h"
#include "Math/BoundingSphere.h"
#include "Math/BoundingBox.h"
#include "Math/Plane.h"
#include "Math/Ray.h"
#include "Math/Rectangle.h"
#include "Math/Path.h"

// Animation
#include "Animation/Animation.h"

// Components
#include "Scene/Scene.h"
#include "Scene/SceneObject.h"
#include "Scene/Component.h"
#include "Scene/Camera.h"
#include "Scene/Light.h"
#include "Scene/Tileset.h"

//#include "Emitter.h"
//#include "Terrain.h"
//#include "Water.h"
//#include "Tree.h"
//#include "Decal.h"
//#include "Video.h"

// UI
//#include "UI.h"
//#include "UITheme.h"
//#include "UIThemeSet.h"
//#include "UILayout.h"
//#include "UIControl.h"
//#include "UIButton.h"
//#include "UICheckBox.h"
//#include "UIRadioButon.h"
//#include "UILabel.h"
//#include "UITextBox.h"
//#include "UIProgressBar.h"
//#include "UISlider.h"
//#include "UIComboBox.h"
//#include "UIListBox.h"
//#include "UIImageView.h"
//#include "UIVideoView.h"

// Graphics
/*
#include "Graphics/Graphics.h"
#include "Graphics/Format.h"
#include "Graphics/Buffer.h"
#include "Graphics/Texture.h"
#include "Graphics/RenderPass.h"
#include "Graphics/Sampler.h"
#include "Graphics/Shader.h"
#include "Graphics/VertexLayout.h"
#include "Graphics/DescriptorSet.h"
#include "Graphics/RenderPipeline.h"
#include "Graphics/CommandBuffer.h"
#include "Graphics/Semaphore.h"

#include "Graphics/Material.h"
#include "Graphics/Renderer.h"
#include "Graphics/Mesh.h"
#include "Graphics/Sprite.h"
#include "Graphics/Text.h"
*/

// Graphics new
#include "Graphics/Graphics.h"



// Assets
#include "Assets/Asset.h"
#include "Assets/AssetManager.h"
#include "Assets/AssetLoader.h"
#include "Assets/TextureLoader.h"
//#include "MaterialLoader.h"
//#include "../Scene/SceneLoader.h"
//#include "FontLoader.h"
//#include "SpriteLoader.h"

// Audio
#include "Audio/Audio.h"
#include "Audio/AudioClip.h"
#include "Audio/AudioListener.h"
#include "Audio/AudioSource.h"

// Physics
#include "Physics/Physics.h"
#include "Physics/PhysicsMaterial.h"
#include "Physics/PhysicsCollider.h"
#include "Physics/PhysicsRigidBody.h"
#include "Physics/PhysicsJoint.h"
#include "Physics/PhysicsJointFixed.h"
#include "Physics/PhysicsJointHinge.h"
#include "Physics/PhysicsJointSpring.h"
#include "Physics/PhysicsJointSocket.h"
#include "Physics/PhysicsJointGeneric.h"
#include "Physics/PhysicsCharacter.h"
#include "Physics/PhysicsVehicle.h"
#include "Physics/PhysicsCloth.h"

// Script
#include "Script/Script.h"

using namespace rainbow;
