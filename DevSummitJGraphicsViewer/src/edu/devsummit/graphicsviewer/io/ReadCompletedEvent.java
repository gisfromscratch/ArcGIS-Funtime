package edu.devsummit.graphicsviewer.io;

import java.util.EventObject;
import java.util.List;

import com.esri.core.map.Graphic;

/**
 * Read completed event with a reference to a list of graphics.
 */
public class ReadCompletedEvent extends EventObject {

	private static final long serialVersionUID = 5073068860345014333L;
	
	private final List<Graphic> graphics;
	
	public ReadCompletedEvent(Object sender, List<Graphic> graphics) {
		super(sender);
		this.graphics = graphics;
	}

	/**
	 * The graphics that could be read.
	 */
	public List<Graphic> getGraphics() {
		return graphics;
	}
}
