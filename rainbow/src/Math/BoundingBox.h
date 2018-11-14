#pragma once

#include "../Math/Frustum.h"

namespace rainbow
{

/**
 * Defines a 3-dimensional axis-aligned bounding box.
 */
class BoundingBox
{
public:

    /**
     * The minimum point.
     */
    Vector3 min = Vector3::zero();

    /**
     * The maximum point.
     */
    Vector3 max = Vector3::zero();

    /**
     * Constructor.
     */
    BoundingBox();

    /**
     * Constructor.
     *
     * @param min The minimum point of the bounding box.
     * @param max The maximum point of the bounding box.
     */
    BoundingBox(const Vector3& min, const Vector3& max);

    /**
     * Constructor.
     *
     * @param copy The sphere copy.
     */
    BoundingBox(const BoundingBox& copy);

    /**
     * Destructor.
     */
    ~BoundingBox();

    /**
     * Gets an empty bounding box.
     */
    static const BoundingBox& empty();

    /**
     * Gets the center point of the bounding box.
     *
     * This method computes the center point of the box from its min and max.
     *
     * @return The center point of the bounding box.
     */
    Vector3 getCenter() const;

    /**
     * Gets the center point of the bounding box.
     *
     * This method computes the center point of the box from its min and max
     * points and stores the result in dst.
     *
     * @param dst The vector to store the result in.
     */
    void getCenter(Vector3* dst) const;

    /**
     * Gets the corners of the bounding box in the specified array.
     *
     * The corners are returned as follows: 0 to 3 specify the near face starting at the upper left point
     * when looking towards the origin from the positive z-axis in a counter-clockwise fashion; 4 to 7
     * specify the far face starting at the upper left point when looking towards the origin from the negative
     * z-axis in a counter-clockwise fashion.
     *
     * @param dst The array to store the corners in. Must be size 8.
     */
    void getCorners(Vector3* dst) const;

    /**
     * Tests whether this bounding box intersects the specified bounding object.
     *
     * @param box The bounding box to test intersection with.
     * @return true if the specified bounding box intersects this bounding box; false otherwise.
     */
    bool intersects(const BoundingBox& box) const;

    /**
     * Tests whether this bounding box intersects the specified bounding sphere.
     *
     * @param sphere The bounding sphere to test intersection with.
     * @return true if the specified bounding sphere intersects this bounding box; false otherwise.
     */
    bool intersects(const BoundingSphere& sphere) const;

    /**
     * Tests whether this bounding box intersects the specified frustum.
     *
     * @param frustum The frustum to test intersection with.
     * @return true if this bounding sphere intersects the specified frustum; false otherwise.
     */
    bool intersects(const Frustum& frustum) const;

    /**
     * Tests whether this bounding box intersects the specified plane.
     *
     * @param plane The plane to test intersection with.
     * 
     * @return Plane::kIntersectsBack if this bounding box is in the negative half-space of
     *  the plane, Plane::kIntersectsFront if it is in the positive half-space of the plane;
     *  and Plane::kIntersects if it intersects the plane.
     */
    float intersects(const Plane& plane) const;

    /**
     * Tests whether this bounding box intersects the specified ray.
     *
     * @param ray The ray to test intersection with.
     * @return The distance from the origin of the ray to this bounding box or
     *  kIntersectsNone if the ray does not intersect this bounding box.
     */
    float intersects(const Ray& ray) const;

    /**
     * Determines if this bounding box is empty.
     *
     * @return true if this bounding box is empty; false otherwise.
     */
    bool isEmpty() const;

    /**
     * Sets this bounding box to the smallest bounding box
     * that contains both this bounding box and the specified bounding sphere.
     *
     * @param sphere The bounding sphere to merge with.
     */
    void merge(const BoundingSphere& sphere);

    /**
     * Sets this bounding box to the smallest bounding box
     * that contains both this bounding object and the specified bounding box.
     *
     * @param box The bounding box to merge with.
     */
    void merge(const BoundingBox& box);

    /**
     * Sets this bounding box to the specified values.
     *
     * @param min The minimum point of the bounding box.
     * @param max The maximum point of the bounding box.
     */
    void set(const Vector3& min, const Vector3& max);

    /**
     * Sets this bounding box to the specified bounding box.
     *
     * @param box The bounding box to set to.
     */
    void set(const BoundingBox& box);

    /**
     * Sets this box to tightly contain the specified bounding sphere.
     *
     * @param sphere The sphere to contain.
     */
    void set(const BoundingSphere& sphere);

    /**
     * Transforms the bounding box by the given transformation matrix.
     *
     * @param matrix The transformation matrix to transform by.
     */
    void transform(const Matrix& matrix);

    /**
     * @brief operator=
     */
    BoundingBox& operator=(const BoundingBox& other);

    /**
     * Transforms this bounding box by the given matrix.
     * 
     * @param matrix The matrix to transform by.
     * @return This bounding box, after the transformation occurs.
     */
    BoundingBox& operator*=(const Matrix& matrix);
};

/**
 * Transforms the given bounding box by the given matrix.
 * 
 * @param matrix The matrix to transform by.
 * @param box The bounding box to transform.
 * @return The resulting transformed bounding box.
 */
 const BoundingBox operator*(const Matrix& matrix, const BoundingBox& box);

}
