#ifndef SETTINGS2_H
#define SETTINGS2_H

#include <QDialog>

namespace Ui {
class Settings2;
}

class Settings2 : public QDialog
{
    Q_OBJECT

public:
    explicit Settings2(QWidget *parent = nullptr);
    ~Settings2();

private:
    Ui::Settings2 *ui;
};

#endif // SETTINGS2_H
