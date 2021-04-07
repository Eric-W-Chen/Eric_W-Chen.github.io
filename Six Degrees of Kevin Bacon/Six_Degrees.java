import java.io.*;
import java.util.*;
import org.json.simple.*;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

public class A3 {
    private static Scanner scan;

	public static void main(String[] args) throws IOException, ParseException {
    	scan = new Scanner(System.in);
    	JSONParser parser = new JSONParser();
    	String filename = args[0];
    	ArrayList<JSONArray> castMembers = parseFile(filename, parser);
    	Graph graph = createGraph(castMembers);
    	
    	String actor1 = "1";
    	String actor2 = "2";
    	System.out.println("Enter two actors names to get a path: ");
    	System.out.println("Press enter with no input to stop");
    	
    	while(!actor1.isEmpty() && !actor2.isEmpty()) {
    		System.out.print("Actor 1 name: ");
    		actor1 = scan.nextLine();
    		while(!actor1.isEmpty() && !graph.isActor(actor1)) {
    			System.out.println("No such actor.");
    			System.out.print("Actor 1 name: ");
    			actor1 = scan.nextLine();
    		}
    		if(actor1.isEmpty()) {
    			break;
    		}
    		System.out.print("Actor 2 name: ") ;
    		actor2 = scan.nextLine();
    		while(!actor2.isEmpty() && !graph.isActor(actor2)) {
    			System.out.println("No such actor.");
    			System.out.print("Actor 2 name: ");
    			actor2 = scan.nextLine();
    		}
    		if(actor2.isEmpty()) {
    			break;
    		}
    		System.out.print("Path between " + actor1 + " and " + actor2 + ": ");
    		ArrayList <String> path = graph.search(actor1, actor2);
    		if(path != null) {
    			printPath(path);
    		}
    	}
    }
    
    public static ArrayList<JSONArray> parseFile (String filename, JSONParser parser) throws IOException, ParseException{
    	ArrayList<JSONArray> castMembers = new ArrayList<>();
    	try (Scanner file = new Scanner(new File(filename))){ //make an instance of scanner to scan file
    		//read through file
    		file.nextLine();//skip first line
    		while(file.hasNextLine()) {
    			String line = file.nextLine(); 
    			String [] castArray = line.split(",\"\\[");
    			if(castArray.length < 2) {
    				continue; //if cast is empty, skip
    			}
    			String castString = "[" + line.split(",\"\\[")[1].replace("]\"", "]").replace("\"\"", "\""); //find unique characters to separate cast from rest of file to make the file easier to read to parse
    			try {
        			JSONArray cast = (JSONArray) parser.parse(castString); //getting the cast
        			castMembers.add(cast); //adding cast into array list
    			}
    			catch(Exception e) {
    				castString = castString.substring(0, castString.length()-3); //crew is empty
    				JSONArray cast = (JSONArray) parser.parse(castString); //getting the cast
        			castMembers.add(cast); //adding cast into array list
    			}
    		}
    	}
		return castMembers;
    }
    
    public static Graph createGraph(ArrayList<JSONArray> castMembers) {
    	/* going through each movie */
    	Graph graph = new Graph();
    	for(JSONArray cast:castMembers) { //for all the cast members in that movie
    		/*create an edge between all the actors*/
    		for(int i=0; i<cast.size(); i++) { 
    			JSONObject actor = (JSONObject) cast.get(i); 
    			for(int j=0; j<cast.size(); j++) { 
    				if(i!=j) {
    	    			JSONObject otherActor = (JSONObject) cast.get(j); 
    					graph.addEdge((String)actor.get("name"), (String)otherActor.get("name")); 
    				}
    			}
    		}
    	}
        return graph;
    }
    
    public static void printPath(ArrayList<String> path) {
    	for(int i=0; i<path.size()-1; i++) {
    		System.out.print(path.get(i) + " --> ");
    	}
    	System.out.println(path.get(path.size()-1));
    }
}
	