#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startSimulation();

    void on_pushButton_clicked();

private:
    void updateChart(int day, double S, double E, double I, double R);
    void updateTable(int day, double S, double E, double I, double R);

    Ui::MainWindow *ui;
    QLineSeries *seriesS;
    QLineSeries *seriesE;
    QLineSeries *seriesI;
    QLineSeries *seriesR;

    int population = 0;
    double beta = 0;
    double sigma = 0;
    double gamma = 0;
};
#endif // MAINWINDOW_H
