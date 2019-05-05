/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ModMatrix.h"
#include "OdinTreeListener.h"
#include "Voice.h"
#include "audio/Amplifier.h"
#include "audio/FX/Chorus.h"
#include "audio/FX/Delay.h"
#include "audio/FX/Flanger.h"
#include "audio/FX/OversamplingDistortion.h"
#include "audio/FX/Phaser.h"
#include "audio/Filters/CombFilter.h"
#include "audio/Filters/DiodeFilter.h"
#include "audio/Filters/FormantFilter.h"
#include "audio/Filters/Korg35Filter.h"
#include "audio/Filters/LadderFilter.h"
#include "audio/Filters/SEMFilter12.h"
#include "audio/Filters/SEMFilter24.h"
#include "audio/Oscillators/WavetableContainer.h"

#include "DrawableSlider.h"
#include "Knob.h"
#include "LeftRightButton.h"
#include "OdinButton.h"

//==============================================================================
/**
 */
class OdinAudioProcessor : public AudioProcessor {
public:
  //==============================================================================
  OdinAudioProcessor();
  ~OdinAudioProcessor();

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

  void processBlock(AudioBuffer<float> &, MidiBuffer &) override;

  //==============================================================================
  AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const String getProgramName(int index) override;
  void changeProgramName(int index, const String &newName) override;

