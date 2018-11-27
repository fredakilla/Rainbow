#pragma once

#include "../Core/Base.h"
#include "../Scene/Component.h"
#include "../Scene/Mesh.h"

namespace rainbow
{

class Model : public Component
{
public:

    /**
     * Constructor.
     */
    Model();

    /**
     * Destructor.
     */
    ~Model();


    void setMesh(std::shared_ptr<Mesh> mesh);

    /**
     * Returns the Mesh for this Model.
     *
     * @return The Mesh for this Model.
     */
    std::shared_ptr<Mesh> getMesh() const;


protected:

    /**
     * @see Activator::createObject
     */
    static std::shared_ptr<Serializable> createObject();

    /**
     * @see Serializable::getClassName
     */
    std::string getClassName() override;

    /**
     * @see Serializable::onSerialize
     */
    void onSerialize(Serializer* serializer) override;

    /**
     * @see Serializable::onDeserialize
     */
    void onDeserialize(Serializer* serializer) override;

private:

    std::shared_ptr<Mesh> _mesh;

};

}
