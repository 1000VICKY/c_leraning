package main

import (
	"bufio"
	"errors"
	"fmt"
	_ "io"
	"io/ioutil"
	"os"
	exe "os/exec"
	"path/filepath"
	"regexp"
	"runtime"
	"runtime/debug"
	"strings"
	"syscall"
)

/*

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


char* get_command_line ( char* cp) {
    // コマンドラインから取得した1バイト分の文字
     char c ;
    // メモリ拡張時の一時的アドレス
     char* t_cp;
    int slen = 0;
    int index = 0;
    while(c = getchar()) {
        slen++;
        t_cp = ( char*)realloc(cp, sizeof( char) * slen);
        // メモリ確保失敗時
        if (t_cp == NULL) {
            free(cp);
            return(NULL);
        }
        if (feof(stdin) == 1) {
            printf("EOFを検出");
        }
        // アドレスが変化した場合
        if (cp != t_cp) {
            cp = t_cp;
        }
        if (c == '\n') {
            *(cp + (index)) =  0;
            slen = 0;
            index = 0;
            //*(cp) = '\0';
            return(cp);
        }
        cp[index] = c;
        //*(cp + index) = c;
        index++;
    }
}
extern char*  get_command_line( char*);
*/
import "C"

/*
// 関数ポインタの型宣言
	char* sample_function (char* a, char* b) {
		char* inner_char =(char*)malloc(sizeof(char) * (strlen(a) + strlen(b) + 1));
		strcpy(inner_char, a);
		strcpy(inner_char + strlen(a), b);
		return (inner_char);
	}
int main () {

	char* (*get_char_pointer)(char*, char*);
	get_char_pointer = sample_function;
	printf("%s", get_char_pointer("あいうえお", "かきくけこ"));
}



	int function_which_take_function ( int (*local_function) (int, int), int a, int b) {
		int inner_int = 0;
		inner_int = local_function (a, b);
		return (inner_int);
	}
	// プロトタイプ宣言
	int temp_function (int, int);
	typedef int (*Format)(int, int);
	int main () {
		Format ff;
		ff = temp_function;
		printf("%d", ff(12,34));
		printf("%d", function_which_take_function (temp_function, 3, 7));
	}
	int temp_function (int a, int b) {
		int inner_total = a + b;
		return (inner_total);
	}

*/
var format func(...interface{}) (int, error) = fmt.Println
var myPrint func(...interface{}) (int, error) = fmt.Print

func main() {
	cs := C.CString("")
	print(C.GoString(C.get_command_line(cs)))

	const initializer = "<?php " + "\n"
	// 利用変数初期化
	var input string
	var line *string = new(string)
	var ff *os.File
	var err error
	var tentativeFile *string = new(string)
	var writtenByte *int = new(int)
	// ダミー実行ポインタ
	ff, err = ioutil.TempFile("", "__php__main__")
	if err != nil {
		format(err)
		os.Exit(255)
	}
	ff.Chmod(os.ModePerm)
	*writtenByte, err = ff.WriteAt([]byte(initializer), 0)
	if err != nil {
		format(err)
		os.Exit(255)
	}
	// ファイルポインタに書き込まれたバイト数を検証する
	if *writtenByte != len(initializer) {
		format("<スクリプトファイルの初期化に失敗しました.>")
		os.Exit(255)
	}
	// ファイルポインタオブジェクトから絶対パスを取得する
	*tentativeFile, err = filepath.Abs(ff.Name())
	if err != nil {
		format(err)
		os.Exit(255)
	}
	defer ff.Close()
	defer os.Remove(*tentativeFile)

	var count int = 0
	var ss int = 0
	var multiple int = 0
	var backup []byte = make([]byte, 0)
	var currentDir string

	// 末尾はバックスラッシュの場合，以降再びバックスラッシュで終わるまで
	// スクリプトを実行しない
	var openBrace *regexp.Regexp = new(regexp.Regexp)
	var openCount int = 0
	var closeBrace *regexp.Regexp = new(regexp.Regexp)
	var closeCount int = 0
	openBrace, _ = regexp.Compile("^.*{[ \t]*$")
	closeBrace, _ = regexp.Compile("^[ \t]*}.*$")
	// [save]というキーワードを入力した場合の正規表現
	var saveRegex *regexp.Regexp = new(regexp.Regexp)
	saveRegex, err = regexp.Compile("^[ ]*save[ ]*$")
	if err != nil {
		format(err)
		os.Exit(255)
	}
	var scanner *bufio.Scanner = new(bufio.Scanner)
	scanner = bufio.NewScanner(os.Stdin)
	for {
		runtime.GC()
		// ループ開始時に正常動作するソースのバックアップを取得
		ff.Seek(0, 0)
		backup, err = ioutil.ReadAll(ff)
		if err != nil {
			format(err.Error())
			break
		}

		ff.WriteAt(backup, 0)
		if multiple == 1 {
			myPrint("... ")
		} else {
			myPrint(">>> ")
		}
		scanner.Scan()
		*line = scanner.Text()

		if *line == "del" {
			ff, err = deleteFile(ff, "<?php ")
			if err != nil {
				format(err)
				os.Exit(255)
			}
			*line = ""
			input = ""
			count = 0
			continue
		} else if saveRegex.MatchString(*line) {
			/*
			   [save]コマンドが入力された場合，その時点まで入力されたスクリプトを
			   カレントディレクトリに保存する
			*/
			currentDir, err = os.Getwd()
			if err != nil {
				format(err)
				os.Exit(255)
			}
			currentDir, err = filepath.Abs(currentDir)
			if err != nil {
				format(err)
				break
			}
			if runtime.GOOS == "windows" {
				// C:\\aaa\\bbb\\save.php
				currentDir += "\\save.php"
			} else {
				// /aaa/bbb/save.php
				currentDir += "/save.php"
			}
			var saveFp *os.File = new(os.File)
			saveFp, err = os.Create(currentDir)
			if err != nil {
				format(err)
				continue
			}
			saveFp.Chmod(os.ModePerm)
			*writtenByte, err = saveFp.WriteAt(backup, 0)
			if err != nil {
				saveFp.Close()
				fmt.Println(err)
				os.Exit(255)
			}
			format(currentDir + ":入力した内容を保存しました。")
			saveFp.Close()
			*line = ""
			input = ""
			continue
		} else if *line == "" {
			// 空文字エンターの場合はループを飛ばす
			continue
		}

		ob := openBrace.MatchString(*line)
		if ob == true {
			openCount = openCount + 1
		}
		cb := closeBrace.MatchString(*line)
		if cb == true {
			closeCount = closeCount + 1
		}
		// ブレースによる複数入力フラグがfalseの場合
		if openCount == 0 && closeCount == 0 {
			multiple = 0
		} else if openCount != closeCount {
			multiple = 1
		} else if openCount == closeCount {
			multiple = 0
			openCount = 0
			closeCount = 0
		} else {
			panic("Runtime Error happened!:")
		}
		input += *line + "\n"
		if multiple == 0 {
			ss, err = ff.Write([]byte(input))
			if err != nil {
				format("<ファイルポインタへの書き込み失敗>")
				format("    " + err.Error())
				continue
			}
			if ss > 0 {
				input = ""
				*line = ""
				count, err = tempFunction(ff, tentativeFile, count, backup)
				if err != nil {
					continue
				}
			}
		} else if multiple == 1 {
			continue
		} else {
			panic("<Runtime Error>")
		}
	}
}

