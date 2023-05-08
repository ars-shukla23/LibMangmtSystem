#include<iostream>
#include<string>
#include<fstream>
#include<vector>

using namespace std;



bool admin_login_flag=false;

class User{
    
    string password;
    public:
    string user_id;
    int user_type;
    //int find_username_password(string userid,string password);
    int find_id_passwd(string userid,string password,string filename);
    void create_account();
    void user_login();
};

void User::create_account(){
      string temp;
      cout<<"Enter a user_id of your choosing:";
      cin>>user_id;
      cout<<"Enter your password:";
      cin>>temp;
      while(temp.length()<5){
         cout<<"your Password is too short. Please choose another password!"<<endl;
         cout<<"Enter your new password:";
         cin>>temp;
      }
      password=temp;
      if (user_type==3){
        fstream file;
        file.open("customer.csv");
        file<<user_id<<","<<password<<endl;
        file.close();
      }
      
    }

int User :: find_id_passwd(string userid,string password,string filename){
    int value=0;
    ifstream file;
    file.open(filename);
    if (!file) {
        cout <<"Error opening file"<<filename<<endl;
        return -1; 
    }
    string line;
    vector<string>userid_vec;
    vector<string>password_vec;
    while(getline(file,line)){
       int pos=line.find(',');
       string uid= line.substr(0,pos);
       string passwd= line.substr(pos+1);
       userid_vec.push_back(uid);
       password_vec.push_back(passwd);
    }
    
    for(int i=0;i < userid_vec.size();i++){
        if((userid_vec[i]==userid) && (password_vec[i]==password)){
            value=1;
            break;
        }
    }
    return value;   
}

void User::user_login()
{
    string login_id;
    string login_password;
    bool login_success = false; 
    switch (user_type) {
    case 1:
        cout << "Enter your admin id:";
        cin >> login_id;
        cout << "Enter your admin password:";
        cin >> login_password;
        if ((login_id == "admin") && (login_password == "admin")) {
            cout << "Login Successful. Welcome admin!";
            login_success = true;
            admin_login_flag=true;
        }
        else {
            cout << "Incorrect Login ID or password!";
        }
        break;

    case 2:
        while (!login_success) { 
            cout << "Enter your staff id:";
            cin >> login_id;
            cout << "Enter your password:";
            cin >> login_password;
            if (find_id_passwd(login_id, login_password, "staffid.csv" ) == 1) {
                cout << "Login Successful. Welcome staff member!" << endl;
                login_success = true;
            }
            else {
                cout << "Incorrect login ID or password!" << endl;
            }
        }
        break;

    case 3:
        while (!login_success) { 
            cout << "Enter your user id:";
            cin >> login_id;
            cout << "Enter your password:";
            cin >> login_password;
            if (find_id_passwd(login_id, login_password, "staffid.csv" ) == 1) {
                cout << "Login Successful. Welcome user!" << endl;
                login_success = true;
            }
            else {
                cout << "Incorrect login ID or password!" << endl;
            }
        }
        break;
    }
}


class Staff: public User{
   int StaffRno;
   public:
   Staff(){
    user_type=2;
   }
   


};

class Books{
    int bookid;
    string bookname;
    string genre;
    string author;
    string publisher;
    int qty;
    public:
      void increment_qty(string book_name,int increment);
};

void Books::increment_qty(string book_name,int increment){
    fstream file;
    bool book_found;
    file.open("books.csv");
    string lines;
    vector<string>bookdata_vec;
    string book_data;
    while(getline(file,lines)){
        int pos=lines.find(',');
        string bookn=lines.substr(0,pos);
        if(bookn==book_name){
             book_found=true;
             int pos_lastcomma=lines.rfind(',');
             string qty_str=lines.substr(pos_lastcomma+1);
             for (char c : qty_str) {
                if (!isdigit(c)) {
                    throw invalid_argument("Invalid quantity: " + qty_str);
                }
            }
             int qty=stoi(qty_str);
             qty=qty+increment;
             lines=lines.substr(0,pos_lastcomma+1)+ to_string(qty);           
        }
        bookdata_vec.push_back(lines);
    }
    file.close();
    if(!book_found){
        cout<<book_name<<" not found in the books file."<<endl;
        return;
    }
    ofstream file2;
    file2.open("books.csv");
    for(int i=0;i< bookdata_vec.size();i++){
        file2<<bookdata_vec[i]<<endl;
    }
    file2.close();

}  

class Customer: public User{
   string cname;
   int age;
   int phoneno;
   string email;
   public:
   Customer(){
    user_type=3;
   }
    void register_customer();
    void display_profile();
    int is_book_available(string book);
    void borrow_book(Books& obj);
    void return_book();

    friend class Books;
};


void Customer:: register_customer(){
           cout<<"Welcome "<<user_id<<endl;
           cout<<"Please furnish your other details!"<<endl;
           cout<<"Enter your name:"<<endl;
           cin>>cname;
           cout<<"Enter your age:"<<endl;
           cin>>age;
           cout<<"Enter your phone number:"<<endl;
           cin>>phoneno;
           cout<<"Enter your email ID:"<<endl;
           cin>>email;
           fstream file;
           file.open("customer.csv",ios::app);
           file<< user_id<<","<<cname<<","<<age<<","<<phoneno<<","<<email<<endl;
           file.close();
}

