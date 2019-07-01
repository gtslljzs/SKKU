<?php
    include "lib.php";
    $connect = dbconn();
    $img = $_POST[img_name];
    $uploaddir = 'v:/Apache24/htdocs/shop/data/';
    $uploadfile = $uploaddir.basename($_FILES['img_name']['name']);

    if($_POST[img_name]) {
        unlink("../shop/data/".$_POST[old_name]);
        move_uploaded_file($_FILES['img_name']['tmp_name'], $uploadfile);
        $tmp = "img='$_POST[img_name]',";
    }

    $query = "update iphone_model set
                mname='$_POST[mname]',
                mcolor='$_POST[mcolor]',
                storage = '$_POST[storage]',
                os = '$_POST[os]',
                display = '$_POST[display]',
                inch = '$_POST[inch]',
                pixel = '$_POST[pixel]',
                weight = '$_POST[weight]',
                price = '$_POST[price]',
                mstock='$_POST[mstock]',
                $tmp
                where mid='$_POST[mid]'";
    
    mysqli_query($connect, $query);
    echo mysqli_error($connect);
?>

<script>
    location.href = 'shop_list.php';
</script>
