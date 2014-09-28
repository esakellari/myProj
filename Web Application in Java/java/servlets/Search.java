
package servlets;

import beans.*;
import general.*;
import java.io.*;
import java.sql.*;
import java.util.LinkedList;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;



public class Search extends HttpServlet {

    protected void processRequest(HttpServletRequest request, HttpServletResponse response)
    throws ServletException, IOException {

        response.setContentType("text/html;charset=UTF-8");
        PrintWriter out = response.getWriter();

        HttpSession session = request.getSession();


        Path path = (Path)session.getAttribute("path");
        if(path == null) {
            session.setAttribute("path", new Path("Home"));
            path = (Path)session.getAttribute("path");
        } else {
            path.set("Home");
        }
        
        Access access = (Access)session.getAttribute("access");
        if(access == null) {
            session.setAttribute("access", new Access());
            access = (Access)session.getAttribute("access");
        }
        LinkedList<String> msgs = (LinkedList<String>)session.getAttribute("msgs");
        if(msgs == null) {
            session.setAttribute("msgs", new LinkedList<String>());
            msgs = (LinkedList<String>)session.getAttribute("msgs");
        }


        String spath = "Search";
        if(request.getQueryString() != null) {              //-? Ean eixan dothei parametroi sto servlet:
            spath += "?" + request.getQueryString();        //   => Tis prosthetei sto path.
        }
        path.set(spath);                                    //=> Thetei to neo path.


        SearchInfo si = new SearchInfo();
        StringFixer sf = new StringFixer();
        boolean doSearch = false;

        String query = "";


        String owner = sf.fixWhiteChars(request.getParameter("owner"));
        if(owner != null  &&  owner.isEmpty() == false) {       //-? Ean exei dothei owner gia tin anazitisi:
            doSearch = true;                                    //   => Tha ginei anazitisi.
            query += "owner=" + sf.addQuotes(sf.fixForEQUAL(owner)) + " ";
            si.setOwner(owner);
        }

        String category = getCategory(request.getParameter("category"));
        if(category.compareTo("any") != 0) {                    //-? Ean exei dothei katigoria gia tin anazitisi:
            if(doSearch == true) {                              //   -? Ean den einai i 1i sunthiki meta to "WHERE":
                query += "AND ";                                //      => Prosthetei ena "AND" prin eisagei tin
            }                                                   //         katigoria sto query.
            doSearch = true;
            query += "category=" + sf.addQuotes(category) + " ";
        }
        si.setCategory(category);

        String type = getType(request.getParameter("type"));
        si.setType(type.toLowerCase());

        String keywords = sf.fixWhiteChars(request.getParameter("keywords"));
        if(keywords != null  &&  keywords.isEmpty() == false) { //-? Ean exoun dothei lekseis kleidia gia anazitisi:
            if(doSearch == true) {
                query += "AND ";
            }
            doSearch = true;

            String[] words = keywords.split(" ");               //   => Spaei to string twn keywords se
                                                                //      ksexwristes lekseis.
            // Estw oti to split edwse 'N' lekseis. Gia tin enswmatwsi
            // twn keywords sto query, akoloutheitai i eksis logiki:
            //
            // query +=           "name LIKE '%VAL(words[0])%' ";
            // query += "VAL(type) name LIKE '%VAL(words[1])%' ";
            //       ...
            // query += "VAL(type) name LIKE '%VAL(words[N])%' ";
            //
            query += "(name LIKE '%" + sf.fixForLIKE(words[0]) + "%' ";
            for(int i=1 ; i<words.length ; ++i) {
                query += type + " name LIKE '%" + sf.fixForLIKE(words[i]) + "%' ";
            }
            query += ") ";
            si.setKeywords(keywords);
        }

        String exclude  = sf.fixWhiteChars(request.getParameter("exclude"));
        if(exclude != null  &&  exclude.isEmpty() == false) {   //-? Ean exoun dothei lekseis apokleiomenes
                                                                //   apo tin anazitisi:
            if(doSearch == true) {
                query += "AND ";
            }
            doSearch = true;
                                                                //   => Akolouthei paromoia diadikasia me autin
            String[] words = exclude.split(" ");                //      pou akolouthithike kai gia ta keywords.
            query += "name NOT LIKE '%" + sf.fixForLIKE(words[0]) + "%' ";
            for(int i=1 ; i<words.length ; ++i) {
                query += "AND name NOT LIKE '%" + sf.fixForLIKE(words[i]) + "%' ";
            }
            si.setExclude(exclude);
        }

        boolean isPriceError = false;
        int priceFrom = getPrice(sf.fixWhiteChars(request.getParameter("price_from")));
        if(priceFrom >= 0) {                                    //-? Ean dothike apodekti timi (dld arithmos>0):
            if(doSearch == true) {
                query += "AND ";
            }
            doSearch = true;
            query += "price>=" + priceFrom + " ";
            si.setPriceFrom(String.valueOf(priceFrom/100));
        } else if(priceFrom == -2) {                            //-? Alliws, ean dothike mi eguri timi:
            msgs.add("Search:PriceNotNumber");                  //   => Prosthetei katallilo error message stin
                                                                //      message list.
            isPriceError = true;                                //   => Simeiwnei oti ypirkse lathos se timi.
        }
        // Gia tin 'priceTo' akoloutheitai idio skeptiko me auto tis 'priceFrom'...
        //
        int priceTo = getPrice(sf.fixWhiteChars(request.getParameter("price_to")));
        if(priceTo >= 0) {
            if(priceTo < priceFrom) {
                msgs.add("Search:PriceFrom>To");
                isPriceError = true;
            } else {
                if(doSearch == true) {
                    query += "AND ";
                }
                doSearch = true;
                query += "price<=" + priceTo + " ";
                si.setPriceTo(String.valueOf(priceTo/100));
            }
        } else if(priceTo == -2) {
            msgs.add("Search:PriceNotNumber");
            isPriceError = true;
        }


        String queryEnd = "";

        String orderBy = getOrderBy(request.getParameter("order_by"));
        queryEnd += "ORDER BY " + orderBy + " ";
        si.setOrderBy(orderBy);

        String order = getOrder(request.getParameter("order"));
        queryEnd += order + " ";
        si.setOrder(order.toLowerCase());

        String resultsPerPage = getResultsPerPage(request.getParameter("rpp"));
        si.setResultsPerPage(resultsPerPage);


        // Ean exei yparksei kapoio lathos se timi (priceFrom,priceTo) i den exei dothei kammia
        // pliroforia gia tin anazitisi, tote anazitisi den ginetai.
        //
        if(isPriceError == true  ||  doSearch == false) {
            session.setAttribute("sinfo", si);
            try {
                access.set(true);
                request.getRequestDispatcher("search.jsp").include(request, response);
            } catch(Exception e) {
                System.out.println("ERROR [Search.processRequest >> request.getRequestDispatcher(\"search.jsp\").include]");
                access.set(true);
                request.getRequestDispatcher("errorGeneral.jsp").include(request, response);
            } finally {
                session.removeAttribute("sinfo");
                out.close();
                return;
            }
        }


        Connection conn = null;
        try {
            Class.forName("com.mysql.jdbc.Driver");
            conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/test","root","2507");

            try {
                Statement st = conn.createStatement();
                ResultSet rs = null;
                int num = 0;
                int rpp = (resultsPerPage.compareTo("all") == 0 ? -1 : Integer.parseInt(resultsPerPage));
                int page = getPage(request.getParameter("page"));
                int pages = 1;

                st.executeQuery("SELECT COUNT(*) FROM public_files WHERE " + query);

                rs = st.getResultSet();
                if(rs.next() == true) {
                    num = rs.getInt(1);
                }


                if(num > 0) {           // Ean vrike apotelesmata...

                    if(rpp == -1) {     // Ean to results per page einai -1 simainei oti exei epilegei "all"
                        pages = 1;      // tote kai i selida apotelesmatwn einai mia.
                    } else {
                        pages = (int)Math.ceil(num/(double)rpp);
                    }

                    if(page > pages) {
                        page = pages;
                    }

                    if(rpp > 1) {
                        queryEnd += "LIMIT " + (page-1)*rpp + "," + rpp;
                    }


                    st.executeQuery("SELECT * FROM public_files WHERE " + query + queryEnd);
                    rs = st.getResultSet();
                }

                si.setPage(page);
                si.setPages(pages);
                si.setResultsNum(num);
                si.setResultSet(rs);


                session.setAttribute("sinfo", si);
                try {
                    access.set(true);
                    request.getRequestDispatcher("search.jsp").include(request, response);
                } catch(Exception e) {
                    System.out.println("ERROR [Search.processRequest >> request.getRequestDispatcher(\"search.jsp\").include]");
                    access.set(true);
                    request.getRequestDispatcher("errorGeneral.jsp").forward(request, response);
                } finally {
                    session.removeAttribute("sinfo");
                }

                rs.close();
                st.close();
                conn.close();

            } catch(SQLException e) {
                System.out.println("ERROR [Search.processRequest >> *]: SQLException");
                access.set(true);
                request.getRequestDispatcher("errorGeneral.jsp").forward(request, response);
            }
        } catch(ClassNotFoundException e) {
            System.out.println("ERROR [Search.processRequest >> Class.forName]: ClassNotFoundException");
            access.set(true);
            request.getRequestDispatcher("errorGeneral.jsp").forward(request, response);
        } catch(SQLException e) {
            System.out.println("ERROR [Search.processRequest >> DriverManager.getConnection]: SQLException");
            access.set(true);
            request.getRequestDispatcher("errorGeneral.jsp").forward(request, response);
        } finally {
            out.close();
        }

    }








