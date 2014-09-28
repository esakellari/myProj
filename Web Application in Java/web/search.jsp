
<%@page contentType="text/html" pageEncoding="UTF-8" import="beans.*, general.*, java.sql.ResultSet" %>
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
        <title> Storage Cloud - Search </title>
    </head>
    <body>
        <jsp:include page="classic.jsp" />
        
        

        <div style="text-align:center; font-size:32pt; font-weight:bold; color:darkblue; font-family: Impact">
            Search Page
        </div>
        <br /><br />
        

        <form method="get" action="Search">
            <table align="center" style="background-color:#EEEEFF; border-style:solid;
                                         border-width:thin; border-color:darkblue;">


                <tr>
                    <td style="text-align:right;"> &nbsp; Keywords: &nbsp; </td>
                    <td> <input type="text" name="keywords" size="32"
                                value="<%= sinfo.getKeywords() %>" />
                    </td>
                    <td>
                         <button onclick="keywords.value=''" onkeypress="keywords.value=''"
                                 type="button"> <img src="images/broom2.png" alt="clear" /> </button>
                    </td>
                </tr>



                <tr>
                    <td style="text-align:right;"> &nbsp; Type: &nbsp; </td>
                    <td>
                        <select name="type">
                            <option <% if(sinfo.getType().compareTo("and") == 0) { %> SELECTED <% } %> value="and"> All words (AND) </option>
                            <option <% if(sinfo.getType().compareTo("or") == 0) { %> SELECTED <% } %>  value="or">  Any word (OR)   </option>
                        </select>
                    </td>
                </tr>



                <tr>
                    <td style="text-align:right;"> &nbsp; Exclude words: &nbsp; </td>
                    <td> <input type="text" name="exclude" size="32"
                                value='<%= sinfo.getExclude() %>' />
                    </td>
                    <td>
                         <button onclick="exclude.value=''" onkeypress="exclude.value=''"
                                 type="button"> <img src="images/broom2.png" alt="clear" /> </button>
                    </td>
                </tr>



                <tr>
                    <td style="text-align:right;"> &nbsp; Owner: &nbsp; </td>
                    <td> <input type="text" name="owner" size="32"
                                value='<%= sinfo.getOwner() %>' />
                    </td>
                    <td>
                         <button onclick="owner.value=''" onkeypress="owner.value=''"
                                 type="button"> <img src="images/broom2.png" alt="clear" /> </button>
                    </td>
                </tr>



                <tr>
                    <td style="text-align:right;"> &nbsp; Category: &nbsp; </td>
                    <td>
                        <select name="category">
                            <option <% if(sinfo.getCategory().compareTo("any") == 0) { %> SELECTED <% } %>       value="any">        - Any -         </option>
                            <option <% if(sinfo.getCategory().compareTo("music") == 0) { %> SELECTED <% } %>     value="music">      Music           </option>
                            <option <% if(sinfo.getCategory().compareTo("movies") == 0) { %> SELECTED <% } %>    value="movies">     Movies          </option>
                            <option <% if(sinfo.getCategory().compareTo("series_tv") == 0) { %> SELECTED <% } %> value="series_tv">  Series/TV Shows </option>
                            <option <% if(sinfo.getCategory().compareTo("images") == 0) { %> SELECTED <% } %>    value="images">     Images          </option>
                            <option <% if(sinfo.getCategory().compareTo("games") == 0) { %> SELECTED <% } %>     value="games">      Games           </option>
                            <option <% if(sinfo.getCategory().compareTo("software") == 0) { %> SELECTED <% } %>  value="software">   Software        </option>
                            <option <% if(sinfo.getCategory().compareTo("other") == 0) { %> SELECTED <% } %>     value="other">      Other           </option>
                        </select>
                    </td>
                </tr>



                <tr>
                    <td style="text-align:right;"> &nbsp; Price range: &nbsp; </td>
                    <td>
                        <% if(msgs.contains("Search:PriceNotNumber") == true) {
                                msgs.remove("Search:PriceNotNumber");
                        %>
                                <div style="font-size:8pt; font-weight:bold; color:red;">
                                Prices must be numbers.
                                </div>
                        <% } else if(msgs.contains("Search:PriceFrom>To") == true) {
                                msgs.remove("Search:PriceFrom>To");
                        %>
                                <div style="font-size:8pt; font-weight:bold; color:red;">
                                Must be: price1 <= price2.
                                </div>
                        <% } %>

                        <input type="text" name="price_from" size="5"
                               value='<%= sinfo.getPriceFrom() %>' />
                        &nbsp;-&nbsp;
                        <input type="text" name="price_to"   size="5"
                               value='<%= sinfo.getPriceTo() %>' />
                    </td>
                    <td>
                         <button onclick="price_from.value='';price_to.value='';"
                                 onkeypress="price_from.value='';price_to.value='';"
                                 type="button"> <img src="images/broom2.png" alt="clear" /> </button>
                    </td>
                </tr>



                <tr>
                    <td style="text-align:right;"> &nbsp; Order by: &nbsp; </td>
                    <td>
                        <select name="order_by">
                            <option <% if(sinfo.getOrderBy().compareTo("name") == 0) { %> SELECTED <% } %>      value="name">       Name        </option>
                            <option <% if(sinfo.getOrderBy().compareTo("date") == 0) { %> SELECTED <% } %>      value="date">       Date        </option>
                            <option <% if(sinfo.getOrderBy().compareTo("size") == 0) { %> SELECTED <% } %>      value="size">       Size        </option>
                            <option <% if(sinfo.getOrderBy().compareTo("price") == 0) { %> SELECTED <% } %>     value="price">      Price       </option>
                            <option <% if(sinfo.getOrderBy().compareTo("downloads") == 0) { %> SELECTED <% } %> value="downloads">  Downloads   </option>
                        </select>
                    </td>
                </tr>



                <tr>
                    <td> &nbsp; </td>
                    <td>
                        <select name="order">
                            <option <% if(sinfo.getOrder().compareTo("asc") == 0) { %> SELECTED <% } %>  value="asc">  Ascending   </option>
                            <option <% if(sinfo.getOrder().compareTo("desc") == 0) { %> SELECTED <% } %> value="desc"> Descending  </option>
                        </select>
                    </td>
                </tr>



                <tr>
                    <td style="text-align:right;"> &nbsp; Results Per page: &nbsp; </td>
                    <td>
                        <select name="rpp">
                            <option <% if(sinfo.getResultsPerPage().compareTo("all") == 0) { %> SELECTED <% } %> value="all"> - All -         </option>
                            <option <% if(sinfo.getResultsPerPage().compareTo("10") == 0) { %> SELECTED <% } %>  value="10">  &nbsp;&nbsp;10  </option>
                            <option <% if(sinfo.getResultsPerPage().compareTo("20") == 0) { %> SELECTED <% } %>  value="20">  &nbsp;&nbsp;20  </option>
                            <option <% if(sinfo.getResultsPerPage().compareTo("50") == 0) { %> SELECTED <% } %>  value="50">  &nbsp;&nbsp;50  </option>
                            <option <% if(sinfo.getResultsPerPage().compareTo("100") == 0) { %> SELECTED <% } %> value="100"> 100             </option>
                        </select>
                    </td>
                </tr>



                <tr>
                    <td> &nbsp; </td>
                    <td align="right"> <input type="submit" value="Search" /> </td>
                </tr>



            </table>
        </form>
        
        
        <br /><br />


                        
        <%  ResultSet rs = sinfo.getResultSet();
            if(rs != null) {
                int atPage = sinfo.getPage();
                int pages = sinfo.getPages();
        %>

            <div style="text-align:center; text-decoration:underline;
                        font-size:16pt; font-weight:bold; color:darkblue;">
                '<%= sinfo.getResultsNum() %>' files found:
            </div>
            <br />


            <table align="center" style="border-style:solid; border-width:thin; border-color:darkblue;">

                <tr style="font-weight:bold; background-color:#EEEEFF; text-align:center;">
                    <td> &nbsp;No&nbsp; </td>
                    <td> &nbsp;File Name&nbsp; </td>
                    <td> &nbsp;Category&nbsp; </td>
                    <td> &nbsp;Size&nbsp; </td>
                    <td> &nbsp;Price&nbsp; </td>
                    <td> &nbsp;Downloads&nbsp; </td>
                    <td> &nbsp;Date&nbsp; </td>
                    <td> &nbsp;Owner&nbsp; </td>
                    <td> &nbsp;Add to Cart&nbsp; </td>
                </tr>

                <%  StringFixer sf = new StringFixer();
                    int rpp = (sinfo.getResultsPerPage().compareTo("all") == 0
                               ? 2000000000 : Integer.parseInt(sinfo.getResultsPerPage()));
                    for(int i=0 ;  i<rpp && rs.next() == true  ; ++i) {
                        int price = rs.getInt("price");
                %>

                    <tr style="background-color:<% if(i%2 == 0) { %> #AAEEFF <% } else { %> #AAFFAA <% } %>;">
                        <td> &nbsp;<%= (atPage-1)*rpp+(i+1) %>.&nbsp; </td>
                        <td> &nbsp;<%= rs.getString("name") %>&nbsp; </td>
                        <td> &nbsp;<%= rs.getString("category") %>&nbsp; </td>
                        <td style="text-align:right;"> &nbsp;<%= sf.decodeSize(rs.getInt("size")) %>&nbsp; </td>
                        <td style="text-align:right;"> &nbsp;<%= price/100+"."+(price/10)%10+price%10 %>&nbsp;</td>
                        <td style="text-align:right;"> &nbsp;<%= rs.getInt("downloads") %>&nbsp; </td>
                        <td style="text-align:right;"> &nbsp;<%= rs.getDate("date") %>&nbsp; </td>
                        <td style="text-align:right;"> &nbsp;<%= rs.getString("owner") %>&nbsp; </td>
                        <td style="text-align:center;">
                            <form method="post" action="Cart?cmd=add&id=<%= rs.getInt("id") %>&name=<%= rs.getString("name") %>&size=<%= rs.getInt("size") %>&price=<%= rs.getInt("price") %>">
                                <button type="submit" style="height:20pt; width:20pt; background-color:black;" >
                                    <img src="images/add.png" alt="addToCart" />
                                </button>
                            </form>
                        </td>
                    </tr>

                <%  } %>
            </table>
            <br />


            <div style="text-align:center; font-size:12pt;">

                <%  // Afairei tin parametro "page" apo to path wste na tin ksanaperasei me nea timi.
                    //
                    String[] sa = path.get().split("&");
                    String tempath = "";
                    for(int i=0 ; i<sa.length ; ++i) {
                        if(sa[i].startsWith("page") == false) {
                            if(tempath.compareTo("") != 0) {
                                tempath += "&";
                            }
                            tempath += sa[i];
                        }
                    }


                    if(atPage > 1) { %>
                        <a style="text-decoration:none; color:blue;"
                           href="<%= (tempath+"&page="+1) %>"> [&lt;&lt;First] </a>
                        &nbsp;
                        <a style="text-decoration:none; color:blue;"
                           href="<%= (tempath+"&page="+(atPage-1)) %>"> [&lt;Previous] </a>
                        &nbsp;
                <%      if(atPage > 3) { %>
                            &nbsp;...&nbsp;
                <%      }
                    }
                    for(int i=atPage-2 ; i<atPage ; ++i) {
                        if(i > 0) {
                %>
                            <a style="text-decoration:none; color:blue;"
                               href="<%= (tempath+"&page="+i) %>"> [<%= i %>] </a>
                <%      }
                    } %>

                <%= atPage %>
            
                <%  for(int i=atPage+1 ; i<=atPage+2 ; ++i) {
                        if(i <= pages) {
                %>
                            <a style="text-decoration:none; color:blue;"
                                href="<%= (tempath+"&page="+i) %>"> [<%= i %>] </a>
                <%      }
                    }
                    if(atPage+2 < pages) { %>
                        &nbsp;...&nbsp;
                <%  }
                    if(atPage < pages) { %>
                        &nbsp;
                        <a style="text-decoration:none; color:blue;"
                           href="<%= (tempath+"&page="+(atPage+1)) %>"> [Next&gt;] </a>
                        &nbsp;
                        <a style="text-decoration:none; color:blue;"
                           href="<%= (tempath+"&page="+pages) %>"> [Last&gt;&gt;] </a>
                <%  } %>
                
            </div>
        <% } %>


        <br /><br />
        <% access.set(true); %>
        <jsp:include page="cart.jsp" />

    </body>
</html>
