#ifndef PlayingFairGUI_Included
#define PlayingFairGUI_Included

#include "ProblemHandler.h"
#include <memory>

class PlayingFairGUI: public ProblemHandler {
public:
    PlayingFairGUI(GWindow& window);
    void changeOccurredIn(GObservable* source) override;

protected:
    void repaint(GWindow &) override;

private:
    struct Impl;
    std::shared_ptr<Impl> mImpl;
};

#endif