void Customer:: display_profile(){
      cout<<"     "<<user_id<<" 's Profile"<<endl;
      cout<<" Name: "<<cname<<endl;
      cout<<" Age:  "<<age<<endl;
      cout<<" Phone No: "<<phoneno<<endl;
      cout<<" Email: "<<email<<endl;
    }

void Customer:: borrow_book(Books& obj){
    string borrowed_book;
    cout<<"Enter the book you want to borrow:";
    cin>>borrowed_book;
    if(is_book_available(borrowed_book)==1){
        string date_borrow;
        cout<<"The book you wish to borrow is available!"<<endl;
        cout<<"Enter your date of borrowing in DD/MM/YYYY:";
        cin>>date_borrow;
        obj.increment_qty(borrowed_book,-1);
        ofstream file2;
        file2.open("borrowing.csv",ios::app);
        file2<<user_id<<","<<borrowed_book<<","<<date_borrow<<endl;
        file2.close();

    }
    else{
        cout<<"The book is not available!"<<endl<<"Please borrow some other book.";
    }

}

void Customer::return_book(){

}

int Customer::is_book_available(string book){
    ifstream file;
    file.open("books.csv");
    string lines;
    string reversed_lines;
    vector<string>bookname_vec;
    vector<int>bookqty_vec;
    while(getline(file,lines)){
        int pos1;
        int pos2;
        pos1=lines.find(',');
        string bookn=lines.substr(0,pos1);
        reversed_lines= string(lines.rbegin(),lines.rend());
        pos2=reversed_lines.find(',');
        string rev_qty=reversed_lines.substr(0,pos2);
        string qty= string(rev_qty.rbegin(),rev_qty.rend());
        int qty_int=stoi(qty);
        bookname_vec.push_back(bookn);
        bookqty_vec.push_back(qty_int);
    }
        int req_pos;
        for(int i=0;i< bookname_vec.size();i++){
           if(bookname_vec[i]==book){
             req_pos=i;
           }

        }
        if(bookqty_vec[req_pos]>0){
            return 1;
        } 
        else{
            return 0;
        }

}




class Admin: public User{
     public:
      void remove_user(); 

};
void Admin::remove_user(){
     string user_id_remove;
     cout<<"Enter the User ID of the user you want to remove:";
     cin>>user_id_remove;
     fstream file;
     file.open("customerid.csv");
     string lines;
     vector<string>id_vec;
     vector<string>passwd_vec;
     while(getline(file,lines)){
        int pos;
        string id;
        string passwd;
        pos=lines.find(',');
        id=lines.substr(0,pos);
        passwd=lines.substr(pos+1);
        id_vec.push_back(id);
        passwd_vec.push_back(passwd);  
     }
     int pos_to_be_removed=-1;
     for(int i=0;i< id_vec.size();i++){
        if(id_vec[i] == user_id_remove){
            pos_to_be_removed=i;
            break;
        }
     }
     if (pos_to_be_removed == -1) {
     cout <<"User ID "<< user_id_remove << " not found." << endl;
     return;
     }
     vector<string>::iterator iter1= id_vec.begin()+ pos_to_be_removed; 
     vector<string>::iterator iter2= passwd_vec.begin()+ pos_to_be_removed;
     id_vec.erase(iter1);
     passwd_vec.erase(iter2);
     file.close();
     ofstream file2;
     file2.open("customerid.csv");
     for(int i=0;i<id_vec.size();i++){
            file2<<id_vec[i]<<","<<passwd_vec[i]<<endl;
      }
     file2.close();
     cout<<user_id_remove<<" has been removed."<<endl;
}

void admin_driver(){
    Admin a;
    a.user_login();
    if(admin_login_flag){
        int ch;
        do{
           cout<<"1)Remove User"<<endl;
           cout<<"2) Update User Info"<<endl;
           cout<<"3)Logout"<<endl;
           cout<<"Enter your choice";
           cin>>ch;
           if(ch==1){
                Admin a1;
                a1.remove_user();
           }
           else if(ch==3){
                 break;
           }
                  
        }while(ch!=3);
    }

}

void driver_function(){
    int ch;
     while(ch!=5){
        cout<<"Welcome to The Library Management System!"<<endl;
        cout<<"1)User Login"<<endl;
        cout<<"2)Staff Login"<<endl;
        cout<<"3)New Here?"<<endl;
        cout<<"4)Admin"<<endl;
        cout<<"5)Exit"<<endl;
        cout<<"Enter your choice!"<<endl;
        cin>>ch;

        switch(ch){
            case 1:
              break;
            case 2:
              break;
            case 3:
               break;
            case 4:
                admin_driver();
                



        }



    }

} 




int main(){
   //Staff s;
   //c.create_account();
   //c.register_customer();
   //c.display_profile();
    //s.user_login();
   //Admin a;
   //a.remove_user();
   //Customer c;
   //Books b;
   //c.create_account();
   //c.borrow_book(b);
   driver_function();
   
   return 0;
}