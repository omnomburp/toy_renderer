#pragma once

#include <cmath>
#include <numbers>

struct vec3 {
	float x;
	float y;
	float z;

    inline vec3 operator/(float rhs) const {
        return { (x * 1/rhs), (y * 1/rhs), (z * 1/rhs) }; 
    }
};

struct mat3 {
	float data[3][3];

	inline float* operator[](int row) {
        return data[row];
    }

    inline const float* operator[](int row) const {
        return data[row];
    }

    inline vec3 operator*(vec3 rhs) const {
    	return { (data[0][0] * rhs.x) + (data[0][1] * rhs.y) + (data[0][2] * rhs.z) ,
    			 (data[1][0] * rhs.x) + (data[1][1] * rhs.y) + (data[1][2] * rhs.z),
    			 (data[2][0] * rhs.x) + (data[2][1] * rhs.y) + (data[02][2] * rhs.z)  };
    }
};