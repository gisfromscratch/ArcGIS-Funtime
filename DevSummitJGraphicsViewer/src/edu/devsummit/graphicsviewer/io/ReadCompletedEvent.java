package edu.devsummit.graphicsviewer.io;

import java.util.EventObject;

import com.esri.map.GraphicsLayer;

public class ReadCompletedEvent extends EventObject {

	private static final long serialVersionUID = 5073068860345014333L;
	
	private final GraphicsLayer graphicsLayer;
	
	public ReadCompletedEvent(Object sender, GraphicsLayer graphicsLayer) {
		super(sender);
		this.graphicsLayer = graphicsLayer;
	}

	public GraphicsLayer getGraphicsLayer() {
		return graphicsLayer;
	}
}
