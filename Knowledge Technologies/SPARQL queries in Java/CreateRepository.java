
//queries 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7

import java.beans.Statement;
import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.model.ValueFactory;
import org.openrdf.query.BindingSet;
import org.openrdf.query.GraphQuery;
import org.openrdf.query.GraphQueryResult;
import org.openrdf.query.QueryLanguage;
import org.openrdf.query.TupleQuery;
import org.openrdf.query.TupleQueryResult;
import org.openrdf.repository.Repository;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.RepositoryException;
import org.openrdf.repository.RepositoryResult;
import org.openrdf.repository.sail.SailRepository;
import org.openrdf.rio.RDFFormat;
import org.openrdf.rio.RDFParseException;
import org.openrdf.sail.inferencer.fc.ForwardChainingRDFSInferencer;
import org.openrdf.sail.memory.MemoryStore;
import org.openrdf.sail.nativerdf.NativeStore;

public class CreateRepository {
	
	public static void main(String[] args) throws RepositoryException, RDFParseException, IOException {
			
		//analoga me to poio kommati tis askisis thelw na treksw
		//aposxoliazw tin analogi klisi tis methodou tis klassis.
		Inferencing.inferencing();
		//NamedGraphs.loadBoth();
		//WithoutInferencing.withoutInferencing();
		
		MemoryStore store = new MemoryStore();
		Repository repo = new SailRepository(store);
		
		try {	
			repo.initialize();
			
			org.openrdf.model.ValueFactory vf = repo.getValueFactory();
			URI uri2013 = vf.createURI("http://liza/2013");

			String fileName = "C:\\Users\\Eliza\\Downloads\\complete";
			File dataFile = new File(fileName);
			
			RepositoryConnection con = repo.getConnection();

				//store the file
				try {
					try {
						con.add(dataFile, null, RDFFormat.RDFXML, uri2013);
					
					} finally {
						con.close();
					}
				} catch (RDFParseException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
		} catch (RepositoryException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		RepositoryConnection con = repo.getConnection();
		try{
			String prefix = "PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#>"+
							"PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#>"+
							"PREFIX owl: <http://www.w3.org/2002/07/owl#>"+
							"PREFIX dc: <http://purl.org/dc/elements/1.1/>"+
							"PREFIX dcterms: <http://purl.org/dc/terms/>"+
							"PREFIX foaf: <http://xmlns.com/foaf/0.1/>"+
							"PREFIX swrc: <http://swrc.ontoware.org/ontology#>"+
							"PREFIX swrc-ext: <http://www.cs.vu.nl/~mcaklein/onto/swrc_ext/2005/05#>"+
							"PREFIX geo: <http://www.w3.org/2003/01/geo/wgs84_pos#>"+
							"PREFIX ical: <http://www.w3.org/2002/12/cal/ical#>"+
							"PREFIX swc: <http://data.semanticweb.org/ns/swc/ontology#>";
	
			
			//2.1
			String queryString1 =
					prefix+
					"SELECT DISTINCT ?title ?authors ?paper ?keyword "+
					" WHERE  { ?paper swrc:listAuthor ?authors. "+
					" ?paper dc:title ?title."+
					" ?paper rdf:type swrc:InProceedings."+
					" ?paper swrc:abstract ?keyword."+
					" FILTER regex(?keyword, \"Linked Data\")."+	
					"  FILTER regex(?keyword, \"SPARQL\")."+
					"}";
			


			//2.2
			String queryString2 = 
					prefix+
					" SELECT DISTINCT ?paperTitle ?name  ?coAuthorNames  "+
					" WHERE { "+
					" ?person rdfs:label ?name. "+
					" ?person rdf:type foaf:Person. "+
					" ?person swc:holdsRole ?chair. "+
					" ?chair rdf:type swc:Chair. "+
					" ?paper swrc:author ?person. "+
					" ?paper rdf:type swrc:InProceedings. "+
					" ?paper dc:title ?paperTitle. "+
					" ?paper swrc:listAuthor ?coAuthorNames. "+
					" } "+ 
					"ORDER BY ?name";
			//2.3
			String queryString3 = 
					prefix+
					" SELECT DISTINCT ?name  ?role"+  
					" WHERE {"+
					" ?person rdfs:label ?name."+ 
					" ?person rdf:type foaf:Person."+ 
					" ?person swc:holdsRole ?role."+ 
					" ?chair rdf:type swc:Chair."+ 
					" OPTIONAL {?person foaf:made ?paper."+ 
					" ?paper rdf:type swrc:InProceedings. }"+
					" FILTER (!bound(?paper))"+
					" } "+
					"ORDER BY ?name";

			//2.4
			String queryString4 = 
					prefix+
					" SELECT ?title ?name"+
					" WHERE {"+
					" ?paper dc:title ?title."+
					" ?paper swrc:author ?person."+
					" ?person rdfs:label ?name."+
					" {SELECT  (COUNT( ?person) AS ?count) ?person  "+
					" WHERE {"+
					" ?paper swrc:author ?person."+
					
					"}  GROUP BY ?person "+
					" HAVING (?count>=3)}} ORDER BY ?name";
			
			
			//2.5
			String queryString5 = 
					prefix+
					"SELECT  DISTINCT ?name ?paper ?list "+
					"WHERE {"+
					"?person foaf:name \"Kostis Kyzirakos\"."+
					"?person foaf:name ?name."+
					"?paper swrc:author ?person."+
					"?paper swrc:listAuthor ?list."+
					"}";
			
			//2.6
			String graphQueryString6 = 
					prefix+
					"CONSTRUCT{"
					+ " _:v foaf:name ?name."
					+ " _:v foaf:homepage ?page. "
					+ " _:v swrc:affiliation ?aff."
					+ " ?paper swrc:author _:v"
					+ "}"+
					" WHERE {"+
					" ?paper dc:title ?title."+
					" ?paper swrc:author ?person."+
					" ?person foaf:name ?name."+
					" ?person foaf:homepage ?page."+
					" ?person swrc:affiliation ?aff"+
					" {SELECT  (COUNT( ?person) AS ?count) ?person  "+
					" WHERE {"+
					" ?paper swrc:author ?person."+
					
					"}  GROUP BY ?person "+
					" HAVING (?count=1)}}ORDER BY ?name";
					

			//2.7
			String queryString7 = 
					prefix+
					"SELECT DISTINCT ?name1 "+
					" WHERE {"+
					//first author..
					" ?author1 swrc:affiliation ?aff."+
					" ?paper1 swrc:author ?author1."+
					" ?author1 foaf:name ?name1."+
					" ?paper1 rdf:type swrc:InProceedings."+
					" ?paper1 swc:relatedToEvent ?event1."+
					" ?event1 swc:isSubEventOf ?session1."+
					" ?session1 ical:dtend ?end."+
					" ?session1 ical:dtstart ?start. "+
					//second author..
					" ?author2 swrc:affiliation ?aff."+
					" ?paper2 swrc:author ?author2."+
					" ?author2 foaf:name ?name2."+
					" ?paper2 rdf:type swrc:InProceedings."+
					" ?paper2 swc:relatedToEvent ?event2."+
					" ?event2 swc:isSubEventOf ?session2."+
					" ?session2 ical:dtend ?end."+
					" ?session2 ical:dtstart ?start. "+
					" FILTER (?session1 != ?session2)"+
					"} ORDER BY ?author1";
			
			
			
			String queryString9 = 
					prefix+
					"SELECT DISTINCT ?namedgraph ?label"+
					"WHERE {"+
					"GRAPH ?namedgraph { ?s ?p ?o }"+
					
					"}"+
					"ORDER BY ?namedgraph";

			//tuple queries...
			String queryString = queryString1;
			TupleQuery tupleQuery = con.prepareTupleQuery(QueryLanguage.SPARQL, queryString);
			TupleQueryResult result = tupleQuery.evaluate();
			System.out.println("Query:\n" + queryString);
			
			//construct...
			String graphQueryString = graphQueryString6;
			GraphQuery graphQuery = con.prepareGraphQuery(QueryLanguage.SPARQL, graphQueryString);
			GraphQueryResult graphResult = graphQuery.evaluate();
			
			try{
				while (graphResult.hasNext()) {
					org.openrdf.model.Statement st =  graphResult.next();
					System.out.println(st.toString());
				}
			}finally{
				graphResult.close();
				
			}
			try {
				
				System.out.println("Tuple Query Result Only 2013:");
				while (result.hasNext()) {
					BindingSet bindingSet = result.next();
					
					System.out.println(bindingSet.toString());
					
				}
			}finally{
					result.close();
				}	
		}catch(Exception e) {
			//handle exception
			e.printStackTrace();			
		}
	}
}
