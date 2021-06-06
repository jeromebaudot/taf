*********************************************************************************************************
# NOTES ON THE DEVELOPMENT FOR THE READ-OUT OF THE MIMOSIS SENSOR  

- Overall strategy
- Status
- Step 1: connecting with Gilles' library -> done
- Step 2: File & acquisition reading within BoardReaderClass -> done
- Step 3: Frame decoding and simple event building -> done
- Step 4: Triggers reading and related event building
*********************************************************************************************************

*********************************************************************************************************
## Overall strategy

We will base the decoding on Gilles'Library. The class BoardReaderMIMOSIS will call this library, decode the rawdata, build an event with its pixels and then send the event to the rest of the TAF analysis.
There will be two event building strategies. One where each single frame makes an event.
The second relies on the trigger information to group consecutive frames related to the same trigger into one event.
This strategy was already followed for the MIMOSA-26/28 sensors. Solutions can be extracted from the PXIeBoardReader class.

### Technicalities with GIT
- We work with https://github.com/jeromebaudot/taf (may change to gitlab.cern.ch if we decide to).
- The reference branch will be mimosis, update should only come from pull-requests.
- Each developper creates its own branch to develop his/her code. When ready creates a pull-request to the mimosis branch.

### Required steps
The class BoardReaderMIMOSIS is an empty template but already connected to TAF. The steps described below target to integrate the decoding and event building into the class.
1. connecting with Gilles' library
2. File & acquisition reading within BoardReaderClass
3. Frame decoding and simple event building
4. Triggers reading and related event building

*********************************************************************************************************
## Status


- 2021/05/20: this document has been written (Jerome), only steps 1 and 2 are possible

*********************************************************************************************************
## Step 1: connecting with Gilles' library

- Goal: integrate the main() from gcc_src_loc/msis1_run_read_light_bt_v10.c into BoardReaderMIMOSIS
- Responsible: Ziad
- DONE: commited to main mimosis branch on 2021/05/26

This task requires to extract the elements of Gilles'code and to include them into TAF.

1. Extracting Gilles' library
Under the com/ directory, they are many files. Some are indeed useful others not at all.
One should identify all the useful files, starting from the ones included in msis1_run_read_light_bt_v10.c.
It seems that the library is essentially in com/data.
These files or directories should be copied to a new TAF directory: taf/code/include/mimo_daq_lib

1. The preprocess commands and includes
The program msis1_run_read_light_bt_v10.c starts with preprocessor commands (defines and include).
Let's create a dedicated header file taf/code/include/mimo_daq_lib.h where you include all of them.
Then, simply #include mimo_daq_lib.h from BoardReaderMIMOSIS.h

1. The main() function
Copy the code of the main() from msis1_run_read_light_bt_v10.c into a new member function in the TAF class BoardReaderMIMOSIS
-> Let's call it test()

1. Go!
TAF should compile with the new mimo_daq_lib. To use the test method:
  1. launch TAF: SHELL> TAF
  1. instantiate the reader class: TAF> BoarReaderMimosis r
  1. call the method: TAF> r.test()

*********************************************************************************************************
## Step 2: File & acquisition reading within BoardReaderClass

- Goal: Get the functionalities of opening rawdata files and reading Acquisition in BoarReaderMIMOSIS class
- Responsible: Ziad
- DONE: commited to main mimosis branch on 2021/06/05

This task requires to break up the various parts in the program msis1_run_read_light_bt_v10.c.
The main() contains indeed various functions, which should be interface with TAF.
That could possibly require to modify some of the files in the mimo_daq_lib.
Also, there might be missing arguments in the current creation of the BoarReaderMIMOSIS class.
If that's the case, the creator function should be complemented AND the class DAcq should be updated accordingly. The creator function of DAcq actually instantiates an object BoarReaderMIMOSIS around line 467.

1. Instantiating an object from the MIS1__TBtRunRead class
Gilles'library is based on the class MIS1__TBtRunRead. Such an object is called APP_VGPtRunRead in Gilles' example. Basically, the BoarReaderMIMOSIS class can be seen as a wrapper (with the proper TAF interface) to this MIS1__TBtRunRead class.
The same object has to be instantiated in the creator of the BoarReaderMIMOSIS class.

1. Message handling
There are two types of logging in Gilles'library, errors and messages. They both correspond to a different files.
For TAF, these files should go in the directory Results/[run_number].
This path settings should happen in the creator of the BoarReaderMIMOSIS class

