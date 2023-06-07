//
// Created by Christian Scheer on 02/03/2023.
//

#ifndef VAESYNTH_ADVANCEDSETTINGS_H
#define VAESYNTH_ADVANCEDSETTINGS_H

#include "JuceHeader.h"
#include "../ParameterControl/Slider/CustomSliderComponent.h"
#include "../../../utils/colors.h"
#include "../../../PluginParameters.h"


class AdvancedParameterControl : public juce::Component
{
public:
    explicit AdvancedParameterControl(juce::AudioProcessorValueTreeState& parameters);
    ~AdvancedParameterControl() override;

    void resized() override;
    void paint(juce::Graphics &g) override;

private:
    // Sliders
    CustomSliderComponent transientSplitter1{"Split 1"}; //ms
    CustomSliderComponent transientSplitter2{"Split 2"}; //ms
    CustomSliderComponent raveFade {"Fade"};
    CustomSliderComponent compThreshold{"Threshold"};
    CustomSliderComponent compRatio{"Ratio"};
    CustomSliderComponent compMakeup{"Makeup"};
    CustomSliderComponent compMix{"Dynamics"};
    CustomSliderComponent masterMixParam{"Mix"};

    CustomSliderComponent grainDelay1Param1{"Interval"};
    CustomSliderComponent grainDelay1Param2{"Size"};
    CustomSliderComponent grainDelay1Param3{"Pitch"};
    CustomSliderComponent grainDelay1Param4{"Mix"};

    CustomSliderComponent grainDelay2Param1{"Interval"};
    CustomSliderComponent grainDelay2Param2{"Size"};
    CustomSliderComponent grainDelay2Param3{"Pitch"};
    CustomSliderComponent grainDelay2Param4{"Mix"};

    CustomSliderComponent* sliders[16] = {&transientSplitter1,
                                         &transientSplitter2,
                                         &raveFade,
                                         &compThreshold,
                                         &compRatio,
                                         &compMakeup,
                                         &compMix,
                                         &masterMixParam,
                                         &grainDelay1Param1,
                                         &grainDelay1Param2,
                                         &grainDelay1Param3,
                                         &grainDelay1Param4,
                                         &grainDelay2Param1,
                                         &grainDelay2Param2,
                                         &grainDelay2Param3,
                                         &grainDelay2Param4};
    int numberOfSliders = 16;

    // Geometry
    int sliderWidth = 73;
    int sliderHeight = 250;
    int sliderDistance = 2;

};


#endif //VAESYNTH_ADVANCEDSETTINGS_H
