#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include<iostream>    //cout
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<string>  //string
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<netdb.h> //hostent
#include <vector>
#include<unistd.h>
#include <QTextStream>

using namespace std;
 

class tcp_client
{
private:
    int sock;
    std::string address;
    int port;
    struct sockaddr_in server;
     bool opened;
public:
    tcp_client();
    bool conn(string, int);
    bool disconn(int p=1);
    bool send_data(unsigned char *buf,unsigned int size);
    bool send_data(char *buf,unsigned int size);
	void delay(long value);
    vector<unsigned char> receive(int);
    vector<unsigned char> receiveNoReconnect(int);
	bool isOpen();
	bool reconnect();
};
 

#endif
