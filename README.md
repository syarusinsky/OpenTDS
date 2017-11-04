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
Speaker (QSC K8) Frequency Response at 1 ft away with TEF25
![Speaker (QSC K8) Frequency Response at 1 ft away with TEF25](TEFComparisonSpeakerTrial1_3.PNG?raw=true "Speaker (QSC K8) Frequency Response at 1 ft away with TEF25")
Speaker (QSC K8) Frequency Response at 1 ft away with OpenTDS and Akai EIE Pro audio interface
![Speaker (QSC K8) Frequency Response at 1 ft away with OpenTDS and Akai EIE Pro audio interface](TEFComparisonSpeakerTrial1_4.png?raw=true "Speaker (QSC K8) Frequency Response at 1 ft away with OpenTDS and Akai EIE Pro audio interface")

Documentation on Time Delay Spectrometry, Installation, and Usage coming soon . . .
