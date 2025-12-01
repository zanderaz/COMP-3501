Course Project Submission, COMP3501
By Aidan Lalonde-Novales, Evan Letto, Zander Azevedo

--> Design <--

Reference the final project report for information on design decisions.


--> Build Instructions <--

1. Download the .zip file from Brightspace.
2. Unzip your .zip file into a new folder in /<of_folder_location>/apps/myApps/ .
3. Open /<of_folder_location>/projectGenerator/projectGenerator.exe
4. Click the "import" button and navigate to where you unzipped your submission in /myApps/ .
5. Click "Generate" (or if the button prompts change, click "Update").
6. projectGenerator.exe should now prompt you to open the project. Click the button "Open in IDE".
7. In Visual Studio 2022, run the project (click the green "play" button).


--> Testing Environments <--

Tested in Windows 11 x64 (AMD) on discrete graphics and Windows 10 x64 (AMD) on integrated graphics.


--> Known Bugs <--

- holding shift while using WASD causes keys to become stuck, e.g. 'w' will stay held when released, which constantly applies unwanted forward movement
- if an Nvidia banner pop-up appears (from Nvidia app), mouse tracking will desync/jump somewhere else on the screen
- looking straight up or down causes changes to yaw to increase dramatically
