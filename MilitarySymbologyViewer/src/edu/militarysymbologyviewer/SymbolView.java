package edu.militarysymbologyviewer;

import java.util.HashMap;

import com.esri.core.symbol.advanced.SymbolProperties;

import javafx.scene.control.TextArea;
import javafx.scene.image.ImageView;
import javafx.scene.image.WritableImage;

/**
 * A view consisting of a image view and a text area.
 *
 * @author J.T.
 *
 */
class SymbolView {

	private final ImageView imageView;
	
	private final TextArea textArea;
	
	private final int ImageWidth = 400;
	
	private final int ImageHeight = 400;
	
	private final String SymbolIdName = "SymbolID";

	/**
	 * Creates a new symbol view instance.
	 * 
	 * @param imageView the image view displaying the symbol.
	 * @param textArea the text area displaying the symbol properties.
	 */
	SymbolView(ImageView imageView, TextArea textArea) {
		this.imageView = imageView;
		this.textArea = textArea;
	}
	
	/**
	 * Updates the symbol of this view.
	 * 
	 * @param symbolProperties The symbol which should be displayed.
	 */
	void updateSymbol(SymbolProperties symbolProperties) {
		if (null == symbolProperties) {
			imageView.setImage(null);
			textArea.setText("");
		}
		
		SymbolCell symbolCell = new SymbolCell();
		WritableImage nodeImage = symbolCell.renderSymbol(symbolProperties, ImageWidth, ImageHeight);
		imageView.setImage(nodeImage);
		
		HashMap<String, String> symbolValues = symbolProperties.getValues();
		if (null != symbolValues && symbolValues.containsKey(SymbolIdName)) {
			String sic = symbolValues.get(SymbolIdName);
			textArea.setText(sic);
		}
	}
}
