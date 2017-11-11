#include"disk.h"

//help message
void help()
{   yellow; bold;
    cout<<endl<<setw(20)<<left<<"help"<<"To display help message\n"<<setw(20)<<left<<"info"<<"Show information about disk\n"<<setw(20)<<left<<"exit"<<"exit\n"
    <<setw(20)<<left<<"create file1"<<"To create a file named file1\n"<<setw(20)<<left<<"read file1"<<"To read file named file1\n"<<setw(20)<<left
    <<"del file1"<<"To delete file named file1\n"<<setw(20)<<left<<"rec file1"<<"To recover file named file1\n"<<setw(20)<<left<<
    "rem file1"<<"To delete file1 permanently\n"<<setw(20)<<left<<"mkdir dir1"<<"To create directory named dir1\n"<<setw(20)<<left
    <<"cd dir1"<<"To set dir1 as current directoy\n"<<setw(20)<<left<<"cd .."<<"To move to parent directory\n"
    <<setw(20)<<left<<"cd /"<<"To move to root directory\n"
    <<setw(20)<<left<<"ls"<<"To display the content current dir\n"<<setw(20)<<left<<"all"<<"list all files/dir in disk\n"
    <<setw(20)<<left<<"dis sec1"<<"To display sector #sec1\n"<<setw(20)<<left<<"status sec1"<<"To get status of sector #sec1\n\n";
    regular;
}

bool input(string choice,Disk &disk)
/*
 Objective: Split the command in two arguments and performs the operation respectively
 Parameters:
 choice: command by the user
 disk: reference to object of class Disk
 Return value: bool
 true: continue
 false: exit
 */
{
    vector<string>operations={"exit","help","info","create","read","del","rec","rem","mkdir","cd","dis","ls","status","all"};
    //string stream initialized with command by user
    stringstream s(choice);
    
    string com1,com2;
    int ch=100;
    char name[19];
    
    s>>com1>>com2;
    
    //chosing the operation
    for(int i=0;i<operations.size();i++)
        if(com1==operations[i])
            ch=i;
    
    strcpy(name,com2.c_str());      //conversion from string to char array
    
    if(com2.empty())
    {
        switch(ch)
        {
            case 0: exit(0);
            case 1: help();
                break;
            case 2: disk.info();
                break;
            case 11: disk.list();
                break;
            case 13: disk.wrapper();
                break;
            default: cout<<"Command not found "<<endl;
                break;
        }
    }
    else
    {
        switch(ch)
        {
            case 3: disk.create_file(name);
                break;
            case 4: disk.read_file(name);
                break;
            case 5: disk.delete_file(name);
                break;
            case 6: disk.recover_file(name);
                break;
            case 7: disk.remove_file(name);
                break;
            case 8: disk.create_dir(name);
                break;
            case 9: disk.set_dir(name);
                break;
            case 10: disk.display(atoi(name),512);
                break;
            case 12: { int temp;
                temp=disk.sector_status(atoi(name),1);
                cout<<"Status: "<<temp<<endl; }
                break;
            default: cout<<"Command not found "<<endl;
                break;
        }
    }
    return true;
}


int main()
{
    char dName[20];
    bool work=true;
    cout<<"Enter disk name: ";
    cin>>dName;
    
    Disk disk(dName);       //object of class Disk
    cin.ignore(100,'\n');
    
    help();
    while(work)
    {
        disk.display_path();
        string choice;
        getline(cin,choice);
        work=input(choice,disk);
        cin.ignore(100,'\n');
    }
    return 0;
}
