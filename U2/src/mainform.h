#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainForm; }
QT_END_NAMESPACE

class MainForm : public QMainWindow
{
    Q_OBJECT

public:
    MainForm(QWidget *parent = nullptr);
    ~MainForm();

private slots:
    void on_actionOpen_triggered();

    void on_actionMBR_triggered();

    void on_actionPCA_triggered();

    void on_actionClear_All_triggered();

    void on_actionExit_triggered();

    void on_actionClear_results_triggered();

    void on_actionLongest_edge_triggered();

    void on_actionWall_average_triggered();

    void on_actionWeighted_bisector_triggered();

    void on_actionCovvex_Hull_triggered();

    void on_actionAbout_triggered();

    void on_actionGraham_Scan_triggered();

    void on_actionJarvis_Scan_triggered();

    void on_actionExport_building_triggered();

    void on_actionExport_CH_triggered();

    void on_actionFill_triggered();

    void on_actionOutline_triggered();

private:
    Ui::MainForm *ui;
    bool checkValidation(const QPolygonF &building);

    // This part of code was done by chatGPT
    struct PointComparator {
        bool operator()(const QPointF& p1, const QPointF& p2) const {
            if (p1.x() != p2.x()) {
                // Compare by x-coordinate first
                return p1.x() < p2.x();
            } else {
                // If x-coordinates are equal, compare by y-coordinate
                return p1.y() < p2.y();
            }
        }
    };
    // Here ends the part which was done by chatGPT

    short convexHull = 0;
    std::vector<QPolygonF> results;
    std::vector<QPolygonF> chs;
};

#endif // MAINFORM_H
