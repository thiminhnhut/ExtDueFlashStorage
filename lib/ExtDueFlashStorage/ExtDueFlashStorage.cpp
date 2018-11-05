#include "ExtDueFlashStorage.h"

ExtDueFlashStorage::ExtDueFlashStorage(uint32_t address, unsigned char lengthPerData, unsigned char maxStorage) {
    _address_count = address;
    _address_index_current = address + 1;
    _address_over_data = address + 2;
    _address_start_data = address + 3;
    _max_address_data = _address_start_data + (maxStorage - 1)*lengthPerData;

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
            _index_current = 0;
        }
        _FLASH_DEBUG("(Init) _index_current = " + String(_index_current) + "\n");

        _over_data = read(_address_over_data);
        if (_over_data == 255) {
            _over_data = No_Over;
        }
        _FLASH_DEBUG("(Init) _over_data = " + String(_over_data) + "\n");

        _count = read(_address_count);
        if (_count == 255) {
            _count = 0;
        } else if (_count == _maxStorage) {
            _count = _maxStorage - 1;
        }
        _FLASH_DEBUG("(Init) _count = " + String(_count) + "\n");
    } else {
        _FLASH_DEBUG("(Init) Fail!\n");
    }
}

ExtDueFlashStorage::ErrorFlash ExtDueFlashStorage::writeString(String data) {
    if (_flashInit) {
        unsigned int _size = data.length();
        if (_size < _lenghtPerData) {
            if (_over_data == Over_Times_1 && _index_current >= _maxStorage) {
                String _data[_maxStorage];
                readString(_data);

                String _data_tmp[_maxStorage/2];
                for (unsigned char i = 0; i < _maxStorage/4; i++) {
                    _data_tmp[i] = _data[2*i + 1];
                }

                for (unsigned char i = _maxStorage/4; i < _maxStorage/2; i++) {
                    _data_tmp[i] = _data[4*(i - _maxStorage/4) + 2];
                }

                for (unsigned char i = 0; i < _maxStorage/2; i++) {
                    unsigned int address_data = _address_start_data + i*_lenghtPerData;
                    _size = _data_tmp[i].length();
                    write(address_data, _size);
                    for (unsigned int j = 0; j < _size; j++) {
                        write(address_data + 1 + j, _data_tmp[i][j]);
                    }
                }

                _over_data = No_Over;
                _index_current = _maxStorage/2;
                _count = _maxStorage/2;
            }

            unsigned int address_data = _address_start_data + _index_current*_lenghtPerData;

            write(address_data, _size);
            for (unsigned int i = 0; i < _size; i++) {
                write(address_data + 1 + i, data[i]);
            }

            if (_count < _maxStorage) {
                _count += 1;
            }

            if (_over_data == No_Over && _count == _maxStorage) {
                _over_data = Over_Times_1;
                _index_current = -2;
            }

            switch (_over_data) {
                case No_Over:
                    _index_current += 1;
                    break;
                case Over_Times_1:
                    _index_current += 2;
                    break;
            }

            write(_address_count, _count);
            write(_address_over_data, _over_data);
            write(_address_index_current, _index_current);

            // _FLASH_DEBUG("(writeString) _count = " + String(_count) + "\n");
            // _FLASH_DEBUG("(writeString) _over_data = " + String(_over_data) + "\n");
            // _FLASH_DEBUG("(writeString) Index Current = " + String(_index_current) + "\n");
            // _FLASH_DEBUG("(writeString) No_Error!\n");
            return ExtDueFlashStorage::ErrorFlash::No_Error;
        } else {
            _FLASH_DEBUG("(writeString) Write_Size_Error!\n");
            return ExtDueFlashStorage::ErrorFlash::Write_Size_Error;
        }
    } else {
        _FLASH_DEBUG("(writeString) Init_Error!\n");
        return ExtDueFlashStorage::ErrorFlash::Init_Error;
    }
}

void ExtDueFlashStorage::readString(String arrays[]) {
    for (int i = 0; i < _count; i++) {
        unsigned int address_data = _address_start_data + i*_lenghtPerData;
        String data = "";
        unsigned char _size = read(address_data);
        // _FLASH_DEBUG("(readString) Size = " + String(_size) + "\n");
        for (unsigned int j = 0; j < _size; j++) {
            data += (char) read(address_data + 1 + j);
        }
        // _FLASH_DEBUG("(readString) Data = " + data + "\n");
        arrays[i] = data;
    }
}

unsigned char ExtDueFlashStorage::getCountWriteData() {
    return _count;
}

void ExtDueFlashStorage::reset() {
    _count = 0;
    _index_current = 0;
    _over_data = No_Over;

    write(_address_count, _count);
    write(_address_index_current, _address_count);
    write(_address_over_data, _over_data);
}
