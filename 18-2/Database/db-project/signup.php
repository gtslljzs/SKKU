
<script>
    function chid(){

        document.getElementById("check_flag").value=0;
        var id=document.getElementById("signupID").value;
      
        if(id=="") {
            alert("Inavailable Format");
            exit;
        }
        
        ifrm1.location.href="signup_check.php?loginID="+id; 
    }
</script>

<html>
    <head>
    <title>SIGN UP!</title>
    </head>
 
    <body bgcolor=black>
    <center>
        <form action=signup_complete.php method=post name=frmjoin>
            <table cellpadding=2 cellspacing=2>
            <tr>
                <td colspan=2 align=center><font color=white><b> SIGN&nbsp; UP</td></b>
            </tr>
            <tr> 
                <td align=center><font color=white>ID</td>
                <td>
                    <input type=text name=userID maxlength=15 id="signupID">&nbsp;&nbsp;
                    <input type=button value="Check Duplication" onclick=chid()>
                </td>
                <input type=hidden id="check_flag" name=check_flag value="0">
            </tr>
            <tr> 
                <td align=center><font color=white>Password</td>
                <td><input type=password name=joinpw maxlength=20></td>
            </tr>
            <tr>
                <td align=center><font color=white>Check Password</td>
                <td><input type=password name=joinpw2 maxlength=20></td>
            </tr>
            <tr>
                <td align=center><font color=white>E-Mail</td>
                <td><input type=text name=joinemail maxlength=30></td>
            </tr>
            <tr>
                <td colspan=2 align=center>
                    <input type=submit value="Registration">&nbsp;&nbsp;
                    <input type=reset value="Rewrite">&nbsp;&nbsp;
                    <input type=button value="Cancel" onclick="history.back();">
                </td>
            </tr>
            </table>
        </form>
        <iframe src="" id="ifrm1" scrolling=no frameborder=no width=0 height=0 name="ifrm1">
        </iframe>
    </body>
</html>