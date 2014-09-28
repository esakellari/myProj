
package general;



public class StringFixer {


    public String addQuotes(String s) {
        return "'"+s+"'";
    }

    public String addQuotes(int i) {
        return "'"+i+"'";
    }


    // Metatrepei oles tis akolouthies apo whitespaces se ena mono keno
    // (" "), kai afairei apo tin arxi kai to telos tou string ta kena.
    // px. to string "  \t \t\t Enas \t\r\nMagas sto Votaniko\t"
    // ginetai "Enas Magas sto Votaniko".
    //
    public String fixWhiteChars(String s) {
        if(s != null) {
            return s.replaceAll("\\s{2,}", " ").trim();
        } else {
            return null;
        }
    }


    public String fixForEQUAL(String s) {

        if(s != null) {
            return  s.replaceAll("\\\\", "\\\\\\\\")
                    .replaceAll("'", "\\\\'");
        } else {
            return null;
        }
    }

    
    public String fixForLIKE(String s) {

        return  s.replaceAll("\\\\", "\\\\\\\\\\\\\\\\")
                 .replaceAll("_", "\\\\_")
                 .replaceAll("%", "\\\\%")
                 .replaceAll("'", "\\\\'");
    }


    public String fixForInsert(String s) {

        return s.replaceAll("\\\\", "\\\\\\\\")
                .replaceAll("'", "\\\\'");
    }


    public String decodeSize(long bytes) {

        double kb = 1024;
        double mb = 1048576;    // = 1024*1024
        double gb = 1073741824; // = 1024*1024*1024

        if(bytes < mb) {
            return firstDecimal(bytes/kb) + " KBs";
        } else if(bytes < gb) {
            return firstDecimal(bytes/mb) + " MBs";
        } else {
            return firstDecimal(bytes/gb) + " GBs";
        }
    }

    String firstDecimal(double s) {
        return (int)s + "." + ((int)(s*10))%10;
    }

}
