This program was compiled and ran on flip2 at access.engr.oregonstate.edu.
Follow the instructions below to run this program on flip. 

1. Place chatclient.c and chatserve.py in the same directory.
2. Compile chatclient.c with the following command: 
	
	gcc -o chatclient chatclient.c
	
3. In a new window start chatserve with the following command: 

	python chatserve.py [port number]

4. In the original window, start chatclient with the following command:

	chatclient [hostname] [port number]
	
	Note: you should use localhost if you are following these instructions. 
	port number must be the port number you used when starting chatserve
	
5. Follow onscreen prompts from each program to alternate sending/receiving 
   messages. 

6. Either program can terminate the chat when it is it's turn to type by 
   entering "\quit". chatclient will then terminate and chatserve will 
   continue to listen on the original port.
