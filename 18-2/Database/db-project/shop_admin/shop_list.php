<?php
    include "lib.php";
    $connect = dbconn();

    $query = "select * from iphone_model";
    $result = mysqli_query($connect, $query);
?>

<table border=1 width=100%>
    <tr>
        <td> 이미지
        <td> 모델
        <td> 색상
        <td> 기타
<?php
    while($data = mysqli_fetch_array($result)) {
?>
        <tr>
            <td> <img src=../shop/data/<?=$data[img]?> width=150>
            <td> <?=$data[mname]?>
            <td> <?=$data[mcolor]?>
            <td> <a href=shop_edit.php?mid=<?=$data[mid]?>> 수정 </a>
<?php
    }
?>