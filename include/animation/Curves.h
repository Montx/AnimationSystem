#pragma once

template<typename T>
class Bezier {
public:
	T P1;
	T C1;
	T P2;
	T c2;
};

/*template<typename T>
inline T Interpolate(Bezier<T>& curve, float t) {
	T A = curve.P1 * (1.0f - t) + curve.C1 * t;
	T B = curve.c2 * (1.0f - t) + curve.P2 * t;
	T C = curve.C1 * (1.0f - t) + curve.c2 * t;
	T D = A * (1.0f - t) + C * t;
	T E = C * (1.0f - t) + B * t;
	T R = D * (1.0f - t) + E * t;

	return R;
}*/

// Mathmatical Simplification
template<typename T>
inline T Interpolate(Bezier<T>& curve, float t) {
	return 
		curve.P1 * ((1 - t) * (1 - t) * (1 - t)) + 
		curve.C1 * (3.0f * ((1 - t) * (1 - t)) * t) + 
		curve.c2 * (3.0f * (1 - t) * (t * t)) + 
		curve.P2 * (t * t * t);
}

template<typename T>
T Hermite(float t, T& p1, T& s1, T& p2, T& s2) {
	return
		p1 * ((1.0f + 2.0f * t) * ((1.0f - t) * (1.0f - t))) +
		s1 * (t * ((1.0f - t) * (1.0f - t))) +
		p2 * ((t * t) * (3.0f - 2.0f * t)) +
		s2 * ((t * t) * (t - 1.0f));
}