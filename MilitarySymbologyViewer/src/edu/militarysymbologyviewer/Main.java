package edu.militarysymbologyviewer;

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
			BorderPane root = (BorderPane) FXMLLoader.load(getClass().getResource("MilitarySymbologyView.fxml"));
			Scene scene = new Scene(root, 400, 400);
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
