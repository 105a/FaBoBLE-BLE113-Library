#include "Arduino.h"

class queue
{
public:
  // push データ挿入
  bool push(uint8_t);
  // pop データ読み込み
  byte pop();
  // front 先頭データ読み込み(読み込みのみ）
  byte front();
  // count データ数
  int size();
  // frush バッファクリア
  void flush();
  
private:
  int const buff_size = 255; // バッファサイズ
  byte data[255];            // バッファ

  int next_push = 0;         // データ挿入位置
  int next_pop  = 0;         // データ取得位置
  int data_size = 0;         // データサイズ
};