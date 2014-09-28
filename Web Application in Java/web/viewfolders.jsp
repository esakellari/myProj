

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
<%  }%>




<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <link type="text/css" rel="stylesheet" href="styles.css">
        <title>Storage Cloud - View my folders</title>
    </head>
    <body>
        <jsp:include page="classic.jsp" />


    
        <%@page import="java.util.Vector"%>
<%
        Vector<String> files = (Vector<String>) session.getAttribute("arrayfiles");
        if(files==null){
                        session.setAttribute("arrayfiles", new Vector<String>());
                        files = (Vector<String>) session.getAttribute("arrayfiles");
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
        Vector<String> property = (Vector<String>) session.getAttribute("property");
        if(property==null){
                        session.setAttribute("property", new Vector<String>());
                        property = (Vector<String>) session.getAttribute("property");
                    }
        Vector<String> downloads = (Vector<String>) session.getAttribute("down");
        if(downloads==null){
                        session.setAttribute("down", new Vector<String>());
                        downloads = (Vector<String>) session.getAttribute("down");
                    }
        Vector<String> sizes = (Vector<String>)session.getAttribute("sizes");
        if(sizes==null){
                        session.setAttribute("sizes", new Vector<String>());
                        sizes = (Vector<String>) session.getAttribute("sizes");
                    }
        Vector<String> category = (Vector<String>)session.getAttribute("category");
        if(category==null){
                        session.setAttribute("category", new Vector<String>());
                        category = (Vector<String>) session.getAttribute("category");
                    }

        Vector<Integer> ids = (Vector<Integer>)session.getAttribute("ids");
        if(ids==null){
                        session.setAttribute("ids", new Vector<Integer>());
                        ids = (Vector<Integer>) session.getAttribute("ids");
                    }
        String usrname =  (String)session.getAttribute("usrname");
        session.setAttribute("usrname",usrname);

        session.setAttribute("files", files);
        session.setAttribute("paths",paths);
        session.setAttribute("privacy", privacy);
        session.setAttribute("property",property);
        session.setAttribute("downloads",downloads);
        session.setAttribute("sizes",sizes);
        session.setAttribute("category",category);
        session.setAttribute("ids",ids);
%>
        <table style="border-width:13pt;border-bottom-width:6pt; border-top-width:6pt; border-color:darkblue; border-style:groove;" align="center">
            <tr style="background-color:#AAFFAA">

                <th>Id          </th>
                <th>Name        </th>
                <th>Privacy     </th>
                <th>Category    </th>
                <th>Property    </th>
                <th>Size        </th>
                <th>Downloads   </th>
<%  for(int i=0;i<files.size();i++)
    {
%>
            
            <tr style="background-color:<% if(i%2 == 0) { %> #AAEEFF <% } else { %> #AAFFAA <% } %>;">
                <td><font color ="Blue"><%=ids.get(i)%></font></td>
            <td><a href="ViewFolders?file=<%=files.get(i)%>&new_path=<%=paths.get(i)
                        +"-"+files.get(i)%>"><%=files.get(i)%>
                </a>
            </td>
                   
<%      if(privacy.get(i)!=null){
%>
        <td><font color ="Blue"><%=privacy.get(i)%></font>
        </td>
<%      }else{
%>
        <td>-</td>
<%      }if(category.get(i)!=null){
%>
        <td><font color ="Blue"><%=category.get(i)%></font>
        </td> 
<%      }else{
%>
        <td>-</td>
<%      }if(property.get(i)!=null){
%>
        <td><font color ="Blue"><%=property.get(i)%></font>
        </td>
<%      }else{
%>
        <td>-</td>
<%      }
%>
        <td><font color ="Blue"><%=sizes.get(i)%></font>
        </td>
        <td><font color ="Blue"><%=downloads.get(i)%></font>
        </td>
        <tr>
 <%     }
          String new_path2 = (String) session.getAttribute("new_path2");
                              session.setAttribute("new_path2", new_path2);
%>
        </table>

        
               
    </body>
</html>

