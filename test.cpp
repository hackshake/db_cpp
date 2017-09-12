#include<iostream>
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
void display()
{
string query="select * from "+tablename+"";
const char* q=query.c_str();
qstate=mysql_query(conn,q);
if(!qstate)
{
res=mysql_store_result(conn);
while((row=mysql_fetch_row(res)))
{		cout<<"USerid:"<<row[0]<<endl;
 		cout<<"Username: "<<row[1]<<endl;
                cout<<"Email_id: "<<row[2]<<endl;
                cout<<"DATE OF BIRTH:"<<row[3]<<endl;
                cout<<"Mobile Number: "<<row[4]<<endl;	                    cout<<"Password: "<<row[5]<<endl;
                     cout<<'\n'; }
}
else
{cout<<"query execution problem"<<mysql_error(conn)<<endl; 
}
 }
void insert()
 {
            string userid,pwd,email_id,name,mob,date;
            cout<<"Enter the Username , Email_id , date of birth , Mobile Number , Password in the mentioned sequence"<<endl;
            cin>>name>>email_id>>date>>mob>>pwd;
            //inserting the data
            string query="insert into "+tablename+"(username,emailid,dob,mob,pass) values('"+name+"','"+email_id+"','"+date+"','"+mob+"','"+pwd+"')";
            const char* q=query.c_str();//converting string to const char as required in mysql_query(x,y) format

            qstate=mysql_query(conn,q);
            if(!qstate)
            {

                cout<<"record inserted succefully"<<endl;
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
            const char* q1=query1.c_str();//converting string to const char as required in mysql_query(x,y) format

            qstate=mysql_query(conn,q1);
            if(!qstate)
            {

                cout<<"record deleted successfully"<<endl;
            }
            else{
                cout<<"query execution problem"<<mysql_error(conn)<<endl;
            }



}
};
int main()
{
DB db;
string userid;
if(db.connect())
{
cout<<"Connect to database successfully"<<endl;
db.create();
int option;
while(option!=5)
{
cout<<"what do u want to do: 1.Display, 2.Insert, 3.Delete or 4.Update  and 5.Exit"<<endl;
cin>>option;
switch(option)
{
case 1:
cout<<"Displaying the data"<<endl;
db.display();
break;

case 2:
cout<<"inserting the data"<<endl;
db.insert();
break;

case 3:
cout<<"deleting the user"<<endl;
db.deleting();
break;

case 4:
cout<<"Give the User_id"<<endl;
cin>>userid;
db.update(userid);
break;

default:
cout<<"invalid option"<<endl;
break;

}


}

}
else
{
cout<<"database connection failed"<<endl;
}
return 0;

}

