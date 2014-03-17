package edu.devsummit.graphicsviewer;

import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.File;
import java.io.IOException;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.logging.Logger;

import javax.swing.TransferHandler;

import com.esri.core.map.CallbackListener;
import com.esri.core.map.Graphic;

import edu.devsummit.graphicsviewer.io.GraphicsFileReader;
import edu.devsummit.graphicsviewer.io.ReadCompletedEvent;
import edu.devsummit.graphicsviewer.io.ReaderListener;

/**
 * Enables drop support for JSON files containing graphics instances on each line.
 * The graphics are added into a {@link List} and {@link ReaderListener#readCompleted(ReadCompletedEvent)} is called.
 */
public class JsonFileTransferHandler extends TransferHandler {
	
	private final Logger logger = Logger.getLogger(getClass().getName());
	
	private final GraphicsFileReader graphicsFileReader;
	private final Set<ReaderListener> listeners;

	public JsonFileTransferHandler() {
		graphicsFileReader = new GraphicsFileReader();
		listeners = new HashSet<>();
	}

	/**
	 * Version ID
	 */
	private static final long serialVersionUID = -1080494492463303229L;
	
	public void addListener(ReaderListener listener) {
		listeners.add(listener);
	}
	
	public void removeListener(ReaderListener listener) {
		listeners.remove(listener);
	}

	@Override
	public boolean canImport(TransferSupport support) {
		if (support.isDataFlavorSupported(DataFlavor.javaFileListFlavor)) {
			return true;
		}
		
		return false;
	}
	
	@Override
	public boolean importData(TransferSupport support) {
		if (!support.isDrop()) {
			return false;
		}
		
		Transferable transferable = support.getTransferable();
		try {
			List<File> jsonFiles = (List<File>) transferable.getTransferData(DataFlavor.javaFileListFlavor);
			for (File jsonFile : jsonFiles) {
				graphicsFileReader.readGraphicsAsync(jsonFile.getAbsolutePath(), new CallbackListener<List<Graphic>>() {
					
					@Override
					public void onError(Throwable e) {
						logger.severe(e.getMessage());
					}
					
					@Override
					public void onCallback(List<Graphic> graphics) {
						// Inform the listener
						for (ReaderListener listener : new HashSet<>(listeners)) {
							listener.readCompleted(new ReadCompletedEvent(this, graphics));
						}
					}
				});
			}
			return true;
		} catch (UnsupportedFlavorException | IOException e) {
			logger.severe(e.getMessage());
			return false;
		}
	}
}
