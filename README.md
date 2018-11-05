# Ghi dữ liệu vào bộ nhớ Flash của Arduino Due

**Info:** Github <https://github.com/thiminhnhut/ExtDueFlashStorage>

**Author:** Thi Minh Nhựt <thiminhnhut@gmail.com>

**Date:** 10/10/2018

**Revision:** 1.0.0

## Thư viện hỗ trợ

* Sử dụng thư viện [DueFlashStorage (author sebnil)](https://github.com/sebnil/DueFlashStorage)

## Ý tưởng thực hiện

* Cách dồn dữ liệu trong bộ nhớ Flash:

  * Lưu tối đa N dữ liệu.

  * Ghi N dữ liệu vào N các ô nhớ.

  |  id  |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  | 10  | 11  | 12  | 13  | 14  | 15  | 16  | 17  | 18  | 19  |
  | ---- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
  | data | 0   | 1   | 2   | 3   | 4   | 5   | 6   | 7   | 8   | 9   | 10  | 11  | 12  | 13  | 14  | 15  | 16  | 17  | 18  | 19  |

  * Khi dữ liệu tràn lần thứ nhất: ghi đè dữ liệu vào những vị trí i%2 == 0.

  |  id  |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  | 10  | 11  | 12  | 13  | 14  | 15  | 16  | 17  | 18  | 19  |
  | ---- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
  | data | 20  | 1   | 21  | 3   | 22  | 5   | 23  | 7   | 24  | 9   | 25  | 11  | 26  | 13  | 27  | 15  | 28  | 17  | 29  | 19  |

  * Bỏ đi một nữa dữ liệu trong danh sách theo quy tắc sau:

      b[k] = data[2k + 1] với khoảng [0, N/4] dữ liệu đầu tiên trong bộ dữ liệu hiện tại.

      b[k] = data[4*(k - N/4) + 2] với [N/4, N/2] dữ liệu tiếp theo trong bộ dữ liệu hiện tại.

  |  id  |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  | 10  | 11  | 12  | 13  | 14  | 15  | 16  | 17  | 18  | 19  |
  | ---- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
  | data | 1   | 3   | 5   | 7   | 9   | 21  | 23  | 25  | 27  | 19  |     |     |     |     |     |     |     |     |     |     |

  * Ghi tiếp dữ liệu vào các vùng nhớ trống tiếp theo và lặp lại quá trình dồn dữ liệu như trên.

## Source code

* [ExtDueFlashStorage](https://github.com/thiminhnhut/ExtDueFlashStorage/tree/master/lib/)

* [Examples](https://github.com/thiminhnhut/ExtDueFlashStorage/blob/master/src/main.cpp)