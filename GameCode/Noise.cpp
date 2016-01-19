#include "Noise.hpp"

namespace Henry
{
//---------------------------------------------------------------------------
// Computes a random Perlin noise value based on a 2D input <position> and
//	Perlin noise parameters.  Recursive (for additional octaves).
//
//	<perlinNoiseGridCellSize>: Noise density.  Larger values produce longer
//		wavelength noise (e.g. gentle, sweeping hills).
//	<numOctaves>: 0 is flat, 1 is simple smoothed noise. Values of 2+ add one
//		or more additional "octave" harmonics.  Each additional octave has
//		double the frequency/density but only a fraction of the amplitude of
//		the base noise.
//	<baseAmplitude>: The minimum (-amplitude) and maximum (+amplitude) values
//		produced by the first octave of the noise.  Note that adding
//		additional octaves can push the final total Perlin noise values above
//		or below the maximum base amplitude; the noise can be "normalized" by
//		the caller (omitted from this function for optimization purposes) via:
//				noise *= A / (A + (A * P))
//		...where A is the <baseAmplitude> and P is the <persistance>.
//	<persistance>: The fraction of amplitude of each subsequent octave, based on the amplitude of the previous octave.  For
//		example, with a persistance of 0.3, each octave is only 30% as strong as the previous octave.
//
float ComputePerlinNoiseValueAtPosition2D( const Vector2& position, float perlinNoiseGridCellSize, int numOctaves, float baseAmplitude, float persistance )
{
	if( numOctaves == 0 )
		return 0.f;

	Vector2 perlinPosition = Vector2(position.x / perlinNoiseGridCellSize,position.y / perlinNoiseGridCellSize);
	Vector2 perlinPositionFloor( floor( perlinPosition.x ), floor( perlinPosition.y ) );
	IntVector2 perlinCell( (int) perlinPositionFloor.x, (int) perlinPositionFloor.y );
	Vector2 perlinPositionUV = perlinPosition - perlinPositionFloor;
	Vector2 perlinPositionAntiUV( perlinPositionUV.x - 1.f, perlinPositionUV.y - 1.f );
	float eastWeight = SmoothStep( perlinPositionUV.x );
	float northWeight = SmoothStep( perlinPositionUV.y );
	float westWeight = 1.f - eastWeight;
	float southWeight = 1.f - northWeight;

	Vector2 southwestNoiseGradient = GetPseudoRandomNoiseUnitVector2D( perlinCell.x, perlinCell.y );
	Vector2 southeastNoiseGradient = GetPseudoRandomNoiseUnitVector2D( perlinCell.x + 1, perlinCell.y );
	Vector2 northeastNoiseGradient = GetPseudoRandomNoiseUnitVector2D( perlinCell.x + 1, perlinCell.y + 1 );
	Vector2 northwestNoiseGradient = GetPseudoRandomNoiseUnitVector2D( perlinCell.x, perlinCell.y + 1 );

	float southwestDot = DotProduct( southwestNoiseGradient, perlinPositionUV );
	float southeastDot = DotProduct( southeastNoiseGradient, Vector2( perlinPositionAntiUV.x, perlinPositionUV.y ) );
	float northeastDot = DotProduct( northeastNoiseGradient, perlinPositionAntiUV );
	float northwestDot = DotProduct( northwestNoiseGradient, Vector2( perlinPositionUV.x, perlinPositionAntiUV.y ) );

	float southBlend = (eastWeight * southeastDot) + (westWeight * southwestDot);
	float northBlend = (eastWeight * northeastDot) + (westWeight * northwestDot);
	float fourWayBlend = (southWeight * southBlend) + (northWeight * northBlend);
	float perlinNoiseAtThisOctave = baseAmplitude * fourWayBlend;

	// TODO: Rewrite this to be iterative, rather than recursive.  It's faster, and will port to shader code more easily.
	float perlinNoiseFromAllHigherOctaves = ComputePerlinNoiseValueAtPosition2D( position,
		0.5f * perlinNoiseGridCellSize, numOctaves - 1, baseAmplitude * persistance, persistance );

	float totalPerlinNoise = perlinNoiseAtThisOctave + perlinNoiseFromAllHigherOctaves;
	return totalPerlinNoise;
}

}