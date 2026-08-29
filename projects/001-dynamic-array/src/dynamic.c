#include <stdio.h>
#include <stdlib.h>  // malloc ,reallocなどの標準ライブラリ
#include "dynamic.h" // 自作の動的入れるヘッダー


void init_array(DynamicArray *arr, int initial_capacity){

    arr->size = 0; //現在入ってる要素数を0として初期化
    // nullチェックは一旦省略
    arr->capacity = initial_capacity; //最大サイズを指定
    arr->data = malloc(arr->capacity * sizeof(int)); // int型の動的配列のメモリを確保

    if (arr->data == NULL) {
        printf("メモリ確保失敗\n");
        arr->capacity = 0;
        return;
    }

}

void append_array(DynamicArray *arr, int value){

    if (arr->size == arr->capacity){

        int new_capacity = arr->capacity * 2;

        // データを一時的に保存。メモリも拡張する。
        int *tmp = realloc(
            arr->data,
            new_capacity * sizeof(int)
        );

        // データの引っ越しおよび、新しい容量にメモリを拡張
        // arr->data = realloc(arr->data, arr->capacity * sizeof(int));

        // reallocはメモリ拡張に失敗するとNULLが返ってくるので、エラーチェックを入れる
        // nullがarr->dataに直接代入されると、realloc前ののarr->dataのポインタが示すメモリのアドレスを見失う。
        if (tmp == NULL){
            printf("メモリ拡張失敗\n");
            return ;
        } 

        // reallocに成功したあと、メモリを拡張した配列を格納する
        arr->data = tmp;
        arr->capacity = new_capacity;
    
    }

    // 拡張した配列の末尾に新しく追加したい値を代入
    arr->data[arr->size] = value;

    // 値を動的配列に代入したので、現在のサイズを１増やす
    arr->size++;

}

void free_array(DynamicArray *arr){

    free(arr->data); //malloc, reallocしたメモリを解放
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0; 

}



