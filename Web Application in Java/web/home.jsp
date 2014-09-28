
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
        <title> Storage Cloud - Home </title>
    </head>
    <body>
        <jsp:include page="classic.jsp" />
        
         <div style="text-align:center; position:relative;  font-size:24pt; font-weight:100; color:darkblue; font-family:Impact;">
            Home Page
        </div>
        <br /><br />
         <div style="text-align:center; position:relative;font-size:24pt; font-weight:100; color:darkblue; font-family:Impact;">
            Welcome to the Storage Cloud! <br />
            Here you can find any file you may need, etc etc... ^.^
        </div>


    </body>
</html>
