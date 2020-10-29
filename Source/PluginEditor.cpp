/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StatikAudioProcessorEditor::StatikAudioProcessorEditor (StatikAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    addAndMakeVisible(p1);
    addAndMakeVisible(p2);

    p1.addListener(this);
    p2.addListener(this);



    p1.setRange(0.0f, 44100.0f);
    p2.setRange(0.0f, 100.0f);

    p1.setValue(1323.0f);
    p2.setValue(1.0f);
}

StatikAudioProcessorEditor::~StatikAudioProcessorEditor()
{
}

//==============================================================================
void StatikAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void StatikAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    p1.setBounds(20, 20, 300, 20);
    p2.setBounds(20, 50, 300, 20);
}
