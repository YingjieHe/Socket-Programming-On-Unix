# Socket-Programming-On-Unix
Description
In this project, I setup four servers and one client.The servers communicate with the client using TCP sockets. And the client communicate with the servers using UDP sockets. All the four servers load their own file to get their neighbour's information and send to client. And the client who is going to initiate the process and also do some
computation to give the final topology output. The client and servers are going to communicate with each other to get the neighbor informations from the others and combine all this information together to build the entire map of the network. The original map is a connected graph. Based on the map, the client calculates the minimum spanning tree (MST) of the network using Prim's aldorithm and print it out on the terminal.

Files
1) client.cpp: Create TCP server and listen to socket from four servers, and gain the information then send the information to four servers by using UDP. And more, the client calculate the network's Minimum Spanning Tree.

2) client.h: Contains all the needed hearder file of client and define some arguments.

3) serverA.cpp: Load the neighbour's information from file, and send the information to client by TCP socket. Then create UDP server and receive socket from client.

4) serverA.h: Contains all the needed hearder file of serverA and define some arguments.

5) serverA.txt: Contains the neighbour's information of serverA.

6) serverB.cpp: Load the neighbour's information from file, and send the information to client by TCP socket. Then create UDP server and receive socket from client.

7) serverB.h: Contains all the needed hearder file of serverB and define some arguments.

8) serverB.txt: Contains the neighbour's information of serverB.

9) serverC.cpp: Load the neighbour's information from file, and send the information to client by TCP socket. Then create UDP server and receive socket from client.

10) serverC.h: Contains all the needed hearder file of serverC and define some arguments.

11) serverC.txt: Contains the neighbour's information of serverC.

12) serverD.cpp: Load the neighbour's information from file, and send the information to client by TCP socket. Then create UDP server and receive socket from client.

12) serverD.h: Contains all the needed hearder file of serverD and define some arguments.

13) serverD.txt: Contains the neighbour's information of serverD.

14) makefile: Just run "make -f" to compile all the programs.

Format of massages
Use the buffer with a type of char[], and the information is in the follow format:
"A/B 20/C 15", which mans the message comes from serverA, and its neighbors'information
is serverB and serverC, and costs are 20 and 15,

Running Order
1) Run "make -f makefile" to compile all the codes. It will show warnnings, but it dose not matter
2) Run "./clientoutput" to start TCP server.
(Step 3 to Step 6 can be run in any order)
3) Run "./serverAoutput" to srart serverA and send information to client.
4) Run "./serverBoutput" to srart serverB and send information to client.
5) Run "./serverCoutput" to srart serverC and send information to client.
6) Run "./serverDoutput" to srart serverD and send information to client.

