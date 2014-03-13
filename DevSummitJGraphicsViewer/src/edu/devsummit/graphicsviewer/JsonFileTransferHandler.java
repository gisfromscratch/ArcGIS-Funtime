package edu.devsummit.graphicsviewer;

import java.awt.Image;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.image.BufferedImage;
import java.io.BufferedInputStream;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Logger;

import javax.imageio.ImageIO;
import javax.swing.TransferHandler;

import com.esri.core.map.CallbackListener;
import com.esri.core.map.Graphic;
import com.esri.core.renderer.SimpleRenderer;
import com.esri.core.symbol.PictureMarkerSymbol;
import com.esri.map.GraphicsLayer;
import com.esri.map.GraphicsLayer.RenderingMode;
import com.esri.map.JMap;

import edu.devsummit.graphicsviewer.io.GraphicsFileReader;

public class JsonFileTransferHandler extends TransferHandler {
	
	private final Logger logger = Logger.getLogger(getClass().getName());
	
	private final JMap map;
	private final GraphicsFileReader graphicsFileReader;

	public JsonFileTransferHandler(JMap map) {
		this.map = map;
		graphicsFileReader = new GraphicsFileReader();
	}


	/**
	 * Version ID
	 */
	private static final long serialVersionUID = -1080494492463303229L;

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
						try {
							BufferedImage image = ImageIO.read(getClass().getResourceAsStream("/icons/ship.png"));
							PictureMarkerSymbol pictureMarkerSymbol = new PictureMarkerSymbol(image);
							SimpleRenderer simpleRenderer = new SimpleRenderer(pictureMarkerSymbol);
							GraphicsLayer graphicsLayer = new GraphicsLayer(RenderingMode.DYNAMIC);
							graphicsLayer.setRenderer(simpleRenderer);
							map.getLayers().add(graphicsLayer);
							
							graphicsLayer.addGraphics(graphics.toArray(new Graphic[graphics.size()]));
						} catch (IOException e) {
							logger.severe(e.getMessage());
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
