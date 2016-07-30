###	
### Create the .bmp files needed by the Grasp VR environment.
###

# A program that converts .tiff to .bmp.
# This is used to convert images generated in PowerPoint into usable .bmp files.
# In theory, PowerPoint can save directly in .bmp format, but it gives strange results for me.
CONVERTER = ..\..\Executables\tiff2bmp.exe --height=512 --width=512

# Define the path to the pandoc.exe program that does the conversion.
PANDOC=pandoc.exe
PANDOC_OPTIONS=

# This is the list of btimaps generated from Powerpoint. In general, they are messages that are
# presented in a circular format with the VR presentation.
FROMTIFFS = ReadyToStart.bmp BlockCompleted.bmp HeadMisalignment.bmp RaiseArm.bmp LowerArm.bmp TimeoutTilt.bmp TimeoutResponse.bmp TimeLImit.bmp

# The GRASP VR world requires a few bitmaps to decorate the walls. We keep copies in the source file tree
# then copy as needed to the exection tree. That way, by maintaining this makefile up to date, only those
# .bmp files that are actually needed get distributed in the release repositories.
# Static files (ones that are not automatically generated here from other sources) are
# kept in a subdirectory. That way we can do a del *.bmp to clean the project directory.
STATICBMPS =	StaticBitmaps\metal.bmp StaticBitmaps\NightSky.bmp StaticBitmaps\Rockwall.bmp 

# The bitmaps are moved to this directory to be included in the execution environment.
DESTINATION = ..\..\Bmp

# This is where we can find the tools need to do the build.
EXECUTABLES=..\..\Executables

# This is going to install the bitmaps in the execution arboresence.
# We delete the destination directory so that we eliminate any previous files that are no longer needed
# and then copy in the newly created files. The '-' in front of mkdir tells nmake not to stop if the
# command generates an error (which it does if the directory already exists).
install:	$(FROMTIFFS) $(STATICBMPS) GraspVRMessages.pdf
	rmdir /S /Q $(DESTINATION)
	-mkdir $(DESTINATION)
	copy /Y /V *.bmp $(DESTINATION)
	copy /Y StaticBitmaps\metal.bmp $(DESTINATION)
	copy /Y StaticBitmaps\NightSky.bmp $(DESTINATION)
	copy /Y StaticBitmaps\Rockwall.bmp $(DESTINATION)
	echo BMPs %date% %time% > $@

# Combine all the messages into a pdf document. This is useful when it comes time to generate the documention for the program.
GraspVRMessages.html: *.bmp.html
	type $(**) > $@

GraspVRMessages.pdf: GraspVRMessages.html
	$(EXECUTABLES)\wkhtmltopdf.exe  --page-size A6  --default-header --header-left "Grasp VR Messages" --header-font-size 8 --header-spacing 5 --margin-bottom 20 $(@B).html $@

# The next set of bitmaps are generated from a Powerpoint file entitled GraspCircularPrompts.pptx.
# To genearate the bitmaps, you must first save the latest version of the Powerpoint file as .bmp image files
# (see 'Save as images ...' or 'Enregistrer comme images ...' in the Powerpoint File menu).
# Here we convert the individual images into bitmap files. Note that the images created by Powerpoint have 
# filenames based on the slide number in the file. If you change the order of the slides or insert any slides
# you have to edit here below to link each slide to the correct bitmap filename.

ReadyToStart.bmp: GraspCircularPrompts\Diapositive1.tiff
	copy /Y $** $(@B).tiff
	$(CONVERTER) -i $(@B).tiff
	echo ^<img src="$@" size=50 /^> >$@.html

BlockCompleted.bmp: GraspCircularPrompts\Diapositive2.tiff
	copy /Y $** $(@B).tiff
	$(CONVERTER) -i $(@B).tiff
	echo ^<img src="$@" size=50 /^> >$@.html

HeadMisalignment.bmp: GraspCircularPrompts\Diapositive3.tiff
	copy /Y $** $(@B).tiff
	$(CONVERTER) -i $(@B).tiff
	echo ^<img src="$@" size=50 /^> >$@.html

TimeLimit.bmp: GraspCircularPrompts\Diapositive4.tiff
	copy /Y $** $(@B).tiff
	$(CONVERTER) -i $(@B).tiff
	echo ^<img src="$@" size=50 /^> >$@.html

LowerArm.bmp: GraspCircularPrompts\Diapositive5.tiff
	copy /Y $** $(@B).tiff
	$(CONVERTER) -i $(@B).tiff
	echo ^<img src="$@" size=50 /^> >$@.html

RaiseArm.bmp: GraspCircularPrompts\Diapositive6.tiff
	copy /Y $** $(@B).tiff
	$(CONVERTER) -i $(@B).tiff
	echo ^<img src="$@" size=50 /^> >$@.html

TimeoutTilt.bmp: GraspCircularPrompts\Diapositive7.tiff
	copy /Y $** $(@B).tiff
	$(CONVERTER) -i $(@B).tiff
	echo ^<img src="$@" size=50 /^> >$@.html

TimeoutResponse.bmp: GraspCircularPrompts\Diapositive8.tiff
	copy /Y $** $(@B).tiff
	$(CONVERTER) -i $(@B).tiff
	echo ^<img src="$@" size=50 /^> >$@.html
