#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include<mysql/mysql.h>
using namespace std;
class DB
{
	MYSQL* conn;
	MYSQL_ROW row;
	MYSQL_RES* res;
	int qstate;
	string tablename;
	public:
            
	MYSQL* connect()
	{
		this->conn=mysql_init(0);
		if(conn)
		{cout<<"connection object OK!"<<endl;}
		else
		{
			cout<<"connection Object problem!"<<mysql_error(conn)<<endl;}
		this->conn=mysql_real_connect(conn,"localhost","root","m!lkshake94","db1",0,NULL,0);

		return this->conn;
	}

	void create()
	{
		cout<<"mention the tablename You wanna create"<<endl;
		cin>>this->tablename;
		string query;
		query="create table if not exists "+tablename+"(userid int(6) not null auto_increment primary key,username varchar(30) not null,emailid varchar(50) not null,dob date not null,mob bigint(10) not null,pass varchar(20) not null)";
		const char* q=query.c_str();
		qstate=mysql_query(conn,q);
		if(!qstate)
		{
			cout<<tablename<<" table created successfully"<<endl;}
		else
		{
			cout<<"query execution problem"<<mysql_error(conn)<<endl;}
	}
	string display(string id)
	{
		string pass="";
		string query="select * from "+tablename+" where userid='"+id+"' ";
		const char* q=query.c_str();
		qstate=mysql_query(conn,q);
		if(!qstate)
		{
			res=mysql_store_result(conn);
			while((row=mysql_fetch_row(res)))
			{               cout<<"USerid:"<<row[0]<<endl;
				cout<<"Username: "<<row[1]<<endl;
				cout<<"Email_id: "<<row[2]<<endl;
				cout<<"DATE OF BIRTH:"<<row[3]<<endl;
				cout<<"Password: "<<row[5]<<endl;
				cout<<'\n';
				for(int i=1;i<5;i++)
				{
					pass=pass+row[i]+" ";
				}
				
				
				
				
 			}
		}
		else
		{cout<<"query execution problem"<<mysql_error(conn)<<endl;
		}
		return pass;
	}
};

 bool is_digits(string mob)
{
    return mob.find_first_not_of("0123456789")==string::npos;

}


//Server side
int main(int argc, char *argv[])
{
	DB db;
	//for the server, we only need to specify a port number
	if(argc != 2)
	{
		cerr << "Usage: port" << endl;
		exit(0);
	}
	//grab the port number
	int port = atoi(argv[1]);
	//buffer to send and receive messages with
	char msg[1500];int i;
	int opt=true;
	int activity,sd,new_socket;
	//set socket descriptors
	int max_sd;//highest no. file descriptors
	fd_set readfds;
	fd_set master;

	sockaddr_in newSockAddr;
	socklen_t newSockAddrSize = sizeof(newSockAddr);
        int client[10];
	int max_client=10;
	for(i=0;i<max_client;i++)
	{client[i]=0;}


	//setup a socket and connection tools
	sockaddr_in servAddr;
	bzero((char*)&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = INADDR_ANY;
	servAddr.sin_port = htons(port);
	int bytesRead;
	//open stream oriented socket with internet address
	//also keep track of the socket descriptor
	int master_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(master_socket < 0)
	{
		cerr << "Error establishing the server socket" << endl;
		exit(0);
	}

	//set master socket to allow multiple connections
	if(setsockopt(master_socket,SOL_SOCKET,SO_REUSEADDR,(char*)&opt,sizeof(opt))<0)
	{ cerr<<"setsockopt error"<<endl;	
		exit(0);}

	//bind the socket to its local address
	int bindStatus = bind(master_socket, (struct sockaddr*) &servAddr,sizeof(servAddr));
	if(bindStatus < 0)
	{
		cerr << "Error binding socket to local address" << endl;
		exit(0);
	}
	cout << "Waiting for a client to connect...on " <<port<< endl;
	//listen for up to 5 requests at a time
	if(listen(master_socket, 5)<0)
	{
		cerr<<"listening error"<<endl;exit(0);
	}
	FD_ZERO(&master);
	FD_ZERO(&readfds);
	FD_SET(master_socket,&master);
	max_sd=master_socket;
        
	cout<<"waiting for connections"<<endl;
	while(true)
	{
		//copy
		readfds=master;/*
		for(i=0;i<max_client;i++)
		{ 
			sd=i;
			if(sd>0)
			{FD_SET(sd,&readfds);}
			if(sd>max_sd)
				max_sd=sd;
		}*/
			
		activity=select(max_sd+1,&readfds,NULL,NULL,NULL);
		if(activity<0)
			cerr<<"select error"<<endl;

		cout<<"server-select is ok"<<endl;

		for(i=0;i<=max_sd;i++)
		{ cout<<i<<master_socket<<endl;
			if(FD_ISSET(i,&readfds))
			{
				if(i==master_socket)
				{
					//handle new connection

					int new_socket = accept(master_socket, (sockaddr *)&newSockAddr, &newSockAddrSize);

					if(new_socket< 0)
					{
						cerr << "Error accepting request from client!" << endl;
						exit(1);
					}
					else
					{
						cout<<"accept is ok"<<endl;
						FD_SET(new_socket,&master);

						if(new_socket>max_sd)
							max_sd=new_socket;

						cout << "Connected with client! whose socket fd is " <<new_socket<<",ip is "<<inet_ntoa(newSockAddr.sin_addr)<<",port is"<<ntohs(newSockAddr.sin_port)<< endl;
					}
				}
				else
				{
					//handle the data from client

					memset(&msg, 0, sizeof(msg));//clear the buffer
					bytesRead = recv(i, (char*)&msg, sizeof(msg), 0);
					if(!strcmp(msg, "exit"))
					{
						cout << "Client with ip:"<<inet_ntoa(newSockAddr.sin_addr)<<"and port:"<<ntohs(newSockAddr.sin_port)<<"has quit the session"<<endl;
						close(i);

						FD_CLR(i,&master);
					}
					else
					{
						cout << "Client: " << msg << endl;
						cout << ">";
						string data;
						data=string(msg);
						if (data.find("display") != string::npos)
						{
							if(is_digits(data.substr(data.size()-1,1)))
							{
								string n=data.substr(data.size()-1,1);
								db.connect();
								db.create();
								data=db.display(n);
								memset(&msg, 0, sizeof(msg)); //clear the buffer
								strcpy(msg, data.c_str());

								if( send(i, (char*)&msg, strlen(msg), 0)<0)
								{cerr<<"sending error"<<endl;exit(0);}

								continue;
							}

						}

						getline(cin,data);
						memset(&msg, 0, sizeof(msg)); //clear the buffer
						strcpy(msg, data.c_str());
						if(data == "exit")
						{
							//send to the client that server has closed the connection
							send(i, (char*)&msg, strlen(msg), 0);
							exit(0);
						}
						//send the message to client
						if( send(i, (char*)&msg, strlen(msg), 0)<0)
						{	cerr<<"send error"<<endl;exit(0);
						}


					}
				 }


			}


		}


	}			



	return 0;
}	
