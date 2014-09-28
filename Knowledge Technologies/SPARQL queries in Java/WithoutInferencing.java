//queries 2.11, 2.12, 2.13, 2.14 without inferencing

import java.io.File;
import java.io.IOException;
import org.openrdf.model.URI;
import org.openrdf.query.BindingSet;
import org.openrdf.query.GraphQuery;
import org.openrdf.query.GraphQueryResult;
import org.openrdf.query.QueryLanguage;
import org.openrdf.query.TupleQuery;
import org.openrdf.query.TupleQueryResult;
import org.openrdf.repository.Repository;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.RepositoryException;
import org.openrdf.repository.sail.SailRepository;
import org.openrdf.rio.RDFFormat;
import org.openrdf.rio.RDFParseException;
import org.openrdf.sail.inferencer.fc.ForwardChainingRDFSInferencer;
import org.openrdf.sail.memory.MemoryStore;


public class WithoutInferencing {
	

	public static void withoutInferencing() throws IOException, RepositoryException{
	
			MemoryStore store = new MemoryStore();
			Repository repo = new SailRepository(store);
			try {
				
				repo.initialize();
				
				org.openrdf.model.ValueFactory vf = repo.getValueFactory();
				String fileBaseURI2013 = "http://liza/2013";
				URI uri2013 = vf.createURI("http://liza/2013");
		
				String fileNameswc = "C:\\Users\\Eliza\\Downloads\\swc_2009-05-09.rdf";
				String fileNameOwl = "C:\\Users\\Eliza\\Downloads\\swrc_updated_v0.7.1.owl";
				String fileName = "C:\\Users\\Eliza\\Downloads\\complete";
				
				File dataFileswc = new File(fileNameswc);
				File dataFileowl = new File(fileNameOwl);
				File dataFile = new File(fileName);
				
		
				RepositoryConnection con = repo.getConnection();
		
					//store the file
					try {
						try {
							con.add(dataFile, fileName, RDFFormat.RDFXML);
							con.add(dataFileswc, fileNameswc, RDFFormat.RDFXML);
							con.add(dataFileowl, fileNameOwl, RDFFormat.RDFXML);
							
				
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
								"PREFIX swc: <http://data.semanticweb.org/ns/swc/ontology#>"+
								"PREFIX iswc2013: <http://liza/2013>";

				//2.11
				
				String queryString1 = 
						prefix+
						" SELECT ?x "+
						" WHERE {"+
						" swc:BreakEvent rdfs:subClassOf* ?x}";
				
				//2.12
				String queryString2 = 
						prefix+
						"SELECT DISTINCT ?x "+
						" WHERE { "+
						" ?x rdfs:subClassOf* swrc:Document."+
						" ?x rdf:type ?type."+
						" FILTER NOT EXISTS { ?x rdfs:subClassOf* swrc:Thesis}."
						+ " FILTER NOT EXISTS { ?x rdfs:subClassOf* swrc:Report}"+
						" }";
				//2.13
				String queryString3 = 
						prefix+
						"SELECT  DISTINCT ?person ?title"+
						" WHERE {"+
						"?publ rdfs:subClassOf* swrc:Publication."+
						" ?paper rdf:type* ?publ."+
						" ?paper swrc:author ?person."+
						" ?paper dc:title ?title."+
						" ?paper swrc:year ?year."+
						" ?person foaf:name ?name."+
						" FILTER (?name = \"Ulrike Sattler\")."+
						"FILTER (?year =\"2013\")."+
						
						"}";
				
				//2.14
				String queryString4 = 
						prefix+
						"SELECT ?inst"+
						" WHERE { "+
						" ?x rdfs:subClassOf* swc:Paper."+
						" ?inst rdf:type ?x."+
						
						" }";
				
				//tuple queries...
				String queryString = queryString4;
				TupleQuery tupleQuery = con.prepareTupleQuery(QueryLanguage.SPARQL, queryString);
				TupleQueryResult result = tupleQuery.evaluate();
				System.out.println("Query:\n" + queryString);
				
				try {
					System.out.println("Tuple Query Result Without Inferencing:");
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
