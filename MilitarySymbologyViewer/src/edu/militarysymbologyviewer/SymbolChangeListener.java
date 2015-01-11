package edu.militarysymbologyviewer;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;

import com.esri.core.symbol.advanced.SymbolProperties;

/**
 * Updates an image view control when a symbol change event was raised.
 *
 * @author J.T.
 *
 */
class SymbolChangeListener implements ChangeListener<SymbolProperties> {

	private final SymbolView symbolView;
	
	/**
	 * Creates a new change listener instance.
	 * 
	 * @param symbolView the symbol view displaying a symbol.
	 */
	SymbolChangeListener(SymbolView symbolView) {
		this.symbolView = symbolView;
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
			symbolView.updateSymbol(newItem);
			return;
		}
				
		symbolView.updateSymbol(null);
	}

}
