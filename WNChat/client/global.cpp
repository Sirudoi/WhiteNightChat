#include "global.h"

std::function<void(QWidget*)> g_repolish = [](QWidget* w){
    // repolish qss setting to current QWidget
    w->style()->unpolish(w);
    w->style()->polish(w);
};
