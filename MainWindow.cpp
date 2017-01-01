#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "QMessageBox"
#include "QColorDialog"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _keyboard = new Keyboard();
    if(_keyboard->getDevice() == nullptr) {
        QMessageBox msgBox;
        msgBox.setText("Device not found");
        msgBox.exec();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnRegion1_clicked()
{
    QPalette palette;
    QColor chosenColor = QColorDialog::getColor(Qt::red);
    palette.setColor(QPalette::Button, chosenColor);
    ui->btnRegion1->setPalette(palette);
}

void MainWindow::on_btnRegion2_clicked()
{
    QPalette palette;
    QColor chosenColor = QColorDialog::getColor(Qt::red);
    palette.setColor(QPalette::Button, chosenColor);
    ui->btnRegion2->setPalette(palette);
}

void MainWindow::on_btnRegion3_clicked()
{
    QPalette palette;
    QColor chosenColor = QColorDialog::getColor(Qt::red);
    palette.setColor(QPalette::Button, chosenColor);
    ui->btnRegion3->setPalette(palette);
}

void MainWindow::on_btnConfirm_clicked()
{
    Color left(ui->btnRegion1->palette().color(QPalette::Button).red(),
               ui->btnRegion1->palette().color(QPalette::Button).green(),
               ui->btnRegion1->palette().color(QPalette::Button).blue());

    Color center(ui->btnRegion2->palette().color(QPalette::Button).red(),
                 ui->btnRegion2->palette().color(QPalette::Button).green(),
                 ui->btnRegion2->palette().color(QPalette::Button).blue());

    Color right(ui->btnRegion3->palette().color(QPalette::Button).red(),
                ui->btnRegion3->palette().color(QPalette::Button).green(),
                ui->btnRegion3->palette().color(QPalette::Button).blue());

    switch(ui->cbModes->currentIndex()) {
    case 0: //Normal
        try {
            _keyboard->normal(left, center, right);
        } catch(std::runtime_error& e) {
            QMessageBox msgBox;
            msgBox.setText(e.what());
            msgBox.exec();
        }
        break;
    case 1: //Gaming
        try {
            _keyboard->gaming(left);
        } catch(std::runtime_error& e) {
            QMessageBox msgBox;
            msgBox.setText(e.what());
            msgBox.exec();
        }
        break;
    case 2: //Breathing
        try {
            _keyboard->breathing(left, center, right);
        } catch(std::runtime_error& e) {
            QMessageBox msgBox;
            msgBox.setText(e.what());
            msgBox.exec();
        }
        break;
    case 3: //Wave
        try {
            _keyboard->wave(left, center, right);
        } catch(std::runtime_error& e) {
            QMessageBox msgBox;
            msgBox.setText(e.what());
            msgBox.exec();
        }
        break;
    default:
        QMessageBox msgBox;
        msgBox.setText("There is a typo somewhere in the mode selection");
        msgBox.exec();
    }
}

void MainWindow::on_btnReset_clicked()
{
    QPalette palette;
    palette.setColor(QPalette::Button, Qt::red);
    ui->btnRegion1->setPalette(palette);
    palette.setColor(QPalette::Button, Qt::green);
    ui->btnRegion2->setPalette(palette);
    palette.setColor(QPalette::Button, Qt::blue);
    ui->btnRegion3->setPalette(palette);
}

void MainWindow::on_cbModes_currentIndexChanged(int index)
{
    if(index == 1) {
        ui->btnRegion2->hide();
        ui->btnRegion3->hide();
    } else {
        ui->btnRegion2->show();
        ui->btnRegion3->show();
    }
}
