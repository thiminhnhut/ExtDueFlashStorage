#include "ExtDueFlashStorage.h"

ExtDueFlashStorage::ExtDueFlashStorage(uint32_t address, unsigned char lengthPerData, unsigned char maxStorage) {
    _address_count = address;
    _address_index_current = address + 1;
    _address_start_data = address + 2;

    _lenghtPerData = lengthPerData;
    _maxStorage = maxStorage;
}

void ExtDueFlashStorage::init() {
    if (!_flashInit) {
        delay(5000);
        _FLASH_DEBUG("(Init) Start!\n");
        _flashInit = true;

        _index_current = read(_address_index_current);
        if (_index_current == 255) {
            _index_current = -1;
        }
        _FLASH_DEBUG("(Init) Index Current = " + String(_index_current) + "\n");

        _count = read(_address_count);
        if (_count == 255) {
            _count = -1;
        }
        _FLASH_DEBUG("(Init) Count Data = " + String(_count) + "\n");
    } else {
        _FLASH_DEBUG("(Init) Fail!\n");
    }
}

ExtDueFlashStorage::ErrorFlash ExtDueFlashStorage::writeString(String data) {
    if (_flashInit) {
        if (_count < _maxStorage) {
            unsigned int _size = data.length();
            _FLASH_DEBUG("(writeString) _size = " + String(_size) + "\n");
            if (_size < _lenghtPerData) {
                unsigned int address_data = _address_start_data + (_index_current + 1)*_lenghtPerData;
                _FLASH_DEBUG("(writeString) address_data = " + String(address_data) + "\n");
                bool status = true;
                if (write(address_data, _size)) {
                    for (unsigned int i = 0; i < _size; i++) {
                        if (!write(address_data + 1 + i, data[i])) {
                            status = false;
                            break;
                        }
                    }
                } else {
                    status = false;
                }

                if (status) {
                    status = write(_address_count, _count + 1);
                    if (status) {
                        unsigned char index_current = _count + 1;
                        status = write(_address_index_current, index_current);
                        if (status) {
                            _count++;
                            _index_current = index_current;
                            _FLASH_DEBUG("(writeString) Count = " + String(_count) + "\n");
                            _FLASH_DEBUG("(writeString) Index Current = " + String(_index_current) + "\n");
                            _FLASH_DEBUG("(writeString) No_Error!\n");
                            return ExtDueFlashStorage::ErrorFlash::No_Error;
                        } else {
                            _FLASH_DEBUG("(writeString) Write_Index_Error!\n");
                            return ExtDueFlashStorage::ErrorFlash::Write_Index_Error;
                        }
                    } else {
                        _FLASH_DEBUG("(writeString) Write_Count_Error!\n");
                        return ExtDueFlashStorage::ErrorFlash::Write_Count_Error;
                    }
                } else {
                    _FLASH_DEBUG("(writeString) Write_Data_Error!\n");
                    return ExtDueFlashStorage::ErrorFlash::Write_Data_Error;
                }
            } else {
                _FLASH_DEBUG("(writeString) Write_Size_Error!\n");
                return ExtDueFlashStorage::ErrorFlash::Write_Size_Error;
            }
        } else {
            _FLASH_DEBUG("(writeString) Over_Error!\n");
            return ExtDueFlashStorage::ErrorFlash::Over_Error;
        }
    } else {
        _FLASH_DEBUG("(writeString) Init_Error!\n");
        return ExtDueFlashStorage::ErrorFlash::Init_Error;
    }
}

void ExtDueFlashStorage::readString(String arrays[]) {
    for (int i = 0; i < _count + 1; i++) {
        unsigned int address_data = _address_start_data + i*_lenghtPerData;
        String data = "";
        unsigned char _size = read(address_data);
        _FLASH_DEBUG("(readString) Size = " + String(_size) + "\n");
        for (unsigned int j = 0; j < _size; j++) {
            data += (char) read(address_data + 1 + j);
        }
        _FLASH_DEBUG("(readString) Data = " + data + "\n");
        arrays[i] = data;
    }
}

unsigned char ExtDueFlashStorage::getCountWriteData() {
    return _count;
}

uint32_t ExtDueFlashStorage::getIndexCurrent() {
    return _index_current;
}
