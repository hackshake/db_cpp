#include<iostream>
#include<cstdlib>
#include<pthread.h>
#include<fstream>
using namespace std;
struct thread_data
{ 
  string city;
};
void *search_city(void *i)
{ 
struct thread_data *name;
name=(struct thread_data*) i;
int offset;
string line;
int count=0;int count1=0;
ifstream file("text.txt",ios::in);
if(file.is_open())
{
  while(!file.eof())
  {
	getline(file,line);
	size_t found=line.find(name->city);
	if(found!=string::npos)
	{
          cout<<name->city<<"found at:"<<count+1<<endl;	
	count1++;	
	}	
	count++;
  }
  if(count1==0)
   cout<<"not found"<<endl;



}
else
{ 
	cout<<"could not open the file"<<endl;
	
}


pthread_exit(NULL);
}
int main(int argc,char* argv[])
{

	int nthread=atoi(argv[1]);
	string cityname=string(argv[2]);
	struct thread_data td[nthread];
	

	cout<<nthread<<" "<<cityname<<endl;
	int rc;

 	 pthread_t threads[nthread];
	for(int i=0;i<nthread;i++)
	{
		td[i].city=cityname;
		rc=pthread_create(&threads[i],NULL,search_city,(void*)&td[i]);
		if(rc)
		{		
			cout<<"error"<<endl;
			exit(-1);
		}

	}
	pthread_exit(NULL);

return 0;
}
