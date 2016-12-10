package edu.militarysymbologyviewer;

import java.awt.image.BufferedImage;

import com.esri.core.symbol.advanced.SymbolProperties;

import javafx.scene.control.ListCell;
import javafx.scene.control.ListView;
import javafx.util.Callback;

/**
 * Cell factory for displaying symbol images.
 *
 * @author J.T.
 *
 */
class SymbolCellFactory {

	/**
	 * Creates a callback for the rendering of the cells.
	 * 
	 * @return Callback for the rendering of the cells. 
	 */
	public Callback<ListView<SymbolProperties>, ListCell<SymbolProperties>> createCallback() {
		return new Callback<ListView<SymbolProperties>, ListCell<SymbolProperties>>() {

			/**
			 * Creates a new cell displaying a symbol.
			 * 
			 * @param listView the list view showing the symbols.
			 * @return A new cell displaying a symbol.
			 */
			@Override
			public ListCell<SymbolProperties> call(ListView<SymbolProperties> listView) {
				return new SymbolCell();
			}
			
		};
	}
}
