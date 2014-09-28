
<%@page contentType="text/html" pageEncoding="UTF-8" import="beans.*, general.*, java.util.*" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">

<jsp:useBean id="user"   class="beans.User"          scope="session" />
<jsp:useBean id="access" class="beans.Access"        scope="session" />
<jsp:useBean id="path"   class="beans.Path"          scope="session" />
<jsp:useBean id="cart"   class="java.util.ArrayList" scope="session" />

<%  if(access.get() == false) { %>
        <jsp:forward page="error404.jsp" />
<%  } %>



<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <link type="text/css" rel="stylesheet" href="styles.css">
        <title> Storage Cloud - Purchase Completion </title>
    </head>
    <body>
        <jsp:include page="classic.jsp" />


        <div style="text-align:center;">
            <img src="images/cart.png" alt="Shopping Cart"/>
            &nbsp;&nbsp;You are about to
            <span style="font-weight:bold; font-style:italic;">buy & download</span>
            the following files&nbsp;
            <img src="images/cart.png" alt="Shopping Cart"/>
        </div>
        <br />

        <table align="center" style="border-style:solid; border-width:thin; border-color:darkblue;">
            <tr style="font-weight:bold; text-align:center; background-color:#CCCCFF;">
                <td> &nbsp; </td>
                <td> &nbsp;File Name&nbsp; </td>
                <td> &nbsp;Size&nbsp; </td>
                <td> &nbsp;Price&nbsp; </td>
            </tr>
            <%  StringFixer sf = new StringFixer();
                int i = 0;
                Iterator<CartElement> it = cart.iterator();
                while(it.hasNext() == true) {
                    CartElement e = it.next();
                    int price = e.getPrice();
                    ++i;
            %>
                    <tr style="background-color:#EEEEFF; color:#111111">
                        <td style="text-align:right;"> &nbsp;<%= i %>.&nbsp; </td>
                        <td> &nbsp;<%= e.getName()  %>&nbsp; </td>
                        <td style="text-align:right;"> &nbsp;<%= sf.decodeSize(e.getSize())  %>&nbsp; </td>
                        <td style="text-align:right;"> &nbsp;<%= price/100+"."+(price/10)%10+price%10 %>&nbsp; </td>
                    </tr>
            <%  } %>
            <tr style="font-weight:bold;  background-color:#CCCCFF;">
                <%  if(cart.isEmpty() == true) { %>
                    <td> &nbsp; </td>
                    <td> &nbsp;Empty </td>
                    <td> &nbsp; </td>
                    <td> &nbsp; </td>
                <%  } else { %>
                    <td> &nbsp; </td>
                    <td> &nbsp;Total: </td>
                    <td> &nbsp; </td>
                    <td style="text-align:right;">
                        <%  int total = 0;
                            it = cart.iterator();
                            while(it.hasNext() == true) {
                                CartElement e = it.next();
                                total += e.getPrice();
                            }
                        %>
                        &nbsp;<%= total/100+"."+(total/10)%10+total%10 %>&nbsp;
                    </td>
                <%  } %>
            </tr>
        </table>
        <br />

        <table align="center">
        <form method="post" action="CompletePurchase">
            <tr>
                <%  if(user.getUsername() != null) { %>
                        <td> You will be charged at the </td>
                        <td> Credit Card you have given us. </td>
                <%  } else { %>
                        <td> Please enter the Credit Card Code: </td>
                        <td> <input type="text" name="card_code" /> </td>
                <%  } %>
            </tr>
            <tr>
                <td align="left">
                    <button type="submit" name="cmd" value="cancel"> <img src="images/back.png" alt="cancel" /> Cancel </button>
                </td>
                <td align="right">
                    <button type="submit" name="cmd" value="buy"> <img src="images/apply.png" alt="buy" /> Buy </button>
                </td>
            </tr>
        </form>
        </table>

    </body>
</html>
