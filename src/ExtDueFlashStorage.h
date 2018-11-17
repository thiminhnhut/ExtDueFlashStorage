#ifndef ExtDueFlashStorage_h
#define ExtDueFlashStorage_h

#include <Arduino.h>
#include <DueFlashStorage.h>

class ExtDueFlashStorage: public DueFlashStorage {
private:
    uint32_t _address_count;
    uint32_t _address_over_data;
    uint32_t _address_index_current;

    unsigned char _lenghtPerData;
    unsigned char _maxStorage;

    uint32_t _address_start_data;

    uint32_t _max_address_data;

    int _index_current;
    int _count;
    int _over_data;

    bool _flashInit = false;

    enum OverData {
        No_Over,
        Over_Times_1
    };

public:

    enum ErrorFlash {
        No_Error,
        Over_Error,
        Write_Error,
        Write_Data_Error,
        Write_Count_Error,
        Write_Index_Error,
        Write_Size_Error,
        Write_Over_Data_Error,
        Init_Error
    };

    ExtDueFlashStorage(uint32_t address, unsigned char lengthPerData, unsigned char maxStorage);

    void init();

    ExtDueFlashStorage::ErrorFlash writeString(String data);
    void readString(String arrays[]);

    unsigned char getCountWriteData();

    void reset();
};
#endif
