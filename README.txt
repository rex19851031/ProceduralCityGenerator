Keys:
	W,S,A,D		: Control direction and move the camera.
	Shift		: Move with faster speed (4x)
	SPACEBAR	: Move the camera upward.
	TAB		: Toggle to bind colored texture / bing white texture
	1~9		: Load different Obj / C23 model file

Command Line "-cook"	: Check time between raw data and cooked data , and only cook the newer raw data.
Command Line "-cookall" : Cook all raw data.

Bugs:
	Currently the exporter is not copy the image automatically. It only shows the texture file name in log file , needs to copy the file manually to the Data folder.
	
	The file should be export to .\Run_Win32\Data\Models\simplified\ and saved as simplified.c23

	The texture already been set up to the correct name .

	The test file and texture is for zak walk , hit 3 to play the correct model.

	The model will be at position (500,500,0) , so needs to turn the camera in order to view the whole model.



SLN Name & Location:
	SkeletalAnimation.sln

Executable File Name:
	SkeletalAnimation.exe	