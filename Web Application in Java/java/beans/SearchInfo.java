
package beans;

import java.sql.ResultSet;



public class SearchInfo {

    String  keywords,
            exclude,
            type,
            priceFrom,
            priceTo,
            owner,
            category,
            orderBy,
            order,
            resultsPerPage;
    int resultsNum,
        page,
        pages;
    ResultSet rs;


    public SearchInfo() {
        keywords        = "";
        exclude         = "";
        type            = "";
        priceFrom       = "";
        priceTo         = "";
        owner           = "";
        category        = "";
        orderBy         = "";
        order           = "";
        resultsPerPage  = "";

        resultsNum = 0;
        page = 1;
        pages = 0;
        rs = null;
    }

    public String getKeywords() {
        return keywords;
    }
    public String getExclude() {
        return exclude;
    }
    public String getType() {
        return type;
    }
    public String getPriceFrom() {
        return priceFrom;
    }
    public String getPriceTo() {
        return priceTo;
    }
    public String getOwner() {
        return owner;
    }
    public String getCategory() {
        return category;
    }
    public String getOrderBy() {
        return orderBy;
    }
    public String getOrder() {
        return order;
    }
    public String getResultsPerPage() {
        return resultsPerPage;
    }
    public ResultSet getResultSet() {
        return rs;
    }
    public int getResultsNum() {
        return resultsNum;
    }
    public int getPage() {
        return page;
    }
    public int getPages() {
        return pages;
    }

    
    public void setKeywords(String value) {
        keywords = value;
    }
    public void setExclude(String value) {
        exclude = value;
    }
    public void setType(String value) {
        type = value;
    }
    public void setPriceFrom(String value) {
        priceFrom = value;
    }
    public void setPriceTo(String value) {
        priceTo = value;
    }
    public void setOwner(String value) {
        owner = value;
    }
    public void setCategory(String value) {
        category = value;
    }
    public void setOrderBy(String value) {
        orderBy = value;
    }
    public void setOrder(String value) {
        order = value;
    }
    public void setResultsPerPage(String value) {
        resultsPerPage = value;
    }
    public void setResultSet(ResultSet value) {
        rs = value;
    }
    public void setResultsNum(int value) {
        resultsNum = value;
    }
    public void setPage(int value) {
        page = value;
    }
    public void setPages(int value) {
        pages = value;
    }
}
