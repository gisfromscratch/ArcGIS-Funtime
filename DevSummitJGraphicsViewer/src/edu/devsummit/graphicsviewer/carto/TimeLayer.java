package edu.devsummit.graphicsviewer.carto;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.logging.Logger;

import com.esri.core.map.Graphic;
import com.esri.core.map.TimeExtent;
import com.esri.map.GraphicsLayer;

/**
 * Holds reference to a graphics layer and offers time restriction on the managed graphics.
 */
public class TimeLayer {
	
	private final Logger logger;

	private final GraphicsLayer graphicsLayer;
	
	private final TimeFieldInfo timeInfo;
	
	private final SimpleDateFormat simpleDateFormat;
	
	private TimeExtent timeExtent;
	
	private boolean queriedTimeExtent;
	
	public TimeLayer(GraphicsLayer graphicsLayer, TimeFieldInfo timeInfo, String dateTimeFormat) {
		logger = Logger.getLogger(getClass().getName());
		this.graphicsLayer = graphicsLayer;
		this.timeInfo = timeInfo;
		this.simpleDateFormat = new SimpleDateFormat(dateTimeFormat);
	}
	
	/**
	 * Time extent of the underlying graphics layer.
	 */
	public TimeExtent getTimeExtent() {
		if (queriedTimeExtent) {
			return timeExtent;
		}
		
		timeExtent = queryTimeExtent();
		queriedTimeExtent = true;
		return timeExtent;
	}
	
	private TimeExtent queryTimeExtent() {
		Calendar invalidCalendar = Calendar.getInstance();
		TimeExtent timeExtent = new TimeExtent(invalidCalendar);
		timeExtent.setStartDate(invalidCalendar);
		timeExtent.setEndDate(invalidCalendar);
		
		String startTimeField = timeInfo.getStartTimeField();
		String endTimeField = timeInfo.getEndTimeField();
		if (startTimeField.isEmpty() && endTimeField.isEmpty()) {
			logger.warning("No start or end time field defined!");
			return timeExtent;
		}
		
		int[] graphicIds = graphicsLayer.getGraphicIDs();
		for (int graphicId : graphicIds) {
			Graphic graphic = graphicsLayer.getGraphic(graphicId);
			if (!startTimeField.isEmpty()) {
				Object startTimeAttributeValue = graphic.getAttributeValue(startTimeField);
				try {
					Calendar startDate = Calendar.getInstance();
					startDate.setTime(simpleDateFormat.parse(String.valueOf(startTimeAttributeValue)));
					if (timeExtent.getStartDate().equals(invalidCalendar) || startDate.before(timeExtent.getStartDate())) {
						timeExtent.setStartDate(startDate);
					}
					
					// Use start time field as end time field
					if (endTimeField.isEmpty()) {
						if (timeExtent.getEndDate().equals(invalidCalendar) || timeExtent.getEndDate().before(startDate)) {
							timeExtent.setEndDate(startDate);
						}
					}
				} catch (ParseException e) {
					logger.severe(e.getMessage());
				}
			}
			
			if (!endTimeField.isEmpty()) {
				Object endTimeAttributeValue = graphic.getAttributeValue(endTimeField);
				try {
					Calendar endDate = Calendar.getInstance();
					endDate.setTime(simpleDateFormat.parse(String.valueOf(endTimeAttributeValue)));
					if (timeExtent.getEndDate().equals(invalidCalendar) || timeExtent.getEndDate().before(endDate)) {
						timeExtent.setEndDate(endDate);
					}
				} catch (ParseException e) {
					logger.severe(e.getMessage());
				}
			}
		}
		return timeExtent;
	}
}
