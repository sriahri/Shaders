*******************************************************
*  Name      : Srihari Inukurthi     
*  Student ID: 110712807             
*  Class     :  CSC 2421           
*  HW#       :   1               
*  Due Date  :  02/04/2024
*******************************************************


                 Read Me


*******************************************************
*  Description of the program
*******************************************************

The program implements the Phong Multi-Light shader. The
program uses Phong illumination model to light 2 objects(
1 display object and ground) that shows 4 different colors.


*******************************************************
*  Source files
*******************************************************

Name:  main.cpp
   Main program. Contains the basic scene setup, light properties. 
   Responsible for loading and rendering the objects.

Name:  MultiLight.vert
   Contains the definition for the vertx shader.
   Contains the logic for calculating and interpolating position.  

Name: MultiLight.frag
   Defines a function for computing the total fragment color.
   Defines a function for computing the fragment color for each light.
   Contains the material properties.
   Contains the logic for calculating the diffuse, specular and 
   ambient component for each light.
   
   
*******************************************************
*  Circumstances of programs
*******************************************************

   The program runs successfully.  
   
   The program was developed, compiled, run and tested on Windows 11 using Microsoft Visual
   Studio CE 17.8. The hardware specifications are as follows:
   Operating System: Windows 11 Home
   System type: 64-bit operating system, x64-based processor
   Processor: AMD Ryzen 7 5800H with Radeon Graphics
   


*******************************************************
*  How to build and run the program
*******************************************************

1. Uncompress the homework.  The homework file is compressed.  
   To uncompress it use the following commands 
       % unzip [homework_filename]

   Now you should see a directory named 2807HW1 with the files:
        SGPU_HW1.sln
        SGPU_HW1
        readme.txt
	Screenshot 1
	Screenshot 2

2. Build and run the program.

    Open the SGPUHW1.sln file in the Visual Studio.
    Make sure that the Startup Project is set to the SGPUHW1.
    Change to “Release” mode rather than “Debug” mode to improve the performance of loading the *.obj text file
    Right-click the project in the “Solution Explorer” and selecting “Set as Startup Project“