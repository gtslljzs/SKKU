<?php
    include "lib.php";
    $connect = dbconn();
    $mid = $_GET['mid'];

    $query = "select * from iphone_model where mid = '$mid' ";
    $result = mysqli_query($connect, $query);
    $data = mysqli_fetch_array($result);
?>

<li> 제품 수정
<form action=shop_edit_post.php method=POST enctype=multipart/from-data>
<input type=hidden name=mid value="<?mid?>">
<input type=hidden name=old_name value="<?=$data[img]?>">
<table width = 100% border = 1>
    <tr>
        <td> 모델명
        <td> <input type=text name=mname size=20 value="<?=$data[mname]?>">
    <tr>
        <td> 색상
        <td> <input type=text name=mcolor size=20 value="<?=$data[mcolor]?>">
    <tr>
        <td> 용량
        <td> <input type=text name=storage size=20 value="<?=$data[storage]?>">
    <tr>
        <td> 운영체제
        <td> <input type=text name=os size=20 value="<?=$data[os]?>">
    <tr>
        <td> 디스플레이
        <td> <input type=text name=display size=20 value="<?=$data[display]?>">
    <tr>
        <td> 베젤 넓이
        <td> <input type=text name=inch size=20 value="<?=$data[inch]?>">
    <tr>
        <td> 픽셀
        <td> <input type=text name=pixel size=20 value="<?=$data[pixel]?>">
    <tr>
        <td> 무게
        <td> <input type=text name=weight size=20 value="<?=$data[weight]?>">
    <tr>
        <td> 가격
        <td> <input type=text name=price size=20 value="<?=$data[price]?>">
    <tr>
        <td> 재고
        <td> <input type=text name=mstock size=20 value="<?=$data[mstock]?>">
    <tr>
        <td> 이미지
        <td> <img src=../shop/data/<?=$data[img]?> width=150> <br>
            <input type=file name=img_name size=30>
    <tr>
        <td colspan=2>
        <input type=submit value='수정하기'>
</table>
</from>