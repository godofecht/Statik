/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StatikAudioProcessor::StatikAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    delay.Reset();
}

StatikAudioProcessor::~StatikAudioProcessor()
{
}

//==============================================================================
const juce::String StatikAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StatikAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool StatikAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool StatikAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double StatikAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int StatikAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int StatikAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StatikAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String StatikAudioProcessor::getProgramName (int index)
{
    return {};
}

void StatikAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void StatikAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    phase = 0;
    wtSize = 1024;
    amplitude = 0.25;
}

void StatikAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StatikAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void StatikAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();



    //Print midi messages

    //Sin wave stuff, we just want to focus on midi for now

   

    int noteDuration = static_cast<int> (std::ceil(rate * 0.25f * (0.1f + (1.0f - (0.5f)))));
    MidiMessage msg;
    int ignore;
    for (MidiBuffer::Iterator it(midiMessages); it.getNextEvent(msg, ignore);)
    {
        if (msg.isNoteOn())
        {
            notes.add(msg.getNoteNumber());
            presentNote = msg.getNoteNumber();
      //             DBG(msg.getNoteNumber());
        }
        else if (msg.isNoteOff()) notes.removeValue(msg.getNoteNumber());
    }

    presentNote = 55;

    frequency = 440.0 * pow(2.0, (presentNote - 69.0) / 12.0);

    increment = frequency * wtSize / getSampleRate();
//    DBG(presentNote);




    waveTable.clear();
    //one cycle of a sin wave
    for (int i = 0; i < wtSize; i++)
    {
        waveTable.insert(i, sin(2.0 * double_Pi * i / wtSize));
    }







    for (int channel = 0; channel < 1; ++channel)
    {
        auto* channelDataL = buffer.getWritePointer (0);
        auto* channelDataR = buffer.getWritePointer (1);

        for (int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            channelDataL[sample] = waveTable[(int)phase] * amplitude * releaseMod;

            phase = fmod(( phase + increment),wtSize);

            delay.process(channelDataL[sample]);
            channelDataR[sample] = channelDataL[sample];
        }

        // ..do something to the data...
    }

    //I need to actually trigger the ADSR envelope with note presses, and multiply the envelope to the output of the audio.
    if (notes.size() == 0)
    {
        releaseMod -= 0.01;
        if (releaseMod < 0.0) releaseMod = 0.0;
    }
    else
    {
        releaseMod += 0.01;
        if (releaseMod > 1.0) releaseMod = 1.0;
    }



}

//==============================================================================
bool StatikAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* StatikAudioProcessor::createEditor()
{
    return new StatikAudioProcessorEditor (*this);
}

//==============================================================================
void StatikAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void StatikAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StatikAudioProcessor();
}
