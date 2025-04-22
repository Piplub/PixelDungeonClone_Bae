#pragma once
#pragma comment(lib, "Winmm.lib")

#include <Windows.h>
#include <string>
#include <iostream>
#include <bitset>
#include <map>
#include <vector>
#include <random>
#include <queue>
#include <functional>

using namespace std;

#include "KeyManager.h"
#include "MouseManager.h"
// #include "ImageManager.h"
#include "TimerManager.h"
#include "SceneManager.h"

/*
	컴파일러에서 해당 코드를 뒤에 정의된 코드로 변경한다.
*/
#define WINSIZE_X	1080
#define WINSIZE_Y	720
#define TILEMAPTOOL_X	1420
#define TILEMAPTOOL_Y	700

#define DEG_TO_RAD(degree) ((3.14 / 180.0) * degree)
#define RAD_TO_DEG(radian) ((180.0 / 3.14) * radian)

typedef struct tagFPOINT
{
	float x;
	float y;

	tagFPOINT operator+(const tagFPOINT& other) {
		return { x + other.x , y + other.y };
	}
	tagFPOINT operator-(const tagFPOINT& other) {
		return { x - other.x , y - other.y };
	}
	void operator=(const tagFPOINT& other) {
		x = other.x;
		y = other.y;
	}
	void operator+=(const tagFPOINT& other) {
		x += other.x;
		y += other.y;
	}
	void operator-=(const tagFPOINT& other) {
		x -= other.x;
		y -= other.y;
	}
	bool operator==(const tagFPOINT& other) {
		return x == other.x && y == other.y;
	}
	bool operator!=(const tagFPOINT& other) {
		return x != other.x || y != other.y;
	}
	bool operator<(const tagFPOINT& other) const
	{
		if (x != other.x)
			return x < other.x;

		return y < other.y;
	}
	bool operator>(const tagFPOINT& other) const
	{
		if (x != other.x)
			return x > other.x;

		return y > other.y;
	}

	float LengthSquared()
	{
		return x * x + y * y;
	}

	float Length()
	{
		return ::sqrt(LengthSquared());
	}

	void Normalize()
	{
		float length = Length();
		if (length < 0.00000000001f)
			return;

		x /= length;
		y /= length;
	}

	float Dot(tagFPOINT other)
	{
		return x * other.x + y * other.y;
	}

} FPOINT;

/*
	extern 키워드 : 변수나 함수가 다른 파일에 정의되어 있다 라는
	사실을 알리는 키워드.
*/
extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;

// 샘플 타일 정보
// 640 * 288
#define SAMPLE_TILE_X	20
#define SAMPLE_TILE_Y	9
#define TILE_SIZE	90

// 메인 그리기 공간 정보
#define TILE_X	20
#define TILE_Y	20

typedef struct tagTile
{
	RECT rc;
	int frameX;
	int frameY;
} TILE_INFO;

#pragma once
typedef enum class TileType {
	NONE,
	WALL,
	FLOOR,
	COUNT
}TT;


struct Map {
	TT type;
	RECT rc;
	FPOINT center;

	bool CanGo() const
	{
		return type == TT::FLOOR || type == TT::NONE;
	}
};

struct TileData {
	int frameX;
	int frameY;

	TT type;
};