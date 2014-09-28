

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

<%
     String new_path2 = (String) session.getAttribute("new_path2");
     session.setAttribute("new_path2", new_path2);
%>
 


<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <link type="text/css" rel="stylesheet" href="styles.css">
        <title>Storage Cloud - Upload File</title>
    </head>
    <body>
        <% access.set(true); %>
        <jsp:include page="classic.jsp" />



       <table align="center" style="border-width:13pt; border-color:darkblue; border-style:groove;">
        <form action="uploadFile" method="POST">
        <div style="text-align:left; position:relative; margin-left:43%; font-size:24pt; font-weight:100; color:darkblue; font-family:Impact;">
                Upload a file<br/>
            

                <td>Name:<td/>
                <td><input type="text" name="datafileName" value="file.txt"></td>
                <td>
                <%
                       if(msgs.contains("NoFileName") == true){
                            msgs.remove("NoFileName");
                 %>
                        <a style="font-size:10pt; font-weight:normal; color:brown">
                                You must give a name for your file
                            </a>
                <%
                       }
                %>
                 <%
                       if(msgs.contains("longFileName") == true){
                            msgs.remove("longFileName");
                 %>
                        <a style="font-size:10pt; font-weight:normal; color:darkslateblue">
                                The filename you have given is too long
                            </a>
                <%
                       }
                %>
                <%
                       if(msgs.contains("fileExists") == true){
                            msgs.remove("fileExists");
                 %>
                        <a style="font-size:10pt; font-weight:normal; color:darkslateblue">
                                This file name already exists
                            </a>
                <%
                       }
                %>
                </td>
            <tr>

                <td>Size:<td/>
                <td><input type="text" name="size" value="5">
                    <input type="radio" name ="bytes" value="kb"/>KB
                    <input type="radio" name ="bytes" value="mb"/>MB
                    <input type="radio" name ="bytes" value="gb"/>GB
                    <td/>
                    <td>
                    <%
                       if(msgs.contains("WrongSize") == true){
                            msgs.remove("WrongSize");
                    %>
                        <a style="font-size:10pt; font-weight:normal; color:brown">
                                You have not given the size correctly
                            </a>
                    <%
                       }
                    %>
                    </td>
                    <td>
                          <%
                       if(msgs.contains("NegSize") == true){
                            msgs.remove("NegSize");
                    %>
                        <a style="font-size:10pt; font-weight:normal; color:brown">
                                You have not given the size correctly
                            </a>
                    <%
                       }
                    %>
                    </td>
            <tr>
                <td>Privacy:<td/>
                <td><input type="radio" name ="privacy" value="public"/>Public
                    <input type="radio" name ="privacy" value="private"/>Private<td/>
                    <td>
                    <%
                       if(msgs.contains("Privacy") == true){
                            msgs.remove("Privacy");
                    %>
                        <a style="font-size:10pt; font-weight:normal; color:brown">
                                You must declare the privacy of your file
                            </a>
                    <%
                       }
                    %>
                    &nbsp;</td>
            <tr>
                <td>Location:<td/>
                <td><input type="radio" name ="folder" value="inSpecial"/>Put in special folder
                    <input type="radio" name ="folder" value="inCurrent"/>Put in current folder<td/>
                    <td>
                    <%
                       if(msgs.contains("location") == true){
                            msgs.remove("location");
                    %>
                        <a style="font-size:10pt; font-weight:normal; color:brown">
                                You must declare the location of your file
                            </a>
                    <%
                       }
                    %>
                     <%
                       if(msgs.contains("PropertyDoesnotExist") == true){
                            msgs.remove("PropertyDoesnotExist");
                    %>
                        <a style="font-size:10pt; font-weight:normal; color:brown">
                                There is no folder with this property
                            </a>
                    <%
                       }
                    %>
                    </td>
            <tr>
          <td>
        
            Category:
            <select name="Category">
            <option value="any">- Any - </option>
            <option value="music"> Music   </option>
            <option value="movies">Movies</option>
            <option value="series_tv">Series/TV Shows</option>
            <option value="images">Images </option>
            <option value="games">Games </option>
            <option value="software">Software </option>
            <option value="other">Other </option>
            </select>
         </td>

              <br/>
              <tr>
              <td><input type="submit" value="Upload"><td/>
              </div>
        </form>
        </table>
        
                   
            <div style="text-align:left; position:relative; margin-left:40%; font-size:24pt; font-weight:100; color:darkblue; font-family:Impact;">
           Or Create a folder
            </div>

            <table align="center" style="border-width:13pt; border-color:darkblue; border-style:groove;">
            <form action="CreateFolder" method="POST">
              <td>Name:<td/>
                <td><input type="text" name="FolderName" value="MP3"><td/>
                 <%
                       if(msgs.contains("NoFileName") == true){
                            msgs.remove("NoFileName");
                 %>
                        <br/><div style="text-align:left; font-size:24pt; font-weight:bold;">
                                You must give a name for your folder
                            </div>
                <%
                       }
                %>
                 <%
                       if(msgs.contains("folderExists") == true){
                            msgs.remove("folderExists");
                 %>
                        <br/><div style="text-align:left; font-size:10pt; font-weight:normal; color:brown">
                                This file name already exists
                            </div>
                <%
                       }
                %>
                <tr>


                &nbsp;&nbsp;&nbsp;<td>Property:<td/>
                <td><input type="text" name="property" value="mp3"><td/>
                <%
                       if(msgs.contains("NoProperty") == true){
                            msgs.remove("NoProperty");
                 %>
                        <br/><div style="text-align:left; font-size:10pt; font-weight:normal; color:brown">
                                You have not declared the property of this file
                            </div>
                <%
                       }
                %>
                 <%
                       if(msgs.contains("propertyExists") == true){
                            msgs.remove("propertyExists");
                 %>
                        <br/><div style="text-align:left; font-size:10pt; font-weight:normal; color:brown">
                                There is already a file with this property
                            </div>
                <%
                       }
                %>
                <tr>


                    <td><input type="submit" value="Create"><td/>
                <tr>

             </form>
            </table>

    </body>
</html>


