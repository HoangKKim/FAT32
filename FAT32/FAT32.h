#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <iomanip>
#include <math.h>

using namespace std;

class FAT32
{
private:
	LPCWSTR  drive;
	unsigned int bytesPerSector; // so byte cho mot sector
	unsigned int sectorsPerCluster; // so sector tren mot cluster
	unsigned int reservedSectors; // kich thuoc Bootsector
	unsigned int numbersOfFATs; // so bang FAT
	unsigned int totalSectors; // kich thuoc volume
	unsigned int sectorsPerFAT; // kich thuoc bang FAT
	unsigned int rootCluster; // cluster bat dau RDECT
	unsigned int firstSectorOfFAT1; // the first sector of FAT1 table
	unsigned int firstSectorOfRDET; // the first sector of RDET
	unsigned int firstSectorOfData; // the first sector of Data

	BYTE* sector;

public:
	FAT32();
	FAT32(LPCWSTR drive);
	~FAT32();
	void readBootSector(LPCWSTR  drive, int readPoint, BYTE*& sector, int size);
	string decToHex(int decNumber);
	void printBootSector(BYTE*& sector);	
	string getByteToString(BYTE* sector, int offset, int nByte);
	unsigned int hexToDec(string hexNumber);
	void printDataFromBootSector();

	void printRDET();
};