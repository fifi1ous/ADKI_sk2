// terrain3dform.h
#ifndef TERRAIN3DFORM_H
#define TERRAIN3DFORM_H

#include <QDialog>
#include <vector>
#include "qpoint3df.h"

namespace Ui {
class Terrain3DForm;
}

class Terrain3DCanvas;

class Terrain3DForm : public QDialog
{
    Q_OBJECT

public:
    explicit Terrain3DForm(QWidget *parent = nullptr);
    ~Terrain3DForm();

    void setPoints(const std::vector<QPoint3DF> &points);

private:
    Ui::Terrain3DForm *ui;
    Terrain3DCanvas *canvas3D;
};

#endif // TERRAIN3DFORM_H
