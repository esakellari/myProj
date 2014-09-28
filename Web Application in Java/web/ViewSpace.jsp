

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
    

    String space_size= (String)session.getAttribute("space_size");
    String space_u_size = (String)session.getAttribute("space_u_size");
    String left_size = (String)session.getAttribute("left2_size");
    String space_b_size = (String)session.getAttribute("space_b_size");
    String space_p_size = (String)session.getAttribute("space_p_size");
    String less_than = (String)session.getAttribute("less_than_a_mb");

    session.removeAttribute("space_size");
    session.removeAttribute("space_u_size");
    session.removeAttribute("left2_size");
    session.removeAttribute("space_b_size");
    session.removeAttribute("space_p_size");

%>


<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <link type="text/css" rel="stylesheet" href="styles.css">
        <title>Storage Cloud - ViewSpace</title>
    </head>
    <body>
        <jsp:include page="classic.jsp" />

        <table align ="center"  style="border-width:13pt;border-bottom-width:6pt; border-top-width:6pt; border-color:darkblue; border-style:groove;">
            <tr style="background-color:#AAEEFF">
            <th>Total Space</th>
            <th>Space Used</th>
            <th>Space Left</th>
            <th>Space Bonus</th>
            <th>Space Public</th>
            <tr style="background-color:#AAFFAA">
                <td><%
                        out.println(space_size);
                    %></td>
                <td><%
                        out.println(space_u_size);
                    %></td>
                <td><%
                        out.println(left_size);
                    %></td>
                <td><%
                        out.println(space_b_size);
                    %></td>
                <td><%
                        out.println(space_p_size);
                    %></td>


         </table>
                    <br/><br/>

            <%
                if(less_than!=null){
            %>
            <div style="text-align:left; position:relative; margin-left:38%; font-size:x-large; font-family:Impact; color:brown">
                You have less than a Mb free!<br/><br/>
                To buy more space press here!
           

                <%access.set(true);%>
                    <form method="post" action="MoreSpace.jsp" >
                        <input type="submit" value="Buy More space"
                               style="background-color:Aqua ; text-align:left; position:relative; margin-left:12%;font-size:10pt; font-variant:small-caps;" />
                    </form> </div><br/><br/>
            <%
                }
            %>



        <form style="text-align:left; position:relative; margin-left:43%;" method="post" action="ViewFolders?new_path=root" >
             <input type="submit" value="Back to My Dirs"
                    style="background-color:Aqua ; font-size:16pt; font-variant:small-caps;" />
        </form>

    </body>
</html>

