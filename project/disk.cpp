#include"disk.h"

Disk::Disk(char disk_name[])
/*
 Constructor of class Disk
 Creates a new disk named 'disk_name' with size specified by the user
 or open if it already exists
 */
{
    strcpy(dName,disk_name);
    ifstream exists(dName,ios::binary|ios::ate);
    if(exists)
    {
        cout<<"Disk  \""<<dName<<"\" already exists!!"<<endl;
        dSize=exists.tellg()/pow(2,20);
        totalSectors=dSize*pow(2,11);
        reservedSectors=totalSectors/pow(2,7);
        currentDir=reservedSectors;
        exists.close();
    }
    else
    {
        exists.close();
        cout<<"Enter size of disk (in MB): ";
        cin>>dSize;
        totalSectors=dSize*pow(2,11);
        reservedSectors=totalSectors/pow(2,7);
        currentDir=reservedSectors;
        
        fstream fp;
        fp.open(dName,ios::binary|ios::out);
        
        convert.num=0;
        for(int i=0;i<sector_size;i++)
            buffer[i]=convert.str[i%4];
        for(int i=0;i<reservedSectors;i++)     //initially status of each sector is 0
            fp.write(buffer,sector_size);
        fp.close();
        
        for(int i=0;i<sector_size;i++)
            buffer[i]='0';
        for(int i=reservedSectors;i<totalSectors;i++)   //reserving space by writing '0' in all sectors
            write_sector(i,buffer);
        
        update_status(reservedSectors,reservedSectors+1);   //update status for root directory
        update_status(reservedSectors+1,4);
    }
}


void Disk::info()
/*
 fn to display information
 */
{
    cout<<endl<<setw(25)<<left<<"Disk"<<dName<<endl<<setw(25)<<left<<"Size of each sector"<<sector_size<<" bytes\n"<<setw(25)<<left
    <<"Total sectors"<<totalSectors<<endl<<setw(25)<<left<<"Reserved sector"<<reservedSectors<<"\nStatus info:\n\t0\tfree\n\t1\tbusy\n\t2\treserved"
    "\n\t3\tinvalid\n\t4\treserved for dir\n";
}


int Disk::sector_status(int sector_no,int display)
/*
 Objective: To return the status of  a given sector
 Parameters:
 sector_no: sector number
 display: if 1: prints sector # and relative bytes position at which status is stored
 Return value: status of sector
 */
{
    if (sector_no < reservedSectors)
        return 2;
    
    if(sector_no>totalSectors-1)
        return 3;
    
    int pos=(sector_no-reservedSectors)*4;
    int secNo=(pos/sector_size);
    int byteNo=(pos%sector_size);
    if(display==1)
    {
        cout<<"\nStatus of sector #"<<sector_no<<" is saved at sector number "<<secNo<<" at byte number "<<byteNo<<endl;
    }
    
    read_sector(secNo);
    
    for(int i=0;i<4;i++)
    {
        convert.str[i]=buffer[byteNo];
        byteNo++;
    }
    
    return convert.num;
}


void Disk::write_sector(int sector_no,char buffer[])
/*
 Objective: To write on a given sector
 Parameters:
 sector_no: sector number
 buffer: data to be written
 Return value: None
 */
{
    fstream fp;
    fp.open(dName, ios::binary | ios::in | ios::out);
    fp.seekp(sector_no*sector_size);
    fp.write(buffer, sector_size);
    fp.close();
}


void Disk::read_sector(int sector_no)
/*
 Objective: To read content of a sector
 Parameters:
 sector_no: sector number
 Return value: None
 */
{
    ifstream fin;
    fin.open(dName, ios::in | ios::binary);
    fin.seekg(sector_no*sector_size);
    fin.read(buffer, sector_size);
    fin.close();
}


void Disk::update_status(int sector_no,int x)
/*
 Objective: To update status on a given sector
 Parameters:
 sector_no: sector number
 x: new status
 Return value: None
 */
{
    int pos=(sector_no-reservedSectors)*4;
    int secNo=(pos/sector_size);
    int byteNo=(pos%sector_size);
    read_sector(secNo);
    
    convert.num=x;
    
    for(int i=0;i<4;i++)
        buffer[byteNo+i]=convert.str[i];
    
    write_sector(secNo,buffer);
}



