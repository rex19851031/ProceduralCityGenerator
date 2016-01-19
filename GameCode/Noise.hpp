#include <cmath>

#include "Engine\Math\Vec2.hpp"

#ifndef included_Noise
#define included_Noise
#pragma once

namespace Henry
{

//---------------------------------------------------------------------------
typedef Vec2f Vector2;
typedef Vec2i IntVector2;

#define TWO_PI 6.283185f

//---------------------------------------------------------------------------
float GetPseudoRandomNoiseValueNegOneToOne1D( int position );
float GetPseudoRandomNoiseValueZeroToOne1D( int position );
float GetPseudoRandomNoiseValueNegOneToOne2D( int positionX, int positionY );
float GetPseudoRandomNoiseValueZeroToOne2D( int positionX, int positionY );
float GetPseudoRandomNoiseValueNegOneToOne3D( int positionX, int positionY, int positionZ );
float GetPseudoRandomNoiseValueZeroToOne3D( int positionX, int positionY, int positionZ );
float GetPseudoNoiseAngleRadians1D( int position );
float GetPseudoNoiseAngleRadians2D( int positionX, int positionY );
Vector2 GetPseudoRandomNoiseUnitVector2D( int positionX, int positionY );
float DotProduct(const Vector2 u,const Vector2 v);
float ComputePerlinNoiseValueAtPosition2D( const Vector2& position, float perlinNoiseGridCellSize, int numOctaves, float baseAmplitude, float persistance );
float SmoothStep(float perlinPositionUV);

//---------------------------------------------------------------------------
inline float GetPseudoRandomNoiseValueNegOneToOne1D( int position )
{
	const float ONE_OVER_HALF_MAX_POSITIVE_INT = (1.f / 1073741824.f);
	int bits = (position << 13) ^ position;
	int pseudoRandomPositiveInt = (bits * ((bits * bits * 15731) + 789221) + 1376312589) & 0x7fffffff;
	float pseudoRandomFloatNegOneToOne = 1.f - (ONE_OVER_HALF_MAX_POSITIVE_INT * (float) pseudoRandomPositiveInt);
	return pseudoRandomFloatNegOneToOne;
}


//---------------------------------------------------------------------------
inline float GetPseudoRandomNoiseValueZeroToOne1D( int position )
{
	const float ONE_OVER_MAX_POSITIVE_INT = (1.f / 2147483648.f);
	int bits = (position << 13) ^ position;
	int pseudoRandomPositiveInt = (bits * ((bits * bits * 15731) + 789221) + 1376312589) & 0x7fffffff;
	float pseudoRandomFloatZeroToOne = ONE_OVER_MAX_POSITIVE_INT * (float) pseudoRandomPositiveInt;
	return pseudoRandomFloatZeroToOne;
}


//---------------------------------------------------------------------------
inline float GetPseudoRandomNoiseValueNegOneToOne2D( int positionX, int positionY )
{
	const float ONE_OVER_HALF_MAX_POSITIVE_INT = (1.f / 1073741824.f);
	int position = positionX + (positionY * 57);
	int bits = (position << 13) ^ position;
	int pseudoRandomPositiveInt = (bits * ((bits * bits * 15731) + 789221) + 1376312589) & 0x7fffffff;
	float pseudoRandomFloatNegOneToOne = 1.f - (ONE_OVER_HALF_MAX_POSITIVE_INT * (float) pseudoRandomPositiveInt);
	return pseudoRandomFloatNegOneToOne;
}


//---------------------------------------------------------------------------
inline float GetPseudoRandomNoiseValueZeroToOne2D( int positionX, int positionY )
{
	const float ONE_OVER_MAX_POSITIVE_INT = (1.f / 2147483648.f);
	int position = positionX + (positionY * 57);
	int bits = (position << 13) ^ position;
	int pseudoRandomPositiveInt = (bits * ((bits * bits * 15731) + 789221) + 1376312589) & 0x7fffffff;
	float pseudoRandomFloatZeroToOne = ONE_OVER_MAX_POSITIVE_INT * (float) pseudoRandomPositiveInt;
	return pseudoRandomFloatZeroToOne;
}


//---------------------------------------------------------------------------
inline float GetPseudoRandomNoiseValueNegOneToOne3D( int positionX, int positionY, int positionZ )
{
	const float ONE_OVER_HALF_MAX_POSITIVE_INT = (1.f / 1073741824.f);
	int position = positionX + (positionY * 57) + (positionZ * 3023);
	int bits = (position << 13) ^ position;
	int pseudoRandomPositiveInt = (bits * ((bits * bits * 15731) + 789221) + 1376312589) & 0x7fffffff;
	float pseudoRandomFloatNegOneToOne = 1.f - (ONE_OVER_HALF_MAX_POSITIVE_INT * (float) pseudoRandomPositiveInt);
	return pseudoRandomFloatNegOneToOne;
}


//---------------------------------------------------------------------------
inline float GetPseudoRandomNoiseValueZeroToOne3D( int positionX, int positionY, int positionZ )
{
	const float ONE_OVER_MAX_POSITIVE_INT = (1.f / 2147483648.f);
	int seed = positionX + (positionY * 57) + (positionZ * 3023);
	int bits = (seed << 13) ^ seed;
	int pseudoRandomPositiveInt = (bits * ((bits * bits * 15731) + 789221) + 1376312589) & 0x7fffffff;
	float pseudoRandomFloatZeroToOne = ONE_OVER_MAX_POSITIVE_INT * (float) pseudoRandomPositiveInt;
	return pseudoRandomFloatZeroToOne;
}


//---------------------------------------------------------------------------
inline float GetPseudoNoiseAngleRadians1D( int position )
{
	const float ONE_OVER_MAX_POSITIVE_INT = (1.f / 2147483648.f);
	const float SCALE_FACTOR = ONE_OVER_MAX_POSITIVE_INT * TWO_PI;
	int bits = (position << 13) ^ position;
	int pseudoRandomPositiveInt = (bits * ((bits * bits * 15731) + 789221) + 1376312589) & 0x7fffffff;
	float pseudoRandomFloatZeroToTwoPi = SCALE_FACTOR * (float) pseudoRandomPositiveInt;
	return pseudoRandomFloatZeroToTwoPi;
}


//---------------------------------------------------------------------------
inline float GetPseudoNoiseAngleRadians2D( int positionX, int positionY )
{
	const float ONE_OVER_MAX_POSITIVE_INT = (1.f / 2147483648.f);
	const float SCALE_FACTOR = ONE_OVER_MAX_POSITIVE_INT * TWO_PI;
	int position = positionX + (positionY * 57);
	int bits = (position << 13) ^ position;
	int pseudoRandomPositiveInt = (bits * ((bits * bits * 15731) + 789221) + 1376312589) & 0x7fffffff;
	float pseudoRandomFloatZeroToTwoPi = SCALE_FACTOR * (float) pseudoRandomPositiveInt;
	return pseudoRandomFloatZeroToTwoPi;
}


//---------------------------------------------------------------------------
inline Vector2 GetPseudoRandomNoiseUnitVector2D( int xPosition, int yPosition )
{
	const float ONE_OVER_MAX_POSITIVE_INT = (1.f / 2147483648.f);
	const float SCALE_FACTOR = ONE_OVER_MAX_POSITIVE_INT * TWO_PI;
	int position = xPosition + (yPosition * 57);
	int bits = (position << 13) ^ position;
	int pseudoRandomPositiveInt = (bits * ((bits * bits * 15731) + 789221) + 1376312589) & 0x7fffffff;
	float pseudoRandomFloatZeroToTwoPi = SCALE_FACTOR * (float) pseudoRandomPositiveInt;

	// TODO: Rewrite this to use the randomized int to look up Vector2 from a cos/sin table; vectors don't need to be super-precise,
	//	and we certainly don't want to pay full price for cos/sin if this is merely going to be used for, say, Perlin noise gradiants.
	//	Note however that cos/sin are typically pretty fast on GPUs so this can probably stand as-is in shader code.
	return Vector2( cos( pseudoRandomFloatZeroToTwoPi ), sin( pseudoRandomFloatZeroToTwoPi ) );
}


//---------------------------------------------------------------------------
inline float DotProduct(const Vector2 u,const Vector2 v)
{
	float result = u.x * v.x + u.y * v.y;
	return result;
}



inline float SmoothStep(float perlinPositionUV)
{
	return perlinPositionUV * perlinPositionUV * (3 - 2 * perlinPositionUV);
}
#endif // included_Noise

}