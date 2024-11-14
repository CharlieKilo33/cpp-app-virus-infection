#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCharts/QChart>
#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QChart *chart = new QChart();
    seriesS = new QLineSeries();
    seriesE = new QLineSeries();
    seriesI = new QLineSeries();
    seriesR = new QLineSeries();
    seriesS->setName("Не болевшие");
    seriesS->setColor(Qt::blue);

    seriesE->setName("Латентные");
    seriesE->setColor(Qt::red);

    seriesI->setName("Заболевшие");
    seriesI->setColor(Qt::green);

    seriesR->setName("Выздоровевшие");
    seriesR->setColor(Qt::magenta);

    chart->addSeries(seriesS);
    chart->addSeries(seriesE);
    chart->addSeries(seriesI);
    chart->addSeries(seriesR);
    chart->createDefaultAxes();

    chart->axes(Qt::Vertical).first()->setRange(0, 10000);
    chart->axes(Qt::Horizontal).first()->setRange(0, 500);
    ui->chartView->setChart(chart);
    ui->tableWidget->setRowCount(500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startSimulation()
{

    double S = population - 1;
    double E = 0;
    double I = 1;
    double R = 0;

    for (int day = 0; day < 500; ++day) {
        if(day == 0){
            updateChart(day, qRound(S), qRound(E), qRound(I), qRound(R));
            updateTable(day, qRound(S), qRound(E), qRound(I), qRound(R));
            continue;
        }
        double newExposed = beta * S * I / population;
        double newInfectious = sigma * E;
        double newRecovered = gamma * I;

        S -= newExposed;
        E += newExposed - newInfectious;
        I += newInfectious - newRecovered;
        R += newRecovered;

        updateChart(day, S, E, I, R);
        updateTable(day, S, E, I, R);
    }
}

void MainWindow::updateChart(int day, double S, double E, double I, double R)
{

    int roundedS = qRound(S);
    int roundedE = qRound(E);
    int roundedI = qRound(I);
    int roundedR = qRound(R);

    seriesS->append(day, roundedS);
    seriesE->append(day, roundedE);
    seriesI->append(day, roundedI);
    seriesR->append(day, roundedR);
}

void MainWindow::updateTable(int day, double S, double E, double I, double R)
{

    int roundedS = qRound(S);
    int roundedE = qRound(E);
    int roundedI = qRound(I);
    int roundedR = qRound(R);

    ui->tableWidget->setItem(day, 0, new QTableWidgetItem(QString::number(roundedS)));
    ui->tableWidget->setItem(day, 1, new QTableWidgetItem(QString::number(roundedE)));
    ui->tableWidget->setItem(day, 2, new QTableWidgetItem(QString::number(roundedI)));
    ui->tableWidget->setItem(day, 3, new QTableWidgetItem(QString::number(roundedR)));
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->sPopulation->value() == 0){
        QMessageBox::warning(this, "Ошибка", "Введите количество населения.");
        return;
    }
    if(ui->spinBox_2->value() == 0){
        QMessageBox::warning(this, "Ошибка", "Введите количество передачи инфекции в процентах.");
        return;
    }
    if(ui->spinBox_3->value() == 0){
        QMessageBox::warning(this, "Ошибка", "Введите количество контакта с больными в процентах.");
        return;
    }
    if(ui->spinBox_4->value() == 0){
        QMessageBox::warning(this, "Ошибка", "Введите количество дней инкубационного периода.");
        return;
    }
    if(ui->spinBox->value() == 0){
        QMessageBox::warning(this, "Ошибка", "Введите количество дней продолжительности болезни (больше 2)");
        return;
    }

    seriesS->clear();
    seriesE->clear();
    seriesI->clear();
    seriesR->clear();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(500);

    population = ui->sPopulation->value();
    beta = (ui->spinBox_2->value() * 0.01) * (ui->spinBox_3->value() * 0.01);
    sigma = 1. / ui->spinBox_4->value();
    gamma = 1. / ui->spinBox->value();
    startSimulation();
}

