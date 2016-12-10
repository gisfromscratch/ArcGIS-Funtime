package edu.militarysymbologyviewer.symbols;

import static org.junit.Assert.*;

import java.awt.image.BufferedImage;
import java.util.Arrays;
import java.util.Collection;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.junit.runners.Parameterized.Parameter;
import org.junit.runners.Parameterized.Parameters;

/**
 * Tests for the Runtime based SIC renderer.
 *
 * @author J.T.
 *
 */
@RunWith(Parameterized.class)
public class RuntimeSicRendererTests {
		
	private static RuntimeSicRenderer renderer;
	
	private static final int Width = 200;
	private static final int Height = 200;
	
	@Parameters
	public static Collection<Object[]> data() {
		return Arrays.asList(new Object[][] {
				{ "SNGPEVCTL-" },
				{ "GFOPFA----" }
		});
	}
	
	@Parameter(value = 0)
	public String sicCode;

	@BeforeClass
	public static void setUpBeforeClass() throws Exception {
		renderer = new RuntimeSicRenderer();
	}

	@AfterClass
	public static void tearDownAfterClass() throws Exception {
	}

	/**
	 * Tests the rendering of the specified SIC codes.
	 */
	@Test
	public void renderSicCodes() {
		BufferedImage symbol = renderer.render(sicCode, Width, Height);
		assertNotNull("The generated symbol must not be null!", symbol);
		
		
	}

}
