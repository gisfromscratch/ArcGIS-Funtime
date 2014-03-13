package edu.devsummit.graphicsviewer;

import java.awt.BorderLayout;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;
import javax.swing.TransferHandler;

import com.esri.client.local.ArcGISLocalTiledLayer;
import com.esri.map.JMap;

public class JMainWindow {

	private JFrame window;
	private JMap map;

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
		
		map.setTransferHandler(new JsonFileTransferHandler(map));
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
