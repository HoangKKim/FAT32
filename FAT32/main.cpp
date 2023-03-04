#include<iostream>
#include<cstdlib>
#include<stdio.h>
#include<vector>
#include<windows.h>
#include<iomanip>
#include<sstream>
#include <string>
#include <map>

#include "FAT32.h"
using namespace std;



int main()
{
    wstring disk_name;

    cout << "Nhap ten o dia: ";
    wcin >> disk_name;
    disk_name = L"\\\\.\\" + disk_name + L":";
    LPCWSTR drive = disk_name.c_str(); //chuyen tu string -> cstring
    FAT32 myFAT32(drive);
    myFAT32.printDataFromBootSector();
    cout << endl;
    myFAT32.printRDET();

}