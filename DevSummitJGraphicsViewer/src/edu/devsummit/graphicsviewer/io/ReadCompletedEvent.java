package edu.devsummit.graphicsviewer.io;

import java.util.EventObject;

import com.esri.map.GraphicsLayer;

/**
 * Read completed event with a reference to a graphics layer.
 */
public class ReadCompletedEvent extends EventObject {

	private static final long serialVersionUID = 5073068860345014333L;
	
	private final GraphicsLayer graphicsLayer;
	
	public ReadCompletedEvent(Object sender, GraphicsLayer graphicsLayer) {
		super(sender);
		this.graphicsLayer = graphicsLayer;
	}

	/**
	 * The graphics layer were the graphics were added.
	 */
	public GraphicsLayer getGraphicsLayer() {
		return graphicsLayer;
	}
}
