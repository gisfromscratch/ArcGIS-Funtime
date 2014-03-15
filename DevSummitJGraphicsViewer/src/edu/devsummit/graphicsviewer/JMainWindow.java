package edu.devsummit.graphicsviewer;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.net.URL;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JToggleButton;
import javax.swing.JToolBar;
import javax.swing.SwingUtilities;
import javax.swing.Timer;

import com.esri.client.local.ArcGISLocalTiledLayer;
import com.esri.map.GraphicsLayer;
import com.esri.map.JMap;

import edu.devsummit.graphicsviewer.io.ReadCompletedEvent;
import edu.devsummit.graphicsviewer.io.ReaderListener;

public class JMainWindow {

	private JFrame window;
	private JMap map;
	private JToggleButton activateSelectionItem;
	
	private final JsonFileTransferHandler fileTransferHandler;
	private final Timer animationTimer;
	
	private final List<GraphicsLayer> timeLayers;
	
	private final int UPDATE_TIMEINTERVAL_SECONDS = 2;

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
		
		timeLayers = new ArrayList<>();
		
		animationTimer = new Timer(UPDATE_TIMEINTERVAL_SECONDS * 1000, new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO: Update the time extent of the layers
				System.out.println("Updating time extent..");
			}
		});
		
		// Activate drag & drop
		fileTransferHandler = new JsonFileTransferHandler(map);
		fileTransferHandler.addListener(new ReaderListener() {
			
			@Override
			public void readCompleted(ReadCompletedEvent evt) {
				GraphicsLayer timeLayer = evt.getGraphicsLayer();
				timeLayers.add(timeLayer);
				
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
			
			@Override
			public void mousePressed(MouseEvent evt) {
				if (activateSelectionItem.isSelected()) {
					if (MouseEvent.BUTTON3 == evt.getButton()) {
						// TODO: Unselect all graphics
						for (GraphicsLayer layer : timeLayers) {
							layer.setSelectionIDs(new int[0], false);
						}
						return;
					}
					
					int PixelTolerance = 5;
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
