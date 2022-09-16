#include "tcpClient.h"
#include <iostream>

tcp_client::tcp_client()
{
    sock = -1;
    port = 4014;
    address = "192.168.3.39";
}
 
/**
    Connect to a host on a certain port number
*/
bool tcp_client::conn(string address , int port)
{

	this->address=address;
	this->port=port;

	
	//create socket if it is not already created

    if(true)//sock == -1)
    {
        //Create socket
        sock = socket(AF_INET , SOCK_STREAM , 0);
        if (sock == -1)
        {
            cout<<"Could not create socket";
            opened=false;
            return false;
        }
        struct timeval tv;
        tv.tv_sec=1;
        tv.tv_usec=0;
        setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(const char*)&tv,sizeof(tv));
        int t=1;
        setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&t,sizeof(t));

        printf("Socket created\n");
    }
    else    {   /* OK , nothing */  }
     
    //setup address structure
    if(inet_addr(address.c_str()) == -1)
    {
        struct hostent *he;
        struct in_addr **addr_list;
         
        //resolve the hostname, its not an ip address
        if ( (he = gethostbyname( address.c_str() ) ) == NULL)
        {
            //gethostbyname failed
            herror("gethostbyname");
            cout<<"Failed to resolve hostname\n";
            opened=false;
             
            return false;
        }
         
        //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
        addr_list = (struct in_addr **) he->h_addr_list;
 
        for(int i = 0; addr_list[i] != NULL; i++)
        {
            //strcpy(ip , inet_ntoa(*addr_list[i]) );
            server.sin_addr = *addr_list[i];
             
            cout<<address<<" resolved to "<<inet_ntoa(*addr_list[i])<<endl;
             
            break;
        }
    }
     
    //plain ip address
    else
    {
        server.sin_addr.s_addr = inet_addr( address.c_str() );
    }
     
    server.sin_family = AF_INET;
    server.sin_port = htons( port );
     
    //Connect to remote server

    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        cout<<"connect failed. Error\n";
        disconn();
        opened=false;
        return 0;
    }
     
    printf("Connected\n");
	
	opened=true;
    
    
    return true;
}
 
 bool tcp_client::disconn(int p){
	 opened=false;
    // close(sock);

 shutdown(sock,p);
 cout<<"Socket closed\n";
// sock=-1;
 return true;
 }
 
 
/**
    Send data to the connected host
*/
bool tcp_client::send_data(unsigned char *data, unsigned int size)
{
    //Send some data

    if(opened)
    if( send(sock , data , size , 0) < 0 && opened)
    {
        perror("Send failed : ");
        disconn();
        return false;
    }
    cout<<"Data send\n";
     
    return true;
}

bool tcp_client::send_data(char *data, unsigned int size)
{

    //Send some data
    //
    if(opened)
    if( send(sock , data , size , 0) < 0)
    {
        perror("Send failed : ");
        disconn();
        return false;
    }
    //cout<<"Data send\n";
     
    return true;
}
 
/**
    Receive data from the connected host
*/
void tcp_client::delay(long value){
for(long i=0;i<value;i++);
}

vector<unsigned char> tcp_client::receive(int size=512)
{
    unsigned char buffer[size];
	unsigned int sizeRecv;
   // string reply;
    //Receive a reply from the server
    sizeRecv=recv(sock , buffer , sizeof(buffer) , 0);
    if( sizeRecv<=0 || sizeRecv>size )
    {
		
        puts("recv failed");
        cout<<"recv() failed, trying to reconnect"<<endl; 
        //opened=false;
         disconn();
    //delay(500000);
   // reconnect();
    return vector<unsigned char>(buffer,buffer+1);
    }
    
    opened=true;
    // vector<unsigned char> temp(buffer,buffer+sizeRecv);
    
    return vector<unsigned char>(buffer,buffer+sizeRecv);
}

vector<unsigned char> tcp_client::receiveNoReconnect(int size)
{
    unsigned char buffer[size];
    unsigned int sizeRecv;
   // string reply;
    //Receive a reply from the server
    sizeRecv=recv(sock , buffer , sizeof(buffer) , MSG_DONTWAIT);
    if( sizeRecv<=0 || sizeRecv>size )
    {

        //puts("recv failed");
        //cout<<"recv() failed, trying to reconnect"<<endl;
        //opened=false;
        // disconn();
    //delay(500000);
   // reconnect();
    return vector<unsigned char>(buffer,buffer+1);
    }

    opened=true;
    // vector<unsigned char> temp(buffer,buffer+sizeRecv);

    return vector<unsigned char>(buffer,buffer+sizeRecv);
}

bool tcp_client::isOpen(){

return opened;
}


bool tcp_client::reconnect(){


     disconn();
    //setup address structure
    if(inet_addr(address.c_str()) == -1)
    {
        struct hostent *he;
        struct in_addr **addr_list;
         
        //resolve the hostname, its not an ip address
        if ( (he = gethostbyname( address.c_str() ) ) == NULL)
        {
            //gethostbyname failed
            herror("gethostbyname");
            cout<<"Failed to resolve hostname\n";
             
            return false;
        }
         
        //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
        addr_list = (struct in_addr **) he->h_addr_list;
 
        for(int i = 0; addr_list[i] != NULL; i++)
        {
            //strcpy(ip , inet_ntoa(*addr_list[i]) );
            server.sin_addr = *addr_list[i];
             
            cout<<address<<" resolved to "<<inet_ntoa(*addr_list[i])<<endl;
             
            break;
        }
    }
     
    //plain ip address
    else
    {
        server.sin_addr.s_addr = inet_addr( address.c_str() );
    }
     
    server.sin_family = AF_INET;
    server.sin_port = htons( port );
     
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        opened=false;
       // disconn();
		return 1;
    }
     
    cout<<"Connected\n";
	opened=true;
    return true;
}

