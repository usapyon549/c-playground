# 001-dynamic-array

C言語のポインタ・構造体・動的メモリ確保を用いて、シンプルな動的配列を実装する。

## Summary

C言語の `malloc()` / `realloc()` / `free()` とポインタ、構造体を使って、容量を自動的に拡張できる動的配列を実装した。

今回はC言語の基礎学習を目的として、動的配列そのものだけでなく、ヘッダーファイルによる関数宣言、ソースファイルの分離、インクルードガード、Makefileによるコンパイルまで一通り試した。

---

## Overview

C言語には、Pythonの `list` やC++の `vector` のように、要素数に応じて自動的に拡張される配列は標準機能として用意されていない。

そこで、以下の情報を構造体で管理することで、簡単な動的配列を実装した。

```c
typedef struct {
    int *data;
    int size;
    int capacity;
} DynamicArray;
```

* `data`：動的に確保したメモリへのポインタ
* `size`：現在格納されている要素数
* `capacity`：確保しているメモリの容量

容量がいっぱいになった場合は `realloc()` を使用して容量を2倍に拡張する。

---

## Features

### Dynamic Array

以下の3つの関数を実装した。

| Function         | Description |
| ---------------- | ----------- |
| `init_array()`   | 動的配列の初期化    |
| `append_array()` | 配列の末尾に要素を追加 |
| `free_array()`   | 使用したメモリを解放  |

### Automatic Capacity Expansion

要素を追加した際に、

```text
size == capacity
```

となった場合、現在の容量を2倍にしてメモリを再確保する。

```text
Initial capacity: 5

[ ][ ][ ][ ][ ]
       ↓
5 elements added
       ↓
realloc()
       ↓
Capacity: 10

[ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
```

この方式により、あらかじめ必要な配列サイズが分からない場合でも、要素を追加しながら配列を拡張できる。

---

## Memory Management

動的配列では、`malloc()` でヒープ領域にメモリを確保している。

```c
arr->data = malloc(arr->capacity * sizeof(int));
```

容量が不足した場合は `realloc()` を使用する。

```c
int *tmp = realloc(
    arr->data,
    new_capacity * sizeof(int)
);
```

最後に使用しなくなったメモリを `free()` で解放する。

```c
free(arr->data);
```

また、解放後はポインタやサイズを初期状態に戻している。

```c
arr->data = NULL;
arr->size = 0;
arr->capacity = 0;
```

### `realloc()` の戻り値を一時変数で受ける

`realloc()` によるメモリ拡張が失敗すると `NULL` が返る。

そのため、

```c
arr->data = realloc(arr->data, new_size);
```

のように直接代入すると、失敗時に元のメモリを指していたポインタを失ってしまう可能性がある。

そこで、一度 `tmp` に受けてから成功した場合のみポインタを更新する。

```c
int *tmp = realloc(arr->data, new_size);

if (tmp == NULL) {
    printf("メモリ拡張失敗\n");
    return;
}

arr->data = tmp;
```

今回の実装では、このパターンを通して `realloc()` の扱い方を学習した。

---

## Pointer and Structure

`DynamicArray` は構造体として定義している。

```c
DynamicArray dynamic_array;
```

関数側では構造体へのポインタを受け取る。

```c
void init_array(DynamicArray *arr, int initial_capacity);
```

`main()` からは、構造体そのものではなく、そのアドレスを渡している。

```c
init_array(&dynamic_array, 5);
```

`&` によって変数のアドレスを取得し、`DynamicArray *` 型の引数として渡している。

また、構造体へのポインタからメンバへアクセスする場合は、アロー演算子 `->` を使用する。

```c
arr->size
arr->capacity
arr->data
```

これは、

```c
(*arr).size
```

のようなアクセスを簡潔に記述するための演算子でもある。

---

## Header Files

実装をヘッダーファイルとソースファイルに分離した。

```text
dynamic.h
dynamic.c
```

`dynamic.h` では構造体の定義と関数の宣言を行う。

```c
void init_array(DynamicArray *arr, int initial_capacity);
void append_array(DynamicArray *arr, int value);
void free_array(DynamicArray *arr);
```

一方、`dynamic.c` では実際の関数処理を実装する。

このように、**「何を提供するか」と「どう実装するか」を分離する**構成を試した。

### Include Guard

ヘッダーファイルにはインクルードガードを設定した。

```c
#ifndef DYNAMIC_H
#define DYNAMIC_H

// ...

#endif
```

これにより、同じヘッダーファイルが複数回読み込まれることによる問題を防ぐ。

`add.h` にも同様のインクルードガードを設定している。

---

## Makefile

コンパイル方法を確認するため、簡単な `add()` 関数を作成してMakefileを試した。

```c
int add(int a, int b) {
    return a + b;
}
```

その後、動的配列の実装をコンパイル対象に変更した。

```makefile
SRCS = apps/main.c src/dynamic.c
```

コンパイラには `gcc` を使用している。

