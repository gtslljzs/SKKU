<?php
    session_start();
    $session_id = session_id();

    include "../shop_admin/lib.php";
    $connect = dbconn();
    $mid = $_GET['mid'];

    $query = "delete from bucket_list where mid = '$mid' and session_id = '$session_id'";
    mysqli_query($connect, $query);

?>

<script>
    location.href = 'bucket.php';
</script>