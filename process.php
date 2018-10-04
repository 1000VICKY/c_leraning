<?php

$pidfile = "/var/run/hoged/hoged.pid";
// 全てのエラー出力をOFF
error_reporting(0);
// 子プロセスの生成
$pid = pcntl_fork();
if($pid < 0){
        // 失敗時
        exit();
}else if($pid){
        // 親プロセスの終了
        exit();
}else{
        print(">>>>>>>>>>>");
        $pid = getmypid();
        var_dump($pid);
        var_dump(posix_setsid());
        $fp = fopen($pidfile, "w");
        @fwrite( $fp, $pid, strlen($pid) );
        fclose($fp);
        var_dump($fp);
        $sid = posix_setsid();
        var_dump($sid);
        var_dump("<<<<<<<<<<<<");
        var_dump($sid);
        if($sid<0){
                // 失敗時
                exit();
        }
        // ループ処理
        while(TRUE){
                //////////////////////////
                //　　    定期処理       //
                /////////////////////////
        }
}
?>
