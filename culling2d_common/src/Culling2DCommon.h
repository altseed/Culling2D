#pragma once

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <array>
#include <algorithm>
#include <functional>
#include <atomic>

#include <memory>
#include <chrono>
#include <thread>

#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#else
#endif

namespace culling2d
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	最大値取得
	*/
	template <typename T, typename U>
	T Max(T t, U u)
	{
		if (t > (T)u)
		{
			return t;
		}
		return u;
	}

	/**
	@brief	最小値取得
	*/
	template <typename T, typename U>
	T Min(T t, U u)
	{
		if (t < (T)u)
		{
			return t;
		}
		return u;
	}

	/**
	@brief	範囲内値取得
	*/
	template <typename T, typename U, typename V>
	T Clamp(T t, U max_, V min_)
	{
		if (t > (T)max_)
		{
			t = (T)max_;
		}

		if (t < (T)min_)
		{
			t = (T)min_;
		}

		return t;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	inline float NormalizeAngle(float angle)
	{
		int32_t ofs = (*(int32_t*)&angle & 0x80000000) | 0x3F000000;
		return (angle - ((int)(angle * 0.159154943f + *(float*)&ofs) * 6.283185307f));
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	inline void SinCos(float x, float& s, float& c)
	{
		x = NormalizeAngle(x);
		float x2 = x * x;
		float x4 = x * x * x * x;
		float x6 = x * x * x * x * x * x;
		float x8 = x * x * x * x * x * x * x * x;
		float x10 = x * x * x * x * x * x * x * x * x * x;
		s = x * (1.0f - x2 / 6.0f + x4 / 120.0f - x6 / 5040.0f + x8 / 362880.0f - x10 / 39916800.0f);
		c = 1.0f - x2 / 2.0f + x4 / 24.0f - x6 / 720.0f + x8 / 40320.0f - x10 / 3628800.0f;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	inline void Sleep(int32_t ms)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	template <class T>
	void SafeAddRef(T& t)
	{
		if (t != NULL)
		{
			t->AddRef();
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	template <class T>
	void SafeRelease(T& t)
	{
		if (t != NULL)
		{
			t->Release();
			t = NULL;
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	template <class T>
	void SafeSubstitute(T& target, T& value)
	{
		SafeAddRef(value);
		SafeRelease(target);
		target = value;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	template <typename T>
	inline void SafeDelete(T*& p)
	{
		if (p != NULL)
		{
			delete (p);
			(p) = NULL;
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	template <typename T>
	inline void SafeDeleteArray(T*& p)
	{
		if (p != NULL)
		{
			delete[](p);
			(p) = NULL;
		}
	}


	const float PI = 3.14159265358979f;

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	static float DegreeToRadian(float degree)
	{
		return degree / 180.0f * PI;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	static float RadianToDegree(float radian)
	{
		return radian / PI * 180.0f;
	}

	/**
	@brief	参照カウンタオブジェクト
	*/
	class ReferenceObject
	{
	private:
		mutable std::atomic<int32_t> m_reference;

	public:
		ReferenceObject();

		virtual ~ReferenceObject();

		virtual int AddRef();

		virtual int GetRef();

		virtual int Release();
	};

	/**
	@brief	2次元ベクトル
	*/
	struct Vector2DF
	{
	public:
		/**
		@brief	X
		*/
		float	X;

		/**
		@brief	Y
		*/
		float	Y;

		/**
		@brief	コンストラクタ
		*/
		Vector2DF();

		/**
		@brief	コンストラクタ
		@param	x	X
		@param	y	Y
		*/
		Vector2DF(float x, float y);

		/**
		@brief	このベクトルの長さを取得する。
		*/
		float GetLength() const
		{
			return sqrt(GetSquaredLength());
		}

		/**
		@brief	このベクトルの長さの二乗を取得する。
		*/
		float GetSquaredLength() const
		{
			return X * X + Y * Y;
		}

		/**
		@brief	このベクトルの長さを設定する。
		*/
		void SetLength(float value)
		{
			float length = GetLength();
			(*this) *= (value / length);
		}

		/**
		@brief	このベクトルの単位ベクトルを取得する。
		*/
		Vector2DF GetNormal()
		{
			float length = GetLength();
			return Vector2DF(X / length, Y / length);
		}

		/**
		@brief	このベクトルの単位ベクトル化する。
		*/
		void Normalize()
		{
			float length = GetLength();
			(*this) /= length;
		}

		/**
		@brief	このベクトルの向きを弧度法で取得する。
		*/
		float GetRadian() const
		{
			return atan2(Y, X);
		}

		/**
		@brief	このベクトルの向きを弧度法で設定する。
		*/
		void SetRadian(float value)
		{
			float length = GetLength();
			SinCos(value, Y, X);
			X *= length;
			Y *= length;
		}

		/**
		@brief	このベクトルの向きを度数法で取得する。
		*/
		float GetDegree() const
		{
			return RadianToDegree(atan2(Y, X));
		}

		/**
		@brief	このベクトルの向きを度数法で設定する。
		*/
		void SetDegree(float value)
		{
			float length = GetLength();
			SinCos(DegreeToRadian(value), Y, X);
			X *= length;
			Y *= length;
		}

		bool operator==(const Vector2DF& right);

		bool operator!=(const Vector2DF& right);

		Vector2DF operator-();

		Vector2DF operator+(const Vector2DF& right);

		Vector2DF operator-(const Vector2DF& right);

		Vector2DF operator*(const Vector2DF& right);

		Vector2DF operator/(const Vector2DF& right);

		Vector2DF operator*(float right);

		Vector2DF operator/(float right);

		Vector2DF& operator+=(const Vector2DF& right);

		Vector2DF& operator-=(const Vector2DF& right);

		Vector2DF& operator*=(const Vector2DF& right);

		Vector2DF& operator/=(const Vector2DF& right);

		Vector2DF& operator*=(float right);

		Vector2DF& operator/=(float right);

		/**
		@brief	内積を取得する。
		@param	v1	値1
		@param	v2	値2
		*/
		static float Dot(const Vector2DF& v1, const Vector2DF& v2)
		{
			return v1.X * v2.X + v1.Y * v2.Y;
		}

		/**
		@brief	2点間の距離を取得する。
		@param	v1	値1
		@param	v2	値2
		*/
		static float Distance(const Vector2DF& v1, const Vector2DF& v2)
		{
			float dx = v1.X - v2.X;
			float dy = v1.Y - v2.Y;
			return sqrt(dx * dx + dy * dy);
		}
	};

	/**
	@brief	2次元ベクトル(整数)
	*/
	struct Vector2DI
	{
	public:
		/**
		@brief	X
		*/
		int32_t	X;

		/**
		@brief	Y
		*/
		int32_t	Y;

		/**
		@brief	コンストラクタ
		*/
		Vector2DI();

		/**
		@brief	コンストラクタ
		*/
		Vector2DI(int32_t x, int32_t y);

		bool operator == (const Vector2DI& o);
		bool operator != (const Vector2DI& o);

		Vector2DI operator-();

		Vector2DI operator+(const Vector2DI& right);

		Vector2DI operator-(const Vector2DI& right);

		Vector2DI operator*(const Vector2DI& right);

		Vector2DI operator/(const Vector2DI& right);

		Vector2DI operator*(int32_t right);

		Vector2DI operator/(int32_t right);

		Vector2DI& operator+=(const Vector2DI& right);

		Vector2DI& operator-=(const Vector2DI& right);

		Vector2DI& operator*=(const Vector2DI& right);

		Vector2DI& operator/=(const Vector2DI& right);

		Vector2DI& operator*=(int32_t right);

		Vector2DI& operator/=(int32_t right);
	};

	struct Circle
	{
		Vector2DF Position;
		float Radius;

		Circle()
		{
			Position = culling2d::Vector2DF(0, 0);
			Radius = 0;
		}

		Circle(Vector2DF position, float radius)
		{
			Position = position;
			Radius = radius;
		}

		bool operator==(Circle circle) {
			return Position == circle.Position&&Radius == circle.Radius;
		}

		bool operator!=(Circle circle)
		{
			return Position != circle.Position || Radius != circle.Radius;
		}
	};


	/**
	@brief	長方形(浮動小数点)の構造体
	*/
	struct RectF
	{
	public:
		/**
		@brief	左上X座標
		*/
		float X;

		/**
		@brief	左上Y座標
		*/
		float Y;

		/**
		@brief	横幅
		*/
		float Width;

		/**
		@brief	縦幅
		*/
		float Height;

		/**
		@brief	コンストラクタ
		*/
		RectF();

		/**
		@brief	コンストラクタ
		@param	x	左上X座標
		@param	y	左上Y座標
		@param	width	横幅
		@param	height	縦幅
		*/
		RectF(float x, float y, float width, float height);

		/**
		@brief	左上座標を取得する。
		@return	左上座標
		*/
		Vector2DF GetPosition() const;

		/**
		@brief	大きさを取得する。
		@return	大きさ
		*/
		Vector2DF GetSize() const;

		/**
		@brief	四隅の座標を(X,Y)、(X+Width,Y)、(X+Width,Y+Height)、(X,Y+Height)の順に配列として取得する。
		@return	座標
		*/
		std::array<Vector2DF, 4> GetVertexes() const;

		/**
		@brief	矩形と衝突しているか否かを調べる。
		@param 矩形
		@return	衝突しているか否か
		*/
		bool GetCollision(RectF rect);

		/**
		@brief	円形と衝突しているか否かを調べる。
		@param 円形
		@return	衝突しているか否か
		*/
		bool GetCollision(Circle circle);

		bool operator==(RectF rectF) const;
		bool operator!=(RectF rectF) const;

		bool GetIsContainingPoint(Vector2DF point) const;
	};


	/**
	@brief	長方形(整数)の構造体
	*/
	struct RectI
	{
	public:
		/**
		@brief	左上X座標
		*/
		int X;

		/**
		@brief	左上Y座標
		*/
		int Y;

		/**
		@brief	横幅
		*/
		int Width;

		/**
		@brief	縦幅
		*/
		int Height;

		/**
		@brief	コンストラクタ
		*/
		RectI();

		/**
		@brief	コンストラクタ
		@param	x	左上X座標
		@param	y	左上Y座標
		@param	width	横幅
		@param	height	縦幅
		*/
		RectI(int x, int y, int width, int height);

		/**
		@brief	左上座標を取得する。
		@return	左上座標
		*/
		Vector2DI GetPosition() const;

		/**
		@brief	大きさを取得する。
		@return	大きさ
		*/
		Vector2DI GetSize() const;

		/**
		@brief	四隅の座標を(X,Y)、(X+Width,Y)、(X+Width,Y+Height)、(X,Y+Height)の順に配列として取得する。
		@return	座標
		*/
		std::array<Vector2DI, 4> GetVertexes() const;

		bool operator == (const RectI& other) const;
	};
}