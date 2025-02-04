package edu.militarysymbologyviewer;

import java.util.List;

import com.esri.core.symbol.advanced.SymbolDictionary;
import com.esri.core.symbol.advanced.SymbolDictionary.DictionaryType;
import com.esri.core.symbol.advanced.SymbolProperties;

import javafx.application.Application;
import javafx.stage.Stage;
import javafx.scene.Scene;
import javafx.scene.layout.BorderPane;
import javafx.fxml.FXMLLoader;

/**
 * Main application of the Military Symbology Viewer.
 *
 * @author J.T.
 *
 */
public class Main extends Application {
	
	/**
	 * Shows the main stage.
	 * 
	 * @param primaryStage the primary stage of the application.
	 */
	@Override
	public void start(Stage primaryStage) {
		try {
			primaryStage.setTitle("Military Symbology Viewer");
			
			FXMLLoader loader = new FXMLLoader(getClass().getResource("MilitarySymbologyView.fxml"));
			BorderPane root = (BorderPane) loader.load();
			
			MilitarySymbologyController controller = loader.getController();
			
			// Initializes the cell factory
			controller.initCellFactory();
			
			// Add all symbols from the dictionary
			SymbolDictionary symbolDictionary = new SymbolDictionary(DictionaryType.Mil2525C);
			List<SymbolProperties> symbolProperties = symbolDictionary.findSymbols();
			controller.updateSymbols(symbolProperties);
			
			// Register a selection listener
			controller.registerSelectionListener();
			
			// Displays the main stage in a scene
			Scene scene = new Scene(root, 600, 600);
			primaryStage.setMinHeight(600);
			primaryStage.setMinWidth(600);
			scene.getStylesheets().add(getClass().getResource("application.css").toExternalForm());
			primaryStage.setScene(scene);
			primaryStage.show();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * Starts the main application.
	 * 
	 * @param args the application parameters.
	 */
	public static void main(String[] args) {
		launch(args);
	}
}
