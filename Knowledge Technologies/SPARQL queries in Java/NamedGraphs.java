

//queries 2.8, 2.9, 2.10

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


public class NamedGraphs {

	public static void loadBoth() throws RDFParseException, IOException, RepositoryException{
		
		MemoryStore store = new MemoryStore();
		Repository repo = new SailRepository(store);
		try{
			
			try {
				repo.initialize();
			} catch (RepositoryException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			org.openrdf.model.ValueFactory vf = repo.getValueFactory();
			URI uri2013 = vf.createURI("http://liza/2013");
			URI uri2012 = vf.createURI("http://liza/2012");
			
			String fileName2013 = "C:\\Users\\Eliza\\Downloads\\complete";
			String fileName2012 = "C:\\Users\\Eliza\\Downloads\\complete2012";
			

			File dataFile2013 = new File(fileName2013);
			File dataFile2012 = new File(fileName2012);
			
			RepositoryConnection con = repo.getConnection();
			
			con.add(dataFile2013, null, RDFFormat.RDFXML, uri2013);
			con.add(dataFile2012, null, RDFFormat.RDFXML, uri2012);
			
		}catch (RepositoryException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		RepositoryConnection con = repo.getConnection();
		try{
			String prefix = "PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#>"+
					"PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#>"+
					"PREFIX owl: <http://www.w3.org/2002/07/owl#>"+
					"	PREFIX dc: <http://purl.org/dc/elements/1.1/>"+
					"PREFIX dcterms: <http://purl.org/dc/terms/>"+
					"PREFIX foaf: <http://xmlns.com/foaf/0.1/>"+
					"PREFIX swrc: <http://swrc.ontoware.org/ontology#>"+
					"PREFIX swrc-ext: <http://www.cs.vu.nl/~mcaklein/onto/swrc_ext/2005/05#>"+
					"PREFIX geo: <http://www.w3.org/2003/01/geo/wgs84_pos#>"+
					"PREFIX ical: <http://www.w3.org/2002/12/cal/ical#>"+
					"PREFIX swc: <http://data.semanticweb.org/ns/swc/ontology#>"+
					"PREFIX iswc2013: <http://liza/2013>";
			//queries...
			
			//2.8
			String queryString8 = 
					prefix+
					"SELECT DISTINCT    ?name  "+
					 "FROM NAMED <http://liza/2013>"+
					 "FROM  NAMED <http://liza/2012>"+
					 " WHERE {"+
					 "GRAPH ?g {"+
					 " {SELECT DISTINCT ?person ?name"+ 
					 " WHERE{"+
					 " ?person foaf:name ?name."+
					 " ?paper swrc:author ?person. "+
					 " ?paper swrc:year \"2013\"."+
					 " ?paper rdf:type swrc:InProceedings."+
					 "}}."+
					 " {SELECT DISTINCT ?person ?name "+
					 " WHERE{"+
					 " ?person foaf:name ?name."+
					 " ?paper swrc:author ?person."+ 
					 " ?paper swrc:year \"2012\"."+
					 " ?paper rdf:type swrc:InProceedings."+
					 " }}."+					 
					 " }} ORDER BY ?name";
			
			//2.9
			String queryString9 = 
					prefix+
					"SELECT DISTINCT  ?name "+
					 "FROM NAMED <http://liza/2013>"+
					 "FROM  NAMED <http://liza/2012>"+
					 " WHERE {"+
					 "GRAPH ?g {"+
					 
					 " ?person foaf:made ?paper."+
					 " ?person foaf:name ?name."+
					 " ?paper rdf:type swrc:InProceedings."+
					 "?paper swrc:year ?year"+
			
					  "}FILTER (?year !=\"2013\")"+
					 
					 " } ORDER BY ?name";
			
			//2.10
			String graphQueryString10 =
					prefix+
					"CONSTRUCT { _:person foaf:name ?name. "
					+ "?paper swrc:author _:person."
					+ " _:person swrc:affiliation ?aff. } "+
					 "FROM NAMED <http://liza/2013>"+
					 "FROM  NAMED <http://liza/2012> "+
					 " WHERE {"+
					 "GRAPH ?g {"+
					 " ?paper dc:title ?title."+
					 "?paper swrc:author ?person."+
					 "?person rdfs:label ?name."+
					 "{SELECT  (COUNT( ?person) AS ?count) ?person "+  
					 "WHERE {"+
					"?paper swrc:author ?person."+
					
					"}  GROUP BY ?person "+
					 "HAVING (?count>=3)}"+
					" } }ORDER BY ?name";
			
			String queryString11 = 
					prefix +
					"SELECT (COUNT(?person) AS ?count ) ?person "+
					"FROM NAMED <http://liza/2013>"+
					 "FROM  NAMED <http://liza/2012>"+
					"WHERE {"+
					 "GRAPH ?g {"+
					"?person foaf:made ?paper."+
					 " ?paper rdf:type swrc:InProceedings."+
					"?person foaf:name ?name"+
					 
					 "}}GROUP BY ?person "+
					" HAVING (COUNT(?person) >=3)";
			
			//tuple queries..
			String queryString = queryString9;
			TupleQuery tupleQuery = con.prepareTupleQuery(QueryLanguage.SPARQL, queryString);
			TupleQueryResult result = tupleQuery.evaluate();
			System.out.println("Query:\n" + queryString);
			
			//construct..
			String graphQueryString = graphQueryString10;
			GraphQuery graphQuery = con.prepareGraphQuery(QueryLanguage.SPARQL, graphQueryString);
			GraphQueryResult graphResult = graphQuery.evaluate();
			
			try{
				System.out.println("Graph Query Result Named Graphs:");
				while (graphResult.hasNext()) {
					org.openrdf.model.Statement st =  graphResult.next();
					System.out.println(st.toString());
				}
			}finally{
				graphResult.close();
				
			}
			try {
			
				
				System.out.println("Tuple Query Result Named Graphs:");
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
