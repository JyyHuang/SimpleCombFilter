/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleCombFilterAudioProcessor::SimpleCombFilterAudioProcessor()
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
}

SimpleCombFilterAudioProcessor::~SimpleCombFilterAudioProcessor()
{
}

//==============================================================================
const juce::String SimpleCombFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleCombFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleCombFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleCombFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleCombFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleCombFilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleCombFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleCombFilterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SimpleCombFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleCombFilterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SimpleCombFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    delayBuffer.setSize(1, maxDelayInSamples, false, true, true);
    delayBuffer.clear();
    writePosition = 0;
    
}

void SimpleCombFilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleCombFilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void SimpleCombFilterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    const int numSamples = buffer.getNumSamples();
        const int numChannels = buffer.getNumChannels();

        for (int channel = 0; channel < numChannels; ++channel)
        {
            float* channelData = buffer.getWritePointer(channel);

            for (int i = 0; i < numSamples; ++i)
            {
                float inputSample = channelData[i];
                float outputSample = delayBuffer.getSample(0, readPosition);
                
                // Apply feedback
                float feedback = outputSample * feedbackFactor;
                delayBuffer.setSample(0, writePosition, inputSample + feedback);

                // Output the delayed sample
                channelData[i] = outputSample;

                // Update read and write positions
                ++readPosition;
                ++writePosition;

                if (readPosition >= maxDelayInSamples)
                    readPosition = 0;

                if (writePosition >= maxDelayInSamples)
                    writePosition = 0;
            }
        }
}

//==============================================================================
bool SimpleCombFilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleCombFilterAudioProcessor::createEditor()
{
    return new SimpleCombFilterAudioProcessorEditor (*this);
}

//==============================================================================
void SimpleCombFilterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SimpleCombFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleCombFilterAudioProcessor();
}
