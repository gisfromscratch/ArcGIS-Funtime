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
class SymbolCell extends ListCell<SymbolProperties> {

	private final int ImageWidth = 100;
	
	private final int ImageHeight = 100;
	
	/**
	 * Renders the symbol as image.
	 * 
	 * @param item the symbol which should be rendered
	 * @param width the width of the rendered image
	 * @param height the height of the rendered image
	 * @return A new rendered image of the symbol.
	 */
	WritableImage renderSymbol(SymbolProperties item, int width, int height) {
		BufferedImage symbolImage = item.getImage(width, height);
		return SwingFXUtils.toFXImage(symbolImage, null);
	}
	
	/**
	 * Updates the cell item by displaying the specified symbol.
	 * 
	 * @param item the symbol item which should be displayed
	 * @param empty a flag whether or not the item should be treat as empty
	 */
	@Override
	protected void updateItem(SymbolProperties item, boolean empty) {
		super.updateItem(item, empty);
		
		if (null == item) {
			return;
		}
		
		// Renders the image
		WritableImage nodeImage = renderSymbol(item, ImageWidth, ImageHeight);
		ImageView symbolView = new ImageView(nodeImage);
		setGraphic(symbolView);
	}
}
