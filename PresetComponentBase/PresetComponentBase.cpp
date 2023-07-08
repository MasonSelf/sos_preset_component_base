/*
  ==============================================================================

    PresetComponentBase.cpp
    Created: 30 Apr 2023 8:39:47am
    Author:  Mason Self

  ==============================================================================
*/

#include "PresetComponentBase.h"

PresetComponentBase::PresetComponentBase(juce::StringArray _presets, PresetStateManager& pS)
    :presetState(pS), currentPreset(_presets[0]), presets(_presets)
{
    {
        int index = 1;
        for (auto& p : _presets)
        {
            menu.addItem(index, p);
            ++index;
        }
    }
    presetSelection.addListener(this);
    presetHasBeenDeviatedFrom.addListener(this);
    ResetValues();
    startTimerHz(60);
    HasBeenInitialized = true;
}

void PresetComponentBase::paint(juce::Graphics& g)
{
    auto textOpacity = 0.8f;
    auto backdropOpacity = 0.2f;
    auto borderThickness = 3;
    
    //"Preset" label
    g.setColour(textColor);
    g.setOpacity(textOpacity);
    g.setFont(textSize);
    g.drawFittedText("Preset", borderThickness, borderThickness, getWidth() - borderThickness, getHeight() / 2, juce::Justification::centredLeft, 1);
    
    //backdrop
    g.setColour(backgroundColor);
    g.setOpacity(backdropOpacity);
    g.fillRect(juce::Rectangle<int>(juce::Point<int>(0, getHeight() / 2), juce::Point<int>(getWidth(), getHeight())));
    
    //border
    juce::Path p;
    p.addRectangle (0, 0, getWidth(), getHeight());
    g.strokePath (p, juce::PathStrokeType(static_cast<float>(borderThickness)));
    
    //preset selection text
    g.setColour(textColor);
    g.setOpacity(textOpacity);
    g.setFont(textSize - 2);
    g.drawFittedText(currentPreset, borderThickness, borderThickness + getHeight() / 2, getWidth() - borderThickness, getHeight() / 2, juce::Justification::centredLeft, 1);

}

void PresetComponentBase::timerCallback()
{
    if (presetState.readyForGuiUpdate)
    {
        ResetValues();
        stopTimer();
        presetState.readyForGuiUpdate.store(false);
    }
}

void PresetComponentBase::valueChanged (juce::Value& v)
{
    if (v == presetSelection)
    {
        currentPreset = presets[static_cast<int>(presetSelection.getValue())];
        presetState.presetSelection.store(presetSelection.getValue());
    }
    
    if (v == presetHasBeenDeviatedFrom)
    {
        if ((int)presetHasBeenDeviatedFrom.getValue() == 1)
        {
            currentPreset += "*";
            presetState.presetHasBeenModified.store(true);
        }
        else
        {
            presetState.presetHasBeenModified.store(false);
        }
    }
    repaint();
}

void PresetComponentBase::mouseDown (const juce::MouseEvent& e)
{
    menu.showMenuAsync(juce::PopupMenu::Options(),
                    [this] (int result)
                    {
        if (result != 0)
        {
            presetHasBeenDeviatedFrom.setValue(0);
            if (result - 1 == (int)presetSelection.getValue()) //force update if selection is the same
            {
                if (result - 1 == 0)
                {
                    presetSelection.setValue(1);
                }
                else
                {
                    presetSelection.setValue(0);
                }
                presetSelection.setValue(result - 1);
            }
            else
            {
                presetSelection.setValue(result - 1);
            }
        }
    });
}

void PresetComponentBase::ResetValues()
{
    auto selectionIndex = presetState.presetSelection.load();
    if (static_cast<int>(selectionIndex) >= 0)
    {
        presetSelection.setValue(presetState.presetSelection.load());
    }
    if (presetState.presetHasBeenModified.load() == true)
    {
        presetHasBeenDeviatedFrom.setValue(1);
    }
    else
    {
        presetHasBeenDeviatedFrom.setValue(0);
    }
}
