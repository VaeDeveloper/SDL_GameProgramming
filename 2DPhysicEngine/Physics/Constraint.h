#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "../Physics/Objects/Body.h"
#include "MatMN.h"



/*------------------------------------------------------------------------------------------------------*/
class Constraint
{
public:
    Body* a;
    Body* b;

    Vector2D aPoint;  // The constraint point in A's local space
    Vector2D bPoint;  // The constraint point in B's local space

    virtual ~Constraint() = default;

    MatMN GetInvM() const;
    VectorN GetVelocities() const;

    virtual void PreSolve(const float dt) {}
    virtual void Solve() {}
    virtual void PostSolve() {}
};

class JointConstraint : public Constraint
{
private:
    MatMN jacobian;
    VectorN cachedLambda;
    float bias;

public:
    JointConstraint();
    JointConstraint(Body* a, Body* b, const Vector2D& anchorPoint);
    void PreSolve(const float dt) override;
    void Solve() override;
    void PostSolve() override;
};

class PenetrationConstraint : public Constraint
{
private:
    MatMN jacobian;
    VectorN cachedLambda;
    float bias;
    Vector2D normal;            // Normal direction of the penetration in A's local space
    float friction;             // Friction coefficient between the two penetrating bodies

public:
    PenetrationConstraint();
    PenetrationConstraint(Body* a, Body* b, const Vector2D& aCollisionPoint, const Vector2D& bCollisionPoint, const Vector2D& normal);
    void PreSolve(const float dt) override;
    void Solve() override;
    void PostSolve() override;
};
/*------------------------------------------------------------------------------------------------------*/

#endif
