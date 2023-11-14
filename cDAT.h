#ifndef DEF_CDAT
#define DEF_CDAT
#include <iostream>
#include <string>
#include <vector>

struct sDATHeader
{
    char uniqueID[5]; /// Unique ID used to know if this file is a DAT File from this class
    char version[3]; /// Version of the DAT file format
    unsigned int nb_files; /// Number of files in the DAT file
};

struct sFileEntry
{
    char name[300]; /// Name of the data file
    long size; /// Size of the data file
    long offset; /// Offset, in the DAT file where the file is
};


class cDAT
{
private :
    std::string m_datfile; /// name of the DAT file
    sDATHeader m_header; /// file header
    std::vector<sFileEntry> m_entries; /// vector of files entries
    char* m_buffer; /// Buffer pointing on a file in memory
public :
    cDAT (void);
    ~cDAT (void);
    bool Create (std::vector<std::string> files, std::string destination);
    void Read (std::string source);
    char* GetFile (std::string filename);
    long int GetFileSize (std::string filename);
};

#endif
