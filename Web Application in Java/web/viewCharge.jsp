

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

 

<%
    Float charge = (Float) session.getAttribute("charge");

    session.removeAttribute("charge");
%>


<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <link type="text/css" rel="stylesheet" href="styles.css">
        <title>Storage Cloud - View Charge</title>
    </head>
    <body>
        <jsp:include page="classic.jsp" />



        <table align="center" style="border-width:13pt; border-color:darkblue; border-style:groove;">
            <tr><td style="background-color:paleturquoise">
                     <div style="font-size:18pt; font-weight:100; color:darkblue; font-family:Impact;">
                    Till now you have been charged with
                    <% out.println(charge);
                    %> Euros.
                    </div>
                   
            <td/>
            <tr/>

            </table>
    </body>
</html>
