#ifndef AARECT_H
#define AARECT_H
//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "rtweekend.h"

#include "hittable.h"

// Edited by Gzh
class triangle : public hittable
{
public:
    triangle() {}

    triangle(const vec3& _a, const vec3& _b, const vec3& _c, shared_ptr<material> mat) :
        a(_a), b(_b), c(_c), mp(mat) {}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override
    {
        vec3 ac = c - a;
        vec3 ab = b - a;
        vec3 ao = r.orig - a;

        vec3 face_normal = unit_vector(cross(ab, ac));

        double t, u, v, inv = 1.0 / dot(cross(r.dir, ac), ab);
        u = dot(cross(r.dir, ac), ao) * inv;
        v = dot(cross(ao, ab), r.dir) * inv;
        t = dot(cross(ao, ab), ac) * inv;

        double w = 1 - u - v;
        if (u < 0 || u > 1) return false;
        if (v < 0 || v > 1) return false;
        if (w < 0 || w > 1) return false;
        if (t < t_min || t > t_max) return false;

        rec.u = u;
        rec.v = v;
        rec.t = t;
        rec.set_face_normal(r, face_normal);
        rec.mat_ptr = mp;
        rec.p = r.at(t);

        return true;
    }

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override
    {
        using std::min;
        using std::max;

        const double offset = 0.0001;
        auto min_x = min(a.x(), min(b.x(), c.x())) - offset;
        auto max_x = max(a.x(), max(b.x(), c.x())) + offset;
        auto min_y = min(a.y(), min(b.y(), c.y())) - offset;
        auto max_y = max(a.y(), max(b.y(), c.y())) + offset;
        auto min_z = min(a.z(), min(b.z(), c.z())) - offset;
        auto max_z = max(a.z(), max(b.z(), c.z())) + offset;

        output_box = aabb(point3(min_x, min_y, min_z), point3(max_x, max_y, max_z));
        return true;
    }

    virtual double pdf_value(const point3& origin, const vec3& v) const override
    {
        hit_record rec;
        if (!this->hit(ray(origin, v), 0.001, infinity, rec))
            return 0;

        auto ab = (a - b).length();
        auto ac = (a - c).length();
        auto bc = (b - c).length();
        auto p = (ab + ac + bc) / 2.0;

        auto area = sqrt(p * (p - ab) * (p - ac) * (p - bc));
        auto distance_squared = rec.t * rec.t * v.length_squared();
        auto cosine = fabs(dot(v, rec.normal) / v.length());

        return distance_squared / (cosine * area);
    }

    virtual vec3 random(const point3& origin) const override
    {
        auto wa = random_double();
        auto wb = random_double();
        auto wc = random_double();
        auto sum = wa + wb + wc;

        wa = wa / sum;
        wb = wb / sum;
        wc = 1.0 - wa - wb;

        auto random_point = a * wa + b * wb + c * wc;
        return random_point - origin;
    }

public:
    shared_ptr<material> mp;
    vec3 a,b,c;
};


class xy_rect : public hittable {
    public:
        xy_rect() {}

        xy_rect(
            double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<material> mat
        ) : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            // The bounding box must have non-zero width in each dimension, so pad the Z
            // dimension a small amount.
            output_box = aabb(point3(x0,y0, k-0.0001), point3(x1, y1, k+0.0001));
            return true;
        }

    public:
        shared_ptr<material> mp;
        double x0, x1, y0, y1, k;
};

class xz_rect : public hittable {
    public:
        xz_rect() {}

        xz_rect(
            double _x0, double _x1, double _z0, double _z1, double _k, shared_ptr<material> mat
        ) : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            // The bounding box must have non-zero width in each dimension, so pad the Y
            // dimension a small amount.
            output_box = aabb(point3(x0,k-0.0001,z0), point3(x1, k+0.0001, z1));
            return true;
        }

        virtual double pdf_value(const point3& origin, const vec3& v) const override {
            hit_record rec;
            if (!this->hit(ray(origin, v), 0.001, infinity, rec))
                return 0;

            auto area = (x1-x0)*(z1-z0);
            auto distance_squared = rec.t * rec.t * v.length_squared();
            auto cosine = fabs(dot(v, rec.normal) / v.length());

            return distance_squared / (cosine * area);
        }

        virtual vec3 random(const point3& origin) const override {
            auto random_point = point3(random_double(x0,x1), k, random_double(z0,z1));
            return random_point - origin;
        }

    public:
        shared_ptr<material> mp;
        double x0, x1, z0, z1, k;
};

class yz_rect : public hittable {
    public:
        yz_rect() {}

        yz_rect(
            double _y0, double _y1, double _z0, double _z1, double _k, shared_ptr<material> mat
        ) : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            // The bounding box must have non-zero width in each dimension, so pad the X
            // dimension a small amount.
            output_box = aabb(point3(k-0.0001, y0, z0), point3(k+0.0001, y1, z1));
            return true;
        }

    public:
        shared_ptr<material> mp;
        double y0, y1, z0, z1, k;
};

bool xy_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto t = (k-r.origin().z()) / r.direction().z();
    if (t < t_min || t > t_max)
        return false;

    auto x = r.origin().x() + t*r.direction().x();
    auto y = r.origin().y() + t*r.direction().y();
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;

    rec.u = (x-x0)/(x1-x0);
    rec.v = (y-y0)/(y1-y0);
    rec.t = t;
    auto outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);

    return true;
}

bool xz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto t = (k-r.origin().y()) / r.direction().y();
    if (t < t_min || t > t_max)
        return false;

    auto x = r.origin().x() + t*r.direction().x();
    auto z = r.origin().z() + t*r.direction().z();
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;

    rec.u = (x-x0)/(x1-x0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    auto outward_normal = vec3(0, 1, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);

    return true;
}

bool yz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto t = (k-r.origin().x()) / r.direction().x();
    if (t < t_min || t > t_max)
        return false;

    auto y = r.origin().y() + t*r.direction().y();
    auto z = r.origin().z() + t*r.direction().z();
    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;

    rec.u = (y-y0)/(y1-y0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    auto outward_normal = vec3(1, 0, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);

    return true;
}

#endif
