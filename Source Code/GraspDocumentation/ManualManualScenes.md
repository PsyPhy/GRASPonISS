

The Manual-Manual task starts with a welcome screen that prompts the subject 
to press a button in order to start a block of trials:

CENTER( IMAGE(ready.bmp, SINGLE) )

The subject is prompted to look straight ahead and bring the head to an upright position with respect to the shoulders.

CENTER( IMAGE(upright.bmp, SINGLE) )

The subject should straighten the head first, the press the Select button on the VR Remote to continue.

#if 0
The subject is then further guided to adjust the tilt of the head, if needed.
The halo around the subject's viewpoint into the virtual world changes color, from red when the head 
is tilted far from the upright position through yellow and cyan until it turns suddenly bright green to
indicate that the head is at the proper orientation.  
A circular arrow may appear when the head
is far from the desired posture, giving the subject an indication of which way to tilt the head:

CENTER( IMAGE(leftFarRedress.bmp, SERIES) IMAGE(leftCloserRedress.bmp, SERIES) IMAGE(leftNearRedress.bmp, SERIES) IMAGE(goodRedress.bmp, SERIES) IMAGE(rightNearRedress.bmp, SERIES) IMAGE(rightCloserRedress.bmp, SERIES) IMAGE(rightFarRedress.bmp, SERIES))

CENTER(Tilted to the Left ------------------ Correct Orientation ------------------ Tilted to the Right)
#endif

The subject is then guided to place the hand at the target orientation.
The subject sees a) an orange spherical target at the end of the tunnel, 
b) a virtual representation of a tool in the visual scene (a cylinder) 
that moves with the hand in 3D visual space and 
c) a 'laser pointer' that projects from the tool along the axis of the fingers. 
The virtual tool changes color according to the roll orientation of the hand (pronation/supination).
The subject must raise the arm in front of the body, aim the tool by moving the arm in azimuth and elevation until the 
laser pointer falls on the spherical target and then rotate the hand until the tool turns green.
If the orientation of the hand is far from the target orientation, an arrow appears around the virtual tool showing
which way to rotate the hand.

CENTER( IMAGE(kTargetFar.bmp, SERIES)  IMAGE(kTargetNear.bmp, SERIES)  IMAGE(kTargetGood.bmp, SERIES) )

Once the target orientation has been achieved and held for 1 second, the tool and laser pointer disappear.
The subject must lower the arm and is then prompted to tilt the head to a new orientation through the same system 
of halo colors and direction arrows as described previously:

CENTER( IMAGE(leftFarAlign.bmp, SERIES) IMAGE(leftCloserAlign.bmp, SERIES) IMAGE(leftNearAlign.bmp, SERIES) IMAGE(goodAlign.bmp, SERIES) IMAGE(rightNearAlign.bmp, SERIES) IMAGE(rightCloserAlign.bmp, SERIES) IMAGE(rightFarAlign.bmp, SERIES))

CENTER(Tilted to the Left ------------------ Correct Orientation ------------------ Tilted to the Right)

Finally subject then reproduces the remembered target orientation with the hand. 
The virtual tool, laser pointer and spherical target reappear, but this time the tool and laser pointer are constantly blue.
The subjecct must raise the hand and aim with the laser pointer at the spherical target but then rotate
the hand to the remembered orientation without any further visual cues:

CENTER( IMAGE(kTool.bmp, SINGLE) )

The subject presses the Select Button on the VR Remote to validate the trial and move on to the next.
If the block of trials is completed, a message is displayed to let the subject know:

CENTER( IMAGE(blockCompleted.bmp, SINGLE))

The subject may press the Select button on the VR Remote to exit the VR scene and pause the experiment. 
Or he or she can press and hold the Return button on the VR Remote to continue to the next block of trials.

If all blocks in the subsession are completed, the subject is prompted to doff the Virtual Reality Headset.

CENTER( IMAGE(vrCompleted.bmp, SINGLE))

Note that other circular prompt with text instructions may appear at different times in response to different conditions
or actions by the subject. For instance, a message will be generated if the subject does not maintain the head in the specified tilted position.
All possible messages are shown in a separate section of this document.