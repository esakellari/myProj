
<%@page contentType="text/html" pageEncoding="UTF-8" import="beans.*, java.util.LinkedList"%>

<jsp:useBean id="user"   class="beans.User"           scope="session" />
<jsp:useBean id="access" class="beans.Access"         scope="session" />
<jsp:useBean id="msgs"   class="java.util.LinkedList" scope="session" />

<%  if(access.get() == false) { %>
        <jsp:forward page="error404.jsp" />
<%  } else {
        access.set(false);
    }
%>




        <table align="center" style="border-width:13pt; border-color:darkblue; border-style:groove;"> <tr>

            <td style="background-color:paleturquoise">
                <p style="color:darkblue; font-size:32pt; font-weight:bold; font-family: Impact">
                    &nbsp;&nbsp;&nbsp; ~ Storage Cloud ~ &nbsp;&nbsp;&nbsp;
                </p>
            </td>

            <% if(msgs.contains("onRegistration") == true) {
                  msgs.remove("onRegistration");
               } else {
            %>
              <td style="background-color:lawngreen">
                <% if(user.getUsername() == null) { %>

                    <% if(msgs.contains("SignIn:Failure") == true) {
                            msgs.remove("SignIn:Failure");
                    %>
                        <div style="text-align:center; font-size:8pt; font-weight:bold; color:red;">
                            You have entered wrong Username <br />
                            or Password. Please try again.
                        </div>
                    <% } %>

                    <form method="post" action="SignIn">
                    <table>
                        <tr>
                            <td> &nbsp; Username &nbsp; </td>
                            <td>
                                <input type="text" name="username" maxlength="45" size="24"
                                       style="color:#444444" />
                                &nbsp;
                            </td>
                        </tr>
                        <tr>
                            <td> &nbsp; Password &nbsp; </td>
                            <td>
                                <input type="password" name="password" maxlength="45" size="24" />
                                &nbsp;
                            </td>
                        </tr>
                        <tr>
                            <td> &nbsp; </td>
                            <td> <input type="submit" value="Sign In" /> </td>
                        </tr>
                    </table>
                    </form>

                    <hr />
                    <table>
                        <tr>
                            <td> &nbsp; Haven't an account? Then </td>
                            <td>
                                <form method="post" action="Register">
                                    <input type="submit" value="Register" />
                                </form>
                            </td>
                        </tr>
                    </table>


                <% } else { %>

                    <table>
                        <tr><td>
                            &nbsp;&nbsp;&nbsp;&nbsp;
                            <span style="font-style:italic;"> User: </span>
                            <span style="font-weight:bold;"> <%= user.getUsername() %> </span>
                            &nbsp;&nbsp;&nbsp;&nbsp;
                        </td></tr>
                        <tr><td style="text-align:center;" >
                            <form method="post" action="SignOut">
                                <input type="submit" value="Sign Out" />
                            </form>
                        </td></tr>
                    </table>

                <% } %>
              </td>
            <% } %>
        </tr> </table>
        <br />


        <table align="center">
            <tr>
                <td> Search file: &nbsp; </td>
                <td>
                    <form method="get" action="Search">
                        <input type="text" name="keywords" size="64" value=" Search for files..."
                               onfocus="if(this.value==' Search for files...')this.value='';"
                               onblur="if(this.value=='')this.value=' Search for files...';"
                               style="border-width:thin; border-style:solid; border-color:darkblue;
                                      background-color:#EEEEFF; color:#444444" />
                        <input type="submit" value="Search" />
                    </form>
                </td>
            </tr>
        </table>
        <br />


        <table align="center" style="border-width:13pt; border-bottom-width:4pt; border-top-width:4pt;
                                     border-color:darkblue; border-style:ridge; ">
            <tr>
                <td>
                    <form method="post" action="Home" >
                        <input type="submit" value="Home"
                               style="background-color:lawngreen; font-size:16pt; font-variant:small-caps; width:150pt;" />
                    </form>
                </td>
                <td>
                    <form method="post" action="Search" >
                        <input type="submit" value="Advanced Search"
                               style="background-color:lawngreen; font-size:16pt; font-variant:small-caps; width:150pt;" />
                    </form>
                </td>
                <% if(user.getUsername() != null) {  %>
                <td>
                    <form method="post" action="ViewFolders?new_path=root" >
                        <input type="submit" value="My Dirs"
                               style="background-color:lawngreen; font-size:16pt; font-variant:small-caps; width:150pt;" />
                    </form>
                </td>
                <!--td>
                    <form method="post" action="MyProfile" >
                        <input type="submit" value="My Profile"
                               style="background-color:lawngreen; font-size:16pt; font-variant:small-caps; width:150pt;" />
                    </form>
                </td-->
                <% } %>
            </tr>
        </table>
        <br />

        <%  if(user.getUsername() != null) { %>
                <% access.set(true); %>
                <jsp:include page="links.jsp" />
        <%  } %>
        

