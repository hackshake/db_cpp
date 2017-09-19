#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>
#include <iostream>
#include<mysql.h>
#include<typeinfo>
#include<sstream>
#include<locale>
#include<fstream>
using namespace std;
class DB
{


    MYSQL_ROW row;
    MYSQL_RES* res;
    int qstate;
    string tablename;

    public:
	 MYSQL* conn;



        MYSQL* connect()
        {
             this->conn=mysql_init(0);

    	if(conn)
        	cout<<"connection object ok!"<<endl;
    	else
        	cout << "connection object problem" <<mysql_error(conn)<< endl;

         this->conn=mysql_real_connect(conn,"localhost","abhi94","","db1",0,NULL,0);


         return this->conn;


        }

         void create()
        {

            cout<<"Mention the table name You want to create"<<endl;
            cin>>this->tablename;

            string query;
            query="create table IF NOT EXISTS "+tablename+"(userid int(6) NOT NULL AUTO_INCREMENT primary key,username varchar(30) NOT NULL,emailid varchar(50) NOT NULL,dob DATE NOT NULL,mob bigint(10) NOT NULL,pass varchar(20) NOT NULL,image blob)";

            const char* q=query.c_str();

            qstate=mysql_query(conn,q);



            if(!qstate)
            {
                cout<<tablename<<"table created successfully"<<endl;
            }
            else
                cout<<"query execution problem"<<mysql_error(conn)<<endl;

        }


void display()
{

            //displaying the data
            string query="select * from "+tablename+"";
             const char* q=query.c_str();
             qstate=mysql_query(conn,q);


             if(!qstate)
             {
                 res=mysql_store_result(conn);
                 while((row=mysql_fetch_row(res)))
                 {
                     cout<<"Userid: "<<row[0]<<endl;
                     cout<<"Username: "<<row[1]<<endl;
                     cout<<"Email_id: "<<row[2]<<endl;
                     cout<<"DATE OF BIRTH:"<<row[3]<<endl;
                     cout<<"Mobile Number: "<<row[4]<<endl;
                     cout<<"Password: "<<row[5]<<endl;
                     cout<<"image:"<<row[6]<<endl;
		    
                     cout<<'\n';

                 }

             }
             else
                cout<<"query execution problem"<<mysql_error(conn)<<endl;


}

void inserting(string name,string email_id,string date,string mob,string pwd,string img)
{

const char* i=img.c_str();
streampos size;
char* memblock;
ifstream file(i,ios::in|ios::binary|ios::ate);
size=file.tellg();
memblock=new char[size];
file.seekg(0,ios::beg);
file.read(memblock,size);
file.close();


            //inserting the data
            string query="insert into "+tablename+"(username,emailid,dob,mob,pass,image) values('"+name+"','"+email_id+"','"+date+"','"+mob+"','"+pwd+"','"+string(memblock)+"')";
            const char* q=query.c_str();

            qstate=mysql_query(conn,q);
            if(!qstate)
            {

                cout<<"record inserted successfully"<<endl;
            }
            else{
                cout<<"query execution problem"<<mysql_error(conn)<<endl;
            }


}
void update(string id)
{
	int option;
	const char* q;
	while(option!=6)
	{
		string query;
		string name,email_id,dob,mob,pass;
		cout<<"what do u want to update?  1.name ,2.email_id,3.dob ,4.mobile no.,5.password and 6.NO Updation"<<endl;
		cin>>option;
		switch(option)
		{
		case 1:
		cout<<"enter the new name"<<endl;cin>>name;
		query="update "+tablename+" set username='"+name+"' where userid='"+id+"'";
		q=query.c_str();
		qstate=mysql_query(conn,q);
		if(!qstate)
			cout<<"Updation done successfully"<<endl;
		else
			cout<<"query execution problem"<<mysql_error(conn)<<endl;
		break;

		case 2:
		cout<<"enter the new emaild_id"<<endl;cin>>email_id;
query="update "+tablename+" set emailid='"+email_id+"' where userid='"+id+"'";
		q=query.c_str();
		qstate=mysql_query(conn,q);
		if(!qstate)
			cout<<"Updation done successfully"<<endl;
		else
			cout<<"query execution problem"<<mysql_error(conn)<<endl;
		break;

		case 3:
		cout<<"enter the new date of birth(YYYY-MM-DD)"<<endl;cin>>dob;
		query="update "+tablename+" set dob='"+dob+"' where userid='"+id+"'";
		q=query.c_str();
		qstate=mysql_query(conn,q);
		if(!qstate)
		cout<<"Updation done successfully"<<endl;
		else
		cout<<"query execution problem"<<mysql_error(conn)<<endl;
		break;

		case 4:	
		cout<<"enter the new mobile number"<<endl;cin>>mob;
		query="update "+tablename+" set mob='"+mob+"' where userid='"+id+"'";
		q=query.c_str();
		qstate=mysql_query(conn,q);
		if(!qstate)
			cout<<"Updation done successfully"<<endl;
		else
			cout<<"query execution problem"<<mysql_error(conn)<<endl;
		break;

		case 5:
		cout<<"enter the new password"<<endl;cin>>pass;
		query="update "+tablename+" set pass='"+pass+"' where userid='"+id+"'";
		q=query.c_str();
		qstate=mysql_query(conn,q);
		if(!qstate)
			cout<<"Updation done successfully"<<endl;
		else
			cout<<"query execution problem"<<mysql_error(conn)<<endl;
		break;

		default:
		cout<<"invalid option"<<endl;

}

}
}

void deleting()
{


            //deleting a data
            string id;
            cout<<"Enter the data id to be deleted"<<endl;
            cin>>id;
            string query1="delete from "+tablename+" where userid='"+id+"'";
            const char* q1=query1.c_str();

            qstate=mysql_query(conn,q1);
            if(!qstate)
            {

                cout<<"record deleted successfully"<<endl;
            }
            else{
                cout<<"query execution problem"<<mysql_error(conn)<<endl;
            }



}
void insertfromxml()
{
string name,email_id,dob,mob,pass;
string img="";

    xmlpp::TextReader reader("example.xml");
int d=0;int count=0;int count1=0;
string n=" ";
    while(reader.read())
    { 

if(reader.get_name()=="data")
{
++count1;

}
if(count1==2)
break;



 	
       if(reader.get_name()=="user")
        { d=reader.get_depth();
	++count;
         if(count==2)
{count=0;

inserting(name,email_id,dob,mob,pass,img);
         }
         continue;
        } 

 if(d>reader.get_depth())
{ d--;
        continue;}


        if(d<reader.get_depth() && reader.get_name()=="#text")
{ 
if(n!=" ")
{ 

cout<<"got in"<<endl;
if(n=="name"){name=reader.get_value();n=" ";
continue; }
else if(n=="emailid"){email_id=reader.get_value(); }
else if(n=="dob"){dob=reader.get_value(); }
else if(n=="mob"){mob=reader.get_value(); }
else if(n=="pass"){pass=reader.get_value(); }
else
{
d=reader.get_depth();
continue; 

  }
 }
}

if(d<=reader.get_depth() && (reader.get_name()=="name" || reader.get_name()=="emailid" || reader.get_name()=="dob" ||  reader.get_name()=="mob" || reader.get_name()=="pass") )
{ n=reader.get_name();  
d=reader.get_depth();
continue; }
 


}
}

    

};

 bool is_digits(string mob)
{
    return mob.find_first_not_of("0123456789")==string::npos;

}
bool validate_mob(string mob)
{

 if(is_digits(mob) && mob.size()==10 && mob[0]!='0' )
 {
     return true;

 }
 else
    return false;


}

