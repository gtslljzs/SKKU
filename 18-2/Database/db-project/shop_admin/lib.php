
<?php
    function is_auth() {
        if(!isset($_SERVER['PHP_AUTH_USER'])) return false;
        if(!isset($_SERVER['PHP_AUTH_PW'])) return false;
        $user = $_SERVER['PHP_AUTH_USER'];
        $pw = $_SERVER['PHP_AUTH_PW'];
        if($user=='root' && $pw=='1234') return true;
        return false;
    }
    if(!is_auth()) {
        header('WWW-Authenticate: Basic realm="Basic Auth Test"');
        header('HTTP/1.0 401 Unauthorized');
        echo '<meta charset="utf-8">';
        echo '로그인이 필요합니다.';
        exit;
    }
    echo '<meta charset="utf-8">';
    // echo "{$_SERVER['PHP_AUTH_USER']} 님 반갑습니다.";

    function dbconn() {
        $connect = mysqli_connect("localhost", "root", "1234", "dbproject");
        // mysqli_select_db("dbproject", $connect);

        return $connect;
    }

?>