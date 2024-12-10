#include "lightinputwindow.h"

#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>

LightInputWindow::CustomDialog::CustomDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Цвет полигональной модели");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("Сделать цвета полигонов разными?", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *yesButton = new QPushButton("Да", this);
    buttonLayout->addWidget(yesButton);

    QPushButton *noButton = new QPushButton("Нет", this);
    buttonLayout->addWidget(noButton);

    layout->addLayout(buttonLayout);

    connect(yesButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(noButton, &QPushButton::clicked, this, &QDialog::reject);
}

LightInputWindow::LightInputWindow(Light *l, PolygonModel *model, QWidget *viewer) {
    if (!l || !model || !viewer) return;

    QGridLayout *layout = new QGridLayout(this);

    float _d = realDistToObj;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i == 1 && j == 1) {
                QLabel *centerLabel = new QLabel("O", this);
                centerLabel->setAlignment(Qt::AlignCenter);
                layout->addWidget(centerLabel, i, j);
            } else {
                QPushButton *button = new QPushButton("*", this);
                layout->addWidget(button, i, j);
                connect(button, &QPushButton::clicked, [l, model, viewer, _d, i, j]() {
                    int relativeX = j - 1;
                    int relativeY = i - 1;

                    if (model->isRandColors) {
                        model->setColor(STD_COLOR);
                    }

                    l->position = {relativeX * _d, (float)relativeY * _d, 0};
                    l->isEnable = true;
                    viewer->update();
                });
            }
        }
    }

    QPushButton *bottomButton = new QPushButton("Без освещения", this);
    layout->addWidget(bottomButton, 3, 0, 1, 3);

    connect(bottomButton, &QPushButton::clicked, this, [this, l, model, viewer]() {
        CustomDialog dialog(this);
        auto res = dialog.exec();

        if (res == QDialog::Accepted) {
            model->generateColors();
        } else if (res == QDialog::Rejected) {
            if (model->isRandColors) {
                model->setColor(STD_COLOR);
            }
        }
        l->isEnable = false;
        viewer->update();
    });

    setLayout(layout);
    setWindowTitle("Источник света");
}
