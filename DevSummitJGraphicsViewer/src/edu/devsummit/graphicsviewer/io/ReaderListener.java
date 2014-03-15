package edu.devsummit.graphicsviewer.io;

import java.util.EventListener;

public interface ReaderListener extends EventListener {

	void readCompleted(ReadCompletedEvent evt);
}
