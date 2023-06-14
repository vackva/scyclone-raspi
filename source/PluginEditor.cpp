#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginParameters.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p, juce::AudioProcessorValueTreeState& parameters)
    : AudioProcessorEditor (&p), apvts(parameters), processorRef (p), transientViewer(p), openGLBackground(parameters, p), advancedParameterControl(parameters), parameterControl(parameters), logoButton("logoButton",
                                                                                                                                                                                                            juce::DrawableButton::ButtonStyle::ImageFitted)
{
    juce::ignoreUnused (processorRef);

    for (auto & parameterID : PluginParameters::getPluginParameterList()) {
        parameters.addParameterListener(parameterID, this);
    }

    juce::LookAndFeel::setDefaultLookAndFeel (&customFontLookAndFeel);

    headerComponent = std::make_unique<HeaderComponent>(p, parameters);

    headerComponent->onParameterControlViewChange = [this](bool newState)
    {
        if (newState)
        {
            advancedParameterControl.setVisible(true);
            parameterControl.setVisible(false);
            transientViewer.setVisible(false);
        }
        else
        {
            advancedParameterControl.setVisible(false);
            parameterControl.setVisible(true);
            transientViewer.setVisible(true);
        }
    };

    addAndMakeVisible(headerComponent.get());
    addAndMakeVisible(openGLBackground);
    addAndMakeVisible(advancedParameterControl);
//    advancedParameterControl.setVisible(false);
    addAndMakeVisible(parameterControl);
    addAndMakeVisible(transientViewer);
    addAndMakeVisible(textureComponent);
    bool state = processorRef.advancedParameterControlVisible.getValue();

    headerComponent->detailButton.setToggleState(state, juce::sendNotification);
    if (!state)
        advancedParameterControl.setVisible(false);
    else
    {
        parameterControl.setVisible(false);
        transientViewer.setVisible(false);
    }

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    processorRef.setExternalModelName = [this] (int modelID, juce::String& modelName) {
        openGLBackground.externalModelLoaded(modelID, modelName);
    };

    double ratio = 2;
    setResizable(false, true);
    setResizeLimits(800, 800/ratio, 1400, 1400/ratio);
    getConstrainer()->setFixedAspectRatio(2);
    setSize (800, 400);

    // dirty work around to make the blobs appear correctly from the beginning
    auto fadeParam = parameters.getParameter(PluginParameters::FADE_ID);
    auto fadeStatus = fadeParam->getValue();
    fadeParam->setValueNotifyingHost(0.5f*fadeStatus);
    fadeParam->setValueNotifyingHost(fadeStatus);

    addAndMakeVisible(logoButton);
    logoButton.setImages(logo.get());
    logoButton.onClick = [this] () {
        processorRef.loadExternalModel(juce::File{""}, 1);
    };
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    juce::LookAndFeel::setDefaultLookAndFeel (nullptr);
    for (auto & parameterID : PluginParameters::getPluginParameterList()) {
        apvts.removeParameterListener(parameterID, this);
    }
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colour::fromString(ColorPallete::BG));
}

void AudioPluginAudioProcessorEditor::resized()
{
    auto r = getLocalBounds();
    const float height = r.getHeight();
    const float width = r.getWidth();
    r.removeFromTop(static_cast<int>(height*0.02858f));
    auto headerSection = r.removeFromTop(static_cast<int>(height*0.04572f));
    auto padSection = r.removeFromLeft(width/2);
    auto miniMapSection = r.removeFromLeft(static_cast<int>(width*0.12571f));

    r.removeFromLeft(static_cast<int>(width*0.03214));
    auto sliderSection = r;

    logoButton.setBounds(width/2, 10, 25, 25);

    juce::ignoreUnused(headerSection, miniMapSection, sliderSection);

    openGLBackground.setBounds(padSection);
    transientViewer.setBounds(static_cast<int>(width*0.507143f), static_cast<int>(height*0.642857f), static_cast<int>(width*0.116429f), static_cast<int>(height*0.23285f));
    advancedParameterControl.setBounds(static_cast<int>(width*0.54642f), static_cast<int>(height*0.085714f), static_cast<int>(width*0.42857), static_cast<int>(height*0.857143f));

    auto areaParameter = getLocalBounds().removeFromRight(static_cast<int>((float)getWidth()*0.4f));
    areaParameter.removeFromTop(static_cast<int>(height*0.057143f));
    parameterControl.setBounds(areaParameter);
    headerComponent->setBounds(getLocalBounds());
    textureComponent.setBounds(getLocalBounds());

    processorRef.onNetwork1NameChange(processorRef.network1Name.toString());
    processorRef.onNetwork2NameChange(processorRef.network2Name.toString());
}

void AudioPluginAudioProcessorEditor::parameterChanged(const juce::String &parameterID, float newValue) {
    parameterControl.parameterChanged(parameterID, newValue);
    if (parameterID == PluginParameters::SELECT_NETWORK1_ID && newValue == 1.f) {
        openFileChooser(1);
    } else if (parameterID == PluginParameters::SELECT_NETWORK2_ID && newValue == 1.f) {
        openFileChooser(2);
    }
}

void AudioPluginAudioProcessorEditor::openFileChooser(int networkID) {
    fc = std::make_unique<juce::FileChooser> ("Choose a file to open...", juce::File::getSpecialLocation(juce::File::SpecialLocationType::userHomeDirectory),
                                              "*.onnx", true);

    fc->launchAsync (juce::FileBrowserComponent::openMode
                     | juce::FileBrowserComponent::canSelectFiles,
                     [this, networkID] (const juce::FileChooser& chooser)
                     {
                         juce::File chosen;
                         auto results = chooser.getURLResults();

                         for (const auto& result : results) {
                             if (result.isLocalFile()) {
                                 chosen = result.getLocalFile();
                             }
                             else
                             {
                                 return;
                             }

                         }

                         if (chosen.getSize() != 0) {
                             processorRef.loadExternalModel(chosen.getFullPathName(), networkID);
                         } else {
                             auto param = (networkID == 1) ? PluginParameters::SELECT_NETWORK1_ID : PluginParameters::SELECT_NETWORK2_ID;
                             apvts.getParameter(param)->setValueNotifyingHost(0.f);
                         }

                     });
}
