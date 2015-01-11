package edu.militarysymbologyviewer.symbols;

import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.HashMap;
import java.util.List;

import com.esri.core.symbol.advanced.SymbolDictionary;
import com.esri.core.symbol.advanced.SymbolDictionary.DictionaryType;
import com.esri.core.symbol.advanced.SymbolProperties;

/**
 * Rendering of SIC codes using Runtime technology.
 *
 * @author J.T.
 *
 */
public class RuntimeSicRenderer implements SicRenderer {
	
	private final SymbolDictionary symbolDictionary;
	
	private final String SymbolIdName = "SymbolID";
	
	private final HashMap<String, SymbolProperties> symbolProperties;

	/**
	 * Creates a new renderer instance.
	 * The internal dictionary is accessed and the symbol properties are obtained.
	 */
	public RuntimeSicRenderer() {
		symbolDictionary = new SymbolDictionary(DictionaryType.Mil2525C);
		List<SymbolProperties> symbolEntries = null;
		try {
			symbolEntries = symbolDictionary.findSymbols();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		if (null != symbolEntries) {
			symbolProperties = new HashMap<String, SymbolProperties>(symbolEntries.size());
			for (SymbolProperties symbolEntry : symbolEntries) {
				HashMap<String, String> symbolEntryValues = symbolEntry.getValues();
				if (null != symbolEntryValues && symbolEntryValues.containsKey(SymbolIdName)) {
					String sic = symbolEntryValues.get(SymbolIdName);
					if (null != sic && 0 < sic.length()) {
						symbolProperties.put(sic, symbolEntry);
					}
				}
			}
		} else {
			symbolProperties = new HashMap<String, SymbolProperties>();
		}
	}

	@Override
	public BufferedImage render(String sic, int width, int height) {
		if (symbolProperties.containsKey(sic)) {
			return symbolProperties.get(sic).getImage(width, height);
		}
		
		return null;
	}
}
