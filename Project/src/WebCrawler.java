import java.io.BufferedReader;
import java.io.IOException;
import java.io.StringReader;
import java.net.URL;
import java.util.HashSet;

import opennlp.tools.stemmer.snowball.SnowballStemmer;

/**
 * Web Crawler class to build inverted index from seed URL
 * @author EricChen
 *
 */
public class WebCrawler {

	/** The default stemmer algorithm used by this class. */
	private static SnowballStemmer.ALGORITHM DEFAULT = SnowballStemmer.ALGORITHM.ENGLISH;

	/** Number of threads to be used */
	private int threads;

	/** Fixed limit of unique URLs */
	private int limit;

	/** Work queue to build index from seed URL */
	private WorkQueue workQueue;

	/** Set of seed URLs */
	private final HashSet<URL> links;

	/** Thread safe inverted index */
	private final ThreadSafeInvertedIndex index;

	/**
	 * Constructor for the web crawler
	 * @param index	thread-safe inverted index
	 * @param threads	number of threads to use
	 * @param limit		fixed limit of unique URLs
	 */
	public WebCrawler(ThreadSafeInvertedIndex index, int threads, int limit) {
		this.index = index;
		this.threads = threads;
		this.limit = limit;
		this.links = new HashSet<URL>();
	}

	/**
	 * Creates the work queue with necessary number of threads and adds the seed URL to the set
	 * @param seed	seed URL
	 */
	public void addSeed(URL seed){
		workQueue = new WorkQueue(threads);
		links.add(seed);
		workQueue.execute(new Task(seed));
		try {
			workQueue.finish();
		}
		catch (Exception e) {
			System.err.print("Error: " + e);
		}
		workQueue.shutdown();
	}

	/**
	 * Stemming every line from seed URL before adding it into the index
	 * @param cleaned	cleaned and stemmed line
	 * @param url		seed URL
	 * @param index		the inverted index
	 * @throws IOException	if file not found
	 */
	public void addStem(String cleaned, String url, InvertedIndex index) throws IOException {
		int pos = 0;
		try (BufferedReader reader = new BufferedReader(new StringReader(cleaned));) {
			String line = null;
			SnowballStemmer stemmer = new SnowballStemmer(DEFAULT);
			while ((line = reader.readLine()) != null) {
				for (String word : TextParser.parse(line)) {
					pos++;
					index.add(stemmer.stem(word).toString(), url, pos);
				}
			}
		}
	}

	/**
	 * Task class to allow execution by threads to clean content and HTML before adding it into inverted index
	 * @author EricChen
	 */
	class Task implements Runnable {

		/** Seed URL */
		private URL url;

		/**
		 * Constructor for task class that takes in seed URL
		 * @param url   seed URL
		 */
		public Task(URL url) {
			this.url = url;
		}

		@Override
		public void run() {
			try {
				String content = HtmlFetcher.fetch(url, 3);

				if (content == null) {
					return;
				}

				content = HtmlCleaner.stripComments(content);
				String cleaner = HtmlCleaner.stripHtml(content);
				InvertedIndex local = new InvertedIndex();
				addStem(cleaner, url.toString(), local);
				index.addAll(local);

				synchronized(links) {
					for (URL url : LinkParser.listLinks(url, content)) {
						if (links.size() < limit && links.add(url)) {
							workQueue.execute(new Task(url));
						}
					}
				}
			}
			catch (Exception e) {
				System.err.printf("Error: ", e.toString());
			}
		}
	}
}