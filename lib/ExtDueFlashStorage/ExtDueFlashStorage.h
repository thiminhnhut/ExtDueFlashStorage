#ifndef ExtDueFlashStorage_h
#define ExtDueFlashStorage_h

#include <Arduino.h>
#include <DueFlashStorage.h>

class ExtDueFlashStorage: public DueFlashStorage {
private:
    uint32_t _address_count;
    uint32_t _address_index_current;

    unsigned char _lenghtPerData;
    unsigned char _maxStorage;

    unsigned char _address_start_data;

    int _index_current;
    int _count;

    bool _flashInit = false;

public:

    enum ErrorFlash {
        No_Error,
        Over_Error,
        Write_Error,
        Write_Data_Error,
        Write_Count_Error,
        Write_Index_Error,
        Write_Size_Error,
        Init_Error
    };

    ExtDueFlashStorage(uint32_t address, unsigned char lengthPerData, unsigned char maxStorage);

    void init();

    ExtDueFlashStorage::ErrorFlash writeString(String data);
    void readString(String arrays[]);

    unsigned char getCountWriteData();
    uint32_t getIndexCurrent();
};
#endif
