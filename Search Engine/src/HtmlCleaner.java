import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Cleans simple, validating HTML 4/5 into plain text.
 */
public class HtmlCleaner {

	/**
	 * Constructor to clean HTML and to initialize list of HTTP(S) links
	 * @param url	seed URL
	 * @param HTML	HTML to be cleaned
	 */
	public HtmlCleaner(URL url, String HTML) {
		HTML = stripComments(HTML);
		HTML = stripBlockElements(HTML);
		listLinks(url, HTML);
		HTML = stripTags(HTML);
		HTML = stripEntities(HTML);
	}

	/**
	 * Constructor to ensure max limit of 3 redirects
	 * @param base	seed URL
	 * @param redirects	if HTTP response status is redirect
	 * @throws IOException	if file not found
	 */
	public HtmlCleaner(URL base, int redirects) throws IOException {
		this(base, HtmlFetcher.fetch(base, redirects));
	}

	/**
	 * Returns a list of all the HTTP(S) links found in the href attribute of the
	 * anchor tags in the provided HTML. The links will be converted to absolute
	 * using the base URL and cleaned (removing fragments and encoding special
	 * characters as necessary).
	 *
	 * @param base the base url used to convert relative links to absolute3
	 * @param html the raw html associated with the base url
	 * @return cleaned list of all http(s) links in the order they were found
	 */
	public static ArrayList<URL> listLinks(URL base, String html) {
		ArrayList<URL> links = new ArrayList<URL>();
		String regex = "(?msi)<a\\s+(?:[^>]*?\\s+)?href\\s*=\\s*([\\\\\"])(.*?)\\1";	// Create a pattern to be searched
        Pattern p = Pattern.compile(regex);		// Create regex and matcher
		Matcher m = p.matcher(html);
		int index = 0;

		while (index < html.length() && m.find(index)) {// Keep going while found a match in text
			try {
				links.add(clean(new URL(base, html.substring(m.start(2), m.end(2)))));
			}
			catch (MalformedURLException e) {
				e.printStackTrace();
			}
			if (m.start() == m.end()) {
				index = m.end() + 1;
			}
			else {
				index = m.end();
			}
		}
		return links;
	}

	/**
	 * Removes the fragment component of a URL (if present), and properly encodes
	 * the query string (if necessary).
	 *
	 * @param url the url to clean
	 * @return cleaned url (or original url if any issues occurred)
	 */
	public static URL clean(URL url) {
		try {
			return new URI(url.getProtocol(), url.getUserInfo(), url.getHost(),
					url.getPort(), url.getPath(), url.getQuery(), null).toURL();
		}
		catch (MalformedURLException | URISyntaxException e) {
			return url;
		}
	}

	/**
	 * Removes all HTML tags and certain block elements from the provided text.
	 * The block elements removed include: head, style, script, noscript, and svg.
	 *
	 * @param html the HTML to strip tags and elements from
	 * @return text clean of any HTML tags and certain block elements
	 */
	public static String stripHtml(String html) {
		html = stripBlockElements(html);
		html = stripTags(html);
		html = stripEntities(html);
		return html;
	}

	/**
	 * Removes comments and certain block elements from the provided html. The block
	 * elements removed include: head, style, script, noscript, and svg.
	 *
	 * @param html the HTML to strip comments and block elements from
	 * @return text clean of any comments and certain HTML block elements
	 */
	public static String stripBlockElements(String html) {
		html = stripComments(html);
		html = stripElement(html, "head");
		html = stripElement(html, "style");
		html = stripElement(html, "script");
		html = stripElement(html, "noscript");
		html = stripElement(html, "svg");
		return html;
	}

	/**
	 * Replaces all HTML entities with an empty string. For example,
	 * "2010&ndash;2012" will become "20102012".
	 *
	 * @param html text including HTML entities to remove
	 * @return text without any HTML entities
	 */
	public static String stripEntities(String html) {
		return html.replaceAll("&\\S+?;", "");
	}

	/**
	 * Replaces all HTML tags with an empty string. For example, "A<b>B</b>C" will
	 * become "ABC".
	 *
	 * @param html text including HTML tags to remove
	 * @return text without any HTML tags
	 */
	public static String stripTags(String html) {
		return html.replaceAll("(?s)<.*?>", "");
	}

	/**
	 * Replaces all HTML comments with a single space. For example, "A<!-- B -->C"
	 * will become "A C".
	 *
	 * @param html text including HTML comments to remove
	 * @return text without any HTML comments
	 */
	public static String stripComments(String html) {
		return html.replaceAll("(?s)<!--.*?-->", " ");
	}

	/**
	 * Replaces everything between the element tags and the element tags
	 * themselves with a single space. For example, consider the html code: *
	 *
	 * <pre>
	 * &lt;style type="text/css"&gt;body { font-size: 10pt; }&lt;/style&gt;
	 * </pre>
	 *
	 * If removing the "style" element, all of the above code will be removed, and
	 * replaced with a single space.
	 *
	 * @param html text including HTML elements to remove
	 * @param name name of the HTML element (like "style" or "script")
	 * @return text without that HTML element
	 */
	public static String stripElement(String html, String name) {
		return html.replaceAll("(?s)(?i)<" + name + ".*?</" + name + "\\s*>", " ");
	}
}
