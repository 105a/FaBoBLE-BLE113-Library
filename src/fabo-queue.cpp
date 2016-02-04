#include "Arduino.h"
#include "fabo-queue.h"

  // push データ挿入
  bool queue::push(byte push_data){
    // バッファデータのサイズが上限を超えた場合、データ挿入しない
	if (data_size > buff_size){
	  return false;
	}

    //データ挿入
    data[next_push] = push_data;
    //次の挿入位置を取得
    next_push = (next_push + 1) % buff_size;
    // データサイズ　カウントアップ
	data_size++;
	return true;
  }
  
  // pop データ読み込み(読み込み）
  byte queue::pop(){
    // バッファのデータサイズが0の場合、取得せずに0を返す
    if(data_size < 1){
      return 0;
    }
	//データ読み込み
	byte read_byte = data[next_pop];
    //次の取得位置を取得
	next_pop = (next_pop + 1 ) % buff_size;
    // データサイズ　カウントダウン
	data_size--;
	return read_byte;
  }

  // front 先頭データ読み込み(読み込みのみ）
  byte queue::front(){
    // バッファのデータサイズが0の場合、取得せずに0を返す
    if(data_size < 1){
      return 0;
    }
    //データ読み込み
    byte read_byte = data[next_pop];
	return read_byte;
  }

  // count データ数
  int queue::size(){
    return data_size;
  }
  
  // frush クリア
  void queue::flush(){
    // 挿入位置、取得位置、データサイズを初期化
	next_push = 0;
	next_pop  = 0;
	data_size = 0;
	
  }
