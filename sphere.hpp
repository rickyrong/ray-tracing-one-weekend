#pragma once

#include "hittable.hpp"
#include "vec3.hpp"

class sphere : public hittable {
public:
	sphere(){}
	sphere(point3 cen, double r):center(cen), radius(r){}

	virtual bool hit(
		const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
	point3 center;
	double radius;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) return false;
	auto sqrtd = sqrt(discriminant);

	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root) {
			return false;
		}
	}
	rec.t = root;
	rec.p = r.at(rec.t);
	rec.normal = (rec.p - center) / radius;
	return true;
}
