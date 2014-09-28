
<%@page contentType="text/html" pageEncoding="UTF-8" import="beans.*"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">

<jsp:useBean id="user"   class="beans.User"           scope="session" />
<jsp:useBean id="access" class="beans.Access"         scope="session" />
<jsp:useBean id="msgs"   class="java.util.LinkedList" scope="session" />
<jsp:useBean id="rinfo"  class="beans.RegisterInfo"   scope="session" />

<%  if(access.get() == false) { %>
        <jsp:forward page="error404.jsp" />
<%  } %>



<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <link type="text/css" rel="stylesheet" href="styles.css">
        <title> Storage Cloud - Registration </title>
    </head>
    <body>
        <% msgs.add("onRegistration"); %>
        <jsp:include page="classic.jsp" />


        <div style="text-align:center; position:relative;  font-size:30pt; font-weight:100; color:darkblue; font-family:Impact;">
            Registration Page
        </div>
        <br /><br />


        <form method="post" action="Register">
            <table>
                <tr>
                    <td> &nbsp; Your Name &nbsp; </td>
                    <td>
                        <input type="text" name="name" size="32" maxlength="45"
                               value='<%= rinfo.getName() %>' />
                    </td>
                    <td style="color:red; font-size:10pt; font-weight:bold;">
                        &nbsp; <%= rinfo.getNameComment() %>
                    </td>
                </tr>
                <tr>
                    <td> &nbsp; Your Surname &nbsp; </td>
                    <td>
                        <input type="text" name="surname" size="32" maxlength="45"
                               value='<%= rinfo.getSurname() %>' />
                    </td>
                    <td style="color:red; font-size:10pt; font-weight:bold;">
                        &nbsp; <%= rinfo.getSurnameComment() %>
                    </td>
                </tr>
                <tr>
                    <td> &nbsp; Your Credit Card Code &nbsp; </td>
                    <td>
                        <input type="text" name="card_code" size="32" maxlength="45"
                               value='<%= rinfo.getCardCode() %>' />
                    </td>
                    <td style="color:red; font-size:10pt; font-weight:bold;">
                        &nbsp; <%= rinfo.getCardCodeComment() %>
                    </td>
                </tr>
                <tr>
                    <td> &nbsp; Your E-mail &nbsp; </td>
                    <td>
                        <input type="text" name="email" size="32" maxlength="45"
                               value='<%= rinfo.getEmail() %>' />
                    </td>
                    <td style="color:red; font-size:10pt; font-weight:bold;">
                        &nbsp; <%= rinfo.getEmailComment() %>
                    </td>
                </tr>
                <tr>
                    <td> &nbsp; </td>
                    <td> &nbsp; </td>
                    <td> &nbsp; </td>
                </tr>
                <tr>
                    <td> &nbsp; Select Username &nbsp; </td>
                    <td>
                        <input type="text" name="username" size="32" maxlength="45"
                               value='<%= rinfo.getUsername() %>' />
                    </td>
                    <td style="color:red; font-size:10pt; font-weight:bold;">
                        &nbsp; <%= rinfo.getUsernameComment() %>
                    </td>
                </tr>
                <tr>
                    <td> &nbsp; Select Password &nbsp; </td>
                    <td>
                        <input type="password" name="password" size="32" maxlength="45"
                               value='<%= rinfo.getPassword() %>' />
                    </td>
                    <td style="color:red; font-size:10pt; font-weight:bold;">
                        &nbsp; <%= rinfo.getPasswordComment() %>
                    </td>
                </tr>
                <tr>
                    <td> &nbsp; Re-type the Password &nbsp; </td>
                    <td>
                        <input type="password" name="repass" size="32" maxlength="45"
                               value='<%= rinfo.getRepass() %>' />
                    </td>
                    <td style="color:red; font-size:10pt; font-weight:bold;">
                        &nbsp; <%= rinfo.getRepassComment() %>
                    </td>
                </tr>
                <tr>
                    <td> &nbsp; </td>
                    <td style="text-align:right;">
                        <input type="submit" value="Register" />
                    </td>
                    <td> &nbsp; </td>
                </tr>
            </table>
        </form>

        <br /><br /><br />

        
    </body>
</html>
