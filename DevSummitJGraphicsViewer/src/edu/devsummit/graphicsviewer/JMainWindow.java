package edu.devsummit.graphicsviewer;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.logging.Logger;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JToggleButton;
import javax.swing.JToolBar;
import javax.swing.SwingUtilities;
import javax.swing.Timer;

import com.esri.client.local.ArcGISLocalTiledLayer;
import com.esri.core.map.Graphic;
import com.esri.core.renderer.SimpleRenderer;
import com.esri.core.symbol.PictureMarkerSymbol;
import com.esri.core.symbol.SimpleMarkerSymbol;
import com.esri.core.symbol.SimpleMarkerSymbol.Style;
import com.esri.core.symbol.Symbol;
import com.esri.map.GraphicsLayer;
import com.esri.map.GraphicsLayer.MarkerRotationMode;
import com.esri.map.JMap;
import com.esri.map.GraphicsLayer.RenderingMode;

import edu.devsummit.graphicsviewer.io.ReadCompletedEvent;
import edu.devsummit.graphicsviewer.io.ReaderListener;

public class JMainWindow {
	
	private final Logger logger;

	private JFrame window;
	private JMap map;
	private JToggleButton activateSelectionItem;
	
	private final JsonFileTransferHandler fileTransferHandler;
	private final Timer animationTimer;
	
	private final List<GraphicsLayer> timeLayers;
	
	private final int UPDATE_TIMEINTERVAL_SECONDS = 5;

	public JMainWindow() {
		window = new JFrame();
		window.setTitle("GraphicsViewer");
		window.setSize(800, 600);
		window.setLocationRelativeTo(null); // center on screen
		window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		window.getContentPane().setLayout(new BorderLayout(0, 0));

		// dispose map just before application window is closed.
		window.addWindowListener(new WindowAdapter() {
			@Override
			public void windowClosing(WindowEvent windowEvent) {
				super.windowClosing(windowEvent);
				
				if (animationTimer.isRunning()) {
					animationTimer.stop();
				}
				map.dispose();
			}
		});

		// Create the map
		map = new JMap();
		map.setWrapAroundEnabled(false);
		map.setShowingEsriLogo(true);
		window.getContentPane().add(map);
		
		// Add a local basemap layer
		ArcGISLocalTiledLayer basemapLayer = new ArcGISLocalTiledLayer("/data/Basemap.tpk");
		map.getLayers().add(basemapLayer);
		
		logger = Logger.getLogger(getClass().getName());
		timeLayers = new ArrayList<>();
		animationTimer = new Timer(UPDATE_TIMEINTERVAL_SECONDS * 1000, new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO: Update the time extent of the layers
				System.out.println("Updating time extent..");
			}
		});
		
		// Activate drag & drop
		fileTransferHandler = new JsonFileTransferHandler();
		fileTransferHandler.addListener(new ReaderListener() {
			
			@Override
			public void readCompleted(ReadCompletedEvent evt) {
				// Create the symbol
				Symbol graphicSymbol;
				try {
					BufferedImage image = ImageIO.read(getClass().getResourceAsStream("/icons/ship.png"));
					graphicSymbol = new PictureMarkerSymbol(image);
				} catch (IOException e) {
					logger.severe(e.getMessage());
					
					final int SymbolSize = 5;
					graphicSymbol = new SimpleMarkerSymbol(Color.darkGray, SymbolSize, Style.DIAMOND);
				}
				
				// Create the graphics layer with a renderer
				SimpleRenderer simpleRenderer = new SimpleRenderer(graphicSymbol);
				GraphicsLayer timeLayer = new GraphicsLayer(RenderingMode.DYNAMIC);
				timeLayer.setRenderer(simpleRenderer);
				map.getLayers().add(timeLayer);
				
				// Add the graphics
				List<Graphic> graphics = evt.getGraphics();
				timeLayer.addGraphics(graphics.toArray(new Graphic[graphics.size()]));
				timeLayers.add(timeLayer);
				
				// Start the time animation
				if (!animationTimer.isRunning()) {
					animationTimer.start();
				}
			}
		});
		map.setTransferHandler(fileTransferHandler);
		
		// Activate selection
		JToolBar mainToolbar = new JToolBar();
		URL imageUrl = getClass().getResource("/icons/edit-select.png");
		if (null != imageUrl) {
			activateSelectionItem = new JToggleButton(new ImageIcon(imageUrl));
			mainToolbar.add(activateSelectionItem);
			window.getContentPane().add(mainToolbar, BorderLayout.PAGE_START);
		}
		
		map.addMouseListener(new MouseAdapter() {
			
			private final int[] emptyIdList = new int[0];
			private final int PixelTolerance = 5;
			
			@Override
			public void mousePressed(MouseEvent evt) {
				if (activateSelectionItem.isSelected()) {
					if (MouseEvent.BUTTON3 == evt.getButton()) {
						// Unselect all graphics
						for (GraphicsLayer layer : timeLayers) {
							layer.setSelectionIDs(emptyIdList, false);
						}
						return;
					}
					
					for (GraphicsLayer layer : timeLayers) {
						int graphicsIds[] = layer.getGraphicIDs(evt.getX(), evt.getY(), PixelTolerance);
						for (int graphicId : graphicsIds) {
							if (layer.isGraphicSelected(graphicId)) {
								layer.unselect(graphicId);
							} else {
								layer.select(graphicId);
							}
						}
					}
				}
			}
		});
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		SwingUtilities.invokeLater(new Runnable() {

			@Override
			public void run() {
				try {
					JMainWindow application = new JMainWindow();
					application.window.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}
}
