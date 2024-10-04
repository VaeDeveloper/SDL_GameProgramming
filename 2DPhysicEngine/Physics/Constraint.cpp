#include "Constraint.h"
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
// Mat6x6 with the all inverse mass and inverse I of bodies "a" and "b"
///////////////////////////////////////////////////////////////////////////////
//  [ 1/ma  0     0     0     0     0    ]
//  [ 0     1/ma  0     0     0     0    ]
//  [ 0     0     1/Ia  0     0     0    ]
//  [ 0     0     0     1/mb  0     0    ]
//  [ 0     0     0     0     1/mb  0    ]
//  [ 0     0     0     0     0     1/Ib ]
///////////////////////////////////////////////////////////////////////////////
MatMN Constraint::GetInvM() const
{
    MatMN invM(6, 6);
    invM.Zero();
    invM.rows[0][0] = a->invMass;
    invM.rows[1][1] = a->invMass;
    invM.rows[2][2] = a->invI;
    invM.rows[3][3] = b->invMass;
    invM.rows[4][4] = b->invMass;
    invM.rows[5][5] = b->invI;
    
    return invM;
}



///////////////////////////////////////////////////////////////////////////////
// VectorN with the all linear and angular velocities of bodies "a" and "b"
///////////////////////////////////////////////////////////////////////////////
//  [ va.x ]
//  [ va.y ]
//  [ ωa   ]
//  [ vb.x ]
//  [ vb.y ]
//  [ ωb   ]
///////////////////////////////////////////////////////////////////////////////
VectorN Constraint::GetVelocities() const
{
    VectorN V(6);
    V.Zero();
    V[0] = a->velocity.x;
    V[1] = a->velocity.y;
    V[2] = a->angularVelocity;
    V[3] = b->velocity.x;
    V[4] = b->velocity.y;
    V[5] = b->angularVelocity;
    return V;
}

/**
 * 
 */
JointConstraint::JointConstraint()
: Constraint(), jacobian(1, 6), cachedLambda(1), bias(0.0f)
{
    cachedLambda.Zero();
}

/**
 * 
 */
JointConstraint::JointConstraint(Body* a, Body* b, const Vector2D& anchorPoint)
: Constraint(), jacobian(1, 6), cachedLambda(1), bias(0.0f)
{
    this->a = a;
    this->b = b;
    this->aPoint = a->WorldSpaceToLocalSpace(anchorPoint);
    this->bPoint = b->WorldSpaceToLocalSpace(anchorPoint);
    cachedLambda.Zero();
}

/**
 * 
 */
void JointConstraint::PreSolve(const float dt)
{
    // Get the anchor point position in world space
    const Vector2D pa = a->LocalSpaceToWorldSpace(aPoint);
    const Vector2D pb = b->LocalSpaceToWorldSpace(bPoint);

    const Vector2D ra = pa - a->position;
    const Vector2D rb = pb - b->position;

    jacobian.Zero();

    Vector2D J1 = (pa - pb) * 2.0;
    jacobian.rows[0][0] = J1.x;  // A linear velocity.x
    jacobian.rows[0][1] = J1.y;  // A linear velocity.y

    float J2 = ra.CrossProduct(pa - pb) * 2.0;
    jacobian.rows[0][2] = J2;  // A angular velocity

    Vector2D J3 = (pb - pa) * 2.0;
    jacobian.rows[0][3] = J3.x;  // B linear velocity.x
    jacobian.rows[0][4] = J3.y;  // B linear velocity.y

    float J4 = rb.CrossProduct(pb - pa) * 2.0;
    jacobian.rows[0][5] = J4;  // B angular velocity

    // Warm starting (apply cached lambda)
    const MatMN Jt = jacobian.Transpose();
    VectorN impulses = Jt * cachedLambda;

    // Apply the impulses to both bodies
    a->ApplyImpulseLinear(Vector2D(impulses[0], impulses[1]));      // A linear impulse
    a->ApplyImpulseAngular(impulses[2]);                            // A angular impulse
    b->ApplyImpulseLinear(Vector2D(impulses[3], impulses[4]));      // B linear impulse
    b->ApplyImpulseAngular(impulses[5]);                            // B angular impulse

    // Compute the bias term (baumgarte stabilization)
    const float beta = 0.02f;
    float C = (pb - pa).DotProduct(pb - pa);
    C = std::max(0.0f, C - 0.01f);
    bias = (beta / dt) * C;
}

/**
 * 
 */
void JointConstraint::Solve()
{
    const VectorN V = GetVelocities();
    const MatMN invM = GetInvM();

    const MatMN J = jacobian;
    const MatMN Jt = jacobian.Transpose();

    // Compute lambda using Ax=b (Gauss-Seidel method)
    MatMN lhs = J * invM * Jt;  // A
    VectorN rhs = J * V * -1.0f;   // b
    rhs[0] -= bias;
    VectorN lambda = MatMN::SolveGaussSeidel(lhs, rhs);
    cachedLambda += lambda;

    // Compute the impulses with both direction and magnitude
    VectorN impulses = Jt * lambda;

    // Apply the impulses to both bodies
    a->ApplyImpulseLinear(Vector2D(impulses[0], impulses[1]));  // A linear impulse
    a->ApplyImpulseAngular(impulses[2]);                    // A angular impulse
    b->ApplyImpulseLinear(Vector2D(impulses[3], impulses[4]));  // B linear impulse
    b->ApplyImpulseAngular(impulses[5]);                    // B angular impulse
}

