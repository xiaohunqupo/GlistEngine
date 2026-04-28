/*
 * gGUIBoxPlot.h
 *
 *  Created on: Ap 27, 2026
 *      Author: ahmet celal
 */

#ifndef UI_GGUIBOXPLOT_H_
#define UI_GGUIBOXPLOT_H_

#include "gGUIControl.h"
#include <vector>
#include <string>

class gGUIBoxPlot : public gGUIControl {
public:
    gGUIBoxPlot();
    virtual ~gGUIBoxPlot();

    // Standard GUI lifecycle
    void set(gBaseApp* root,
             gBaseGUIObject* topParentGUIObject,
             gBaseGUIObject* parentGUIObject,
             int parentSlotLineNo,
             int parentSlotColumnNo,
             int x, int y, int w, int h) override;

    void draw() override;
    void update() override;

    // Data
    void setData(const std::vector<float>& values);
    void clearData();

    // Options
    void setShowOutliers(bool show);
    bool getShowOutliers() const;

    void setTitle(const std::string& t);
    const std::string& getTitle() const;

    // Styling (optional)
    void setBoxWidth(int w);
    void setMarkerSize(int px);

private:
    // Stats
    bool computeStats(); // returns false if not enough data
    static float medianSorted(const std::vector<float>& sorted, int l, int rExclusive);

    // Drawing helpers
    int yToPix(float y) const;
    void rebuildYScale();
    void drawOutlierMarker(int x, int y) const;

private:
    std::vector<float> raw;
    std::vector<float> sorted;
    std::vector<float> outliers;

    bool showOutliers = true;
    std::string title = "Box Plot";

    // computed
    bool hasStats = false;
    float q1 = 0.0f, med = 0.0f, q3 = 0.0f;
    float iqr = 0.0f;
    float whiskLow = 0.0f, whiskHigh = 0.0f;
    float minY = 0.0f, maxY = 1.0f;

    // layout
    int padLeft = 60;
    int padRight = 40;
    int padTop = 60;
    int padBottom = 60;

    int boxWidth = 160;
    int markerSize = 6;

    // cache
    int chartX = 0, chartY = 0, chartW = 0, chartH = 0;
};

#endif /* UI_GGUIBOXPLOT_H_ */
