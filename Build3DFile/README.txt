Each line in ScanData.txt is a (distance, pan, tilt) triple (i.e., data from the LIDAR sensor)

Build3DFile.cpp creates a 3D STL mesh from the data in ScanData.txt.

MakeSphere.cpp creates a sample ScanData.txt file.

If you're on Windows and have the g++ compiler set up, all you have to do is open the Build3DFile 
folder in VSCode and run the default build task, and everything will be compiled and executed, 
and the 3D file will be opened.
