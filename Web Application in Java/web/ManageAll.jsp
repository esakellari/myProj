

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
        <title>Storage Cloud - Manage my files</title>
    </head>
    <body>
        <jsp:include page="classic.jsp" />


        
<%
                Vector<String> files = (Vector<String>) session.getAttribute("filenames");
                    if(files==null){
                        session.setAttribute("files", new Vector<String>());
                        files = (Vector<String>) session.getAttribute("filenames");
                    }

                Vector<String> paths = (Vector<String>) session.getAttribute("paths");
                 if(paths==null){
                        session.setAttribute("paths", new Vector<String>());
                        paths = (Vector<String>) session.getAttribute("paths");
                    }
                Vector<String> privacy = (Vector<String>) session.getAttribute("privacy");
                 if(privacy==null){
                        session.setAttribute("privacy", new Vector<String>());
                        privacy = (Vector<String>) session.getAttribute("privacy");
                    }
                Vector<String> category = (Vector<String>) session.getAttribute("category");
                 if(category==null){
                        session.setAttribute("category", new Vector<String>());
                        category = (Vector<String>) session.getAttribute("category");
                    }
                Vector<String> sizes = (Vector<String>)session.getAttribute("sizes");
                 if(sizes==null){
                        session.setAttribute("sizes", new Vector<String>());
                        sizes = (Vector<String>) session.getAttribute("sizes");
                    }
                Vector<Integer> ids = (Vector<Integer>)session.getAttribute("ids");
                 if(ids==null){
                        session.setAttribute("ids", new Vector<Integer>());
                        ids = (Vector<Integer>) session.getAttribute("ids");
                    }


                session.removeAttribute("filenames");
                session.removeAttribute("paths");
                session.removeAttribute("privacy");
                session.removeAttribute("category");
                session.removeAttribute("sizes");
                session.removeAttribute("ids");
%>
         <table align ="center"style="border-width:13pt;border-bottom-width:6pt; border-top-width:6pt; border-color:darkblue; border-style:groove;" >
             <tr style="background-color:#AAFFAA">
            <th>Id</th>
            <th>Name</th>
            <th>Path</th>
            <th>Privacy</th>
            <th>Category</th>
            <th>Size</th>


            
<%      for(int i=0;i<files.size();i++)
        {
%>
             <tr style="background-color:<% if(i%2 == 0) { %> #AAEEFF <% } else { %> #AAFFAA <% } %>;">
                 <td><%=ids.get(i)%>    </td>
                <td><%=files.get(i)%>   </td>
                <td><%=paths.get(i)%>   </td>
                <td><%=privacy.get(i)%> </td>
                <td><%=category.get(i)%></td>
                <td><%=sizes.get(i)%>   </td>

                <td> <a href="ChangePrivacy?file=<%=files.get(i)%>&privacy=<%=privacy.get(i)%>&path_file=<%=paths.get(i)%>">
                Change Privacy</a>
                </td>
                <td> <a href="RemoveFile?file=<%=files.get(i)%>&path_file=<%=paths.get(i)%>&privacy=<%=privacy.get(i)%>&id=<%=ids.get(i)%>">
                Remove File</a>
                </td>
            <tr>
<%
        }
%>
        
         </table>

          
    </body>
</html>


