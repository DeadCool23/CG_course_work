#include "rotationviewer.h"

#include <QPainter>
#include <QMatrix4x4>
#include <QElapsedTimer>

RotationViewer::RotationViewer(const QVector<QPointF> &curve, double rotationAxisX, QWidget *parent)
    : QWidget(parent) {

    // Замер времени для разного количества сегментов
    // int mes_cnt = 20;
    // for (int i = 50; i <= 1000; i += 50) {
    //     qint64 sum = 0;
    //     for (int j = 0; j < mes_cnt; ++j) {
    //         QElapsedTimer timer;
    //         timer.start();

    //         model = PolygonModel::generateFromCurve(curve, rotationAxisX, i);

    //         qint64 elapsedTime = timer.nsecsElapsed();
    //         sum += elapsedTime;
    //     }
    //     qDebug() << i << "|" << sum / mes_cnt;
    // }

    model = PolygonModel::generateFromCurve(curve, rotationAxisX, segments);

    liWin = new LightInputWindow(&light, &model, this);

    setMinimumSize(800, 600);
    model.generateColors();

    setFocusPolicy(Qt::StrongFocus);
}

void RotationViewer::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    render = Render(height(), width(), Qt::black);

    Light transformedLight = light;
    transformedLight.position = transformToCameraSpace(light.position);

    if (light.isEnable && model.isRandColors) {
        model.setColor(STD_COLOR);
    }

    auto tranformedModel = transformModelToCameraSpace();
    auto renderedImage = render.renderPolygonalModel(tranformedModel, transformedLight);

    drawScene(renderedImage, painter);
}

void RotationViewer::drawScene(const QVector<QVector<QColor>> &pixels, QPainter &painter) {
    for (int y = 0; y < height(); ++y) {
        for (int x = 0; x < width(); ++x) {
            painter.setPen(pixels[y][x]);
            painter.drawPoint(x, y);
        }
    }
}

PolygonModel RotationViewer::transformModelToCameraSpace() {
    auto transformedModel = model;
    QMatrix4x4 afinMatrix = camera.getViewMatrix();

    QVector<QVector3D> afinnedPoints;
    QVector<QVector3D> afinnedNormals;

    for (const auto &point : model.vertices) {
        afinnedPoints.append(afinMatrix.map(point));
    }
    transformedModel.vertices = afinnedPoints;
    for (const auto &normal : model.normals) {
        afinnedNormals.append(afinMatrix.map(normal));
    }
    transformedModel.normals = afinnedNormals;

    return transformedModel;
}

QVector<QVector3D> RotationViewer::transformToCameraSpace(const QVector<QVector3D> &points) {
    QMatrix4x4 afinMatrix = camera.getViewMatrix();
    QVector<QVector3D> afinnedPoints;

    for (const auto &point : points) {
        afinnedPoints.append(afinMatrix.map(point));
    }

    return afinnedPoints;
}

QVector3D RotationViewer::transformToCameraSpace(const QVector3D &point) {
    QMatrix4x4 afinMatrix = camera.getViewMatrix();

    return afinMatrix.map(point);
}

#include <QKeyEvent>
#include <QFileDialog>
#include <QColorDialog>
void RotationViewer::keyPressEvent(QKeyEvent *event) {
    QString filePath;
    QColor selectedColor;
    switch (event->key()) {
    case Qt::Key_A:
        camera.adjustAngleOxz(-5);
        break;
    case Qt::Key_D:
        camera.adjustAngleOxz(5);
        break;
    case Qt::Key_W:
        camera.adjustAngleOzy(-5);
        break;
    case Qt::Key_S:
        camera.adjustAngleOzy(5);
        break;
    case Qt::Key_Q:
        camera.adjustRadius(-100);
        break;
    case Qt::Key_E:
        camera.adjustRadius(100);
        break;
    case Qt::Key_R:
        camera.reset();
        break;
    case Qt::Key_I:
        filePath = QFileDialog::getSaveFileName(this, "Сохранить изображение", "", "Images (*.png *.jpg *.bmp)");
        if (!filePath.isEmpty()) {
            saveSceneToImage(filePath);
        }
        break;
    case Qt::Key_C:
        selectedColor = QColorDialog::getColor(Qt::white, this, "Выберите цвет");
        if (selectedColor.isValid()) {
            model.setColor(selectedColor);
        }
        break;
    case Qt::Key_L:
        liWin->show();
        break;
    default:
        break;
    }
    update();
}


#include <QImage>
#include <QMessageBox>
void RotationViewer::saveSceneToImage(const QString &filePath) {
    QImage image(size(), QImage::Format_ARGB32);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);

    auto renderedImage = render.getRenderImage();

    drawScene(renderedImage, painter);

    painter.end();

    if (image.save(filePath)) {
        QMessageBox::information(this, "Сохранение сцены", QString("Сцена успешно сохранена \nв %1").arg(filePath));
    } else {
        QMessageBox::warning(this, "Сохранение сцены", QString("Не удалось сохранить сцену \nв %1").arg(filePath));
    }
}
