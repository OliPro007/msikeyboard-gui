#pragma once

#include <QMainWindow>

#include "Keyboard.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private slots:
    void on_btnRegion1_clicked();
    void on_btnRegion2_clicked();
    void on_btnRegion3_clicked();

    void on_btnConfirm_clicked();
    void on_btnReset_clicked();

    void on_cbModes_currentIndexChanged(int index);

private:
    Ui::MainWindow* ui;

    void readPreviousConfig();

};
