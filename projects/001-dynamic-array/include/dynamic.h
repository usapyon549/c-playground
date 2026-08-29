#ifndef DYNAMIC_H // インクルードガード(２回目以降の読み込みをスキップ)
#define DYNAMIC_H


// 動的配列を管理する構造体の定義
typedef struct {
    int *data;      // 値を格納するメモリへのポインタ
    int size;       // 現在入っている要素の数
    int capacity;   // 最大容量
} DynamicArray;

void init_array(DynamicArray *arr, int initial_capacity);

void append_array(DynamicArray *arr, int value);

void free_array(DynamicArray *arr);

#endif