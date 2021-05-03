The way to run the program: 

Run the Emulator
1) Open the project2 Folder.
2) Run the make command to build the project.
3) In order to run the project insert the following command "./profemulator <Number Of Students> <Office Student limit>".
	Ex. ./profemulator 25 5
4)enjoy the show

Run the LKM
1) You will need 2 command line windows (1 to run the emulator and one to insert the LKM) In the demo I have the second window set up as an SSH.
2) The vm provided to us by the TA does not have the symbolic link for <linux/module.h> set up properly so make sure to set that up before hand or it will not compile.
3) in the first window go over to the project2 folder and run the command "./profemulator <Number Of Students> <Office Student limit>" preferable with a large amount of students to allow it run for a while.
4) in the second window go to the Modules folder inside of the project 2 folder and run the make command. (Make sure the emulator is still running before insertion).
5) run the command "sudo insmod proflkm.ko" to insert the module.
6) run the "dmesg" command to view the output.
7) run the "sudo rmmod proflkm.ko" command to remove the module.