/**
 * 
 */
void JointConstraint::PostSolve()
{
    // Limit the warm starting to reasonable limits
    cachedLambda[0] = std::clamp(cachedLambda[0], -10000.0f, 10000.0f);
}

/**
 * 
 */
PenetrationConstraint::PenetrationConstraint() : Constraint(), jacobian(2, 6), cachedLambda(2), bias(0.0f)
{
    cachedLambda.Zero();
    friction = 0.0f;
}

/**
 * 
 */
PenetrationConstraint::PenetrationConstraint(Body* a, Body* b, const Vector2D& aCollisionPoint, const Vector2D& bCollisionPoint, const Vector2D& normal)
    : Constraint(), jacobian(2, 6), cachedLambda(2), bias(0.0f)
{
    this->a = a;
    this->b = b;
    this->aPoint = a->WorldSpaceToLocalSpace(aCollisionPoint);
    this->bPoint = b->WorldSpaceToLocalSpace(bCollisionPoint);
    this->normal = a->WorldSpaceToLocalSpace(normal);
    cachedLambda.Zero();
    friction = 0.0f;
}

/**
 * 
 */
void PenetrationConstraint::PreSolve(const float dt)
{
    // Get the collision points and normal in world space
    const Vector2D pa = a->LocalSpaceToWorldSpace(aPoint);
    const Vector2D pb = b->LocalSpaceToWorldSpace(bPoint);
    Vector2D n = a->LocalSpaceToWorldSpace(normal);

    const Vector2D ra = pa - a->position;
    const Vector2D rb = pb - b->position;

    jacobian.Zero();

    // Populate the first row of the Jacobian (normal vector)
    jacobian.rows[0][0] = -n.x;                                 // A linear velocity.x
    jacobian.rows[0][1] = -n.y;                                 // A linear velocity.y
    jacobian.rows[0][2] = -ra.CrossProduct(n);                  // A angular velocity
    jacobian.rows[0][3] = n.x;                                  // B linear velocity.x
    jacobian.rows[0][4] = n.y;                                  // B linear velocity.y
    jacobian.rows[0][5] = rb.CrossProduct(n);                   // B angular velocity

    // Populate the second row of the Jacobian (tangent vector)
    friction = std::max(a->friction, b->friction);
    
    if (friction > 0.0)
    {
        Vector2D t = n.Normal();                                // The tangent is the vector perpendicular to the normal
        jacobian.rows[1][0] = -t.x;                             // A linear velocity.x
        jacobian.rows[1][1] = -t.y;                             // A linear velocity.y
        jacobian.rows[1][2] = -ra.CrossProduct(t);              // A angular velocity
        jacobian.rows[1][3] = t.x;                              // B linear velocity.x
        jacobian.rows[1][4] = t.y;                              // B linear velocity.y
        jacobian.rows[1][5] = rb.CrossProduct(t);               // B angukar velocity
    }

    // Warm starting (apply cached lambda)
    const MatMN Jt = jacobian.Transpose();
    VectorN impulses = Jt * cachedLambda;

    // Apply the impulses to both bodies
    a->ApplyImpulseLinear(Vector2D(impulses[0], impulses[1]));  // A linear impulse
    a->ApplyImpulseAngular(impulses[2]);                        // A angular impulse
    b->ApplyImpulseLinear(Vector2D(impulses[3], impulses[4]));  // B linear impulse
    b->ApplyImpulseAngular(impulses[5]);                        // B angular impulse

    // Compute the bias term (baumgarte stabilization)
    const float beta = 0.2f;
    float C = (pb - pa).DotProduct(-n);
    C = std::min(0.0f, C + 0.01f);
    bias = (beta / dt) * C;
}

/**
 * 
 */
void PenetrationConstraint::Solve()
{
    const VectorN V = GetVelocities();
    const MatMN invM = GetInvM();

    const MatMN J = jacobian;
    const MatMN Jt = jacobian.Transpose();

    // Compute lambda using Ax=b (Gauss-Seidel method)
    MatMN lhs = J * invM * Jt;  // A
    VectorN rhs = J * V * -1.0f;   // b
    rhs[0] -= bias;
    VectorN lambda = MatMN::SolveGaussSeidel(lhs, rhs);

    // Accumulate impulses and clamp it within constraint limits
    const VectorN oldLambda = cachedLambda;
    cachedLambda += lambda;
    cachedLambda[0] = (cachedLambda[0] < 0.0f) ? 0.0f : cachedLambda[0];

    // Keep friction values between -(λn*µ) and (λn*µ)
    if (friction > 0.0)
    {
        const float maxFriction = cachedLambda[0] * friction;
        cachedLambda[1] = std::clamp(cachedLambda[1], -maxFriction, maxFriction);
    }

    lambda = cachedLambda - oldLambda;

    // Compute the impulses with both direction and magnitude
    VectorN impulses = Jt * lambda;

    // Apply the impulses to both bodies
    a->ApplyImpulseLinear(Vector2D(impulses[0], impulses[1]));  // A linear impulse
    a->ApplyImpulseAngular(impulses[2]);                        // A angular impulse
    b->ApplyImpulseLinear(Vector2D(impulses[3], impulses[4]));  // B linear impulse
    b->ApplyImpulseAngular(impulses[5]);                        // B angular impulse
}

void PenetrationConstraint::PostSolve()
{
    // TODO: Maybe we should clamp the values of cached lambda to reasonable limits
}
