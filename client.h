#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <sstream>
#include <limits.h>

#define SERVER_NUM 4
#define SERVER_A_UDP 21638
#define SERVER_B_UDP 22638
#define SERVER_C_UDP 23638
#define SERVER_D_UDP 24638
#define CLIENT_TCP 25638