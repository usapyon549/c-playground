
# コンパイラの指定
CC = gcc
# コンパイルオプション（警告を全部出す、includeフォルダを検索対象にする）
CFLAGS = -Wall -Iinclude

# 実行ファイル名
TARGET = app.out

# コンパイルの対象ファイル(動作確認用の、add関数)
# SRCS = apps/main.c src/add.c

# コンパイルの対象ファイル(動的配列)
SRCS = apps/main.c src/dynamic.c

# 実行ファイルを作るコマンドの定義
all:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# 作成された実行ファイルを消去するコマンドの定義
clean:
	rm -f $(TARGET)