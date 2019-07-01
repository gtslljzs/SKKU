<?php
    include "../shop_admin/lib.php";
    $connect = dbconn();

?>

<a href=bucket.php> [장바구니 바로가기] </a>
<table width=100% border=1>

<?php
    $query = "select * from iphone_model";
    $result = mysqli_query($connect, $query);
    while($data = mysqli_fetch_array($result)) {
?>
        <tr>
            <td> <a href=view.php?mid=<?=$data[mid]?>> <img src=data/<?=$data[img]?> width=150 border=0> </a>
            <td>
                <b> <a href=view.php?mid=<?=$data[mid]?>> <?=$data[mname]?> </a> </b> <br>
                <?=$data[mcolor]?>
<?php
    }
?>

</table>