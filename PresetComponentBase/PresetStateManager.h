/*
  ==============================================================================

    PresetStateManager.h
    Created: 1 May 2023 2:26:27pm
    Author:  Mason Self

  ==============================================================================
*/

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
class PresetStateManager
{
public:
    void SaveState(juce::AudioProcessorValueTreeState& stateToAppendTo)
    {
        juce::ValueTree temp
        { name, {},
            {
                { selection , {{valueStringAsID, presetSelection.load()}}},
                { hasBeenModified , {{valueStringAsID, presetHasBeenModified.load()}}}
            }
        };
        
        if (stateToAppendTo.state.getChildWithName(name).isValid())
        {
            stateToAppendTo.state.getChildWithName(name).copyPropertiesAndChildrenFrom(temp, nullptr);
        }
        else
        {
            stateToAppendTo.state.appendChild(temp, nullptr);
        }
        
    }
    void LoadStateAndRemoveFromAPTVS(juce::ValueTree& apvtsTree)
    {
        auto presetStateTree = apvtsTree.getChildWithName(name);
        if (presetStateTree.isValid())
        {
            SetState(presetStateTree);
            RemoveThisTreeFromAPVTS(apvtsTree, presetStateTree);
            readyForGuiUpdate.store(true);
        }
    }

    std::atomic<int> presetSelection { -1 };
    std::atomic<bool> presetHasBeenModified { false };
    
    std::atomic<bool> readyForGuiUpdate{false};
    
private:
    void SetState(juce::ValueTree& presetStateTree)
    {
        presetSelection.store(presetStateTree.getChildWithName(selection).getProperty(valueStringAsID, -1));
        presetHasBeenModified.store(presetStateTree.getChildWithName(hasBeenModified).getProperty(valueStringAsID, -1));
    }
    
    void RemoveThisTreeFromAPVTS(juce::ValueTree& apvtsTree, juce::ValueTree& thisTree)
    {
        apvtsTree.removeChild(thisTree, nullptr);
    }
    
    const juce::Identifier name {"preset_state"};
    const juce::Identifier selection {"selection"};
    const juce::Identifier hasBeenModified {"preset_modified"};
    const juce::Identifier valueStringAsID {"value"};
};
