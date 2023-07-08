/*
  ==============================================================================

    PresetComponentBase.h
    Created: 30 Apr 2023 8:39:47am
    Author:  Mason Self

  ==============================================================================
*/

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PresetStateManager.h"


class PresetComponentBase : public juce::Component, public juce::Timer, public juce::Value::Listener
{
public:
    PresetComponentBase(juce::StringArray _presets, PresetStateManager& pS);
    void paint(juce::Graphics& g) override;
    void timerCallback() override;
    void valueChanged (juce::Value& v) override;
    void mouseDown (const juce::MouseEvent& e) override;
    
    juce::Value presetSelection {(juce::var)0};
    juce::Value presetHasBeenDeviatedFrom {(juce::var)0}; //TODO: can this just be a bool?
    bool HasBeenInitialized {false};
private:
    
    void ResetValues();
    PresetStateManager& presetState;
    
    juce::PopupMenu menu;
    const juce::Colour backgroundColor {juce::Colours::black}, textColor {juce::Colours::white};
    juce::String currentPreset;
    const juce::StringArray presets;
    const float textSize{20.0f};
};
