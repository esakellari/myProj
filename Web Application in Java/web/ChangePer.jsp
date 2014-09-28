

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
        <title> Storage Cloud - Change Price </title>
    </head>
    <body>
        <jsp:include page="classic.jsp" />
      
         

        <%
            String namePar = request.getParameter("file");
            String pricePar = request.getParameter("price");
            String pathPar = request.getParameter("path_file");
            String idPar = request.getParameter("id");
            
            String nameAttr = (String) session.getAttribute("nameAttr");
            String pathAttr = (String) session.getAttribute("pathAttr");
            String priceAttr = (String) session.getAttribute("priceAttr");
            String idAttr = (String) session.getAttribute("idAttr");
            String initPrice = (String) session.getAttribute("initialPrice");
            
            String name=null;
            String path_file=null;
            String price=null;
            String id = null;
            
            if(namePar!=null){
                name=namePar;
            }else if(nameAttr!=null){
                name=nameAttr;
            }

            if(pathPar!=null){
                path_file=pathPar;
            }else if(pathAttr!=null){
                path_file=pathAttr;
            }
            if(idPar!=null){
                id=idPar;
            }else if(idAttr!=null){
                id=idAttr;
            }
            if(priceAttr!=null){
                price=priceAttr;
            }else if(pricePar!=null){
                price=pricePar;
                initPrice = pricePar;
                session.setAttribute("initialPrice",pricePar);
            }

            if(pricePar == null){
                session.setAttribute("initialPrice",initPrice);

            }
            session.setAttribute("path_file",path_file);
            session.setAttribute("name",name);
            session.setAttribute("priceAttr",price);
            session.setAttribute("id",id);
    %>
        <table border="1" style="text-align:left; position:relative; margin-left:45%;">
            <tr>
                <td><div style="font-weight:normal; color:darkblue;">Name</div></td>

        
                <td><div style="font-weight:normal; color:darkblue;">Current Price</div></td>

            <tr>
                <td><%=id%></td>
                <td><%=name%></td>
      
                <td><%=initPrice%></td>
            <tr>
            </table>

            <form action="ChangePer" method="POST">
            <div style="text-align:left; position:relative; margin-left:40%; font-size:14pt; font-weight:bold; color:darkblue; font-family:Gill;">
                Change the price of this file </div>

        
            <table style="text-align:left; position:relative; margin-left:40%;">
                <td>Price:<td/>
                <td><input type="text" name="price" value=""><td/>
                <td><%
                       if(msgs.contains("NoPrice") == true){
                            msgs.remove("NoPrice");
                 %>
                        <a style="font-size:10pt; font-weight:normal; color:brown">
                                You have not given a price to this product
                            </a>
                <%
                       }
                %><td/>
                <td><%
                       if(msgs.contains("NegPrice") == true){
                            msgs.remove("NegPrice");
                 %>
                        <a style="font-size:10pt; font-weight:normal; color:deeppink">
                                The price must be a positive number (>=0)
                            </a>
                <%
                       }
                %></td>

            </table>

            <input style="text-align:left; position:relative; margin-left:45%;" type="submit" value="Change">
        </form>
        

    </body>
</html>
