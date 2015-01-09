package edu.militarysymbologyviewer.symbols;

import java.awt.image.BufferedImage;

/**
 * Offers rendering by specifying the SIC code.
 * 
 * @author J.T.
 *
 */
public interface SicRenderer {

	/**
	 * Generates a buffered image using the specified SIC.
	 * 
	 * @param sic the SIC code.
	 * @return A buffered image.
	 */
	public BufferedImage render(String sic);
}
