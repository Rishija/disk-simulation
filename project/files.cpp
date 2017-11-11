#include"disk.h"

void Disk::create_file(char fname[])
/*
 Objective: To create a new file
 Parameters:
 fname: file name
 Return value: None
 */
{
    if(already_exists(fname,'f'))
    {
        cout<<"File "<<fname<<" already exists\n";
        return;
    }
    
    int next=currentDir;
    for(int count=0;count<2;count++)
    {
        read_sector(next);
        for (int i=0; i<16; i++)
        {
            for(int j=0;j<32;j++)
                entry.entry_buffer[j]=buffer[32*i+j];
            
            if(entry.dir.type=='0')
            {   int bytes;
                char alpha;
                cout<<"Enter size of file in bytes: ";      //file size in bytes
                cin>>bytes;
                
                entry.dir.size=((bytes-1)/sector_size)+1;       //number of sectors file require
                entry.dir.bytes=bytes;
                
                if(empty_sectors(entry.dir.size))               //if required sectors are found
                {
                    //file entry
                    strcpy(entry.dir.name,fname);
                    entry.dir.parent=currentDir;
                    entry.dir.type='f';
                    entry.dir.start=empty[0];
                    
                    cout<<"Enter alphabet: ";
                    cin>>alpha;
                    
                    for(int k=0;k<sector_size;k++)
                        buffer[k]=alpha;
                    for(int k=0;k<entry.dir.size-1;k++)
                        write_sector(empty[k],buffer);
                    
                    read_sector(empty[entry.dir.size-1]);
                    for(int k=0;k<bytes%sector_size;k++)
                        buffer[k]=alpha;
                    write_sector(empty[entry.dir.size-1],buffer);
                    
                    //update file entry
                    read_sector(next);
                    for(int k=0;k<32;k++)
                        buffer[32*i+k]=entry.entry_buffer[k];
                    write_sector(next,buffer);
                    
                    //status linked
                    for(int i=0;i<entry.dir.size;i++)
                    {
                        if(i==entry.dir.size-1)
                            update_status(empty[i],1);
                        else
                            update_status(empty[i],empty[i+1]);
                    }
                    return;
                    
                }
                
                cout<<"Not enough space"<<endl;
                return;
            }
        }
        next=sector_status(currentDir);
    }
    cout<<"Not enough space in the directory"<<endl;
    return;
}


void Disk::read_file(char fname[])
/*
 Objective: To read a file
 Parameters:
 fname: file name
 Return value: None
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
            
            if(!strcmp(entry.dir.name,fname) && entry.dir.type=='f')
            {   if(sector_status(entry.dir.start)<0)        //if file is marked deleted
            {
                cout<<"\nFile don't exists any more. Do you want to retrieve? (y/n): ";
                char ch;
                cin>>ch;
                if(ch=='y' || ch=='Y')
                    recover_file(fname);
                return;
            }
                
                //sector: sector number in which file data is stored
                int sector=entry.dir.start;
                
                while(sector_status(sector)!=1)
                {
                    display(sector);
                    sector=sector_status(sector);
                };
                display(sector,entry.dir.bytes%sector_size);
                return;
            }
        }
        next=sector_status(currentDir);
    }
    cout<<"\nFile \""<<fname<<"\" not found!!\n";
    return;
}


void Disk::delete_file(char fname[])
/*
 Objective: To marks a file delete
 Marks the status of each sector(of file) negative without actually deleting the file entry from directory
 Parameters:
 fname: file name
 Return value: None
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
            
            if(!strcmp(entry.dir.name,fname) && entry.dir.type=='f')
            {   int pos=entry.dir.start;
                if(sector_status(pos)<0)        //if file is already marked deleted
                {
                    cout<<"\nCan't delete \""<<fname<<"\". File do not exists!\n";
                    return;
                }
                do
                {   int temp;
                    temp=sector_status(pos);
                    update_status(pos,(-1)*temp);
                    pos=temp;
                }while(pos!=1);
                
                cout<<"\nFile deleted\n";
                return;
            }
            
        }
        next=sector_status(currentDir);
    }
    cout<<"\nCan't delete \""<<fname<<"\". File do not exists!\n";
    return;
}


void Disk::recover_file(char fname[])
/*
 Objective: To recover a file marked deleted
 Mark status positve again ( each containing the address of next lined sector )
 Parameters:
 fname: file name
 Return value: None
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
            
            if(!strcmp(entry.dir.name,fname) && sector_status(entry.dir.start)<0 && entry.dir.type=='f')
            {   int pos=entry.dir.start;
                do
                {   int temp;
                    temp=(-1)*sector_status(pos);
                    update_status(pos,temp);
                    pos=temp;
                }while(pos!=1);
                return;
            }
            
        }
        next=sector_status(currentDir);
    }
    cout<<"\nCan't retrieve file. No deleted file with name \""<<fname<<"\"\n";
    return;
}


void Disk::remove_file(char fname[])
/*
 Objective: To delete a file permanently
 Sets status of each sector (where the file was stored) to 0 and delete file entry from the directory
 Parameters:
 fname: file name
 Return value: None
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
            if(!strcmp(entry.dir.name,fname) && entry.dir.type=='f')
            {
                if(sector_status(entry.dir.start)<0)
                    recover_file(fname);
                int pos=entry.dir.start;
                do
                {
                    int temp;
                    temp=sector_status(pos);
                    update_status(pos,0);           //new status=0
                    pos=temp;
                }while(pos!=1);
                
                //deleting file entry
                read_sector(next);
                strcpy(entry.dir.name,"0000000000000000000");
                entry.dir.parent=0;
                entry.dir.size=0;
                entry.dir.type=0;
                entry.dir.start=0;
                entry.dir.bytes=0;
                for(int j=0;j<32;j++)
                    buffer[32*i+j]=entry.entry_buffer[j];
                write_sector(next,buffer);
                return;
                
            }
        }
        next=sector_status(currentDir);
    }
    cout<<"\nNo such file found!"<<endl;
}