```makefile
CC = gcc
CFLAGS = -Wall -Iinclude
```

`make` によって実行ファイルを作成し、`make clean` によって生成された実行ファイルを削除できるようにした。

```bash
make
make clean
```

---

## Project Structure

```text
001-dynamic-array/
├── apps/
│   └── main.c
├── include/
│   ├── add.h
│   └── dynamic.h
├── src/
│   ├── add.c
│   └── dynamic.c
├── Makefile
└── README.md
```

`add.c` / `add.h` はMakefileやヘッダーファイルの分離を試すために作成した簡単なサンプルで、動的配列の実装自体は `dynamic.c` / `dynamic.h` にまとめている。

---

## Execution Flow

`main.c` では、初期容量5の動的配列を作成し、10個の値を追加する。

```text
DynamicArray
     │
     ├── size = 0
     ├── capacity = 5
     └── data → [ ][ ][ ][ ][ ]
                    │
                    ▼
              append_array()
                    │
             size == capacity?
                    │
              ┌─────┴─────┐
             No           Yes
              │             │
              │          realloc()
              │             │
              └──────┬──────┘
                     ▼
                 要素を追加
                     │
                     ▼
                size++
```

初期容量を超えて要素を追加すると、`realloc()` によって容量が2倍になる。

最後に `free_array()` を呼び出して、確保したメモリを解放する。

---

## What I Learned

### 1. 構造体

構造体は、複数のデータをまとめて扱うためのユーザー定義型。

今回の動的配列では、

```c
typedef struct {
    int *data;
    int size;
    int capacity;
} DynamicArray;
```

として、配列本体・現在の要素数・容量をひとまとめにして管理した。

---

### 2. ポインタ

`*` を使ってポインタ変数を定義できる。

```c
int *data;
```

また、`&` を使うことで変数のアドレスを取得できる。

```c
init_array(&dynamic_array, 5);
```

今回の実装では、**「値そのもの」ではなく「メモリ上の場所」を扱う**というC言語の特徴を学んだ。

---

### 3. 動的メモリ確保

`malloc()` によって実行時に必要なメモリを確保できる。

```c
malloc(arr->capacity * sizeof(int));
```

`sizeof(int)` によって `int` 型1個分のサイズを取得し、必要な要素数を掛け合わせて確保するメモリサイズを決めている。

C言語では常に具体的な型名を直接書かなければならないわけではなく、`sizeof` のように型のサイズを利用する書き方もある。

---

### 4. `malloc()` と `free()`

動的に確保したメモリは、不要になったら `free()` で解放する。

ただし、**「C言語だから全部freeする」わけではない。**

重要なのは、自分が保持しているヒープメモリについて、最後にアクセスできるポインタから適切に解放すること。

今回の場合は `arr->data` が確保したメモリを指しているため、

```c
free(arr->data);
```

として解放している。

---

### 5. `realloc()`

`realloc()` を使うことで、すでに確保したメモリ領域のサイズを変更できる。

今回の動的配列では、容量がいっぱいになったときに容量を2倍にしている。

```c
int new_capacity = arr->capacity * 2;
```

---

### 6. `realloc()` と一時ポインタ

`realloc()` が失敗すると `NULL` が返るため、元のポインタを直接上書きしないようにする必要がある。

```c
int *tmp = realloc(arr->data, new_size);
```

成功した場合のみ、

```c
arr->data = tmp;
```

と更新する。

このパターンを通して、**動的メモリ操作では「ポインタを失わないこと」が重要**だと学んだ。

---

### 7. アロー演算子 `->`

構造体へのポインタからメンバにアクセスするときは `->` を使用する。

```c
arr->size
arr->capacity
arr->data
```

構造体そのものからメンバへアクセスする `.` と使い分ける。

---

### 8. 関数の宣言と実装の分離

関数の実装とは別に、ヘッダーファイルで関数の返り値の型と引数の型を宣言する。

```c
void append_array(DynamicArray *arr, int value);
```

実際の処理は `dynamic.c` に記述する。

```text
dynamic.h
    ↓
「この関数があります」

dynamic.c
    ↓
「この関数はこう動きます」
```

---

### 9. `printf()` とデータ型

`printf()` で値を表示するときにも、データ型に対応したフォーマット指定子が必要になる。

例えば `int` 型なら、

```c
printf("%d\n", value);
```

とする。

C言語では、こうした型を意識した記述が必要になることを改めて確認した。

---

## Reflection

今回の実装では、単純な動的配列を題材として、

* 構造体
* ポインタ
* `malloc()`
* `realloc()`
* `free()`
* ヘッダーファイル
* インクルードガード
* Makefile

といったC言語の基本要素をまとめて触ることができた。

特に、動的メモリを扱うことで、Pythonなどの高水準言語では意識する機会の少ない**「データがメモリ上のどこに存在しているか」**を意識してコードを書くことができた。

今後も小さな実装を積み重ねながら、C言語のメモリ管理やデータ構造への理解を深めていく。
