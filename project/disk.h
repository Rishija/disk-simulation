#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <vector>
#include <sstream>
using namespace std;

//colored text
#define green cout<<"\33[32m"
#define bold cout<<"\33[1m"
#define regular cout<<"\33[0m"
#define yellow cout<<"\33[33m"
#define magenta cout<<"\33[35m"

static const int sector_size=512;

//class disk
class Disk
{
    char buffer[sector_size];
    char dName[20];
    float dSize;
    int totalSectors;
    int reservedSectors;
    vector<int>empty;
    int currentDir;
    vector<string>path;
    vector<int>address;
    
    //converts int to char array of 4 bytes and vice-versa
    typedef struct
    {
        union
        {
            char str[4];
            int num;
        };
    } typecast;
    typecast convert;
    
    // entry for directory/file
    struct directory_entry
    {
        char name[15];
        char type;              // d-directory f-file
        int parent;
        int bytes;              // size in bytes
        int size;               // in number of sectors
        int start;              // starting sector
        
    };
    
    //converts object of structure directory_entry to char array of 32 bytes and vice-versa
    typedef struct
    {
        union
        {
            directory_entry dir;
            char entry_buffer[32];
        };
    }typecast1;
    typecast1 entry;
    
    
public:
    Disk(char disk_name[]);                         //constructor
    void info();                                    //display information about the disk
    void write_sector(int sector_no,char buffer[]);    //writes on a sector
    void read_sector(int sector_no);                //reads content of a sector
    int sector_status(int sector_no,int x=0);        //returns status of sector
    void update_status(int sector_no,int x);        //updates status of sector
    void display(int sector_no,int bytes=512);          //display content of a sector
    
    void display_path();                            //display the current location
    bool empty_sectors(int x);                      //gives list of empty sectors
    bool already_exists(char name[],char type);     //checks if dir/file already exists
    
    void create_file(char fname[]);                 //creates file
    void read_file(char fname[]);                   //reads file
    void delete_file(char fname[]);                 //mark file delete
    void recover_file(char fname[]);                //recover deleted file
    void remove_file(char fname[]);                 //removes file permanently
    
    void create_dir(char dirname[]);                //creates new directory
    void set_dir(char dirname[]);                   //change current directory
    
    void list();                                    //list all files in curent directory
    void wrapper();                                 //wrapper for fn list_all()
    void list_all(int dir,int level);               //list all files in the disk
};
