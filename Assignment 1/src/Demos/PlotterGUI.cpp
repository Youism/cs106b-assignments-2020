#include "ProblemHandler.h"
#include "Plotter.h"
#include "TemporaryComponent.h"
#include "GUIUtils.h"
#include "gobjects.h"
#include "error.h"
#include "filelib.h"
#include "ginteractors.h"
#include <memory>
#include <vector>
using namespace std;

namespace {
    /* Padding from window border to content area. */
    const double kPadding = 20;

    /* File suffix for plotter files. */
    const string kFileSuffix = ".plot";

    /* "Not selected" option. */
    const string kNotSelected = "-";

    /* Text for load button. */
    const string kLoadButtonText = "Plot!";

    /* Window background color - used for everything but the canvas. */
    const string kWindowColor = "#A4DDED"; // Non-Photo Blue: dark enough to clearly make a
                                           // border, not so dark that lines on the border
                                           // won't be seen

    /* Canvas color. */
    const string kCanvasColor = "#FFFFFF"; // Pure white, for simplicity

    class PlotterGUI: public ProblemHandler {
    public:
        PlotterGUI(GWindow& window);
        ~PlotterGUI();

        void addLine(double x0, double y0, double x1, double y1, PenStyle style);
        void actionPerformed(GObservable* obj) override;

    protected:
        void repaint(GWindow& window) override;

    private:
        /* Geometry. */
        double baseX, baseY, scale;

        /* Lines to plot. */
        vector<unique_ptr<GLine>> lines;

        /* Two-row grid of items. The layout looks like this
         *
         *                     Dropdown   Load
         *    ------------------- status line ------------------------
         */
        Temporary<GContainer> container;

        /* Load file. */
        GComboBox*  fileChooser;
        GButton*    loadButton;

        /* Status reporting. */
        GLabel*     statusLine;

        /* Initialization helpers. */
        void calculateGeometry(GWindow& window);
        void setUpChrome(GWindow& window);

        /* Resets graphics state. */
        void clearGraphics();
    };

    /* Returns all plotter files found in the example directory. */
    vector<string> sampleProblems() {
        vector<string> result;
        for (const auto& file: listDirectory(".")) {
            if (endsWith(file, kFileSuffix)) {
                result.push_back(file);
            }
        }
        return result;
    }

    /* Ewww - global variables! */
    PlotterGUI* theGUI = nullptr;

    /* Constructor sets up graphics and hooks us in as the One True GUI. */
    PlotterGUI::PlotterGUI(GWindow& window) {
        if (theGUI) error("Why are there two copies of us?");
        theGUI = this;

        setUpChrome(window);
        calculateGeometry(window);
    }

    PlotterGUI::~PlotterGUI() {
        theGUI = nullptr;
    }

    /* Hook from the global graphics system to drawing a line. */
    void PlotterGUI::addLine(double x0, double y0, double x1, double y1, PenStyle style) {
        /* TODO: Once C++14 support is everywhere, use make_unique. */
        unique_ptr<GLine> line(new GLine(x0 * scale + baseX, -y0 * scale + baseY,
                                         x1 * scale + baseX, -y1 * scale + baseY));
        line->setColor(style.color);
        line->setLineWidth(style.width);
        lines.push_back(move(line));

        requestRepaint();
    }

    /* Calculates window geometry information - scale, base X, base Y, etc. */
    void PlotterGUI::calculateGeometry(GWindow& window) {
        /* Computing the scaling factors needed to scale to the window width and window height. */
        double width  = window.getCanvasWidth()  - 2 * kPadding;
        double height = window.getCanvasHeight() - 2 * kPadding;

        scale = min(width, height) / 2.0;

        /* Compute base x and y based on the scale. */
        baseX = kPadding + width / 2.0;
        baseY = kPadding + height / 2.0;
    }

    /* Sets up window controls ("chrome"). */
    void PlotterGUI::setUpChrome(GWindow& window) {
        auto* rawContainer = new GContainer(GContainer::LAYOUT_GRID);

        fileChooser = new GComboBox();
        fileChooser->addItem(kNotSelected); // Initially, nothing shows
        for (const string& file: sampleProblems()) {
            fileChooser->addItem(file);
        }
        fileChooser->setEditable(false);

        loadButton  = new GButton(kLoadButtonText);
        statusLine  = new GLabel("");

        rawContainer->addToGrid(fileChooser, 0, 0);
        rawContainer->addToGrid(loadButton,  0, 1);
        rawContainer->addToGrid(statusLine,  1, 0, 1, 2);

        container = Temporary<GContainer>(rawContainer, window, "SOUTH");
    }

    void PlotterGUI::repaint(GWindow& window) {
        /* We have to draw the border on top of the lines in case the plotter
         * goes out of bounds!
         */
        clearDisplay(window, kCanvasColor);

        for (const auto& line: lines) {
            window.draw(*line);
        }

        /* Draw above, to the left of, to the right of, and below the canvas. */
        window.setColor(kWindowColor);
        window.fillRect(0, 0, baseX - scale, window.getHeight()); // Left
        window.fillRect(baseX + scale, 0, window.getWidth() - baseX - scale, window.getHeight()); // Right
        window.fillRect(0, 0, window.getWidth(), baseY - scale);
        window.fillRect(0, baseY + scale, window.getWidth(), window.getHeight() - baseY - scale);
    }

    void PlotterGUI::actionPerformed(GObservable* obj) {
        /* Was it the load button? If so, load things. */
        if (obj == loadButton) {
            string toLoad = fileChooser->getSelectedItem();
            if (toLoad != kNotSelected) {
                clearGraphics();

                ifstream input(toLoad);
                runPlotterScript(input);
            }
        }
    }

    void PlotterGUI::clearGraphics() {
        lines.clear();
        requestRepaint();
    }
}

void drawLine(double x0, double y0, double x1, double y1, PenStyle style) {
    if (theGUI) {
        theGUI->addLine(x0, y0, x1, y1, style);
    }
}

shared_ptr<ProblemHandler> makePlotterGUI(GWindow& window) {
    return make_shared<PlotterGUI>(window);
}