void Disk::display(int sector_no,int bytes)
/*
 Display the content of sector
 Parameters:
 sector_no: sector number
 Return value: None
 */
{
    int x=sector_status(sector_no);
    if(x==3)
    {
        cout<<"Invalid sector no\n";
        return;
    }
    read_sector(sector_no);
    
    if(x==2)                                //reserved sector
        for(int i=0;i<sector_size;i++)
            cout<<int(buffer[i]);
    
    for(int i=0;i<bytes;i++)
        cout<<buffer[i];
}


void Disk::display_path()
/*
 Objective: To display the path to current location
 */
{   green; bold;
    cout<<dName<<":~";
    magenta;
    for(int i=0;i<path.size();i++)
        cout<<"/"<<path[i];
    cout<<" $ ";
    regular;
}


bool Disk::empty_sectors(int reqSectors)
/*
 Objective: To read sector numbers which are empty in vector `empty`
 Parameters:
 reqSectors: Number of empty sectors required
 Return Value: boolean
 true - if x sectors can be found
 false - if required sectors can't be found
 */
{
    int count=0,index=0;
    empty.clear();
    ifstream fin;
    fin.open(dName, ios::in | ios::binary);
    while(fin.tellg()<(reservedSectors*sector_size) && count<reqSectors)
    {
        fin.read(buffer, sector_size);
        
        for(int j=0;j<128;j++)
        {
            
            for(int i=0;i<4;i++)
                convert.str[i]=buffer[4*j+i];
            if(convert.num==0)
            {    empty.push_back(j+reservedSectors+index*128);
                count++;
            }
            if(count==reqSectors)
            {   fin.close();
                return true;
            }
        }
        index++;
    }
    fin.close();
    return false;
}


bool Disk::already_exists(char name[], char type)
/*
 Objective: To check whether a file/dir already exists
 Parameters:
 name: dir/file name
 type: type
 d - dir
 f - file
 Return value: bool
 true - if file/dir already exists
 false - if not
 */
{
    int next=currentDir;
    for(int count=0;count<2;count++)
    {
        read_sector(next);
        for (int i=0; i<16; i++)
        {
            for(int j=0;j<32;j++)
                entry.entry_buffer[j]=buffer[32*i+j];
            if(!strcmp(entry.dir.name,name) && entry.dir.type==type)
                return true;
        }
        next=sector_status(currentDir);
    }
    return false;
}

void Disk::list()
/*
 Objective: To print all dir/file name, size, type and status in current directory
 */
{
    cout<<"\n"<<setw(25)<<left<<"NAME"<<setw(8)<<left<<"TYPE"<<setw(10)<<left<<"SIZE"<<"STATUS\n\n";
    int next=currentDir;
    for(int count=0;count<2;count++)
    {
        for (int i=0; i<16; i++)
        {   read_sector(next);
            for(int j=0;j<32;j++)
                entry.entry_buffer[j]=buffer[32*i+j];
            if(entry.dir.name[0]!='0')
            {
                if(entry.dir.type=='d')                    //dir
                    cout<<setw(25)<<left<<entry.dir.name<<"dir"<<endl;
                
                else                //file
                {
                    cout<<setw(25)<<left<<entry.dir.name<<setw(8)<<left<<"file";
                    if(sector_status(entry.dir.start)<0)
                        cout<<setw(10)<<left<<entry.dir.bytes<<"deleted"<<endl;
                    else
                        cout<<entry.dir.bytes<<endl;
                }
            }
        }
        next=sector_status(currentDir);
        
    }
    cout<<endl;
}


void Disk::wrapper()
/*
 Wrapper function for list_all()
 */
{
    list_all(reservedSectors,0);
}


void Disk::list_all(int dir,int level)
/*
 Objective: To print all file/dir in the disk
 Approach: Recursion
 Starts printing from the root directory
 Parameters:
 dir: directory name
 level: level in the hierarchy
 Return value: None
 */
{
    int next=dir;
    for(int count=0;count<2;count++)
    {
        for(int i=0;i<16;i++)
        {   read_sector(next);
            for (int j=0; j<32; j++)
                entry.entry_buffer[j]=buffer[32*i+j];
            if(entry.dir.name[0]!='0')
            {
                for(int tab=0;tab<level;tab++)
                    cout<<"   ";
                if(entry.dir.type=='f')     //file
                {
                    cout<<"-";
                    cout<<entry.dir.name<<endl;
                }
                else                    //directory
                {
                    cout<<"*";
                    cout<<entry.dir.name<<endl;
                    list_all(entry.dir.start,level+1);
                }
            }
        }
        next=sector_status(next);
    }
}
