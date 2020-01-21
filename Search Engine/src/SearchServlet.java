import java.io.IOException;
import java.io.PrintWriter;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.nio.charset.StandardCharsets;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.text.StringEscapeUtils;
import org.eclipse.jetty.util.log.Log;
import org.eclipse.jetty.util.log.Logger;

/**
 * An alternative implemention of the {@MessageServlet} class but using the
 * Bulma CSS framework.
 *
 * @see SearchServlet
 */
public class SearchServlet extends CookieBaseServlet {

	/** Identifier used for serialization (unused). */
	private static final long serialVersionUID = 1L;

	/** The title to use for this webpage. */
	private static final String TITLE = "Eric's Search Engine";

	/** The logger to use for this servlet. */
	private static Logger log = Log.getRootLogger();

	/** The thread-safe data structure to use for storing messages. */
	private QueryBuilderInterface builder;

	/** Used to fetch the visited date from a cookie. */
	public static final String VISIT_DATE = "Visited";

	/** Used to fetch the visited count from a cookie. */
	public static final String VISIT_COUNT = "Count";

	/**
	 * Initializes this message board. Each message board has its own collection
	 * of messages.
	 * @param builder query builder
	 */

	public SearchServlet(QueryBuilderInterface builder) {
		super();
		this.builder = builder;
	}

	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {

		response.setContentType("text/html");
		response.setStatus(HttpServletResponse.SC_OK);

		log.info("MessageServlet ID " + this.hashCode() + " handling GET request.");

		PrintWriter out = response.getWriter();

		out.printf("<!DOCTYPE html>%n");
		out.printf("<html>%n");
		out.printf("<head>%n");
		out.printf("	<meta charset=\"utf-8\">%n");
		out.printf("	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">%n");
		out.printf("	<title>%s</title>%n", TITLE);
		out.printf("	<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bulma@0.8.0/css/bulma.min.css\">%n");
		out.printf("	<script defer src=\"https://use.fontawesome.com/releases/v5.3.1/js/all.js\"></script>%n");
		out.printf("</head>%n");
		out.printf("%n");
		out.printf("<body>%n");
		out.printf("	<section class=\"hero is-primary is-bold\">%n");
		out.printf("	  <div class=\"hero-body\">%n");
		out.printf("	    <div class=\"container\">%n");
		out.printf("	      <h1 class=\"title\">%n");
		out.printf("   <a href=\"http://localhost:8080\"style=\"color:white\" >");
		out.printf("	        Search Engine%n");
		out.printf("	      </a></h1></a>%n");
		out.printf("	      <h2 class=\"subtitle\">%n");
		out.printf("					<i class=\"fas fa-calendar-alt\"></i>%n");
		out.printf("					&nbsp;Updated %s%n", getDate());
		out.printf("	      </h2>%n");
		out.printf("	    </div>%n");
		out.printf("	  </div>%n");
		out.printf("	</section>%n");
		out.printf("%n");
		out.printf("	<section class=\"section\">%n");
		out.printf("		<div class=\"container\">%n");
		out.printf("%n");

		String query = request.getParameter("query");

		//protect against XSS attacks
		query = StringEscapeUtils.escapeHtml4(query);

		ArgumentParser parser = new ArgumentParser();

		if (query != null && !query.isEmpty()) {
			out.printf("			<h2 class=\"title\">Search Results for %s </h2>%n", query);
			for (InvertedIndex.SearchResult result : builder.getResults(query, parser.hasFlag("exact"))) {
				if (result.getPath().isEmpty()) {
					out.printf("				<p>No results.</p>%n");
				}
				else {
						response.setContentType("text/html");
						out.printf("				<div class=\"box\">%n");
						String link = String.format("<a href=\"%s\">%s</a>",
								result.getPath(), result.getPath());
						out.printf(link);
						out.printf("				</div>%n");
						out.printf("%n");
				}
			}
			out.printf("			</div>%n");
			out.printf("%n");
			out.printf("		</div>%n");
			out.printf("	</section>%n");
			out.printf("%n");
			out.printf("	<section class=\"section\">%n");
			out.printf("		<div class=\"container\">%n");
			out.printf("			<h2 class=\"title\">Search</h2>%n");
			out.printf("%n");
			out.printf("			<form method=\"%s\" action=\"%s\">%n", "GET", request.getServletPath());
			out.printf("				<div class=\"field\">%n");
			out.printf("					<label class=\"label\">What are you looking for today?</label>%n");
			out.printf("					<div class=\"control has-icons-left\">%n");
			out.printf("						<input class=\"input\" type=\"text\" name=\"%s\" placeholder=\"Enter what you're searching here.\">%n", "query");
			out.printf("						<span class=\"icon is-small is-left\">%n");
			out.printf("							<i class=\"fas fa-user\"></i>%n");
			out.printf("						</span>%n");
			out.printf("					</div>%n");
			out.printf("				</div>%n");
			out.printf("%n");
			out.printf("				<div class=\"control\">%n");
			out.printf("			    <button class=\"button is-primary\" type=\"submit\">%n");
			out.printf("						<i class=\"fas fa-comment\"></i>%n");
			out.printf("						&nbsp;%n");
			out.printf("						Submit%n");
			out.printf("					</button>%n");
			out.printf("			  </div>%n");
			out.printf("			</form>%n");
			out.printf("		</div>%n");
			out.printf("	</section>%n");
		}
		else {
			out.printf("			</div>%n");
			out.printf("%n");
			out.printf("		</div>%n");
			out.printf("	</section>%n");
			out.printf("%n");
			out.printf("	<section class=\"section\">%n");
			out.printf("		<div class=\"container\">%n");
			out.printf("			<h2 class=\"title\">Search</h2>%n");
			out.printf("%n");
			out.printf("			<form method=\"%s\" action=\"%s\">%n", "GET", request.getServletPath());
			out.printf("				<div class=\"field\">%n");
			out.printf("					<label class=\"label\">What are you looking for today?</label>%n");
			out.printf("					<div class=\"control has-icons-left\">%n");
			out.printf("						<input class=\"input\" type=\"text\" name=\"%s\" placeholder=\"Enter what you're searching here.\">%n", "query");
			out.printf("						<span class=\"icon is-small is-left\">%n");
			out.printf("							<i class=\"fas fa-user\"></i>%n");
			out.printf("						</span>%n");
			out.printf("					</div>%n");
			out.printf("				</div>%n");
			out.printf("%n");
			out.printf("				<div class=\"control\">%n");
			out.printf("			    <button class=\"button is-primary\" type=\"submit\">%n");
			out.printf("						<i class=\"fas fa-comment\"></i>%n");
			out.printf("						&nbsp;%n");
			out.printf("						Submit%n");
			out.printf("					</button>%n");
			out.printf("			  </div>%n");
			out.printf("			</form>%n");
			out.printf("		</div>%n");
			out.printf("	</section>%n");
		}
		log.info("GET " + request.getRequestURL().toString());

		if (request.getRequestURI().endsWith("favicon.ico")) {
			response.sendError(HttpServletResponse.SC_NOT_FOUND);
			return;
		}

		prepareResponse("Cookies!", response);

		Map<String, Cookie> cookies = getCookieMap(request);

		Cookie visitDate = cookies.get(VISIT_DATE);
		Cookie visitCount = cookies.get(VISIT_COUNT);

		out.printf("<p>");

		// Update visit count as necessary and output information.
		if (visitDate == null || visitCount == null) {
			visitCount = new Cookie(VISIT_COUNT, "0");
			visitDate = new Cookie(VISIT_DATE, "");

			out.printf("	  <div class=\"content has-text-centered\">%n");
			out.printf("	    <p>%n");
			out.printf("You have never been to this webpage before! ");
			out.printf("Thank you for visiting.");
			out.printf("	    </p>%n");
			out.printf("	  </div>%n");
		}
		else {
			try {
				// If there was anything worrisome in the value, the parseInt removes it.
				int count = Integer.parseInt(visitCount.getValue());
				visitCount.setValue(Integer.toString(count + 1));

				// We have to do extra work to make sure the String value is safe
				String decoded = URLDecoder.decode(visitDate.getValue(), StandardCharsets.UTF_8);
				String escaped = StringEscapeUtils.escapeHtml4(decoded);
				log.info("Encoded: " + visitDate.getValue() + ", Decoded: " + decoded + ", Escaped: " + escaped);
				out.printf("	  <div class=\"content has-text-centered\">%n");
				out.printf("You have visited this website %s times. ", visitCount.getValue());
				out.printf("Your last visit was on %s.", escaped);
			}
			catch (NullPointerException | IllegalArgumentException e) {
				// reset to safe values
				visitCount = new Cookie(VISIT_COUNT, "0");
				visitDate = new Cookie(VISIT_DATE, "");

				out.printf("Unable to determine if you have visited this website before. ");
			}
		}
		out.printf("</p>%n");

		// Checks if the browser indicates visits should not be tracked.
		// This is not a standard header!
		// https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/DNT
		// https://support.mozilla.org/en-US/kb/how-do-i-turn-do-not-track-feature
		// https://www.macworld.com/article/3338152/apple-safari-removing-do-not-track.html
		if (request.getIntHeader("DNT") != 1) {
			String encoded = URLEncoder.encode(getLongDate(), StandardCharsets.UTF_8);
			visitDate.setValue(encoded);
			response.addCookie(visitDate);
			response.addCookie(visitCount);
		}
		else {
			clearCookies(request, response);
			out.printf("<p>Your visits will not be tracked.</p>");
		}

		String myvar =
				"<form action=\"http://localhost:8080/config\">"+
				"    <input type=\"submit\" value=\"Clear Cookies\" />"+
				"</form>";

		out.printf("	  <div class=\"content has-text-centered\">%n");
		out.printf("	    <p>%n");
		out.printf(myvar);
		out.printf("	    </p>%n");
		out.printf("	  </div>%n");

		out.printf("	<footer class=\"footer\">%n");
		out.printf("	  <div class=\"content has-text-centered\">%n");
		out.printf("	    <p>%n");
		out.printf("	      This request was handled by thread %s.%n", Thread.currentThread().getName());
		out.printf("	    </p>%n");
		out.printf("	  </div>%n");
		out.printf("	</footer>%n");
		out.printf("</body>%n");
		out.printf("</html>%n");


		response.setStatus(HttpServletResponse.SC_OK);
	}

	/**
	 * Returns the date and time in a long format. For example: "12:00 am on
	 * Saturday, January 01 2000".
	 *
	 * @return current date and time
	 */
	private static String getDate() {
		String format = "hh:mm a 'on' EEEE, MMMM dd yyyy";
		DateFormat formatter = new SimpleDateFormat(format);
		return formatter.format(new Date());
	}
}
