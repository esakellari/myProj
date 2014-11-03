import java.util.*;
import java.io.*;

class ListStruct{
	public  Integer first;
	public  Integer second;
	
	public ListStruct(Integer f, Integer s){
		first=f;
		second=s;
	}
}
public class Pairs { 
   public static void main(String[] args) { 
        try{
                BufferedReader br =  new BufferedReader(new InputStreamReader(System.in));

	        String nextLine;
	        LinkedList<ListStruct> list = new LinkedList<ListStruct>();
	        Map<Integer, HashMap<Integer,Integer>> map = new HashMap<Integer, HashMap<Integer, Integer>>();

		        while((nextLine = br.readLine())!= null){

			        String[] values = nextLine.split("\\|");

			        Integer first = Integer.valueOf(values[0]);
			        Integer second = Integer.valueOf(values[1]);

			        if(map.containsKey(first)){
				        HashMap secMap = (HashMap)map.get(first);
				
				        if(!secMap.containsKey(second))
                                                secMap.put(second, secMap.size()+1);
					
			        }else{
				        HashMap secMap = new HashMap<Integer, Integer>();
				        secMap.put(second, 1);
				        map.put(first,secMap);                                        
			        }
			        list.add(new ListStruct(first,second));
		        }
                        br.close();

                        Iterator it = list.iterator();      

                        while(it.hasNext()){
                                ListStruct next = (ListStruct)it.next();
                                HashMap secMap = (HashMap)map.get(next.first);
                                Integer position = (Integer)secMap.get(next.second);
                                System.out.println(next.first+"|"+next.second+"["+position+" of "+secMap.size()+"]");
				
                        }   
 
	}catch(Exception fnfe){}
   }
}
