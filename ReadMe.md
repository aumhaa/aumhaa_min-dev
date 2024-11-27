# aumhaa
This package was created using the Min-DevKit for Max, an API and supporting tools for writing externals in modern C++.



## Prerequisites

You can use the objects provided in this package as-is.

To code your own objects, or to re-compile existing objects, you will need a compiler:

* On the Mac this means **Xcode 9 or later** (you can get from the App Store for free). 
* On Windows this means **Visual Studio 2017** (you can download a free version from Microsoft). The installer for Visual Studio 2017 offers an option to install Git, which you should choose to do.

You will also need the Min-DevKit, available from the Package Manager inside of Max or [directly from Github](https://github.com/Cycling74/min-devkit).




## Contributors / Acknowledgements

The aumhaa is the work of some amazing and creative artists, researchers, and coders.



## Support

For support, please contact the developer of this package.


aumhaa.midi4lin/aumhaa.midi4lout

was having problem with the userData object, it was getting past without a casting. 

Also, was using a global variable “messages” instead of having it inserted in the class body. 

Was crashing and when multiple instances were present. 

Why This Works
1. Callback Context:
    * The callback is static and doesn't have direct access to non-static members. By passing this as userData, we give it a way to know which instance it's operating on.
2. Instance-Specific Data:
    * After casting userData, the callback has access to instance-specific data, like midi_messages.
3. Deferred Execution:
    * The defer timer ensures thread-safe processing of the midi_messages queue.
This approach avoids global variables, making the class safe to use in multi-instance environments. Let me know if you have more questions!


Successfully built both versions for Live 12.1.5b3, Max 9.02, XCode 15.1, on 112124.  
However, the project files are actually from Max 8 Packages aumhaa, because the file references are all from Max 8.  Therefore, aumhaa needs to be updated for Max 9, and any further work has to take place in the Max8 version (which is the version in repo) and copied manually to the Max 9 local copy. 

To regenerate the projects (e.g., when moving to a different folder, as has happened when migrating from Max 8 to Max 9), one must futz with cMake.  

NEXT, one has to completely relink all the files.  Annoying.  So, in the case of the midi4l stuff, you need to readd all files from rtMIDI (in the local folders, so actually easy) and then add the librarys via the Build Phases panel (CoreMidi, CoreAudio, CoreFoundation).  Jitter and Max can actually be removed, although not necessary.  probably makes the bin smaller. 

Then, make sure to compile those sources that were added from rtMidi. Annoyingggggg. 