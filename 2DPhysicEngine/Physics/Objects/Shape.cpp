#include "Shape.h"
#include <iostream>
#include <limits>
#include "../Constants.h"
#include "../Math.h"

namespace ShapeConstants
{
const double MomentInertiaDot = 0.5;
const double TwoDoubleValue = 2.0;
const double MultipliedRigidbody = 0.083333;
}

/*-------------------------------------------------------------------------------------------------------*/
CircleShape::CircleShape(float radius)
{
    this->radius = radius;
    std::cout << "CircleShape constructor called!" << std::endl;
}
/*-------------------------------------------------------------------------------------------------------*/
CircleShape::~CircleShape()
{
    std::cout << "CircleShape destructor called!" << std::endl;
}
/*-------------------------------------------------------------------------------------------------------*/
Shape* CircleShape::Clone() const
{
    return new CircleShape(radius);
}
/*-------------------------------------------------------------------------------------------------------*/
void CircleShape::UpdateVertices(float angle, const Vector2D& position)
{
    return;  // Circles don't have vertices... nothing to do here
}
/*-------------------------------------------------------------------------------------------------------*/
ShapeType CircleShape::GetType() const
{
    return CIRCLE;
}
/*-------------------------------------------------------------------------------------------------------*/
float CircleShape::GetMomentOfInertia() const
{
    // For solid circles, the moment of inertia is 1/2 * r^2
    // But this still needs to be multiplied by the rigidbody's mass
    return ShapeConstants::MomentInertiaDot * (radius * radius);
}
/*-------------------------------------------------------------------------------------------------------*/
PolygonShape::PolygonShape(const std::vector<Vector2D> vertices)
{
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();

    // Initialize the vertices of the polygon shape and set width and height
    for (auto vertex : vertices)
    {
        localVertices.push_back(vertex);
        worldVertices.push_back(vertex);

        // Find min and max X and Y to calculate polygon width and height
        minX = Math::Min(minX, vertex.x);
        maxX = Math::Max(maxX, vertex.x);
        minY = Math::Min(minY, vertex.y);
        maxY = Math::Max(maxY, vertex.y);
    }
    width = maxX - minX;
    height = maxY - minY;

    std::cout << "PolygonShape constructor called!" << std::endl;
}
/*-------------------------------------------------------------------------------------------------------*/
PolygonShape::~PolygonShape()
{
    std::cout << "PolygonShape destructor called!" << std::endl;
}
/*-------------------------------------------------------------------------------------------------------*/
ShapeType PolygonShape::GetType() const
{
    return POLYGON;
}
/*-------------------------------------------------------------------------------------------------------*/
Shape* PolygonShape::Clone() const
{
    return new PolygonShape(localVertices);
}
/*-------------------------------------------------------------------------------------------------------*/
float PolygonShape::PolygonArea() const
{
    float area = PhysicEngine::ZERO_FLOAT;

    for (int i = PhysicEngine::ZERO; i < localVertices.size(); i++)
    {
        int j = (i + PhysicEngine::POSITIVE) % localVertices.size();
        area += localVertices[i].CrossProduct(localVertices[j]);
    }
    return area / ShapeConstants::TwoDoubleValue;
}
/*-------------------------------------------------------------------------------------------------------*/
Vector2D PolygonShape::PolygonCentroid() const
{
    Vector2D cg = Vector2D::Zero;

    for (int i = PhysicEngine::ZERO; i < localVertices.size(); i++)
    {
        int j = (i + PhysicEngine::POSITIVE) % localVertices.size();
        cg += (localVertices[i] + localVertices[j]) * localVertices[i].CrossProduct(localVertices[j]);
    }
    return cg / 6 / PolygonArea();
}
/*-------------------------------------------------------------------------------------------------------*/
float PolygonShape::GetMomentOfInertia() const
{
    float acc0 = PhysicEngine::ZERO;
    float acc1 = PhysicEngine::ZERO;

    for (unsigned int i = PhysicEngine::ZERO; i < localVertices.size(); i++)
    {
        auto a = localVertices[i];
        auto b = localVertices[(i + PhysicEngine::POSITIVE) % localVertices.size()];
        auto cross = abs(a.CrossProduct(b));
        acc0 += cross * (a.DotProduct(a) + b.DotProduct(b) + a.DotProduct(b));
        acc1 += cross;
    }
    return acc0 / 6 / acc1;
}
/*-------------------------------------------------------------------------------------------------------*/
Vector2D PolygonShape::EdgeAt(int index) const
{
    int currVertex = index;
    int nextVertex = (index + PhysicEngine::POSITIVE) % worldVertices.size();
    return worldVertices[nextVertex] - worldVertices[currVertex];
}
/*-------------------------------------------------------------------------------------------------------*/
float PolygonShape::FindMinSeparation(const PolygonShape* other, int& indexReferenceEdge, Vector2D& supportPoint) const
{
    float separation = std::numeric_limits<float>::lowest();
    // Loop all the vertices of "this" polygon
    for (int i = PhysicEngine::ZERO; i < this->worldVertices.size(); i++)
    {
        Vector2D va = this->worldVertices[i];
        Vector2D normal = this->EdgeAt(i).Normal();
        // Loop all the vertices of the "other" polygon
        float minSep = std::numeric_limits<float>::max();
        Vector2D minVertex;
        for (int j = PhysicEngine::ZERO; j < other->worldVertices.size(); j++)
        {
            Vector2D vb = other->worldVertices[j];
            float proj = (vb - va).DotProduct(normal);
            if (proj < minSep)
            {
                minSep = proj;
                minVertex = vb;
            }
        }
        if (minSep > separation)
        {
            separation = minSep;
            indexReferenceEdge = i;
            supportPoint = minVertex;
        }
    }
    return separation;
}
/*-------------------------------------------------------------------------------------------------------*/
int PolygonShape::FindIncidentEdge(const Vector2D& normal) const
{
    int indexIncidentEdge = PhysicEngine::ZERO;

    float minProj = std::numeric_limits<float>::max();
    for (int i = PhysicEngine::ZERO; i < this->worldVertices.size(); ++i)
    {
        auto edgeNormal = this->EdgeAt(i).Normal();
        auto proj = edgeNormal.DotProduct(normal);
        if (proj < minProj)
        {
            minProj = proj;
            indexIncidentEdge = i;
        }
    }
    return indexIncidentEdge;
}
/*-------------------------------------------------------------------------------------------------------*/
int PolygonShape::ClipSegmentToLine(
    const std::vector<Vector2D>& contactsIn, std::vector<Vector2D>& contactsOut, const Vector2D& c0, const Vector2D& c1) const
{
    // Start with no output points
    int numOut = PhysicEngine::ZERO;

    // Calculate the distance of end points to the line
    Vector2D normal = (c1 - c0).Normalize();
    float dist0 = (contactsIn[0] - c0).CrossProduct(normal);
    float dist1 = (contactsIn[1] - c0).CrossProduct(normal);

    // If the points are behind the plane
    if (dist0 <= PhysicEngine::ZERO) contactsOut[numOut++] = contactsIn[0];
    if (dist1 <= PhysicEngine::ZERO) contactsOut[numOut++] = contactsIn[1];

    // If the points are on different sides of the plane (one distance is negative and the other is positive)
    if (dist0 * dist1 < PhysicEngine::ZERO)
    {
        float totalDist = dist0 - dist1;

        // Fint the intersection using linear interpolation: lerp(start,end) => start + t*(end-start)
        float t = dist0 / (totalDist);
        Vector2D contact = contactsIn[0] + (contactsIn[1] - contactsIn[0]) * t;
        contactsOut[numOut] = contact;
        numOut++;
    }
    return numOut;
}
/*-------------------------------------------------------------------------------------------------------*/
void PolygonShape::UpdateVertices(float angle, const Vector2D& position)
{
    // Loop all the vertices, transforming from local to world space
    for (int i = PhysicEngine::ZERO; i < localVertices.size(); i++)
    {
        // First rotate, then we translate
        worldVertices[i] = localVertices[i].Rotate(angle);
        worldVertices[i] += position;
    }
}
/*--------------------------------------------------------------------------------------------------------------------------*/
BoxShape::BoxShape(float width, float height)
{
    this->width = width;
    this->height = height;

    // Load the vertices of the box polygon
    localVertices.push_back(Vector2D(-width / ShapeConstants::TwoDoubleValue, -height / ShapeConstants::TwoDoubleValue));
    localVertices.push_back(Vector2D(+width / ShapeConstants::TwoDoubleValue, -height / ShapeConstants::TwoDoubleValue));
    localVertices.push_back(Vector2D(+width / ShapeConstants::TwoDoubleValue, +height / ShapeConstants::TwoDoubleValue));
    localVertices.push_back(Vector2D(-width / ShapeConstants::TwoDoubleValue, +height / ShapeConstants::TwoDoubleValue));

    worldVertices.push_back(Vector2D(-width / ShapeConstants::TwoDoubleValue, -height / ShapeConstants::TwoDoubleValue));
    worldVertices.push_back(Vector2D(+width / ShapeConstants::TwoDoubleValue, -height / ShapeConstants::TwoDoubleValue));
    worldVertices.push_back(Vector2D(+width / ShapeConstants::TwoDoubleValue, +height / ShapeConstants::TwoDoubleValue));
    worldVertices.push_back(Vector2D(-width / ShapeConstants::TwoDoubleValue, +height / ShapeConstants::TwoDoubleValue));
}
/*--------------------------------------------------------------------------------------------------------------------------*/
BoxShape::~BoxShape()
{
    // TODO: ...
}
/*--------------------------------------------------------------------------------------------------------------------------*/
ShapeType BoxShape::GetType() const
{
    return BOX;
}
/*--------------------------------------------------------------------------------------------------------------------------*/
Shape* BoxShape::Clone() const
{
    return new BoxShape(width, height);
}
/*--------------------------------------------------------------------------------------------------------------------------*/
float BoxShape::GetMomentOfInertia() const
{
    // For a rectangle, the moment of inertia is 1/12 * (w^2 + h^2)
    // But this still needs to be multiplied by the rigidbody's mass
    return (ShapeConstants::MultipliedRigidbody) * (width * width + height * height);
}
/*--------------------------------------------------------------------------------------------------------------------------*/
