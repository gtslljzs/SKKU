<?php
    include "../shop_admin/lib.php";
    $connect = dbconn();
    $mid = $_GET['mid'];

    $query = "select * from iphone_model where mid = '$mid'";
    $result = mysqli_query($connect, $query);
    $data = mysqli_fetch_array($result);

?>
<form action=bucket_post.php method=post>
    <input type=hidden name=mid value='<?=$mid?>'>
<table width=100% border=1>
    <tr>
        <td> <img src=data/<?=$data[img]?> width=150 border=0>
        <td>
            <b> <?=$data[mname]?> </b> <br>
            <?=$data[mcolor]?>
    <tr> 제품 사양
        <td colspan=2>
            용량 : <?=$data[storage]?> GB <br>
            운영체제 : <?=$data[os]?> <br>
            디스플레이 :  <?=$data[display]?> <br>
            베젤 : <?=$data[inch]?> 형 <br>
            픽셀 : <?=$data[pixel]?> <br>
            무게 : <?=$data[weight]?> g <br>
            가격 : ￦<?=number_format($data[price])?> <br>
            수량 : 
            <select name=count>
                <option value='1'> 1개
                <option value='2'> 2개
                <option value='3'> 3개
                <option value='4'> 4개
                <option value='5'> 5개
                <option value='6'> 6개
                <option value='7'> 7개
                <option value='8'> 8개
                <option value='9'> 9개
                <option value='10'> 10개
            </select>
            <input type=submit value='장바구니에 담기'>
</table>

<a href=list.php> [목록] </a>
</form>