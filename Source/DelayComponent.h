/*
  ==============================================================================

    DelayComponent.h
    Created: 27 Feb 2019 12:02:33am
    Author:  frot

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"
#include "Knob.h"
#include "OdinButton.h"
#include "SyncTimeSelector.h"
#include "OdinControlAttachments.h"


#define TIME_POS_X 17
#define TIME_POS_Y 21
#define FEEDBACK_POS_X 83
#define FEEDBACK_POS_Y 51
#define DELAY_HP_POS_X 141
#define DELAY_HP_POS_Y 17
#define DUCKING_POS_X 180
#define DUCKING_POS_Y DELAY_HP_POS_Y
#define DRY_POS_X 160
#define DRY_POS_Y 64
#define WET_POS_X 200
#define WET_POS_Y DRY_POS_Y
#define DELAY_SYNC_POS_X 25
#define DELAY_SYNC_POS_Y 78
#define DELAY_PINGPONG_POS_X 85
#define DELAY_PINGPONG_POS_Y 10
#define SYNC_TIME_DELAY_POS_X 2
#define SYNC_TIME_DELAY_POS_Y 36

#define HP_FREQ_MIN 80
#define HP_FREQ_MAX 18000
#define HP_FREQ_MID 1280
#define HP_FREQ_DEFAULT HP_FREQ_MIN
#define DRY_WET_MID 0.3
#define DRY_DEFAULT 1
#define WET_DEFAULT 0.8
#define FEEDBACK_MID 0.3
#define TIME_MAX 3
#define TIME_MIN 0.01
#define TIME_MID 1
#define TIME_DEFAULT 1

//#define SYNC_TIME_DELAY_POS_X 17
//#define SYNC_TIME_DELAY_POS_Y 10

//==============================================================================
/*
*/
class DelayComponent : public Component {
public:
	DelayComponent(AudioProcessorValueTreeState &vts, bool p_is_standalone);
	~DelayComponent();

	void forceValueTreeOntoComponents(ValueTree p_tree);

	void paint(Graphics &) override;

	void setImage(juce::Image p_background) {
		m_background = p_background;
	}

	void setSync(bool p_sync_on) {
		if (p_sync_on != m_sync_on) {
			m_sync_on = p_sync_on;
			if (p_sync_on) {
				m_time.setVisible(false);
				m_sync_time.setVisible(true);
			} else {
				m_time.setVisible(true);
				m_sync_time.setVisible(false);
			}
			repaint();
		}
	}

private:
	bool m_is_standalone_plugin;
	bool m_sync_on = false;

	juce::Image m_background;

	Knob m_time;
	Knob m_feedback;
	Knob m_HP;
	Knob m_ducking;
	Knob m_dry;
	Knob m_wet;

	SyncTimeSelector m_sync_time;

	OdinButton m_sync;
	OdinButton m_pingpong;

	AudioProcessorValueTreeState &m_value_tree;

	std::unique_ptr<OdinKnobAttachment> m_delay_time_attach;
	std::unique_ptr<OdinKnobAttachment> m_delay_feedback_attach;
	std::unique_ptr<OdinKnobAttachment> m_delay_hp_attach;
	std::unique_ptr<OdinKnobAttachment> m_delay_ducking_attach;
	std::unique_ptr<OdinKnobAttachment> m_delay_dry_attach;
	std::unique_ptr<OdinKnobAttachment> m_delay_wet_attach;

	std::unique_ptr<OdinButtonAttachment> m_sync_attach;
	std::unique_ptr<OdinButtonAttachment> m_pingpong_attach;

	Identifier m_delay_synctime_numerator_identifier;
	Identifier m_delay_synctime_denominator_identifier;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayComponent)
};
