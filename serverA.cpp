/*
*EE450 Final Project - Socket Programming
*Name: Yingjie He
*USCID: 9776272638
*NetID: yingjieh
*/

#include "serverA.h"

using namespace std;


int adjacency_matrix[SERVER_NUM][SERVER_NUM];
int portNumTCP;
char* localIP;
string send_str;

void booting_up_A() {
	cout << "The Server A is up and running." << endl;
    //Initialize the adjacency matrix
	for (int i = 0; i < SERVER_NUM; i++) {
		for (int j = 0; j < SERVER_NUM; j++) {
			adjacency_matrix[i][j] = 0;
		}
	}
    //Get the localhost IP
    hostent* localHost = gethostbyname("localhost");
    localIP = inet_ntoa(*(struct in_addr *)*localHost->h_addr_list);
    cout << '\n';
}

//Read serverA.txt to get serverA's neighbor information
void readfileA() {
    string str;
    int lines_index = 0;
	ifstream readfile("serverA.txt", ios::in); //load file
	if (!readfile){ //check if the file is loaded
		cout << "Cannot Open File" << endl;
		return;
	}
	cout << "The Server A has the following neighbor information :" << '\n' << "Neighbor---Cost" << endl;
	send_str.append("A/");
    //Deal with the file line by line, and print its neighbor's information
    while (getline(readfile, str))
	{
        int loc = str.find(" ");
		string str1 = str.substr(0, loc);
		string str2 = str.substr(loc + 1);
		cout << str1 << "     " << str2 << endl;
		int cost = atoi(str2.c_str());
		if (str1 == "serverB") {
			adjacency_matrix[1][0] = cost;
			adjacency_matrix[0][1] = cost;
            send_str.append("B/");
		}
		else if (str1 == "serverC") {
			adjacency_matrix[2][0] = cost;
			adjacency_matrix[0][2] = cost;
            send_str.append("C/");
		}
		else if (str1 == "serverD") {
			adjacency_matrix[3][0] = cost;
			adjacency_matrix[0][3] = cost;
            send_str.append("D/");
		}
        stringstream ss;
        ss << cost;
        string str_int = ss.str();
        send_str.append(str_int + "/");
	}
    cout << '\n';
}

//Set up TCP connection with client
void setupTCP(){
    int server;
    struct sockaddr_in server_addr, myaddr;
    socklen_t size = sizeof(server_addr);
    
    //Create a socket to send 
    server = socket(AF_INET, SOCK_STREAM, 0);
    if(server < 0){
        cout << "Error: Cannot establish socket." << endl;
        exit(1);
    }
    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(CLIENT_TCP);
    server_addr.sin_addr.s_addr = inet_addr(localIP);

    //Connect with client and check whether it is successful
    if(connect(server,(struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) != 0){
        cout << "Error: Cannot connect to server." << endl;
        exit(1);
    }

    //Send string to client and check whether it is successful
    if(send(server, send_str.c_str(), send_str.length(), 0) < 0){
        cout << "Error: Cannot send." << endl;
        exit(1);
    }

    memset((char *) &myaddr, 0, sizeof(myaddr));

    //Get the dynamic TCP port number
    if(getsockname(server, (struct sockaddr*)&myaddr, &size) < 0){
        cout << "Error: Cannot get socket port number." << endl;
        exit(1);
    }
    portNumTCP = ntohs(myaddr.sin_port);
    string ip = inet_ntoa(myaddr.sin_addr);
    cout << "The Server A finishes sending its neighbor information to the Client with TCP port number " << CLIENT_TCP << " and IP address " << localIP << endl;
    cout << "For this connection with the Client, the Server A has TCP port number "<< portNumTCP << " and IP address "<< ip << endl;
    cout << '\n';
    close(server);
}

//print out the information of the network of topology
void getFromServer(string input){
    istringstream ss(input);
    string token;
    string server;
    cout << "Edge-----Cost" << endl;
    while(getline(ss, token, '/')){
        int loc = token.find(" ");
        string str1 = token.substr(0, loc);
        string str2 = token.substr(loc + 1);
        cout << " " <<str1 << "       " << str2 << endl;
    }
}

//Setup UDP connection
void setupUDP(){
    int server;
    int buffersize = 256;
    char* buffer = new char[buffersize]();
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t client_addr_length = sizeof(client_addr);
    
    //Create a socket and check whether it is successful
    server = socket(AF_INET, SOCK_DGRAM, 0);
    if(server < 0){
        cout << "Error: Cannot establish socket." << endl;
        exit(1);
    }
    
    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_A_UDP);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(bind(server,(struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) < 0){
        cout << "Error: Cannot bind." << endl;
        exit(1);
    }

    //Recieve socket from client
    if(recvfrom(server, buffer, buffersize, 0, (struct sockaddr*)&client_addr, &client_addr_length) < 0){
        cout << "Error: Cannot receive." << endl;
        exit(1);
    }
    
    int port_num = ntohs(client_addr.sin_port);
    string ip = inet_ntoa(client_addr.sin_addr);
    cout << "The server A has received the network topology from the Client with UDP port number "<< port_num << " and IP address "<< ip <<" as follows:" << endl;
    string str(buffer);
    getFromServer(str);
    close(server);
    cout << "For this connection with Client, The Server A has UDP portnumber " << SERVER_A_UDP << " and IP address " << localIP << endl;
}

int main(){
	/*Create a Socket for incoming connections
	port # = 21000+638 = 21638*/
	booting_up_A();
	readfileA();
	setupTCP();
    setupUDP();
	return 0;
}

