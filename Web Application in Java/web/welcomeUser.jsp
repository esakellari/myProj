
<%@page contentType="text/html" pageEncoding="UTF-8" import="beans.*" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">

<jsp:useBean id="user"   class="beans.User"   scope="session" />
<jsp:useBean id="path"   class="beans.Path"   scope="session" />
<jsp:useBean id="access" class="beans.Access" scope="session" />

<%  if(access.get() == false) { %>
        <jsp:forward page="error404.jsp" />
<%  } %>



<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <link type="text/css" rel="stylesheet" href="styles.css">
        <title> Storage Cloud - Welcome </title>
    </head>
    <body>
        <jsp:include page="classic.jsp" />

        <br /><br />
        <div style="text-align: center;color:darkblue; font-size:32pt; font-weight:100; font-family: Impact"> Hello "<%= user.getUsername() %>"! </div>
        <div style="text-align: center;color:darkblue; font-size:32pt; font-weight:100; font-family: Impact"> Welcome to the <br /> Storage Cloud! </div>

        <br /><br />

        <table align="center" style="background-color:paleturquoise; border-width:4pt; border-color:darkblue; border-style:ridge;">
            <tr><td align="center">
                <form method="post" action="Home" >
                    <input type="submit" value="[ Go to Start Page ]"
                           style="background-color:lawngreen; font-size:16pt;" />
                </form>
            </td></tr>
            <tr><td align="center">
                <form method="post" action="<%= response.encodeURL(path.get()) %>" >
                     <input type="submit" value="[ Go to The page you were previously viewing ]"
                            style="background-color:lawngreen; font-size:16pt;" />
                </form>
            </td></tr>
        </table>

    </body>
</html>
