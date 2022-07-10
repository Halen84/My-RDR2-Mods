#pragma once

#include <windows.h>
#include <cstdint>

typedef uint64_t Void;
typedef uint64_t Any;
typedef unsigned int uint;
typedef unsigned int Hash;

typedef int AnimScene;
typedef int Blip;
typedef int Cam;
typedef int CoverPoint;
typedef int Entity;
typedef int FireId;
typedef int Group;
typedef int Interior;
typedef int ItemSet;
typedef Hash Model;
typedef int Object;
typedef int Ped;
typedef int PersChar;
typedef int Pickup;
typedef uint Player;
typedef int PopZone;
typedef int Prompt;
typedef int PropSet;
typedef int Rope;
typedef int Scenario;
typedef int ScrHandle;
typedef int Sphere;
typedef int TaskSequence;
typedef int Train;
typedef int Vehicle;
typedef int Volume;
typedef Hash Weapon;

#define ALIGN8 __declspec(align(8))

struct Vector3
{
	ALIGN8 float x;
	ALIGN8 float y;
	ALIGN8 float z;

	Vector3 operator =(Vector3 xyz) { return Vector3(x = xyz.x, y = xyz.y, z = xyz.z); }
	Vector3 operator +(Vector3 xyz) { return Vector3(x + xyz.x, y + xyz.y, z + xyz.z); }
	Vector3 operator +=(Vector3 xyz) { return Vector3(x += xyz.x, y += xyz.y, z += xyz.z); }
	Vector3 operator -(Vector3 xyz) { return Vector3(x - xyz.x, y - xyz.y, z - xyz.z); }
	Vector3 operator -=(Vector3 xyz) { return Vector3(x -= xyz.x, y -= xyz.y, z -= xyz.z); }
	Vector3 operator *(Vector3 xyz) { return Vector3(x * xyz.x, y * xyz.y, z * xyz.z); }
	Vector3 operator *=(Vector3 xyz) { return Vector3(x *= xyz.x, y *= xyz.y, z *= xyz.z); }
	Vector3 operator /(Vector3 xyz) { return Vector3(x / xyz.x, y / xyz.y, z / xyz.z); }
	Vector3 operator /=(Vector3 xyz) { return Vector3(x /= xyz.x, y /= xyz.y, z /= xyz.z); }

	bool operator ==(Vector3 xyz) { return (x == xyz.x && y == xyz.y && z == xyz.z); }

	Vector3 operator +(float right) { return Vector3(x + right, y + right, z + right); }
	Vector3 operator -(float right) { return Vector3(x - right, y - right, z - right); }
	Vector3 operator /(float right) { return Vector3(x / right, y / right, z / right); }
	Vector3 operator *(float right) { return Vector3(x * right, y * right, z * right); }
	Vector3 operator +=(float right) { return Vector3(x += right, y += right, z += right); }
	Vector3 operator -=(float right) { return Vector3(x -= right, y -= right, z -= right); }
	Vector3 operator *=(float right) { return Vector3(x *= right, y *= right, z *= right); }
	Vector3 operator /=(float right) { return Vector3(x /= right, y /= right, z /= right); }
};

static_assert(sizeof(Vector3) == 24, "");
