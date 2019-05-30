import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Queue;

public class Graph {
	HashMap <String, Node> nodes;
	
	public Graph() {
		this.nodes = new HashMap<>();
	}
	
	public void addEdge(String source, String target) {
		if(nodes.get(source) == null) {
			nodes.put(source, new Node(source)); //making new node and putting it in hashmap to keep track of actor (string) to nodes
		}
		if(nodes.get(target) == null) {
			nodes.put(target, new Node(target)); 
		}

		nodes.get(source).edges.add(nodes.get(target)); //adds the edge
	}
	
	public ArrayList<String> search(String source, String target){
		Queue<GraphNode> queue = new LinkedList<>();
		GraphNode s = new GraphNode(nodes.get(source), null);
		queue.add(s);
		HashSet <String> visited = new HashSet<>(); //checks if item is visited
		visited.add(source); //if we visit it during the BFS, add it to the visited set
		while(!queue.isEmpty()) {
			GraphNode gNode = queue.remove();
			if(gNode.node.name.equals(target)) {
				return reconstructPath(gNode);
			}
			/* going through all the neighbors that we haven't visited already */
			for(Node neighbor:gNode.node.edges) {
				if(!visited.contains(neighbor.name)) {
					GraphNode newGNode = new GraphNode(neighbor, gNode);
					queue.add(newGNode);
					visited.add(neighbor.name);
				}
			}
		}
		return null;
	}
	
	public ArrayList<String> reconstructPath(GraphNode gNode){
		ArrayList<String> path = new ArrayList<String>();
		while(gNode.parent != null) {
			path.add(gNode.node.name);
			gNode = gNode.parent;
		}
		path.add(gNode.node.name);
		Collections.reverse(path); //reverse the path
		return path;
	}
	
	public boolean isActor(String name) {
		/* check if actor exists */
		return (nodes.get(name)!= null);
	}
}

class Node {
	String name;
	ArrayList <Node> edges;
	
	public Node(String name){
		this.name = name;
		edges = new ArrayList<>();
	}
}

class GraphNode {
	/* Keep track of parents to reconstruct path */
	Node node;
	GraphNode parent;
	
	public GraphNode(Node node, GraphNode parent) {
		this.node = node;
		this.parent = parent;
	}
}
