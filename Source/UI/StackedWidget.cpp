#include "StackedWidget.h"

QSize StackedWidget::sizeHint() const
{
    if (currentWidget()) {
        return currentWidget()->sizeHint();
    }
    return QSize(1000, 1000);
}

QSize StackedWidget::minimumSizeHint() const
{
    if (currentWidget()) {
        return currentWidget()->minimumSizeHint();
    }
    return QSize(1000, 1000);
}
