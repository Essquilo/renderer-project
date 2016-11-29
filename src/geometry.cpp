//
// Created by Ivan Prymak on 11/29/2016.
//
#include "geometry.h"

template <> template <> Vec3<int>::Vec3<>(const Vec3<double> &v) : x(int(v.x+.5)), y(int(v.y+.5)), z(int(v.z+.5)) {
}

template <> template <> Vec3<double>::Vec3<>(const Vec3<int> &v) : x(v.x), y(v.y), z(v.z) {
}