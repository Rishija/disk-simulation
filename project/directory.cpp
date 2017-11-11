#include"disk.h"

void Disk::create_dir(char dirname[])
/*
 Objective: To create a new directory in the current directoy
 Parameters:
 dirname: directory name
 Return valie: None
 */
{
    if(already_exists(dirname,'d'))
    {
        cout<<"Directory "<<dirname<<" already exists!\n";
        return;
    }
    
    int next=currentDir;
    for(int count=0;count<2;count++)
    {
        
        for (int i=0; i<16; i++)
        {   read_sector(next);
            for(int j=0;j<32;j++)
                entry.entry_buffer[j]=buffer[32*i+j];
            if(entry.dir.type=='0')
            {
                if(empty_sectors(2))                    //size of directory (=2 sectors)
                {
                    //directory entry
                    strcpy(entry.dir.name,dirname);
                    entry.dir.parent=currentDir;
                    entry.dir.type='d';
                    entry.dir.start=empty[0];
                    entry.dir.size=2;
                    
                    read_sector(next);
                    for(int k=0;k<32;k++)
                        buffer[32*i+k]=entry.entry_buffer[k];
                    write_sector(next,buffer);
                    
                    //updating status
                    for(int i=0;i<entry.dir.size;i++)
                    {
                        if(i==entry.dir.size-1)
                            update_status(empty[i],4);
                        else
                            update_status(empty[i],empty[i+1]);
                    }
                    
                }
                else
                    cout<<"\nNot enough space\n";
                return;
            }
        }
        next=sector_status(currentDir);
    }
    cout<<"No space for entry in directory '"<<currentDir<<"'\n";
}


void Disk::set_dir(char dirname[])
/*
 Objective: To change the current directory
 Parameters:
 dirname: directory name
 Return value: None
 */
{
    if(!strcmp(dirname,".."))
    {
        if(path.size()!=0)
        {
            currentDir=address[address.size()-1];
            address.pop_back();
            path.pop_back();
        }
        return;
    }
    
    if(!strcmp(dirname,"/"))
    {
        while(path.size()!=0)
            path.pop_back();
        currentDir=reservedSectors;
        address.clear();
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
            if(!strcmp(entry.dir.name,dirname) && entry.dir.type=='d')
            {
                address.push_back(currentDir);
                path.push_back(entry.dir.name);
                currentDir=entry.dir.start;
                return;
            }
        }
        next=sector_status(currentDir);
    }
    cout<<"\nNo such directory found"<<endl;
}
