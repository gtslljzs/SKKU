<?php
    include "lib.php";
    $connect = dbconn();
    $img = $_POST[img_name];
    $uploaddir = 'v:/Apache24/htdocs/shop/data/';
    $uploadfile = $uploaddir.basename($_FILES['img_name']['name']);

    if($_POST[img_name]) {
        move_uploaded_file($_FILES['img_name']['tmp_name'], $uploadfile);
    }

    $query = "insert into iphone_model(mname, mcolor, storage, os, display, inch, pixel, weight, price, mstock, img)
                values('$_POST[mname]', '$_POST[mcolor]', '$_POST[storage]', '$_POST[os]', '$_POST[display]',
                '$_POST[inch]', '$_POST[pixel]', '$_POST[weight]', '$_POST[price]', '$_POST[mstock]', '$_POST[img_name]'
                )";

    mysqli_query($connect, $query);
    echo mysqli_error($connect);
?>

<script>
    location.href = 'shop_add.php';
</script>