    // <editor-fold defaultstate="collapsed" desc="HttpServlet methods. Click on the + sign on the left to edit the code.">
    /**
     * Handles the HTTP <code>GET</code> method.
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
    throws ServletException, IOException {
        processRequest(request, response);
    }

    /**
     * Handles the HTTP <code>POST</code> method.
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
    throws ServletException, IOException {
        processRequest(request, response);
    }

    /**
     * Returns a short description of the servlet.
     * @return a String containing servlet description
     */
    @Override
    public String getServletInfo() {
        return "Short description";
    }// </editor-fold>



    String getType(String type) {

        if(type == null || type.compareTo("or") != 0) {
            return "AND";
        } else {
            return "OR";
        }
    }

    String getCategory(String category) {

        if(category == null
        || (category.compareTo("music")     != 0
        &&  category.compareTo("movies")    != 0
        &&  category.compareTo("series_tv") != 0
        &&  category.compareTo("images")    != 0
        &&  category.compareTo("games")     != 0
        &&  category.compareTo("software")  != 0
        &&  category.compareTo("other")     != 0)) {
            return "any";
        } else {
            return category;
        }
    }

    int getPrice(String s) {

        if(s != null  &&  s.isEmpty() == false) {
            int price;
            try {
                price = (int)((new Float(s)).floatValue() * 100);
                if(price >= 0) {
                    return price;
                } else {
                    return -2;
                }
            } catch(NumberFormatException e) {
                return -2;
            }
        } else {
            return -1;
        }
    }

    String getOrderBy(String orderBy) {

        if(orderBy == null
        || (orderBy.compareTo("date")      != 0
        &&  orderBy.compareTo("size")      != 0
        &&  orderBy.compareTo("price")     != 0
        &&  orderBy.compareTo("downloads") != 0)) {
            return "name";
        } else {
            return orderBy;
        }
    }

    String getOrder(String order) {

        if(order == null || order.compareTo("desc") != 0) {
            return "ASC";
        } else {
            return "DESC";
        }
    }

    String getResultsPerPage(String rpp) {

        if(rpp == null
        || (rpp.compareTo("20")  != 0
        &&  rpp.compareTo("50")  != 0
        &&  rpp.compareTo("100") != 0
        &&  rpp.compareTo("all") != 0)) {
            return "10";
        } else {
            return rpp;
        }
    }

    int getPage(String s) {

        if(s != null) {
            int page;
            try {
                page = (new Integer(s)).intValue();
                if(page > 0) {
                    return page;
                } else {
                    return 1;
                }
            } catch(NumberFormatException e) {
                return 1;
            }
        } else {
            return 1;
        }
    }

}
