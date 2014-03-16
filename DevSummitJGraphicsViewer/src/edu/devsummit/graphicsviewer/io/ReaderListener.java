package edu.devsummit.graphicsviewer.io;

import java.util.EventListener;

/**
 * Event listener for the {@link ReadCompletedEvent}. 
 */
public interface ReaderListener extends EventListener {

	void readCompleted(ReadCompletedEvent evt);
}
