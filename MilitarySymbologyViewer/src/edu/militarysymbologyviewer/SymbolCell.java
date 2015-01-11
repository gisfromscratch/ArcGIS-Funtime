package edu.militarysymbologyviewer;

import java.awt.image.BufferedImage;

import javafx.embed.swing.SwingFXUtils;
import javafx.scene.control.ListCell;
import javafx.scene.image.ImageView;
import javafx.scene.image.WritableImage;

import com.esri.core.symbol.advanced.SymbolProperties;

/**
 * Cell displaying a symbol image.
 *
 * @author J.T.
 *
 */
public class SymbolCell extends ListCell<SymbolProperties> {

	private final int ImageWidth = 100;
	
	private final int ImageHeight = 100;
	
	@Override
	protected void updateItem(SymbolProperties item, boolean empty) {
		super.updateItem(item, empty);
		
		if (null == item) {
			return;
		}
		
		// Renders the image
		BufferedImage symbolImage = item.getImage(ImageWidth, ImageHeight);
		WritableImage nodeImage = SwingFXUtils.toFXImage(symbolImage, null);
		ImageView symbolView = new ImageView(nodeImage);
		setGraphic(symbolView);
	}
}
