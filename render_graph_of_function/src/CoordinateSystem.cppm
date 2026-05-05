export module CoordinateSystem;
import Canvas;
import Config;

export void drawAxes(Canvas& canvas, int width, int height) {
    int midY = height / 2;
    int midX = width / 2;

    for (int x = 0; x < width; ++x) {
        canvas.setCustomPoint(x, midY, PlotSettings::AXIS_H_SYMBOL);
    }
    for (int y = 0; y < height; ++y) {
        canvas.setCustomPoint(midX, y, PlotSettings::AXIS_V_SYMBOL);
    }
    canvas.setCustomPoint(midX, midY, PlotSettings::CENTER_SYMBOL);
}