func tempFunction(fp *os.File, filePath *string, beforeOffset int, temporaryBackup []byte) (int, error) {
	defer debug.FreeOSMemory()
	var strOutput []string = make([]string, 0)
	var output []byte = make([]byte, 0)
	var e error = nil
	var index *int = new(int)
	runtime.GC()
	// バックグラウンドでPHPをコマンドラインで実行
	// (1)まずは終了コードを取得
	e = exe.Command("php", *filePath).Run()
	if e != nil {
		var ok bool
		var exitError *exe.ExitError = nil
		var exitStatus int = 0
		if exitError, ok = e.(*exe.ExitError); ok == true {
			if s, ok := exitError.Sys().(syscall.WaitStatus); ok == true {
				exitStatus = s.ExitStatus()
				if exitStatus != 0 {
					// スクリプトを実行した結果、実行失敗の場合
					output, e = exe.Command("php", *filePath).Output()
					castStr := string(output)
					// 改行で区切って[]string型に代入する
					strOutput = strings.Split(castStr, "\n")
					for key, value := range strOutput {
						fmt.Println("    " + value)
						strOutput[key] = ""
					}
					fmt.Println("    " + e.Error())
					fp.Truncate(0)
					fp.Seek(0, 0)
					fp.WriteAt(temporaryBackup, 0)
					debug.FreeOSMemory()
					return beforeOffset, e
				}
			} else {
				panic(errors.New("Unimplemented for system where exec.ExitError.Sys() is not syscall.WaitStatus."))
			}
		}
	}
	output, _ = exe.Command("php", *filePath).Output()
	/*
		var s string = string(output);
		var sarray []string = strings.Split(s, "\n");
	*/
	strOutput = strings.Split(string(output), "\n")
	strOutput = strOutput[beforeOffset:]
	*index = len(strOutput) + beforeOffset
	for key, value := range strOutput {
		fmt.Println("    " + value)
		strOutput[key] = ""
	}
	output = nil
	strOutput = nil
	fp.Write([]byte("echo(PHP_EOL);"))
	// プログラムが確保したメモリを強制的にOSへ返却
	debug.FreeOSMemory()
	//fmt.Println("    " + strconv.Itoa(*index) + "byte")
	return *index, e
}

func deleteFile(fp *os.File, initialString string) (*os.File, error) {
	defer debug.FreeOSMemory()
	var size int
	var err error
	fp.Truncate(0)
	fp.Seek(0, 0)
	size, err = fp.WriteAt([]byte(initialString), 0)
	fp.Seek(0, 0)
	if err == nil && size == len(initialString) {
		return fp, err
	} else {
		return fp, errors.New("一時ファイルの初期化に失敗しました。")
	}
}
