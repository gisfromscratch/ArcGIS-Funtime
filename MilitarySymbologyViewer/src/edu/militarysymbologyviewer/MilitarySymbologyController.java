package edu.militarysymbologyviewer;

import java.util.List;

import com.esri.core.symbol.advanced.SymbolProperties;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.ListView;
import javafx.scene.control.TextArea;
import javafx.scene.image.ImageView;

/**
 * Controller implementation for the Main View.
 *
 * @author J.T.
 *
 */
public class MilitarySymbologyController {
	
	@FXML
	private TextArea textArea;
	
	@FXML
	private ImageView imageView;
	
	@FXML
	private ListView<SymbolProperties> listView;
	
	/**
	 * Initializes the cell factory for displaying symbols.
	 */
	void initCellFactory() {
		if (null == listView) {
			throw new IllegalStateException("The list view was not instantiated!");
		}
		
		SymbolCellFactory cellFactory = new SymbolCellFactory();
		listView.setCellFactory(cellFactory.createCallback());
	}
	
	/**
	 * Registers a selection listener for the list view.
	 * When the selection changes the symbol of the image view is updated.
	 */
	void registerSelectionListener() {
		if (null == listView) {
			throw new IllegalStateException("The list view was not instantiated!");
		}
		if (null == imageView) {
			throw new IllegalStateException("The image view was not instantiated!");
		}
		if (null == textArea) {
			throw new IllegalStateException("The text area was not instantiated!");
		}
		
		SymbolView symbolView = new SymbolView(imageView, textArea);
		listView.getSelectionModel().selectedItemProperty().addListener(new SymbolChangeListener(symbolView));
	}
	
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