#include <stdio.h>
#include <stdlib.h>
#include "add.h" //　自作したadd関数ヘッダーファイルを読み込む
#include "dynamic.h" //自作したdynamic関数のヘッダーファイルを読み込む

int main(){
    // printf("Hello World!\n");

    // Makefileなどの動作確認を含めて、自作したadd関数を使ってみる
    // int result = add(1, 1);
    // printf("1 + 1 = %d\n", result);

    // 動的配列
    DynamicArray dynamic_array;

    // 動的配列の初期化
    init_array(&dynamic_array, 5);

    // 動的配列に値を追加
        
    for (int i = 0; i<10; i++){
        
        int value =rand() %10; //動的配列に代入する値を乱数で生成
        append_array(&dynamic_array, value);

    }

    // 配列の中身の表示
    for (int i = 0; i<dynamic_array.size; i++){
        printf(
            "%d番目の配列の中身: %d\n", i, dynamic_array.data[i]
        );
    }

    // 使用したメモリを解放
    free_array(&dynamic_array);

    return 0;

}