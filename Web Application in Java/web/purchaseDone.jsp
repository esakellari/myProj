
<%@page contentType="text/html" pageEncoding="UTF-8" import="java.util.*"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">

<jsp:useBean id="failed" class="java.util.ArrayList" scope="session" />
<jsp:useBean id="bought" class="java.util.ArrayList" scope="session" />
<jsp:useBean id="access" class="beans.Access"        scope="session" />
<jsp:useBean id="path"   class="beans.Path"          scope="session" />

<%  if(access.get() == false) { %>
        <jsp:forward page="error404.jsp" />
<%  } %>



<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <link type="text/css" rel="stylesheet" href="styles.css">
        <title> Storage Cloud - Purchase Completed </title>
    </head>
    <body>
        <jsp:include page="classic.jsp" />


        <br />
        <table align="center" style="background-color:white; border-style:solid; border-width:thin; border-color:darkblue;">
            <tr style="font-weight:bold; background-color:#CCCCFF;"><td>
                &nbsp;&nbsp;You bought the following files:&nbsp;&nbsp;
            </td></tr>
            <%  int i = 0;
                Iterator<String> it = bought.iterator();
                while(it.hasNext() == true) { %>

                    <tr style="background-color:#EEEEFF; color:#111111"><td>
                        &nbsp;&nbsp;&nbsp;&nbsp;
                        <%= it.next() %>
                        &nbsp;&nbsp;&nbsp;&nbsp;
                    </td></tr>

            <%  } %>
        </table>
        <br /><br />


        <table align="center" style="background-color:white; border-style:solid; border-width:thin; border-color:darkblue;">
            <tr style="font-weight:bold; background-color:#CCCCFF;"><td>
                &nbsp;&nbsp;The following files failed to be bought:&nbsp;&nbsp;
            </td></tr>
            <%  i = 0;
                it = failed.iterator();
                while(it.hasNext() == true) { %>

                    <tr style="background-color:#EEEEFF; color:#111111"><td>
                        &nbsp;&nbsp;&nbsp;&nbsp;
                        <%= it.next() %>
                        &nbsp;&nbsp;&nbsp;&nbsp;
                    </td></tr>

            <%  } %>
        </table>
        <br /><br />

        <table align="center" style="background-color:paleturquoise; border-width:4pt; border-color:darkblue; border-style:ridge;">
            <tr><td align="center">
                <form method="post" action="<%= response.encodeURL(path.get()) %>" >
                     <input type="submit" value="[ Go to The page you were previously viewing ]"
                            style="background-color:lawngreen; font-size:16pt;" />
                </form>
            </td></tr>
        </table>
        <br /><br />


    </body>
</html>