  //==============================================================================
  void getStateInformation(MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;

  void startMidiLearn(Knob *p_knob) {
    DBG("MIDI LEARN WAS SIGNALED");
    if (m_midi_learn_slider) {
      m_midi_learn_slider->stopMidiLearn();
    }
    if (m_midi_learn_knob) {
      m_midi_learn_knob->stopMidiLearn();
    }
    if (m_midi_learn_lrbutton) {
      m_midi_learn_lrbutton->stopMidiLearn();
    }
    if (m_midi_learn_odinbutton) {
      m_midi_learn_odinbutton->stopMidiLearn();
    }
    m_midi_learn_knob = p_knob;
    m_midi_learn_lrbutton_active = false;
    m_midi_learn_slider_active = false;
    m_midi_learn_odinbutton_active = false;
    m_midi_learn_knob_active = true;
  }

  void midiForget(Knob *p_knob) {
    for (auto const &control : m_midi_control_list_knob) {
      if (control.second == p_knob) {
        m_midi_control_list_knob.erase(control.first);
      }
    }
  }

  void startMidiLearn(DrawableSlider *p_slider) {
    DBG("MIDI LEARN WAS SIGNALED");
    if (m_midi_learn_slider) {
      m_midi_learn_slider->stopMidiLearn();
    }
    if (m_midi_learn_knob) {
      m_midi_learn_knob->stopMidiLearn();
    }
    if (m_midi_learn_lrbutton) {
      m_midi_learn_lrbutton->stopMidiLearn();
    }
    if (m_midi_learn_odinbutton) {
      m_midi_learn_odinbutton->stopMidiLearn();
    }
    m_midi_learn_slider = p_slider;
    m_midi_learn_knob_active = false;
    m_midi_learn_lrbutton_active = false;
    m_midi_learn_odinbutton_active = false;
    m_midi_learn_slider_active = true;
  }

  void midiForget(DrawableSlider *p_slider) {
    for (auto const &control : m_midi_control_list_slider) {
      if (control.second == p_slider) {
        m_midi_control_list_slider.erase(control.first);
      }
    }
  }

  void startMidiLearn(LeftRightButton *p_button) {
    DBG("MIDI LEARN WAS SIGNALED");
    if (m_midi_learn_slider) {
      m_midi_learn_slider->stopMidiLearn();
    }
    if (m_midi_learn_knob) {
      m_midi_learn_knob->stopMidiLearn();
    }
    if (m_midi_learn_lrbutton) {
      m_midi_learn_lrbutton->stopMidiLearn();
    }
    if (m_midi_learn_odinbutton) {
      m_midi_learn_odinbutton->stopMidiLearn();
    }
    m_midi_learn_lrbutton = p_button;
    m_midi_learn_knob_active = false;
    m_midi_learn_slider_active = false;
    m_midi_learn_odinbutton_active = false;
    m_midi_learn_lrbutton_active = true;
  }

  void midiForget(LeftRightButton *p_button) {
    for (auto const &control : m_midi_control_list_lrbutton) {
      if (control.second == p_button) {
        m_midi_control_list_lrbutton.erase(control.first);
      }
    }
  }

  void startMidiLearn(OdinButton *p_button) {
    DBG("MIDI LEARN WAS SIGNALED");
    if (m_midi_learn_slider) {
      m_midi_learn_slider->stopMidiLearn();
    }
    if (m_midi_learn_knob) {
      m_midi_learn_knob->stopMidiLearn();
    }
    if (m_midi_learn_lrbutton) {
      m_midi_learn_lrbutton->stopMidiLearn();
    }
    if (m_midi_learn_odinbutton) {
      m_midi_learn_odinbutton->stopMidiLearn();
    }
    m_midi_learn_odinbutton = p_button;
    m_midi_learn_knob_active = false;
    m_midi_learn_slider_active = false;
    m_midi_learn_lrbutton_active = false;
    m_midi_learn_odinbutton_active = true;
  }

  void midiForget(OdinButton *p_button) {
    for (auto const &control : m_midi_control_list_odinbutton) {
      if (control.second == p_button) {
        m_midi_control_list_odinbutton.erase(control.first);
      }
    }
  }

  void stopMidiLearn() {
    m_midi_learn_knob_active = false;
    m_midi_learn_knob = nullptr;
    m_midi_learn_slider_active = false;
    m_midi_learn_slider = nullptr;
    m_midi_learn_lrbutton_active = false;
    m_midi_learn_lrbutton = nullptr;
    m_midi_learn_odinbutton_active = false;
    m_midi_learn_odinbutton = nullptr;
  }

private:
  bool m_midi_learn_knob_active = false;
  Knob *m_midi_learn_knob = nullptr;
  std::multimap<int, Knob *> m_midi_control_list_knob;

  bool m_midi_learn_slider_active = false;
  DrawableSlider *m_midi_learn_slider = nullptr;
  std::multimap<int, DrawableSlider *> m_midi_control_list_slider;

  bool m_midi_learn_lrbutton_active = false;
  LeftRightButton *m_midi_learn_lrbutton = nullptr;
  std::multimap<int, LeftRightButton *> m_midi_control_list_lrbutton;

  bool m_midi_learn_odinbutton_active = false;
  OdinButton *m_midi_learn_odinbutton = nullptr;
  std::multimap<int, OdinButton *> m_midi_control_list_odinbutton;

  float m_osc_vol_smooth[3] = {1.f, 1.f, 1.f};   // factor
  float m_fil_gain_smooth[3] = {1.f, 1.f, 1.f};  // factor
  float m_osc_vol_control[3] = {1.f, 1.f, 1.f};  // factor
  float m_fil_gain_control[3] = {1.f, 1.f, 1.f}; // factor
  float m_fil_freq_control[3] = {2000, 2000, 2000};
  float m_fil_freq_smooth[3] = {2000, 2000, 2000};
  float m_pitch_bend_smooth = 0.f;
  float m_pitch_bend_smooth_and_applied = 0.f;
  float m_modwheel_smooth = 0.f;
  float m_x_smooth = 0.f;
  float m_y_smooth = 0.f;
  float m_master_smooth = 1.f;
  float m_master_control = 1.f; // factor

  int m_last_midi_note = -1;

  void setSampleRate(float p_samplerate);
  void initializeModules();

  VoiceManager m_voice_manager;
  AudioProcessorValueTreeState m_parameters;
  OdinTreeListener m_tree_listener;

  Voice m_voice[VOICES];
  Amplifier m_amp;
  OversamplingDistortion m_distortion[2];

  LadderFilter m_ladder_filter[2];
  // SEMFilter24 m_SEM_filter_24[2];
  SEMFilter12 m_SEM_filter_12[2];
  Korg35Filter m_korg_filter[2];
  DiodeFilter m_diode_filter[2];
  FormantFilter m_formant_filter[2];
  CombFilter m_comb_filter[2];
  Delay m_delay[2];
  Phaser m_phaser[2];
  Flanger m_flanger[2];
  Chorus m_chorus[2];

  ModMatrix m_mod_matrix;
  ModSources m_mod_sources;
  ModDestinations m_mod_destinations;

  void setPitchWheelValue(int p_value);

  void setModulationPointers();
  void treeValueChanged(const String &p_ID, float p_new_value);

  // MOD SOURCES
  float m_adsr[VOICES][4] = {0.f};
  float m_lfo[VOICES][4] = {0.f};
  float m_filter_output[VOICES][2] = {0.f};
  float m_osc_output[VOICES][3] = {0.f};
  float m_MIDI_aftertouch = 0.f;
  float m_constant = 1.f;
  // float m_modwheel = 0.f;
  // float m_pitchwheel = 0.f;
  float m_x = 0.f;
  float m_y = 0.f;
  // MOD DEST
  float *m_master_mod;

  int m_counter = 0; // todo remove
#include "AudioVarDeclarations.h"
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OdinAudioProcessor)
};
