#ifndef LIGHTINPUTWINDOW_H
#define LIGHTINPUTWINDOW_H

#include <QWidget>
#include <QDialog>

#include "light.h"
#include "polygonmodel.h"

class LightInputWindow : public QWidget {
public:
    LightInputWindow(Light *l, PolygonModel *model, QWidget *viewer);
private:
    float realDistToObj = 1000.;
    class CustomDialog : public QDialog {
    public:
        CustomDialog(QWidget *parent = nullptr);
    };
};

#endif // LIGHTINPUTWINDOW_H
