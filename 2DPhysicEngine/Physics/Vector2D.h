#ifndef Vector2D_H
#define Vector2D_H

#include <iostream>

/**-------------------------------------------------------------------------------------------------------
 * @Discrioption: Vector2D Class (class 2D vector and functions)
 * @functions   Add(), Sub(), Scale(), Rotate(return Vector2D), Magnitude(return float), MagnitudeSquared(return float)
 * Normalize(return Vector2D&), Unitvector(return Vector2D), Normal(return Vector2D), DotProduct(return float), CrossProduct(return float)
 */
/*------------------------------------------------------------------------------------------------------*/
struct Vector2D
{

public:

    float x;
    float y;


    Vector2D();
    Vector2D(float x, float y);

    ~Vector2D() = default;

    void Add(const Vector2D& v);                    // v1.Add(v2)
    void Sub(const Vector2D& v);                    // v1.Sub(v2)
    void Scale(const float n);                      // v1.Scale(n)
    Vector2D Rotate(const float angle) const;       // v1.Rotate(angle)

    float Magnitude() const;                        // v1.Magnitude()
    float MagnitudeSquared() const;                 // v1.MagnitudeSquared()

    Vector2D& Normalize();                          // v1.Normalize()
    Vector2D UnitVector() const;                    // v1.UnitVector()
    Vector2D Normal() const;                        // n = v1.Normal()

    float DotProduct(const Vector2D& v) const;      // v1.Dot(v2)
    float CrossProduct(const Vector2D& v) const;    // v1.Cross(v2)
    float Length() const;
    float LengthSq() const;
    static inline Vector2D ZeroVector();                   // ZeroVector return (Vector2D(0.0, 0.0)

#pragma region Operator_
    /* Override operators function */
    Vector2D& operator=(const Vector2D& v);         // v1 = v2
    bool operator==(const Vector2D& v) const;       // v1 == v2
    bool operator!=(const Vector2D& v) const;       // v1 != v2

    Vector2D operator+(const Vector2D& v) const;    // v1 + v2
    Vector2D operator-(const Vector2D& v) const;    // v1 - v2
    Vector2D operator*(const float n) const;        // v1 * n
    Vector2D operator/(const float n) const;        // v1 / n
    Vector2D operator-();                           // -v1

    Vector2D& operator+=(const Vector2D& v);        // v1 += v2
    Vector2D& operator-=(const Vector2D& v);        // v1 -= v2
    Vector2D& operator*=(const float n);            // v1 *= n
    Vector2D& operator/=(const float n);            // v1 /= n
#pragma endregion Operator_

#pragma region StaticConstants
    static const Vector2D Zero;
    static const Vector2D UnitX;
    static const Vector2D UnitY;
    static const Vector2D NegUnitX;
    static const Vector2D NegUnitY;
#pragma endregion StaticConstants
};
/*------------------------------------------------------------------------------------------------------*/
#endif