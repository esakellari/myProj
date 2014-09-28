

<%@page import="beans.User"%>
<%@page import="java.util.Vector"%>
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
        <title>Storage Cloud - Manage my public files</title>
    </head>
    <body>
        <jsp:include page="classic.jsp" />
        

        <div style="text-align:left; position:relative; margin-left:43%; font-size:24pt; font-weight:100; color:darkblue; font-family:Impact;">
                        Public files<br/>
            </div>

        
       <%
                Vector<String> files = (Vector<String>) session.getAttribute("files");
                if(files==null){
                        session.setAttribute("files", new Vector<String>());
                        files = (Vector<String>) session.getAttribute("files");
                    }
                Vector<String> path_file = (Vector<String>) session.getAttribute("path_file");
                if(path_file==null){
                        session.setAttribute("path_file", new Vector<String>());
                        path_file = (Vector<String>) session.getAttribute("path_file");
                    }
                Vector<Integer> prices = (Vector<Integer>) session.getAttribute("prices");
                if(prices==null){
                        session.setAttribute("prices", new Vector<Integer>());
                        prices = (Vector<Integer>) session.getAttribute("prices");
                    }
                Vector<Integer> ids = (Vector<Integer>) session.getAttribute("ids");
                if(ids==null){
                        session.setAttribute("id", new Vector<Integer>());
                        ids = (Vector<Integer>) session.getAttribute("ids");
                    }

                session.setAttribute("files",files);
                session.setAttribute("prices", prices);
                session.setAttribute("path_file",path_file);
                session.setAttribute("ids",ids);

                session.setAttribute("pathAttr",null);
                session.setAttribute("nameAttr",null);
                session.setAttribute("priceAttr",null);
        %>
        <table style="border-width:13pt;border-bottom-width:6pt; border-top-width:6pt; border-color:darkblue; border-style:groove;" align="center">
             <tr style="background-color:#AAFFAA">
                <th>Id</th>
                <th>Name</th>
                <th>Path</th>
                <th>Price</th>

         
        <%      int pr =0;
                for(int i=0;i<files.size();i++)
                {
        %>
                   <tr style="background-color:<% if(i%2 == 0) { %> #AAEEFF <% } else { %> #AAFFAA <% } %>;">
                        <td><%= ids.get(i)%></td>
                        <td><%=files.get(i)%></td>
                        <td><%=path_file.get(i)%></td>
                  
        <%
                    if(prices.get(i)!=0){
                        pr = prices.get(i);
        %>
                        <td><%=prices.get(i)%></td>

        <%
                    }
                    else{
        %>
                        <td>-</td>
            
        <%          }
        %>
        <td> <a href="ChangePer.jsp?file=<%=files.get(i)%>&price=
                <%=Integer.toString(pr)%>&id=<%=ids.get(i)%>&path_file=<%=path_file.get(i)%>">
                Change</a>
        </td>

            <tr>
        <%
                }
        %>
        </table>

       
        

    </body>
</html>

