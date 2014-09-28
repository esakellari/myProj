
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
<%  } else {
        access.set(false);
    }
%>
    <table align="center" style="border-width:13pt;border-bottom-width:6pt; border-top-width:6pt; border-color:darkblue; border-style:groove;">

                <td>
                <form method="post" action="ManagePublic" >
                        <input type="submit" value="Manage my public files"
                               style="background-color:Aqua ; font-size:10pt; font-variant:small-caps;" />
                    </form>
                
                </td>

                <td>
                    <form method="post" action="ManageAll" >
                        <input type="submit" value="Manage all my files"
                               style="background-color:Aqua ; font-size:10pt; font-variant:small-caps;" />
                    </form>
                
                </td>
                <td>
                    <form method="post" action="ViewFolders?new_path=root" >
                        <input type="submit" value="My Dirs"
                               style="background-color:Aqua ; font-size:10pt; font-variant:small-caps;" />
                    </form>
                </td>

                <td><%access.set(true);%>
                    <form method="post" action="upload.jsp" >
                        <input type="submit" value="Upload a file"
                               style="background-color:Aqua ; font-size:10pt; font-variant:small-caps;" />
                    </form>
                </td>
                <td>
                    <form method="post" action="ViewSpace" >
                        <input type="submit" value="View my space"
                               style="background-color:Aqua ; font-size:10pt; font-variant:small-caps;" />
                    </form>
                </td>
                
                <td><%access.set(true);%>
                    <form method="post" action="MoreSpace.jsp" >
                        <input type="submit" value="Buy More space"
                               style="background-color:Aqua ; font-size:10pt; font-variant:small-caps;" />
                    </form>
                </td>
                <td>
                    <form method="post" action="ViewCharge" >
                        <input type="submit" value="View my Charge"
                               style="background-color:Aqua ; font-size:10pt; font-variant:small-caps;" />
                    </form>
                </td>

            </table>  <br/><br/><br/>

            

       

