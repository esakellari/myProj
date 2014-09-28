
<%@page contentType="text/html" pageEncoding="UTF-8" import="beans.*" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">

<jsp:useBean id="user"   class="beans.User"           scope="session" />
<jsp:useBean id="access" class="beans.Access"         scope="session" />
<jsp:useBean id="msgs"   class="java.util.LinkedList" scope="session" />

<%  if(access.get() == false) { %>
        <jsp:forward page="error404.jsp" />
<%  } %>



<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <link type="text/css" rel="stylesheet" href="styles.css">
        <title> Storage Cloud - Already Logged In </title>
    </head>
    <body>
        <jsp:include page="classic.jsp" />

        <br /><br />
        <div style="text-align:center; position:relative;  font-size:30pt; font-weight:100; color:darkblue; font-family:Impact;">Hey "<%= user.getUsername() %>"! </div>
       <div style="text-align:center; position:relative;  font-size:30pt; font-weight:100; color:darkblue; font-family:Impact;">

            <%  if(msgs.contains("AlreadyLoggedIn") == true) {
                    msgs.remove("AlreadyLoggedIn");
            %>
                  
                       You are already logged in!
            <%  } else if(msgs.contains("NotLoggedIn") == true) {
                    msgs.remove("NotLoggedIn");
            %>
                    You are not logged in!
            <%  } else if(msgs.contains("longFileName") == true) {
                    msgs.remove("longFileName");
            %>
                    The filename must be less than 128 characters
            <%  } else if(msgs.contains("WrongSize") == true) {
                    msgs.remove("WrongSize");
            %>
                    You have not given the size correctly(KB,MB,GB)
            <%  } %>
        </div>

        <br /><br />

        <table align="center" style="background-color:paleturquoise; border-width:4pt; border-color:darkblue; border-style:ridge;">
            <tr><td align="center">
                <form method="post" action="Home" >
                    <input type="submit" value="[ Go to Start Page ]"
                           style="background-color:lawngreen; font-size:16pt;" />
                </form>
            </td></tr>
        </table>

    </body>
</html>
