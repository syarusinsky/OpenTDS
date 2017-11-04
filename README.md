# OpenTDS
An Open Source Alternative to Generating Frequency Response and Time Response Graphs by Time Delay Spectrometry

This project was designed as an Open Source alternative to Gold Line's TEF system and other similar (expensive) software and hardware bundles. These systems use Time Delay Spectrometry to achieve frequency response measurements of loudspeakers, rooms, audio circuits, and many other minimal-phase systems without the need of an anechoic chamber. This project benefits from being capable of taking measurements using any modern consumer-grade audio interface, while TEF and other systems require you to purchase their proprietray hardware. The goal of this project is to provide hobbyist and professional acousticians and audio engineers with a means to measure frequency response and time response of without the need to purchase any additional hardware or software.

## Current Status

Currently, the program is capable of producing reliable frequency response graphs comparable to Goldline's TEF system and other TDS based systems. However, the accuracy of the results are currently dependant on the user's ability to correctly measure the microphone's distance from the loudspeaker and the temperature of the room (ie: correctly measure the time-of-flight from loudspeaker to microphone). In future versions of the program, calculation of the time-of-flight will instead be measured by the user using the Time Response Graphing Module. This module, along with the Phase Response Module, will be among the landmark goals of the project.

### Landmark Goals

* Time Response Module
* Phase Response Module
* Improved TDS Functionality
  * Improved BandPass Filtering (A Custom IIR Filter Class)
  * Improved Amplitude Calculation (Algorithmically-Selected FFT-Bin-Based Magnitude Instead of Block-Based Magnitude Averages)
  * dB Referencing (Capability to Calibrate the System to Reference Pressure Levels. ie: 20uPa for dBSPL)

## JUCE
This project was written using the [JUCE framework](https://juce.com/) which provides excellent cross-platform support.


## License
This project is licensed under the GNU General Public License v3.0 - see the [LICENSE.md](LICENSE.md) file for details.


## Authors
Stevey Yarusinsky - Initial Work


## Acknowledgements
Richard C. Heyser - For his work developing the method of Time Delay Spectrometry. An anthology of his published works ("Time Delay Spectrometry: An Anthology of the Works of Richard C. Heyser on Measurement Analysis and Perception") was an invaluable asset in developing this project.

## Documentation

### TDS Background Information and Concepts

In traditional swept signal analysis, a sinusoidal waveform is sent to a device-under-test (DUT) and the resulting magnitude and phase changes on the output of the DUT are plotted on a graph for each frequency under consideration. The plotted magnitudes are the *Frequency Response* of the graph. Often included are the plotted phases (or *Phase Response*) which are often plotted on the same graph usually in a different color.

Frequency response is critical to achieving the required performance of many systems such as loudspeakers. However, loudspeakers cannot be accurately measured with traditional swept signal analysis unless measurement is performed in an anechoic chamber. An anechoic chamber is a room designed to be completely acoustically absorptive which eliminates the possiblity of sound being reflected on any surfaces of the room. In this chamber a loudspeaker can be measured by inputting sinusoidal waves and measuring the magnitude and phase changes with a calibrated reference microphone, though most professionals and hobbyists do not have access to an anechoic chamber. In a normal room the waves generated by the speaker would reflect off of the surfaces of the room and eventually reach the microphone, skewing the measurement results considerably. *Time Delay Spectrometry* was designed to solve this problem.

Time Delay Spectrometry introduces a variable-bandwidth band-pass filter capable of both tracking the frequency of a paired oscillator and delaying that oscillator to compensate for the time-of-flight. This process filters out any reflections generated by the conditions of the space where measurement is taking place. The concepts are difficult to grasp initially so a procedural description is helpful.

* First, a sweeping oscillator generating a sinusoidal waveform is sent to the loudspeaker.
* The sound waves generated by the loudspeaker propagate and reflect off of surfaces in the measurement space
* The direct sound reaches the microphone first followed later by the reflected sound.
* These sounds are recorded by the software then processed with the band pass filter, which is sweeping its cutoff frequency with a paired oscillator.
* The reflections take more time to reach the microphone and therefore are lower frequency than the direct sound.
* Since the reflections are lower frequency than the direct sound they are filtered out by the band pass filter whose cutoff frequency is equivalent to the paired oscillator, leaving only the direct sound in the signal.
* This signal is then processed to determine its amplitude and phase information.
* That information is then plotted as a frequency response graph.

It is also important to note that the paired oscillator is actually a slightly lower frequency than the oscillator being fed into the loudspeaker due to the delay placed on the paired oscillator. This is necessary to compensate for the amount of time required for the direct sound to reach the microphone. Figure 1a. and Figure 1b. should help visualize the TDS process.

Figure 1a. Block Diagram of TDS
![Block Diagram of TDS](TDS_Block_Diagram.PNG?raw=true "Block Diagram of TDS")

Figure 1b. TDS Geometry Example
![TDS Geometry Example](TDS_Geometry_Example.PNG?raw=true "TDS Geometry Example")

### Frequency Response Comparison

Speaker (QSC K8) Frequency Response at 1 ft away with TEF25
![Speaker (QSC K8) Frequency Response at 1 ft away with TEF25](TEFComparisonSpeakerTrial1_3.PNG?raw=true "Speaker (QSC K8) Frequency Response at 1 ft away with TEF25")
Speaker (QSC K8) Frequency Response at 1 ft away with OpenTDS and Akai EIE Pro audio interface
![Speaker (QSC K8) Frequency Response at 1 ft away with OpenTDS and Akai EIE Pro audio interface](TEFComparisonSpeakerTrial1_4.png?raw=true "Speaker (QSC K8) Frequency Response at 1 ft away with OpenTDS and Akai EIE Pro audio interface")

Documentation on Time Delay Spectrometry, Installation, and Usage coming soon . . .
