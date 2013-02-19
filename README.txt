Creator: Jason Carlisle Mann (jcm2207@columbia.edu)

-----------
DESCRIPTION
-----------

This project was designed for an assignment in COMS 4735 Visual Interfaces for Computers under the supervision of Prof. John Kender at Columbia University in Spring 2013.

The goal of this project was to filter out skin regions in imagery (static or motion), detect
regions belonging to hands, and then interpret specific pre-defined gestures from the captures, as part of a password grammar.

The project is coded in C++, relying on OpenCV 2.4.3 for computer vision algorithms, and
QT 4.8 (and QT Creator) for the design of the GUI forms. Some code and inspiration was found in the OpenCV2 Cookbook by Robert Laganiere, specifically the design of the SkinDetector and Controller classes (though not neccesarily the implementation).

The skin regions are differentiated from the background by manual configuration of
an HSV thresholding mask (setting min and max for each value) through the use of sliders on an introductory form. While photos can be thresholded, they are not implemented for use in the password grammar. The masks are applied using cv::inRange() to provide a binary image of skin regions, and that image is then blurred and closed (eroded()/dilate()). Once the correct mask is found, you procede to the detection form, which begins looking for hand gestures.

Immediately, any faces in the frame are identified and eliminated from consideration, using 
OpenCV's built-in Haar cascade classifier (frontalface_alt_tree). Any other skin regions in the frame are considered to be hands, so a long sleeve shirt is required. These regions are captured as contours using cv::findContours. The gestures used for the passwords are captured at regular intervals (hardcoded to 3 seconds) to allow the user time to configure the next gesture.

I chose five gestures (or absence of) for my grammar, a closed fist (FIST), single finger vertical point (POINT), and closed finger extended palm (PALM), no detected gestures (NONE), and a skin region that was unidentified (UNK). These are differentiated based on ratios (width/height) of their respective upright and minimum fit bounding rectangles, as well as the general mass of the moment of the contour points (see cv::Moment::m00). Any number of hand gestures can be captured in the frame, although a maximum of 2 is used for the password grammar. If there are two gestures in the capture, there is no differentiation between them, they are viewed as a non-ordered set. The passwords are hardcoded into the PasswordCheck class as a tree of conditional statements, though this could easily be expanded to a search tree. The password sequences are all 3 sequences long (although they can have up to two gestures per sequence). There are two special sequences different from the passwords, two PALMs in one frame, and a sequence of 3 captures with no gesture detected. Two PALMs signifies that the password attempt is complete, and to check against the grammar. This check also occurs automatically if a sequence of 6 uninterupted captures is encountered. If the 3 first captures are all NONE or UNK, the grammar restarts with the 4th capture. 

The hardcoded accepted password sequences are as follows (Capture 1 | Capture 2 | Capture 3):
	1.	FIST | FIST | FIST
	2.	POINT | FIST | PALM
	3.	PALM | (NONE OR UNK) | PALM
	4.	(PALM & POINT) | (FIST & FIST) | (POINT & FIST)

Two special sequences:
	1.	(PALM & PALM) 
			>immediately check the recorded sequence preceding
	2.	(NONE OR UNK) | (NONE OR UNK) |(NONE OR UNK) 
			>reset, and check for one of the passwords listed above starting with the 4th capture



------
ISSUES
------
1.	The masks are configurable with the first form, but if there are skin-similar (in HSV) regions in the background, it can be hard to find the right masks. I have added a hardcoded inverse option that allows for a bit more configurability (specifically if the sought after color bridges the max and min of the hue spectrum) but there are environments that can still cause malfunction. This could be avoided by using either background substitution (Codebook method), or histogram training.

2.	The gesture recognition relies on ranges probably specific to my hands, and will have to be customized to each user in the code. This can be solved by an introductory training/calibration session for each user, but is beyond the scope of this project.

3.	Auto-focus, and auto-exposure on modern web-cameras can cause a problem with the mask no longer being a valid fit, so if possible disable these options on your camera. On OSX there is no direct option, though through the use of iGlasses(Shareware/$19.95) you can disable auto-exposure on the built-in MacBook web cameras.



------------
ORGANIZATION
------------
Source code specific to this program is contained in src/, extendable classes are contained in include/ as follows:

GesturePasswords/
	data/
		gesture data.xlsx (recorded data for my hand gestures)
	include/
		colorhistogram.h 	-a class for displaying a histogram of the image (modified for HSV)
		hand.h
		skindetectcontroller.cpp	-a singleton wrapper with ease of use interfaces for 
		skindetectcontroller.h 		the SkinDetector class
		skindetector.cpp
		skindetector.h 				-the main processing class for detecting skin regions
	src/
		gesturedetector.cpp		-detector form functions
		gesturedetector.h 		-detector form header
		gesturedetector.u 		-detector form design (QDialog QT 4.8)
		main.cpp				-project main method
		maskselection.cpp		-mask selection form functions
		maskselection.h 		-mask selection form header
		maskselection.ui 		-mask selection form design (QWindow QT 4.8)
		passwordcheck.h 		-class for storing and processing hand gesture password sequences