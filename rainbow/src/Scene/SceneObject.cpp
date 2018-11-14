#include "../Core/Base.h"
#include "../Scene/SceneObject.h"
#include "../Scene/Camera.h"
#include "../Scene/Light.h"

#define SCENEOBJECT_NAME ""
#define SCENEOBJECT_STATIC true
#define SCENEOBJECT_ENABLED true
#define SCENEOBJECT_POSITION Vector3::zero()
#define SCENEOBJECT_EULER_ANGLES Vector3::zero()
#define SCENEOBJECT_SCALE Vector3::one()
#define SCENEOBJECT_DIRTY_MATRIX_LOCAL 1
#define SCENEOBJECT_DIRTY_MATRIX_WORLD 2
#define SCENEOBJECT_DIRTY_ALL (SCENEOBJECT_DIRTY_MATRIX_LOCAL | SCENEOBJECT_DIRTY_MATRIX_WORLD)

namespace rainbow
{

SceneObject::SceneObject() :
    _name(SCENEOBJECT_NAME),
    _enabled(SCENEOBJECT_ENABLED),
    _static(SCENEOBJECT_STATIC),
    _position(SCENEOBJECT_POSITION),
    _rotation(Quaternion::identity()),
    _eulerAngles(SCENEOBJECT_EULER_ANGLES),
    _scale(SCENEOBJECT_SCALE),
    _localMatrix(Matrix::identity()),
    _worldMatrix(Matrix::identity()),
    _worldToLocalMatrix(Matrix::identity()),
    _dirtyBits(SCENEOBJECT_DIRTY_ALL)
{
}

SceneObject::~SceneObject()
{
}

std::string SceneObject::getName() const
{
    return _name;
}

void SceneObject::setName(const std::string& name)
{
    _name = name;
}

void SceneObject::resetLocalTransform()
{
    setLocalPosition(SCENEOBJECT_POSITION);
    setLocalEulerAngles(SCENEOBJECT_EULER_ANGLES);
    setLocalScale(SCENEOBJECT_SCALE);
    _dirtyBits |= SCENEOBJECT_DIRTY_MATRIX_LOCAL;
}

bool SceneObject::isStatic() const
{
    return _static;
}

void SceneObject::setStatic(bool isStatic)
{
    _static = isStatic;
}

bool SceneObject::isEnabled() const
{
    return _enabled;
}

void SceneObject::setEnabled(bool enabled)
{
    if (_enabled != enabled) 
    {
        _enabled = enabled;
    }
}

const Vector3& SceneObject::getLocalScale() const
{
    return _scale;
}

void SceneObject::setLocalScale(const Vector3& scale)
{
    _scale = scale;
    _dirtyBits |= SCENEOBJECT_DIRTY_MATRIX_LOCAL;
}

const Vector3& SceneObject::getLocalPosition() const
{
    return _position;
}

void SceneObject::setLocalPosition(const Vector3& position)
{
    _position = position;
    _dirtyBits |= SCENEOBJECT_DIRTY_MATRIX_LOCAL;
}

const Vector3& SceneObject::getLocalEulerAngles() const
{
    return _eulerAngles;
}

void SceneObject::setLocalEulerAngles(const Vector3& eulerAngles)
{
    _eulerAngles = eulerAngles;
    _rotation.set(eulerAngles);
    _dirtyBits |= SCENEOBJECT_DIRTY_MATRIX_LOCAL;
}

const Quaternion& SceneObject::getLocalRotation() const
{
    return _rotation;
}

void SceneObject::setLocalRotation(const Quaternion& rotation)
{
    _rotation = rotation;
    _rotation.toEulerAngles(&_eulerAngles);
    _dirtyBits |= SCENEOBJECT_DIRTY_MATRIX_LOCAL;
}

Vector3 SceneObject::getPosition()
{
    return getWorldMatrix().getTranslation();
}

void SceneObject::setPosition(const Vector3& position)
{
    if (_parent.lock() == nullptr)
    {
        _position = position;
    }
    else
    {
        _worldMatrix = getWorldMatrix();
        _worldMatrix.invert(&_worldToLocalMatrix);
        _worldToLocalMatrix.transformPoint(position, &_position);
    }
    _dirtyBits |= SCENEOBJECT_DIRTY_MATRIX_LOCAL;
}

Vector3 SceneObject::getEulerAngles()
{
    return getWorldMatrix().getEulerAngles();
}

void SceneObject::setEulerAngles(const Vector3& eulerAngles)
{
    setLocalEulerAngles(eulerAngles);
    if (_parent.lock() != nullptr)
    {
        Quaternion inversParentRotation;
        _parent.lock()->getRotation().inverse(&inversParentRotation);
        Quaternion::multiply(_rotation, inversParentRotation, &_rotation);
    }
    _dirtyBits |= SCENEOBJECT_DIRTY_MATRIX_LOCAL;
}

Quaternion SceneObject::getRotation()
{
    Quaternion rotation;
    rotation.set(getWorldMatrix());
    return rotation;
}

void SceneObject::setRotation(const Quaternion& rotation)
{
    if (_parent.lock() == nullptr)
    {
        setLocalRotation(rotation);
    }
    else
    {
        Quaternion inversParentRotation = _parent.lock()->getRotation();
        inversParentRotation.inverse();
        Quaternion::multiply(inversParentRotation, rotation, &_rotation);
    }
    _dirtyBits |= SCENEOBJECT_DIRTY_MATRIX_LOCAL;
}

void SceneObject::translateLocal(const Vector3& translation)
{
    Vector3 tx = translation;
    _rotation.transformVector(tx, &tx);
    _position.add(tx);
    _dirtyBits |= SCENEOBJECT_DIRTY_MATRIX_LOCAL;
}

void SceneObject::translate(const Vector3& translation)
{
    Vector3 tx = translation;
    tx.add(getPosition());
    setPosition(tx);
    _dirtyBits |= SCENEOBJECT_DIRTY_MATRIX_LOCAL;
}

void SceneObject::rotateLocal(const Vector3& eulerAngles)
{
    Quaternion rotation = Quaternion::identity();
    rotation.set(eulerAngles);

    // TODO: fix me
    Vector3 testRot;
    rotation.toEulerAngles(&testRot);
    _rotation.multiply(rotation);
    
    Vector3 test;
    _rotation.toEulerAngles(&test);
    _dirtyBits |= SCENEOBJECT_DIRTY_MATRIX_LOCAL;
}

void SceneObject::rotate(const Vector3& eulerAngles)
{
    if (_parent.lock() == nullptr)
    {
        Quaternion rotation;
        rotation.set(eulerAngles);
        _rotation.multiply(rotation);
    }
    else
    {
        Quaternion rotation = getRotation();
        Quaternion inverseParentRotation = _parent.lock()->getRotation();
        inverseParentRotation.inverse();
        Quaternion::multiply(inverseParentRotation, rotation, &_rotation);
    }
    _dirtyBits |= SCENEOBJECT_DIRTY_MATRIX_LOCAL;
}

void SceneObject::lookAt(const Vector3& target, Vector3 worldUp)
{
    Matrix lookAt;
    Matrix::createLookAt(getPosition(), target, worldUp, &lookAt);
    Quaternion rotation;
    rotation.set(lookAt);
    setRotation(rotation);
}

Vector3 SceneObject::getRight()
{
    Vector3 right = getWorldMatrix().getX();
    return right.normalize();
}

Vector3 SceneObject::getUp()
{
    Vector3 up = getWorldMatrix().getY();
    return up.normalize();
}

Vector3 SceneObject::getForward()
{
    Vector3 forward = getWorldMatrix().getZ();
    return forward.normalize();
}

const Matrix& SceneObject::getLocalMatrix()
{
    if (_dirtyBits & SCENEOBJECT_DIRTY_MATRIX_LOCAL)
    {
        _localMatrix.set(_position, _rotation, _scale);
        _dirtyBits &= ~SCENEOBJECT_DIRTY_MATRIX_LOCAL;
    }
    return _localMatrix;
}

void SceneObject::onInitialize()
{
}

void SceneObject::onFinalize()
{
}

void SceneObject::onUpdate(float elapsedTime)
{
}

void SceneObject::onRender(float elapsedTime)
{
}

const Matrix& SceneObject::getWorldMatrix()
{
    if (_dirtyBits & (SCENEOBJECT_DIRTY_MATRIX_WORLD | SCENEOBJECT_DIRTY_MATRIX_LOCAL))
    {
        _dirtyBits &= ~SCENEOBJECT_DIRTY_MATRIX_WORLD;
        if (_parent.lock())
        {
            Matrix::multiply(_parent.lock()->getWorldMatrix(), getLocalMatrix(), &_worldMatrix);
        }
        else
        {
            _worldMatrix = getLocalMatrix();
        }
        for (auto child : _children)
        {
            child->getWorldMatrix();
        }
    }
    return _worldMatrix;
}

const Matrix& SceneObject::getWorldToLocalMatrix()
{
    return _worldToLocalMatrix;
}

void SceneObject::transformPoint(const Vector3& point, Vector3* dst)
{
    GP_ASSERT(dst);
    getWorldMatrix().transformPoint(point, dst);
}

void SceneObject::transformVector(const Vector3& vector, Vector3* dst)
{   
    GP_ASSERT(dst);
    getWorldMatrix().transformVector(vector, dst);
}

void SceneObject::addChild(std::shared_ptr<SceneObject> object)
{
    if (object->_parent.lock() == shared_from_this())
        return;
    if (object->_parent.lock())
        removeChild(object);

    _children.push_back(object);

    object->_parent = shared_from_this();
    object->_dirtyBits |= SCENEOBJECT_DIRTY_MATRIX_LOCAL;
}

void SceneObject::removeChild(std::shared_ptr<SceneObject> object)
{
     auto itr = std::find(_children.begin(), _children.end(), object);
     if (itr != _children.end())
     {
         (*itr)->_parent.reset();
         _children.erase(itr);
     }
}

void SceneObject::clearChildren()
{
    for (auto itr = _children.begin(); itr != _children.end();)
    {
        (*itr)->_parent.reset();
        _children.erase(itr);
    }
}

size_t SceneObject::getChildCount() const
{
    return _children.size();
}

std::vector<std::shared_ptr<SceneObject>> SceneObject::getChildren()
{
    std::vector<std::shared_ptr<SceneObject>> children;
    for (auto child : _children)
    {
        if (child)
        {
            children.push_back(child);
        }
    }
    return children;
}

std::shared_ptr<SceneObject> SceneObject::getParent() const
{
    return _parent.lock();
}

std::shared_ptr<SceneObject> SceneObject::findObject(const std::string& name, bool recursive, bool exactMatch) 
{
    for (auto child : _children)
    {
        if ((exactMatch && child->_name == name) || (!exactMatch && child->_name.find(name) == 0))
        {
            return child;
        }
    }
    if (recursive)
    {
        for (auto child : _children)
        {
            std::shared_ptr<SceneObject> match = child->findObject(name, true, exactMatch);
            if (match)
            {
                return match;
            }
        }
    }
    return nullptr;
}

size_t SceneObject::findObjects(const std::string& name, std::vector<std::shared_ptr<SceneObject>>& objects, bool recursive, bool exactMatch)
{
    size_t count = 0;
    for (auto child : _children)
    {
        if ((exactMatch && child->_name == name) || (!exactMatch && child->_name.find(name) == 0))
        {
            objects.push_back(child);
            ++count;
        }
    }
    if (recursive)
    {
        for (auto child : _children)
        {
            count += child->findObjects(name, objects, recursive, exactMatch);
        }
    }
    return count;
}


void SceneObject::addComponent(std::shared_ptr<Component> component)
{
    // TODO: Support certain components to allow multiple components added but not others...
    // Ex. Scripts, PhysicsColliders, PhysicsJoints, etc.
    bool existing = false;
    for (const auto& c : _components)
    {
        if (component->getClassName().compare(c->getClassName()) == 0)
        {
            existing = true;
            break;
        }
    }
    if (!existing)
    {
        _components.push_back(component);
        component->setObject(shared_from_this());
    }
}

void SceneObject::removeComponent(std::shared_ptr<Component> component)
{
    auto itr = std::find(_components.begin(), _components.end(), component);
    if (itr != _components.end())
    {
        _components.erase(itr);
    }
}

template<class T>
std::shared_ptr<T> SceneObject::getComponent() const 
{
    for (const auto& component : _components)
    {
        if (dynamic_cast<T*>(component.get()) != nullptr) 
        {
            return std::dynamic_pointer_cast<T>(component);
        }
    }
    return nullptr;
}

template<class T>
void SceneObject::findComponents(std::vector<std::shared_ptr<T>>& components) const
{
    for (const auto& component : _components)
    {
        if (dynamic_cast<T*>(component.get()) != nullptr) 
        {
            components.push_back(std::dynamic_pointer_cast<T>(component));
        }
    }
}

const std::vector<std::shared_ptr<Component>>& SceneObject::getComponents() const
{
    return _components;
}

std::shared_ptr<Serializable> SceneObject::createObject()
{
    return std::static_pointer_cast<Serializable>(std::make_shared<SceneObject>());
}

std::string SceneObject::getClassName()
{
    return "rainbow::SceneObject";
}

void SceneObject::onSerialize(Serializer* serializer)
{
    serializer->writeString("name", _name.c_str(), SCENEOBJECT_NAME);
    serializer->writeBool("enabled", isEnabled(), SCENEOBJECT_ENABLED);
    serializer->writeBool("static", isStatic(), SCENEOBJECT_STATIC);
    serializer->writeVector("position", getLocalPosition(), SCENEOBJECT_POSITION);
    serializer->writeVector("eulerAngles", getLocalEulerAngles(), SCENEOBJECT_EULER_ANGLES);
    serializer->writeVector("scale", getLocalScale(), SCENEOBJECT_SCALE);
    if (_children.size() > 0)
    {
        serializer->writeObjectList("children", _children.size());
        for (auto child : _children)
        {
            serializer->writeObject(nullptr, std::static_pointer_cast<Serializable>(child));
        }
    }
    if (_components.size() > 0)
    {
        serializer->writeObjectList("components", _components.size());
        for (auto component : _components)
        {
            serializer->writeObject(nullptr, std::static_pointer_cast<Serializable>(component));
        }
    }
}

void SceneObject::onDeserialize(Serializer* serializer)
{
    serializer->readString("name", _name, SCENEOBJECT_NAME);
    _enabled = serializer->readBool("enabled", SCENEOBJECT_STATIC);
    _static = serializer->readBool("static", SCENEOBJECT_STATIC);
    _position = serializer->readVector("position", SCENEOBJECT_POSITION);
    _eulerAngles = serializer->readVector("eulerAngles", SCENEOBJECT_EULER_ANGLES);
    _scale = serializer->readVector("scale", SCENEOBJECT_SCALE);
    size_t childCount = serializer->readObjectList("children");
    if (childCount > 0)
    {
        _children.resize(childCount);
        for (size_t i = 0; i < _children.size(); i++)
        {
            _children[i] = std::static_pointer_cast<SceneObject>(serializer->readObject(nullptr));
        }
    }
    size_t componentCount =  serializer->readObjectList("components");
    if (componentCount > 0)
    {
        _components.resize(componentCount);
        for (size_t i = 0; i < _components.size(); i++)
        {
            _components[i] = std::static_pointer_cast<Component>(serializer->readObject(nullptr));
        }
    }
}

}
