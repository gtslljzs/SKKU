<?php
    session_start();
    $session_id = session_id();

    include "../shop_admin/lib.php";
    $connect = dbconn();

?>

<li> 장바구니
<table border=1 width=100%>
    <tr>
        <td> 모델명
        <td> 금액
        <td> 수량
        <td> 합계
        <td> 기타
<?php
    $query = "select * from bucket_list where session_id = '$session_id'";
    $result = mysqli_query($connect, $query);
    $total = 0;
    while($data = mysqli_fetch_array($result)) {
?>
        <tr>
            <td> <?=$data[mname]?>
            <td> ￦<?=number_format($data[price])?>
            <td> <?=$data[count]?>
            <td> ￦<?=number_format($data[total_price])?>
            <td> <a href=bucket_del.php?mid=<?=$data[mid]?> onclick = "return confirm('정말 삭제하시겠습니까?');"> [삭제] </a>

<?php
        $total += $data[total_price];
    }
?>

    <tr>
        <td> 합계
        <td>
        <td>
        <td> ￦<?=number_foramt($total)?>

</table>

<a href=list.php> [목록으로] </a>