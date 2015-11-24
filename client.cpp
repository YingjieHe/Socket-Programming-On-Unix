/*
*EE450 Final Project - Socket Programming
*Name: Yingjie He
*USCID: 9776272638
*NetID: yingjieh
*/

#include "client.h"

using namespace std;


int adjacency_matrix[SERVER_NUM][SERVER_NUM];
int portNumTCP;
char* localIP;
int UDP_num[] = {SERVER_A_UDP, SERVER_B_UDP, SERVER_C_UDP, SERVER_D_UDP};
string server_name[] = {"server A", "server B", "server C", "server D"};

void booting_up_Client() {
    portNumTCP = CLIENT_TCP;
    //Get the localhost IP
    hostent* localHost = gethostbyname("localhost");
    localIP = inet_ntoa(*(struct in_addr *)*localHost->h_addr_list);
    //Initialize the adjacency matrix
    for (int i = 0; i < SERVER_NUM; i++) {
        for (int j = 0; j < SERVER_NUM; j++) {
            adjacency_matrix[i][j] = 0;
        }
    }
	cout << "The Client has TCP port number: " << portNumTCP << " and IP address: " << localIP << endl;
    cout << endl;
}

//Decode the string from serverX, determining where the information comes from
//The format of the string is, e.g. A/B/20/C/15, which means the string comes from
//serverA, and its information is that serverA connects with serverB and serverC, 
//and the costs are 20 for B, 15 for C  
void getFromServer(string input, string ip, int port_num){
    istringstream ss(input);
    string token;
    int count = 0;
    int index, index_after;
    string server;
    while(getline(ss, token, '/')) {
        //Determine where the string comes from
        if(count == 0){
            if(token == "A"){
                server = "Server A";
                index = 0;
            }
            else if(token == "B"){
                server = "Server B";
                index = 1;
            }
            else if(token == "C"){
                server = "Server C";
                index = 2;
            }
            else if(token == "D"){
                server = "Server D";
                index = 3;
            }
            cout << "The Client receivers neighbor information from the " << server << " with TCP port number "<<port_num << " and IP address "<< ip << endl;
            cout << "The " << server << " has the following neighbor information :" << '\n' << "Neighbor---Cost" << endl;
        }
        //Get the serverX's information
        else{
            int cost = atoi(token.c_str());
            // if cost is less than or equals 0, it means the character at position count is a letter, else a number
            if(cost <= 0){
                if(token == "A"){
                    index_after = 0;
                    cout << "serverA" << "     ";
                }
                else if(token == "B"){
                    index_after = 1;
                    cout << "serverB" << "     ";
                }
                else if(token == "C"){
                    index_after = 2;
                    cout << "serverC" << "     ";
                }
                else if(token == "D"){
                    index_after = 3;
                    cout << "serverD" << "     ";
                }
            }
            else{
                cout << cost << endl;
                adjacency_matrix[index][index_after] = cost;
                adjacency_matrix[index_after][index] = cost;
            }
        }
        count++;
    }
    cout << "For this connection with "<< server << ", The Client has TCP port number " << portNumTCP <<" and IP address "<< localIP << endl;
}

// Setup TCP to listen socket from serverX
void setupTCP(){
    int server_num = 0;
    int server, client;
    int buffersize = 256;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t sizeA = sizeof(server_addr);
    
    //Create a socket
    server = socket(AF_INET, SOCK_STREAM, 0);
    if(server < 0){
        cout << "Error: Cannot establish socket." << endl;
        exit(1);
    }
    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNumTCP);
    
    //Bind the socket and check whether it is successful
    if(bind(server,(struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) < 0){
        cout << "Error: Cannot bind." << endl;
        exit(1);
    }

    //Listen to incomming socket
    if(listen(server, 1) < 0){
        cout << "Error: Cannot listen." << endl;
        exit(1);
    }
    
    //Use while loop to get all four servers' socket
    while(server_num < SERVER_NUM){
        client = accept(server, (struct sockaddr*)&client_addr, &sizeA);
        if(client < 0){
            cout << "Error: Cannot accept." << endl;
            continue;
        }

        //Recieve socket form servers
        char* buffer = new char[buffersize]();
        if(recv(client, buffer, buffersize, 0) < 0){
            cout << "Error: Cannot receive." << endl;
            continue;
        }
        else{
            server_num++;
        }
        
        //Get the servers' dynamic TCP port number
        if(getpeername(client, (struct sockaddr*)&client_addr, &sizeA) < 0){
            cout << "Error: Cannot get socket port number." << endl;
            exit(1);
        }
        int port_num = ntohs(client_addr.sin_port);
        string ip = inet_ntoa(client_addr.sin_addr);

        //Convert char array to string so that it can be used for next step
        string str(buffer);
        getFromServer(str, ip, port_num);
        close(client);
        
        
    }
}

