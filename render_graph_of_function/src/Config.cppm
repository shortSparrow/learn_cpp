export module Config;

export namespace PlotSettings {
    constexpr char PLOT_SYMBOL = '#';
    constexpr char AXIS_V_SYMBOL = '|';
    constexpr char AXIS_H_SYMBOL = '-';
    constexpr char CENTER_SYMBOL = '+';

    struct Range {
        double min;
        double max;
    };
}