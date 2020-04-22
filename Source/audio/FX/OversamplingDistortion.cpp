#include "OversamplingDistortion.h"
#include "../../GlobalIncludes.h"
#include <cmath>

OversamplingDistortion::OversamplingDistortion() {
}

OversamplingDistortion::~OversamplingDistortion() {
}

double OversamplingDistortion::doDistortion(double p_input) {

	// do linear interpolation
	double input_upsampled[3] = {
	    0.66666666 * m_last_input + 0.33333333 * p_input, 0.33333333 * m_last_input + 0.66666666 * p_input, p_input};

	m_last_input = p_input;

	m_threshold_smooth =
	    m_threshold_smooth * THRESHOLD_SMOOTHIN_FACTOR + (1 - THRESHOLD_SMOOTHIN_FACTOR) * (m_threshold);

	//theshold is now boost, so we need to subtract mod (control setter is inverted as well)
	float threshold_modded = (m_threshold_smooth - *m_threshold_mod) * (1.f - THRESHOLD_MIN) + THRESHOLD_MIN;
	threshold_modded       = threshold_modded > 1 ? 1 : threshold_modded;
	threshold_modded       = threshold_modded < THRESHOLD_MIN ? THRESHOLD_MIN : threshold_modded;

	// do distortion
	switch (m_algorithm) {
	case Clamp:
		for (int sample = 0; sample < 3; ++sample) {
			if (input_upsampled[sample] > m_bias && input_upsampled[sample] > m_bias + threshold_modded) {
				input_upsampled[sample] = m_bias + threshold_modded;
			} else if (input_upsampled[sample] < m_bias && input_upsampled[sample] < m_bias - threshold_modded) {
				input_upsampled[sample] = m_bias - threshold_modded;
			}
		}
		break;
	case Zero:
		// half "boost" for zero
		threshold_modded = 0.5f + threshold_modded * 0.5f;
		for (int sample = 0; sample < 3; ++sample) {
			if (input_upsampled[sample] > m_bias && input_upsampled[sample] > m_bias + threshold_modded) {
				input_upsampled[sample] = 0.;
			} else if (input_upsampled[sample] < m_bias && input_upsampled[sample] < m_bias - threshold_modded) {
				input_upsampled[sample] = 0.;
			}
		}
		break;
	case Sine:
		for (int sample = 0; sample < 3; ++sample) {
			input_upsampled[sample] = sin(input_upsampled[sample] /*/ threshold_modded*/);
		}
	case Cube:
		for (int sample = 0; sample < 3; ++sample) {
			//input_upsampled[sample] /= threshold_modded;
			input_upsampled[sample] *= input_upsampled[sample] * input_upsampled[sample];
		}
		break;
	case Fold:
		for (int sample = 0; sample < 3; ++sample) {
			//threshold_modded = threshold_modded < 0.05 ? 0.05 : threshold_modded;
			while (fabs(input_upsampled[sample]) > threshold_modded) {
				if (input_upsampled[sample] > threshold_modded) {
					input_upsampled[sample] = 2 * threshold_modded - input_upsampled[sample];
				} else {
					input_upsampled[sample] = -2 * threshold_modded - input_upsampled[sample];
				}
			}
		}
		break;
	}

	// do downsamplingfilter: The filter coefficients are generated by
	// http://www-users.cs.york.ac.uk/~fisher/cgi-bin/mkfscript Butterworth, LP,
	// order 9, SR 132300, CornerFreq1 40000
	xv[0] = xv[1];
	xv[1] = xv[2];
	xv[2] = xv[3];
	xv[3] = xv[4];
	xv[4] = xv[5];
	xv[5] = xv[6];
	xv[6] = xv[7];
	xv[7] = xv[8];
	xv[8] = xv[9];
	xv[9] = input_upsampled[0] * 0.019966841051093;
	yv[0] = yv[1];
	yv[1] = yv[2];
	yv[2] = yv[3];
	yv[3] = yv[4];
	yv[4] = yv[5];
	yv[5] = yv[6];
	yv[6] = yv[7];
	yv[7] = yv[8];
	yv[8] = yv[9];
	yv[9] = (xv[0] + xv[9]) + 9 * (xv[1] + xv[8]) + 36 * (xv[2] + xv[7]) + 84 * (xv[3] + xv[6]) +
	        126 * (xv[4] + xv[5]) + (-0.0003977153 * yv[0]) + (-0.0064474617 * yv[1]) + (-0.0476997403 * yv[2]) +
	        (-0.2185829743 * yv[3]) + (-0.6649234123 * yv[4]) + (-1.4773657709 * yv[5]) + (-2.2721421641 * yv[6]) +
	        (-2.6598673212 * yv[7]) + (-1.8755960587 * yv[8]);
	// next output value = yv[9];
	xv[0] = xv[1];
	xv[1] = xv[2];
	xv[2] = xv[3];
	xv[3] = xv[4];
	xv[4] = xv[5];
	xv[5] = xv[6];
	xv[6] = xv[7];
	xv[7] = xv[8];
	xv[8] = xv[9];
	xv[9] = input_upsampled[1] * 0.019966841051093;
	yv[0] = yv[1];
	yv[1] = yv[2];
	yv[2] = yv[3];
	yv[3] = yv[4];
	yv[4] = yv[5];
	yv[5] = yv[6];
	yv[6] = yv[7];
	yv[7] = yv[8];
	yv[8] = yv[9];
	yv[9] = (xv[0] + xv[9]) + 9 * (xv[1] + xv[8]) + 36 * (xv[2] + xv[7]) + 84 * (xv[3] + xv[6]) +
	        126 * (xv[4] + xv[5]) + (-0.0003977153 * yv[0]) + (-0.0064474617 * yv[1]) + (-0.0476997403 * yv[2]) +
	        (-0.2185829743 * yv[3]) + (-0.6649234123 * yv[4]) + (-1.4773657709 * yv[5]) + (-2.2721421641 * yv[6]) +
	        (-2.6598673212 * yv[7]) + (-1.8755960587 * yv[8]);
	// next output value = yv[9];
	xv[0] = xv[1];
	xv[1] = xv[2];
	xv[2] = xv[3];
	xv[3] = xv[4];
	xv[4] = xv[5];
	xv[5] = xv[6];
	xv[6] = xv[7];
	xv[7] = xv[8];
	xv[8] = xv[9];
	xv[9] = input_upsampled[2] * 0.019966841051093;
	yv[0] = yv[1];
	yv[1] = yv[2];
	yv[2] = yv[3];
	yv[3] = yv[4];
	yv[4] = yv[5];
	yv[5] = yv[6];
	yv[6] = yv[7];
	yv[7] = yv[8];
	yv[8] = yv[9];
	yv[9] = (xv[0] + xv[9]) + 9 * (xv[1] + xv[8]) + 36 * (xv[2] + xv[7]) + 84 * (xv[3] + xv[6]) +
	        126 * (xv[4] + xv[5]) + (-0.0003977153 * yv[0]) + (-0.0064474617 * yv[1]) + (-0.0476997403 * yv[2]) +
	        (-0.2185829743 * yv[3]) + (-0.6649234123 * yv[4]) + (-1.4773657709 * yv[5]) + (-2.2721421641 * yv[6]) +
	        (-2.6598673212 * yv[7]) + (-1.8755960587 * yv[8]);

	float drywet_modded = m_drywet + *m_drywet_mod;
	drywet_modded       = drywet_modded > 1 ? 1 : drywet_modded;
	drywet_modded       = drywet_modded < 0 ? 0 : drywet_modded;

	// return only the last of the three samples
	switch (m_algorithm) {
	case Clamp:
	case Fold:
	case Zero:
		return yv[9] * drywet_modded / threshold_modded * DISTORTION_OUTPUT_SCALAR + p_input * (1.f - drywet_modded);
	case Sine:
	case Cube:
		return yv[9] * drywet_modded + p_input * (1.f - drywet_modded);
	default:
		return p_input;
		break;
	}
}
