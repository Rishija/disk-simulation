# Simulating UNIX/Linux file system

This a program to create hierarchical file system simulation written in c++

It creates a virtual disk on your system of specified size and lets you perform the following operations:
-  create/ read/ delete/ recover/ remove a file
-  create a directory
-  switch between different directories
-  list files

## Download and run

``` bash
$ git clone https://github.com/Rishija/diskSimulation.git
$ cd project
$ make
$ ./disk
```

## Working

### Approach
All read and write operations are done sector by sector. Some sectors are reserved for storing status of other sectors. As soon as user creates a new directory, 2 sectors are reserved for it. Entry for any dir/file is of 32 bytes. Whenever changes are made to any sector, its status is changed simultaneously.

### Calculations
Default sector size is set to 512 bytes (can be changed but calculations change accordingly).

#### Example:
- Size of the disk(in mb) = 1
- Size of the disk(in bytes) = size_in_mb * ( 2 ^ 20 ) = 2 ^ 20
- Size of each sector = 2 ^ 9
- Total no. of sectors = size of disk(in bytes) / sector_size = ( 2 ^ 20 ) / ( 2 ^ 9 ) = 2 ^ 11
- Status is of 4 bytes
- Each sector contains status of ( 2 ^ 7 ) sectors
- Reserved sectors = Total_no_of_sectors / ( 2 ^ 7 ) = 2 ^ 4
- 2 Sectors are reserved for root directory (Sector no: reserved sectors and reserved sectors+1)

### File operations:
- Create
- Read
- Delete
- Recover
- Remove

### Directory operations:
- Access parent directory
- Move to root directory
- Change current directory

### Helping Functions:
- Read sector
- Write sector
- Status of a sector
- Update status of sector
- Check if file/directory already exists
- Storing path to working directory
- List files in a directory/Disk

## Sample screenshots

![1](https://i.imgur.com/3YUWixg.jpg)

![2](https://i.imgur.com/4DBgwkO.jpg)

![3](https://i.imgur.com/AdX4Rb6.jpg)
