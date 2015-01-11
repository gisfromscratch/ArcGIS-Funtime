package edu.militarysymbologyviewer;

import java.util.List;

import com.esri.core.symbol.advanced.SymbolProperties;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.ListView;
import javafx.scene.image.ImageView;

/**
 * Controller implementation for the Main View.
 *
 * @author J.T.
 *
 */
public class MilitarySymbologyController {
	
	@FXML
	private ImageView imageView;
	
	@FXML
	private ListView<SymbolProperties> listView;
	
	/**
	 * Updates the symbols shown in the list view.
	 * 
	 * @param symbolProperties a list of symbols which should be shown
	 */
	void updateSymbols(List<SymbolProperties> symbolProperties) {
		if (null == listView) {
			throw new IllegalStateException("The list view was not instantiated!");
		}
		
		ObservableList<SymbolProperties> listViewItems = FXCollections.observableList(symbolProperties);
		listView.setItems(listViewItems);
	}
}