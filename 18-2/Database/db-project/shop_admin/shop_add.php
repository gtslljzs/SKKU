<?php
    include "lib.php";
?>

<li> 제품 추가
<form action=shop_add_post.php method=POST enctype=multipart/from-data>
<table width = 100% border = 1>
    <tr>
        <td> 모델명
        <td> <input type=text name=mname size=20>
    <tr>
        <td> 색상
        <td> <input type=text name=mcolor size=20>
    <tr>
        <td> 용량
        <td> <input type=text name=storage size=20>
    <tr>
        <td> 운영체제
        <td> <input type=text name=os size=20>
    <tr>
        <td> 디스플레이
        <td> <input type=text name=display size=20>
    <tr>
        <td> 베젤 넓이
        <td> <input type=text name=inch size=20>
    <tr>
        <td> 픽셀
        <td> <input type=text name=pixel size=20>
    <tr>
        <td> 무게
        <td> <input type=text name=weight size=20>
    <tr>
        <td> 가격
        <td> <input type=text name=price size=20>
    <tr>
        <td> 재고
        <td> <input type=text name=mstock size=20>
    <tr>
        <td> 이미지
        <td> <input type=file name=img_name size=30>
    <tr>
        <td colspan=2>
        <input type=submit value='등록하기'>
</table>
</form>