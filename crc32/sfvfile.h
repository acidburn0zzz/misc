#ifndef __SFV_FILE__
#define __SFV_FILE__

class SfvFile {
    private:
    char *_sfvFile;
    
    public:
    SfvFile(char *file);
    ~SfvFile();
    
    void checkSfvFile(bool presence);
    void printHeader();
    void printFooter();
};

#endif //__SFV_sfvFile__
