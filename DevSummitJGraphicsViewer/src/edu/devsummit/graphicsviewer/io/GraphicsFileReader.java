package edu.devsummit.graphicsviewer.io;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.LineNumberReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.logging.Logger;

import javax.swing.SwingUtilities;

import org.codehaus.jackson.JsonFactory;
import org.codehaus.jackson.JsonNode;
import org.codehaus.jackson.JsonParser;
import org.codehaus.jackson.map.ObjectMapper;
import org.codehaus.jackson.type.TypeReference;

import com.esri.core.geometry.Point;
import com.esri.core.map.CallbackListener;
import com.esri.core.map.Graphic;

/**
 * Reads graphics asynchronously from a JSON file.
 */
public class GraphicsFileReader {

	private final Logger logger = Logger.getLogger(getClass().getName());
	private final JsonFactory jsonFactory;
	private final ObjectMapper jsonMapper;

	private final ExecutorService executorService;
	private final AtomicBoolean cancel = new AtomicBoolean(false);

	public GraphicsFileReader() {
		executorService = Executors.newCachedThreadPool();
		jsonFactory = new JsonFactory();
		jsonMapper = new ObjectMapper(jsonFactory);
	}
	
	/**
	 * Cancels all current read operations.
	 */
	public void cancel() {
		cancel.set(true);
	}

	/**
	 * Reads graphics asynchronously from a JSON file.
	 * @param filePath	The path to the JSON file.
	 * @param callback	The callback which gets called from the UI thread after the read operation is completed.
	 */
	public void readGraphicsAsync(final String filePath, final CallbackListener<List<Graphic>> callback) {
		executorService.execute(new Runnable() {

			@Override
			public void run() {
				readGraphics(filePath, callback);
			}
		});
	}

	private void readGraphics(String filePath, final CallbackListener<List<Graphic>> callback) {
		File jsonFile = new File(filePath);
		if (!jsonFile.canRead()) {
			logger.warning(String.format("%s open failed", jsonFile.getAbsolutePath()));
			return;
		}

		logger.fine(String.format("Reading %s", jsonFile.getAbsolutePath()));

		final List<Graphic> graphics = new ArrayList<Graphic>();
		try {
			FileReader reader = new FileReader(jsonFile);
			LineNumberReader lineReader = new LineNumberReader(reader);
			try {
				String graphicAsJson;
				while (null != (graphicAsJson = lineReader.readLine()) && !cancel.get()) {
					JsonParser parser = jsonFactory.createJsonParser(graphicAsJson);
					try {
						HashMap<String, Object> graphicAsMap = jsonMapper.readValue(graphicAsJson, HashMap.class);
						HashMap<String, Object> attributesAsMap = (HashMap<String, Object>) graphicAsMap.get("attributes");
						HashMap<String, Object> geometryAsMap = (HashMap<String, Object>) graphicAsMap.get("geometry");
						double x = Double.parseDouble(geometryAsMap.get("x").toString());
						double y = Double.parseDouble(geometryAsMap.get("y").toString());
						Graphic graphic = new Graphic(new Point(x, y), null, attributesAsMap);
						graphics.add(graphic);
						
//						Graphic graphic = Graphic.fromJson(parser);
//						if (null != graphic)
//						{
//							graphics.add(graphic);
//						}
//						else
//						{
//							logger.warning("Graphic is null!");
//						}
					} catch (Exception e) {
						logger.severe(e.getMessage());
					}
				}
			} finally {
				lineReader.close();
			}

			SwingUtilities.invokeLater(new Runnable() {

				@Override
				public void run() {
					if (!cancel.get()) {
						callback.onCallback(graphics);
					}
				}
			});
		} catch (IOException e) {
			logger.severe(e.getMessage());
		}

	}
}
