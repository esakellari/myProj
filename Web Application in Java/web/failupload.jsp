

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">

<jsp:useBean id="user"   class="beans.User"             scope="session" />
<jsp:useBean id="access" class="beans.Access"           scope="session" />
<jsp:useBean id="sinfo"  class="beans.SearchInfo"       scope="session" />
<jsp:useBean id="msgs"   class="java.util.LinkedList"   scope="session" />
<jsp:useBean id="path"   class="beans.Path"             scope="session" />

<%  if(access.get() == false) { %>
        <jsp:forward page="error404.jsp" />
<%  } %>



<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <link type="text/css" rel="stylesheet" href="styles.css">
        <title>Storage Cloud - FailUpload</title>
    </head>
    <body>
        <jsp:include page="classic.jsp" />



          <div style="text-align:center; position:relative;  font-size:30pt; font-weight:100; color:darkblue; font-family:Impact;">
              You have no more free space!<br/>
                To buy more space press here</div>

          <form method="post" action="MoreSpace.jsp" >
            <input type="submit" value="Buy More space"
                style="background-color:Aqua ; text-align:left;
                               position:relative; margin-left:44%;font-size:15pt; font-variant:small-caps;" />
         </form>


          <br/><br/>
        <form method="post" action="ViewFolders?new_path=root" >
                        <input type="submit" value="Back to My Dirs"
                               style="background-color:Aqua ;text-align:left;
                               position:relative; margin-left:45%; font-size:13pt; font-variant:small-caps;" />
        </form>


    </body>
</html>
