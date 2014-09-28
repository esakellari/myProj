

<%@page import="java.util.Vector"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">

<jsp:useBean id="user"   class="beans.User"             scope="session" />
<jsp:useBean id="access" class="beans.Access"           scope="session" />
<jsp:useBean id="sinfo"  class="beans.SearchInfo"       scope="session" />
<jsp:useBean id="msgs"   class="java.util.LinkedList"   scope="session" />
<jsp:useBean id="path"   class="beans.Path"             scope="session" />

<%  if(user.getUsername() == null) {
        access.set(true);
        msgs.add("NotLoggedIn");
%>
        <jsp:forward page="errorUser.jsp" />
<%  } %>



<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <link type="text/css" rel="stylesheet" href="styles.css">
        <title>Storage Cloud - More space</title>
    </head>
    <body>
        <% access.set(true); %>
        <jsp:include page="classic.jsp" />

        

        
        <table align="center" style="border-width:13pt; border-color:darkblue; border-style:groove;">
            <td><form action="MoreSpace" method="POST">
        <div style="color:darkblue;font-weight:100; font-size:x-large;font-family: Impact
                               ">
            &nbsp;&nbsp;&nbsp;Buy more space&nbsp;&nbsp;&nbsp;
       <select name="NewSpace">
        <option value="1">1</option>
        <option value="2">2</option>
        <option value="3">3</option>
        <option value="4">4</option>
        <option value="5">5</option>
        <option value="6">6</option>
        <option value="7">7</option>
        <option value="8">8</option>
        <option value="9">9</option>
        <option value="10">10</option>
        </select>
             GB&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp; <input style="color:darkblue;font-weight:bold; font-size:large;
                               "type="submit" value="Buy">&nbsp;&nbsp;&nbsp;
        </div>
         </form></td>
        </table>

    </body>
</html>
