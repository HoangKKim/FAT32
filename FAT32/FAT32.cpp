#include "FAT32.h"

FAT32::FAT32()
{
    this->drive = L"\\\\.\\ G:";
    this->sector = new BYTE[512];
}

FAT32::FAT32(LPCWSTR drive)
{
    this->sector = new BYTE[512];
    // read Bootsector to get data
    this->readBootSector(drive, 0, sector, 512);
    this->drive = drive;
    this->bytesPerSector = this->hexToDec(this->getByteToString(sector, 0x00B, 2));
    this->sectorsPerCluster = this->hexToDec(this->getByteToString(sector, 0x00D, 1));
    this->reservedSectors = this->hexToDec(this->getByteToString(sector, 0x00E, 2)); // SB
    this->numbersOfFATs = this->hexToDec(this->getByteToString(sector, 0x010, 1)); // NF
    this->totalSectors = this->hexToDec(this->getByteToString(sector, 0x020, 4));
    this->sectorsPerFAT = this->hexToDec(this->getByteToString(sector, 0x024, 4)); //SF
    this->rootCluster = this->hexToDec(this->getByteToString(sector, 0x02C, 4));
    this->firstSectorOfFAT1 = this->reservedSectors;
    this->firstSectorOfRDET = this->reservedSectors + this->numbersOfFATs * this->sectorsPerFAT;
    this->firstSectorOfData = this->reservedSectors + this->numbersOfFATs * this->sectorsPerFAT + 
        (this->rootCluster - 2)*sectorsPerCluster;
}

FAT32::~FAT32()
{
    delete[]sector;
    sector = NULL;
}

void FAT32::readBootSector(LPCWSTR  drive, int readPoint, BYTE*& sector, int size)
{
    int retCode = 0;
    DWORD bytesRead;
    HANDLE device = NULL;

    device = CreateFile(drive,    // Drive to open
        GENERIC_READ,           // Access mode
        FILE_SHARE_READ | FILE_SHARE_WRITE,        // Share Mode
        NULL,                   // Security Descriptor
        OPEN_EXISTING,          // How to create
        0,                      // File attributes
        NULL);                  // Handle to template

    if (device == INVALID_HANDLE_VALUE) // Open Error
    {
        printf("CreateFile: %u\n", GetLastError());
        return;
    }

    SetFilePointer(device, readPoint, NULL, FILE_BEGIN);//Set a Point to Read

    if (!ReadFile(device, sector, size, &bytesRead, NULL))
    {
        printf("ReadFile: %u\n", GetLastError());
        return;
    }
    else
    {
        printf("Success!\n");
        return;
    }
}

void FAT32::printBootSector(BYTE*& sector)
{
    int count = 0;
    int num = 0;

    cout << "         0  1  2  3  4  5  6  7    8  9  A  B  C  D  E  F" << endl;

    cout << "0x00" << this->decToHex(num) << "0 ";
    bool flag = 0;
    for (int i = 0; i < 512; i++)
    {
        if (i % 16 ==0 && i!=0) // kiem tra het mot offset
        {
            num++;
            cout << endl;
            string tmp = decToHex(num);
            
            if (tmp.length() == 1)
            {
                tmp = "0x00" + tmp;
            }
            else tmp = "0x0" + tmp;
            tmp.push_back('0');
            cout << tmp << " ";
        }
        if (i % 8 == 0)
        {
            cout << "  ";
        }
        printf("%02X ", sector[i]); // in sector thu i dang hex
    }
    cout << endl;
}

string FAT32::decToHex(int decNumber)
{
    string res;
    string tmp;
    string smpl = "0123456789ABCDEF";
    if (decNumber < 16)
    {
        tmp.push_back(smpl[decNumber]);
        return tmp;
    }
    while (decNumber > 0)
    {
        int a = decNumber % 16;
        tmp.push_back(smpl[a]);
        decNumber = decNumber / 16;
    }
    for (int i = tmp.length() - 1; i >= 0; i--)
    {
        res.push_back(tmp[i]);
    }

    return res;
}

// nhan dang offset theo format 0xAB : dong A cot B
string FAT32::getByteToString(BYTE* sector, int offset, int nByte)
{
    string res;
    for (int i = nByte-1; i >=0; i--)
    {
        int tmp = (int)sector[offset + i];
        if (tmp < 10)
        {
            res = res + "0";
        }
        res = res + decToHex(tmp);
    }
    return res;
}

unsigned int FAT32::hexToDec(string hexNumber)
{
    string smpl = "0123456789ABCDEF";
    unsigned int res = 0;
    int count = 0;
    int length = hexNumber.length();
    for (int i = length - 1; i >= 0; i--)
    {
        int tmp = smpl.find_first_of(hexNumber[i]);
        res = res + tmp * pow(16, count);
        count++;
    }
    return res;
}

void FAT32::printDataFromBootSector()
{
    cout << drive << endl;
    printBootSector(sector);
    cout << endl;
    cout << "Bytes per sector: " << this->bytesPerSector << endl;
    cout << "Sectors per cluster: " << this->sectorsPerCluster << endl;
    cout << "Reserved sectors: " << this->reservedSectors << endl;
    cout << "Number of FATs: " << this->numbersOfFATs << endl;
    cout << "Total sectors: " << this->totalSectors << endl;
    cout << "Sectors per FAT: " << this->sectorsPerFAT << endl;
    cout << "Root cluster: " << this->rootCluster << endl;
    cout << "The first sector of FAT1 table: " << firstSectorOfFAT1 << endl;
    cout << "The first sector of RDET: " << firstSectorOfRDET << endl;
    cout << "The first sector of Data: " << firstSectorOfData << endl;
}

void FAT32::printRDET()
{
    readBootSector(this->drive, firstSectorOfData * 512, sector, 512);
    printBootSector(sector);
}