bool validate_name(string name)
{

    locale loc;

    for(string::iterator it=name.begin();it!=name.end();++it)
    {


        if(!isalpha(*it,loc))
            {
                cout<<"error";
                return false;
            }
    }
    return true;

}
bool validate_date(string date)
{
    if(date.size()!=10)
        return false;

    if(date[4]!='-' && date[7]!='-')
        return false;


    string year=date.substr(0,4);
    string month=date.substr(5,2);
    string day=date.substr(8,2);

    string m = month;
    string d=day;

    stringstream geek(m);

    int x = 0;
    geek >> x;

    if(x<=0 || x>12)
        return false;


    stringstream geek1(d);
    x=0;
    geek1>>x;

    if(x<=0 || x>31)
        return false;

return true;
}


bool validate_emailid(string email)
{
    int size=email.size();
    string com=email.substr(size-4,4);
    if(com!=".com")
     return false;



    size_t found=email.find('@');

    //string com=email_id.substr()
    if (found==std::string::npos)
        return false;

    return true;



}
bool validate_pass(string name)
{

    locale loc;

    for(string::iterator it=name.begin();it!=name.end();++it)
    {


    if(!isalnum(*it,loc))
    {
        cout<<"error";
        return false;
    }
    }
    return true;

}

int main()
{
    DB db;
string userid,pwd,email_id,name,mob,date,img;




        if(db.connect())
            {
            cout<<"database connected successfully"<<endl;

   db.create();

            int option;
            while(option!=6)
            {



            cout<<"What do you want to do : 1.Display,2.Insert,3.Delete, 4.update or 5.Insert_from_XML  and 6.Exit"<<endl;
            cin>>option;


            switch(option)
            {
            case 1:
                cout<<"Displaying the data"<<endl;
                db.display();
                break;

            case 2:
             cout<<"insert data"<<endl;
            start:
            cout<<"Enter the Username , Email_id , date of birth , Mobile Number , Password,image_file_name in the mentioned sequence"<<endl;
           cin>>name>>email_id>>date>>mob>>pwd>>img;   
            if(!validate_name(name))
            {
                cout<<"enter a correct user name"<<endl;
                goto start;
            }
            if(!validate_emailid(email_id))
            {
                cout<<"enter a correct email_id"<<endl;
                goto start;
            }
            if(!validate_date(date))
            {
                cout<<"enter a correct date in the given format(YYYY-MM-DD)"<<endl;
                goto start;
            }
            if(!validate_pass(pwd))
            {
                cout<<"enter a valid password"<<endl;
                goto start;
            }
            if(!validate_mob(mob))
            {
                cout<<"enter a correct 10 digit mobile number"<<endl;
                goto start;
            }
 
           
            db.inserting(name,email_id,date,mob,pwd,img);
            break;

            	case 3:
                cout<<"delete a data"<<endl;
                db.deleting();
                break;
                
		case 4:
		cout<<"Give the User_id"<<endl;
		cin>>userid;
		db.update(userid);
		break;

            	case 5:
                cout<<"insert from Xml"<<endl;
                db.insertfromxml();
                break;

            default:
                cout<<"Invalid Option"<<endl;



            }
            }
            }
             else
            cout<<"database connection failed"<<endl;

            mysql_close(db.conn);



    return 0;


}

