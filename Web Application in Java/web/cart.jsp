
<%@page contentType="text/html" pageEncoding="UTF-8" import="beans.*, general.*, java.util.*" %>

<jsp:useBean id="access" class="beans.Access"        scope="session" />
<jsp:useBean id="cart"   class="java.util.ArrayList" scope="session" />

<%  if(access.get() == false) { %>
        <jsp:forward page="error404.jsp" />
<%  } else {
        access.set(false);
    }
%>



<table style="background-color:white; border-style:solid; border-width:thin; border-color:darkblue;">

    <tr>
        <td> <img src="images/cart.png" alt="Shopping Cart"/> </td>
        <td> &nbsp; </td>
        <td> &nbsp; </td>
        <td> &nbsp; </td>
    </tr>

    <tr style="font-weight:bold; text-align:center; background-color:#CCCCFF;">
        <td> &nbsp;File Name&nbsp; </td>
        <td> &nbsp;Size&nbsp; </td>
        <td> &nbsp;Price&nbsp; </td>
        <td> &nbsp; </td>
    </tr>
    
    <%  StringFixer sf = new StringFixer();
        int i = 0;
        Iterator<CartElement> it = cart.iterator();
        while(it.hasNext() == true) {
            CartElement e = it.next();
            int price = e.getPrice();
    %>
            <tr style="background-color:#EEEEFF; color:#111111">

                <td> &nbsp;<%= e.getName()  %>&nbsp; </td>
                <td style="text-align:right;"> &nbsp;<%= sf.decodeSize(e.getSize()) %>&nbsp; </td>
                <td style="text-align:right;"> &nbsp;<%= price/100+"."+(price/10)%10+price%10 %>&nbsp; </td>
                <td style="text-align:center;">
                    <form method="post" action="Cart?cmd=remove&index=<%= i %>">
                        <input type="image" src="images/remove2.png" alt="remove" />
                    </form>
                </td>

            </tr>

    <%      ++i;
        } %>

    <tr style="font-weight:bold;  background-color:#CCCCFF;">
        <%  if(cart.isEmpty() == true) { %>
                <td> &nbsp;Empty </td>
                <td> &nbsp; </td>
                <td> &nbsp; </td>
                <td> &nbsp; </td>
        <%  } else { %>
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
                <td> &nbsp; </td>
        <%  } %>
    </tr>


    <tr>
        <td>
            <form method="post" action="Cart?cmd=clear">
                <input style="background-color:lawngreen" type="submit" value="Clear" />
            </form>
        </td>
        <td> &nbsp; </td>
        <td> &nbsp; </td>
        <td>
            <form method="post" action="Cart?cmd=buy">
                <input style="background-color:lawngreen" type="submit" value="Buy!" />
            </form>
        </td>
    </tr>

</table>
        