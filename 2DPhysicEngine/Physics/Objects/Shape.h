#ifndef SHAPE_H
#define SHAPE_H

#include "../Vector2D.h"
#include <vector>

enum ShapeType
{
    CIRCLE,
    POLYGON,
    BOX
};

struct Shape
{
    virtual ~Shape() = default;
    virtual ShapeType GetType() const = 0;
    virtual Shape* Clone() const = 0;
    virtual void UpdateVertices(float angle, const Vector2D& position) = 0;
    virtual float GetMomentOfInertia() const = 0;
};

struct CircleShape : public Shape
{
    float radius;

    CircleShape(const float radius);
    virtual ~CircleShape();
    ShapeType GetType() const override;
    Shape* Clone() const override;
    void UpdateVertices(float angle, const Vector2D& position) override;
    float GetMomentOfInertia() const override;
};

struct PolygonShape : public Shape
{
    float width;
    float height;

    std::vector<Vector2D> localVertices;
    std::vector<Vector2D> worldVertices;

    PolygonShape() = default;
    PolygonShape(const std::vector<Vector2D> vertices);
    virtual ~PolygonShape();
    ShapeType GetType() const override;
    Shape* Clone() const override;
    Vector2D EdgeAt(int index) const;
    float FindMinSeparation(const PolygonShape* other, int& indexReferenceEdge, Vector2D& supportPoint) const;
    int FindIncidentEdge(const Vector2D& normal) const;
    int ClipSegmentToLine(const std::vector<Vector2D>& contactsIn, std::vector<Vector2D>& contactsOut, const Vector2D& c0, const Vector2D& c1) const;
    float PolygonArea() const;
    Vector2D PolygonCentroid() const;
    float GetMomentOfInertia() const override;
    void UpdateVertices(float angle, const Vector2D& position) override;
};

struct BoxShape : public PolygonShape
{
    BoxShape(float width, float height);
    virtual ~BoxShape();
    ShapeType GetType() const override;
    Shape* Clone() const override;
    float GetMomentOfInertia() const override;
};

#endif