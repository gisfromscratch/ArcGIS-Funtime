package edu.devsummit.graphicsviewer.carto;

public class SimpleTimeFieldInfo implements TimeFieldInfo {
	
	private final String startTimeField;
	
	private final String endTimeField;
	
	public SimpleTimeFieldInfo(String startTimeField, String endTimeField) {
		if (null == startTimeField) {
			throw new IllegalArgumentException("startTimeField must not been null!");
		}
		if (null == endTimeField) {
			throw new IllegalArgumentException("endTimeField must not been null!");
		}
		this.startTimeField = startTimeField;
		this.endTimeField = endTimeField;
	}

	@Override
	public String getStartTimeField() {
		return startTimeField;
	}

	@Override
	public String getEndTimeField() {
		return endTimeField;
	}
}
