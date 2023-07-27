#include <iostream>
#include <fstream>
#include <cstdio>
#include "vec3.hpp"
#include "ray.hpp"
#include "color.hpp"


//(o + dt - c) * (o + dt - c) = r * r
//(o-c)*(o-c) + 2dt(o - c) + t2d*d = r * r

double hit_sphere(const vec3& center, double radius, const ray& r) {
	vec3 oc = r.origin() - center;
	auto a = dot(r.direction(), r.direction());
	auto b = 2 * dot(r.direction(), oc);
	auto c = dot(oc, oc) - radius * radius;
	auto discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return -1;
	}
	else {
		return (-b - sqrt(discriminant)) / (2.0 * a);
	}
}


color ray_color(const ray& r) {
	auto sphere_center = vec3(0, 0, -1);
	auto t = hit_sphere(sphere_center, 0.5, r);
	if (t > 0.0) {
		vec3 N = unit_vector(r.at(t) - sphere_center);
		return 0.5 * vec3(N.x() + 1.0, N.y() + 1.0, N.z() + 1.0);
	}
	vec3 unit_direction = unit_vector(r.direction());
	t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int> (image_width / aspect_ratio);
	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio * viewport_height;
	auto focal_length = 1.0;

	auto origin = point3(0, 0, 0);
	auto horizontal = vec3(viewport_width, 0, 0);
	auto vertical = vec3(0, viewport_height, 0);
	auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);


	std::ofstream outfile;
	outfile.open("image.ppm", std::ios::out | std::ios::trunc);
	outfile << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	for (int j = image_height - 1; j >= 0; --j) {
		std::cerr << "\rScanline Remaining: " << j << " " << std::flush;
		for (int i = 0; i < image_width; ++i) {
			auto u = double(i) / (image_width - 1);
			auto v = double(j) / (image_height - 1);
			ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
			color pixel_color = ray_color(r);
			write_color(outfile, pixel_color);
		}
	}
	std::cerr << "Done" << std::endl;
	return 0;
}

