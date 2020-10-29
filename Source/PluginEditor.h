/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Delay.h"

using namespace juce;
//==============================================================================
/**
*/
class StatikAudioProcessorEditor  : public AudioProcessorEditor, public Slider::Listener
{
public:
    StatikAudioProcessorEditor (StatikAudioProcessor&);
    ~StatikAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    Slider p1, p2;

    void sliderValueChanged(Slider* slider)
    {
        if (slider == &p1)
        {
            audioProcessor.delay.targetdelay = p1.getValue();
        }
        
        if (slider == &p2)
        {
            audioProcessor.delay.feedback = p2.getValue();

        }
    }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    StatikAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StatikAudioProcessorEditor)
};
