#include "vec3d.h"

namespace WawMath {

    vec3d::vec3d() : x(0), y(0), z(0), w(1) {
    }

    vec3d::vec3d(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
        w = 1;
    }

    vec3d vec3d::CrossProduct(const vec3d& vec) const {
        vec3d v;
        v.x = y * vec.z - z * vec.y;
        v.y = z * vec.x - x * vec.z;
        v.z = x * vec.y - y * vec.x;
        return v;
    }

    void vec3d::Normalise() {
        float l = Length();
        x /= l;
        y /= l;
        z /= l;
    }

    float vec3d::DotProduct(const vec3d& vec) const {
        return x * vec.x + y * vec.y + z * vec.z;
    }

    float vec3d::Length() const {
        return sqrtf(DotProduct(*this));
    }

    const vec3d operator+(const vec3d& lhs, const vec3d& rhs) {
        return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
    }

    const vec3d operator-(const vec3d& lhs, const vec3d& rhs) {
        return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
    }

    const vec3d operator/(const vec3d& vec, const float k) {
        return { vec.x / k, vec.y / k, vec.z / k };
    }

    const vec3d operator*(const vec3d& vec, const float k) {
        return { vec.x * k, vec.y * k, vec.z * k };
    }

    vec3d IntersectPlane(const vec3d& plane_p, vec3d& plane_n, const vec3d& lineStart, const vec3d& lineEnd) {
        plane_n.Normalise();
        float plane_d = -plane_n.DotProduct(plane_p);
        float ad = lineStart.DotProduct(plane_n);
        float bd = lineEnd.DotProduct(plane_n);
        float t = (-plane_d - ad) / (bd - ad);
        vec3d lineStartToEnd = lineEnd - lineStart;
        vec3d lineToIntersect = lineStartToEnd * t;
        return lineStart + lineToIntersect;
    }

}