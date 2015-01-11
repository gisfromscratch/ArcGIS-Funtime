package edu.militarysymbologyviewer;

import com.esri.core.symbol.advanced.SymbolProperties;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.scene.image.ImageView;
import javafx.scene.image.WritableImage;

/**
 * Updates an image view control when a symbol change event was raised.
 *
 * @author J.T.
 *
 */
class SymbolChangeListener implements ChangeListener<SymbolProperties> {

	private final ImageView imageView;
	
	private final int ImageWidth = 400;
	
	private final int ImageHeight = 400;
	
	/**
	 * Creates a new change listener instance.
	 * 
	 * @param imageView the image view displaying a symbol.
	 */
	SymbolChangeListener(ImageView imageView) {
		this.imageView = imageView;
	}
	
	/**
	 * Occurs when the selected symbol changed and updates the underlying image view with the new symbol.
	 * 
	 * @param observedItem the observed symbol
	 * @param oldItem the last selected symbol
	 * @param newItem the new selected symbol 
	 */
	@Override
	public void changed(ObservableValue<? extends SymbolProperties> observedItem, SymbolProperties oldItem, SymbolProperties newItem) {
		if (null != newItem) {
			SymbolCell symbolCell = new SymbolCell();
			WritableImage nodeImage = symbolCell.renderSymbol(newItem, ImageWidth, ImageHeight);
			imageView.setImage(nodeImage);
			return;
		}
				
		imageView.setImage(null);
	}

}
