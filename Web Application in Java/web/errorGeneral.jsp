
<%@page contentType="text/html" pageEncoding="UTF-8" import="beans.*" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">

<jsp:useBean id="access" class="beans.Access" scope="session" />

<%  if(access.get() == false) { %>
        <jsp:forward page="error404.jsp" />
<%  } %>


<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <link type="text/css" rel="stylesheet" href="styles.css">
        <title> Storage Cloud - Error </title>
    </head>
    <body>
        <jsp:include page="classic.jsp" />



        <br /><br />
        <div style="text-align:center; font-size:24pt; font-weight:bold;">
            We're sorry! An error occurred during the process.<br/>
        </div>
        

        <br /><br />

        <table align="center" style="background-color:paleturquoise; border-width:4pt; border-color:darkblue; border-style:ridge;">
            <tr><td align="center">
                <form method="post" action="index.jsp" >
                    <input type="submit" value="[ Go to Start Page ]"
                           style="background-color:lawngreen; font-size:16pt;" />
                </form>
            </td></tr>
        </table>

    </body>
</html>