2. File connection
Usually in TAF, files are added to the BoardReader class from the creator function of the DAcq class. This is the plan currently implemented from line 475 to 501 of the Dacq.cxx file. The mechanism should be changed a bit. One can still check that the files exist with the BoarReaderMIMOSIS::AddFileList function. But there is no need to open the stream within that function.
The data stream is handled within Gilles'library. So to actually initialise the connection to the file, one should use the FRunConf. It has to be seen where to do it, possibly in the AddFileList function.
If FRunConf, TAF should stop or at least return a clear ERROR message.
Similarly, it is to be seen where and when to call Gilles' FRunClose function.

Note that TAF is capable to find by itself the filename of the rawdata, just from a directory path and a run_number. This is done through trials over various combinations, shared between the DAcq class and the   BoarReaderMIMOSIS::AddFileList function.
This is extremely usefull since it allows to use the configuration file for various runs! To make this mechanism works again with Gilles'library might require some adjustments.

3. Acquisition reading
Before Gilles has provided the data decoding itself, the suggestion is to read acquisition as if they were events.
For that, one would copy the methods used to handle acquisitions (all named FAcqXXXXX) and wrap them to new BoarReaderMIMOSIS functions (like PrintRunHeader, PrintAcqHeader and so on).
Then the BoarReaderMIMOSIS::HasData shall call Gilles' FAcqNext function to get to the next acquisition.
Use the debugging levels to allow various printouts (headers and such).

5. Go!
Now, the BoarReaderMIMOSIS class could be used in a standard way in TAF, that means with a config file.
A configuration file is already available in config_TEST/mimosis.cfg
Launch TAF with the GUI (taf -run XXX -cfg ./config_TEST/mimosis.cfg -gui) and click on the "RAW CHANNELS 2D" button from the menu to advance through events.
You should see the printouts prepared depending on the debug level set (gTAF->SetDebugLevel(...)).  


*********************************************************************************************************
## Step 3: Frame decoding and simple event building

- Goal: Get the functionalities to decode frames and build simple event in BoarReaderMIMOSIS class
- Responsible: Ziad
- DONE: commited to main mimosis branch on 2021/06/03

This tasks assumes Gilles'library provide functions to navigate through frames and decode them.
The triggerMode here will be 0, one event is built from each frame.

1. The event builder
The function BoarReaderMIMOSIS::DecodeNextEvent drives the way events are built. With TriggerMode==0, we want that each frame is an event. So this DecodeNextEvent function should simply call the decoding of a single frame AND stop. This is crucial, because in Step 4, we will be reading several frames to build an event.

2. Decoding the next frame
 Create a new function in BoarReaderMIMOSIS (typically ReadNextFrame), that will call its equivalent from Gilles'class MIS1__TBtRunRead. The method should access the next frame and then decode the header information.
 When TriggerMode==0, we also immediately go to decode the pixel information and add them to the list of pixels (and frame ids and trigger ids (even if we don't use them) for the current event.

 3. Adding pixels
 Adding pixels is done with the BoarReaderMIMOSIS::AddPixel method. Currently, a very simple version is implemented. Look at the same method in PXIeBoardReader to understand how to veto some pixels.

 4. Go!
Same as before, you can launch TAF with the GUI and observe fired pixels in the plots for each event then.
Pay attention that the TriggerMode is properly set in the config file.

*********************************************************************************************************
## Step 4: Triggers reading and related event building

- Goal: Get the functionalities to decode triggers, associate frames and build complex event in BoarReaderMIMOSIS class
- Responsible: ?

Brace yourself, we are almost done!!! But admittedly, this is the most difficult step. This mode of operation will be selected if the TriggerMode==1 in the config file.
It is always tricky to handle triggers properly, since you need to take into account various situation, like: the position is wrong and you'll never find the associated frame, two triggers are very close and they share some frames, ...   

1. Getting the list of trigger positions
The trigger positions are listed in the header of each acquisition. You need to create a method that reads this information for each new acquisition loaded AND update the list of triggers to find. There should be such a list, possibly in the form a C++ vector. Then there should be a variable indicating the memory position of the current trigger (let's call it currentTriggerPosition).

2. Upgrading the event builder
BoarReaderMIMOSIS::DecodeNextEvent should now handle the trigger. That means it gets the next frame and compare its memory position with currentTriggerPosition. If the frame memory position is within a certain limit (there is a variable TimeLimit one can set in the config file for that), then the frame is associated to the trigger. This means the pixels are decoded and added to the current list.
The DecodeNextEvent should keep analysing frame till the frame memory position is past the  currentTriggerPosition+TimeLimit. Then the currentTriggerPosition should be updated to the next trigger position and the method should stop. If some frames have been associated to the trigger, the event is really created by the BoarReaderMIMOSIS::HasData function. Otherwise, nothing is done and we call again DecodeNextEvent with the new trigger position.

As said before, one should plan for some exceptions... good luck with that!

3. Go!
By now, the reader should know what to write here...

*********************************************************************************************************
==> We are done, please enjoy the fabulous analysis with TAF :)
