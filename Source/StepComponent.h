#pragma once

#include "Knob.h"
#include "OdinButton.h"
#include "OdinControlAttachments.h"
#include "StepLED.h"
#include <JuceHeader.h>

#define KNOB_POS_X 1
#define MOD_POS_Y 1
#define TRANSPOSE_POS_Y 43
#define STEP_ON_X 3
#define STEP_ON_Y 89
#define STEP_LED_POS_X 13
#define STEP_LED_POS_Y 121

class StepComponent : public Component {
public:
	StepComponent(AudioProcessorValueTreeState &vts, int p_step_index);
	~StepComponent();

	void paint(Graphics &) override;
	void resized() override;

	void setGUIBig();
	void setGUISmall();

	void setLEDActive(int p_step_index);

private:
	std::unique_ptr<OdinButtonAttachment> m_step_on_attach;
	AudioProcessorValueTreeState &m_value_tree;

	Knob m_mod;
	Knob m_transpose;
	OdinButton m_step_on;
	StepLED m_led;

    int m_step_index;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StepComponent)
};
