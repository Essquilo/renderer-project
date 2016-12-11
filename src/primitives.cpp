#pragma once
//
// Created by Milena on 02.10.2016.
//

#include "primitives.h"
#include <iostream>

using namespace std;

void brezenchem_line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	int dx = x1 - x0;
	int dy = y1 - y0;
	bool steep = false;
	if (abs(x0 - x1) < abs(y0 - y1)) {
		steep = true;
		swap(x0, y0);
		swap(x1, y1);
	}
	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}
	dx = x1 - x0;
	dy = y1 - y0;
	int k = abs(dy);
	int error = 0;
	int pace = dy != 0 ? dy / abs(dy) : 0;
	for (int x = x0; x <= x1; x++) {
		if (!steep)
			image.set(x, y0, color);
		else
			image.set(y0, x, color);
		error += k;
		if (error * 2 > dx) {
			y0 += pace;
			error -= abs(dx);
		}
	}
}

Vec3f barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P) {
	Vec3f s[2];
	for (int i = 1; i >= 0; i--) {
		s[i][0] = C[i] - A[i];
		s[i][1] = B[i] - A[i];
		s[i][2] = A[i] - P[i];
	}
	Vec3f u = cross(s[0], s[1]);
	if (std::abs(u[2]) > 1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
		return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	return Vec3f(-1, 1, 1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}


void triangle(int iface, Vec3i v0, Vec3i v1, Vec3i v2, Vec3i uv0, Vec3i uv1, Vec3i uv2, TGAImage& image, Vec3f light,
              Model& model, int* zbuffer) {
	if (v0.y == v1.y && v0.y == v2.y) return;
	vector<Vec3i> face = model.face(iface);
	Vec3f vn0 = model.vn(face[0][2]).normalize();
	Vec3f vn1 = model.vn(face[1][2]).normalize();
	Vec3f vn2 = model.vn(face[2][2]).normalize();

	if (v0.y > v1.y) {
		swap(v0, v1);
		swap(uv0, uv1);
		swap(vn0, vn1);
	}
	if (v0.y > v2.y) {
		swap(v0, v2);
		swap(uv0, uv2);
		swap(vn0, vn2);
	}
	if (v1.y > v2.y) {
		swap(v1, v2);
		swap(uv1, uv2);
		swap(vn1, vn2);
	}

	int total_height = v2.y - v0.y;
	int segment1_height = v1.y - v0.y;
	int segment2_height = v2.y - v1.y;

	int uv_total_height = v2.y - v0.y;
	int uv_segment1_height = v1.y - v0.y;
	int uv_segment2_height = v2.y - v1.y;

	Vec3f d20 = total_height == 0 ? Vec3f(0, 0, 0) : Vec3f(v2 - v0) / (double)total_height;
	Vec3f d10 = segment1_height == 0 ? Vec3f(0, 0, 0) : Vec3f(v1 - v0) / (double)segment1_height;
	Vec3f d21 = segment2_height == 0 ? Vec3f(0, 0, 0) : Vec3f(v2 - v1) / (double)segment2_height;

	Vec3f uv_d20 = uv_total_height == 0 ? Vec3f(0, 0, 0) : Vec3f(uv2 - uv0) / (double)uv_total_height;
	Vec3f uv_d10 = uv_segment1_height == 0 ? Vec3f(0, 0, 0) : Vec3f(uv1 - uv0) / (double)uv_segment1_height;
	Vec3f uv_d21 = uv_segment2_height == 0 ? Vec3f(0, 0, 0) : Vec3f(uv2 - uv1) / (double)uv_segment2_height;

	Vec3f left_f = Vec3f(v0);
	Vec3f right_f = (v1.y == v0.y) ? Vec3f(v1) : Vec3f(v0);

	Vec3f uv_left_f = Vec3f(uv0);
	Vec3f uv_right_f = (v1.y == v0.y) ? Vec3f(uv1) : Vec3f(uv0);

	for (int i = 0; i < total_height; i++) {
		if (i == segment1_height) {
			right_f = Vec3f(v1);
			uv_right_f = Vec3f(uv1);
		}
		bool second_half = i >= v1.y - v0.y || v1.y == v0.y;
		int segment_height = second_half ? v2.y - v1.y : v1.y - v0.y;
		Vec3f left = Vec3f(left_f);
		Vec3f right = Vec3f(right_f);
		Vec3f uv_left = Vec3f(uv_left_f);
		Vec3f uv_right = Vec3f(uv_right_f);
		if (left.x > right.x) {
			swap(left, right);
			swap(uv_left, uv_right);
		}
		Vec3f d = Vec3f(right.x == left.x ? (Vec3f(right - left)) : (Vec3f(right - left)) / (double)(right.x - left.x));
		Vec3f uv_d = Vec3f(right.x == left.x ? (Vec3f(uv_right - uv_left)) : (Vec3f(uv_right - uv_left)) / (double)(right.x - left.x));
		Vec3f point_f = Vec3f(left);
		Vec3f uv_point_f = Vec3f(uv_left);
		for (int j = left.x; j <= right.x; j++) {
			float phi = right.x == left.x ? 1. : (float)(j - left.x) / (float)(right.x - left.x);
			Vec3i point = Vec3i(point_f);
			Vec3i uv_point = Vec3i(uv_point_f);
			int idx = point.x + point.y * image.get_width();
			if (point.x >= image.get_width() || point.x < 0 || point.y >= image.get_height() || point.y < 0) {
				point_f = point_f + d;
				uv_point_f = uv_point_f + uv_d;
				continue;
			}
			if (zbuffer[idx] < point.z) {
				zbuffer[idx] = point.z;
				Vec3f baricentric = barycentric(Vec2f(v0.x, v0.y), Vec2f(v1.x, v1.y), Vec2f(v2.x, v2.y), Vec2f(point.x, point.y));
				Vec3f interpolated_n = (vn0 * baricentric[0] + vn1 * baricentric[1] + vn2 * baricentric[2]).normalize();
				double intensity = abs(interpolated_n * light);
				TGAColor color = model.diffuse(iface, uv_point);
				//TGAColor color = TGAColor(255, 255, 255, 255);
				image.set(point.x, point.y, TGAColor(intensity * color.r, intensity * color.g, intensity * color.b, 255));
				if (abs(intensity) > 1)
					cerr << "intensity not normalized for iface#" << iface << endl;
			}
			point_f = point_f + d;
			uv_point_f = uv_point_f + uv_d;
		}
		left_f = left_f + d20;
		right_f = right_f + (second_half ? d21 : d10);
		uv_left_f = uv_left_f + uv_d20;
		uv_right_f = uv_right_f + (second_half ? uv_d21 : uv_d10);
	}
}

Matrix look_at(Vec3f eye, Vec3f center, Vec3f up) {
	Vec3f z = (eye - center).normalize();
	Vec3f x = cross(up, z).normalize();
	Vec3f y = cross(z, x).normalize();
	Matrix Minv = Matrix::identity();
	Matrix Tr = Matrix::identity();
	for (int i = 0; i<3; i++) {
		Minv[0][i] = x[i];
		Minv[1][i] = y[i];
		Minv[2][i] = z[i];
		Tr[i][3] = -center[i];
	}
	Matrix res = Minv*Tr;
	return res;
}

Matrix viewport(int x, int y, int w, int h, int d) {
	Matrix m = Matrix::identity();
	m[0][3] = x + w / 2.;
	m[1][3] = y + h / 2.;
	m[2][3] = d / 2.;

	m[0][0] = w / 2.;
	m[1][1] = h / 2.;
	m[2][2] = d / 2.;
	return m;
}