//Construct the string which will be send to servers
//the format of the string is, e.g. A/AB 20/CD 15, which means the destination is serverA, and the topolopy
//information is A connects with B and cost is 20, and C connects with D and cost is 15
string sender(){
    string sender;
    //Convert adjacency matrix to string
    for (int i = 0; i < SERVER_NUM; i++) {
        for (int j = i; j < SERVER_NUM; j++) {
            if(adjacency_matrix[i][j] != 0){
                if(i == 0){
                    if(j == 1){
                        sender.append("AB ");
                    }
                    else if(j == 2){
                        sender.append("AC ");
                    }
                    else if(j == 3){
                        sender.append("AD ");
                    }
                }
                else if(i == 1){
                    if(j == 2){
                        sender.append("BC ");
                    }
                    else if(j == 3){
                        sender.append("BD ");
                    }
                }
                else if(i == 2){
                    if(j == 3){
                        sender.append("CD ");
                    }
                }
                stringstream ss;
                ss << adjacency_matrix[i][j];
                string str_int = ss.str();
                sender.append(str_int + "/");
            }
        }
    }
    return sender;
}

//Print the cost of each edges
void translate(string input){
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
    int client, number = 0;
    //Initialize the string which will be sent to servers
    string send_str = sender();
    while(number < SERVER_NUM){
        struct sockaddr_in server_addr, myaddr;
        socklen_t size = sizeof(server_addr);
        
        client = socket(AF_INET, SOCK_DGRAM, 0);
        if(client < 0){
            cout << "Error: Cannot establish socket." << endl;
            exit(1);
        }

        memset((char *) &server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(UDP_num[number]);
        server_addr.sin_addr.s_addr = inet_addr(localIP);
        
        //Send socket to four servers
        if(sendto(client, send_str.c_str(), send_str.length(), 0, (struct sockaddr*)&server_addr,sizeof(server_addr)) < 0){
            cout << "Error: Cannot send." << endl;
            exit(1);
        }
        cout << "The Client has sent the network topology to the network topology to the " << server_name[number] <<" with UDP port number "<< UDP_num[number] <<" and IP address "<< localIP <<" as follows:" << endl;
        translate(send_str);
        memset((char *) &myaddr, 0, sizeof(myaddr));
        if(getsockname(client, (struct sockaddr*)&myaddr, &size) < 0){
            cout << "Error: Cannot get socket port number." << endl;
            exit(1);
        }
        portNumTCP = ntohs(myaddr.sin_port);
        close(client);
        cout << "For this connection with "<< server_name[number] <<", The Client has UDP port "<< portNumTCP <<" and IP address " << localIP << endl;
        number++;
        //To make sure all servers have set up UDP connection
        usleep(500000);
    }
}


//Prim's Algorithms to get MST， learn from YouTube
void MST(){
    int MSTNode[SERVER_NUM]; //Store the MST node
    int minedge[SERVER_NUM]; //Store the minimum cost edge
    int cost = 0;
    bool mstSet[SERVER_NUM]; //Show whther a node is in MST
    for(int i = 0; i < SERVER_NUM; i++){
        minedge[i] = INT_MAX;
        mstSet[i] = false;
    }
    minedge[0] = 0;  
    MSTNode[0] = -1; 
    for(int count = 0; count < SERVER_NUM - 1; count++){
        int u, min = INT_MAX;
        //Find the minimum cost edge
        for (int i = 0; i < SERVER_NUM; i++)
            if(mstSet[i] == false && minedge[i] < min){
                u = i;
                min = minedge[i];
            }
        mstSet[u] = true;
        //Find the node in MST
        for(int v = 0; v < SERVER_NUM; v++){
            if(adjacency_matrix[u][v] != 0 && mstSet[v] == false && adjacency_matrix[u][v] <  minedge[v]){
                MSTNode[v]  = u;
                minedge[v] = adjacency_matrix[u][v];
            }
        }
    }
    for(int i = 1; i < SERVER_NUM; i++){
        cost += adjacency_matrix[i][MSTNode[i]];
    }

    //Print the MST 
    cout << "The Client has calculated a tree. The tree cost is " << cost << " :" << endl;
    cout << "Neighbor---Cost" << endl;
    for(int i = 1; i < SERVER_NUM; i++){
        string server;
        if(i == 1){
            if(MSTNode[i] == 0){
                server.append("AB");
            }
            else if(MSTNode[i] == 2){
                server.append("BC");
            }
            else if(MSTNode[i] == 3){
                server.append("BD");
            }
        }
        else if(i == 2){
            if(MSTNode[i] == 0){
                server.append("AC");
            }
            else if(MSTNode[i] == 1){
                server.append("BC");
            }
            else if(MSTNode[i] == 3){
                server.append("CD");
            }
        }
        else if(i == 3){
            if(MSTNode[i] == 0){
                server.append("AD");
            }
            else if(MSTNode[i] == 1){
                server.append("BD");
            }
            else if(MSTNode[i] == 2){
                server.append("CD");
            }
        }
        cout << "   " << server << "       " << adjacency_matrix[i][MSTNode[i]] << endl;
    }
}

int main(){
	booting_up_Client();
	setupTCP();
    usleep(500000);
    setupUDP();
    MST();
	return 0;
}

