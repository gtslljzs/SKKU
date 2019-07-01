<?php
    session_start();
    $session_id = session_id();

    include "../shop_admin/lib.php";
    $connect = dbconn();
    $mid = $_POST['mid'];
    $count = $_POST[count];

    $query = "select * from iphone_model where mid = '$mid'";
    $result = mysqli_query($connect, $query);
    $data = mysqli_fetch_array($result);

    $total = $count * $data[price];
    $regtime = time();

    $query = "insert into bucket_list(session_id, parent, mname, count, price, total_price, regtime)
        values ('$session_id', '$data[mid]', '$data[mname]', '$count',
        '$data[price]', '$total', '$regtime')";
    mysqli_query($connect, $query);

?>

<script>
    location.href = 'bucket.php';
</script>

