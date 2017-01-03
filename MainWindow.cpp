#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>
#include <QColorDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QFile>
#include <QDir>

MainWindow::MainWindow(QWidget* parent) :
QMainWindow(parent), ui(new Ui::MainWindow), _keyboard() {
    ui->setupUi(this);

    if(_keyboard.getDevice() == nullptr) {
        QMessageBox msgBox;
        msgBox.setText("Device not found");
        msgBox.exec();
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_btnRegion1_clicked() {
    QPalette palette;
    QColor chosenColor = QColorDialog::getColor(Qt::red);
    palette.setColor(QPalette::Button, chosenColor);
    ui->btnRegion1->setPalette(palette);
}

void MainWindow::on_btnRegion2_clicked() {
    QPalette palette;
    QColor chosenColor = QColorDialog::getColor(Qt::red);
    palette.setColor(QPalette::Button, chosenColor);
    ui->btnRegion2->setPalette(palette);
}

void MainWindow::on_btnRegion3_clicked() {
    QPalette palette;
    QColor chosenColor = QColorDialog::getColor(Qt::red);
    palette.setColor(QPalette::Button, chosenColor);
    ui->btnRegion3->setPalette(palette);
}

void MainWindow::on_btnConfirm_clicked() {
    Color left(ui->btnRegion1->palette().color(QPalette::Button).red(),
               ui->btnRegion1->palette().color(QPalette::Button).green(),
               ui->btnRegion1->palette().color(QPalette::Button).blue());

    Color center(ui->btnRegion2->palette().color(QPalette::Button).red(),
                 ui->btnRegion2->palette().color(QPalette::Button).green(),
                 ui->btnRegion2->palette().color(QPalette::Button).blue());

    Color right(ui->btnRegion3->palette().color(QPalette::Button).red(),
                ui->btnRegion3->palette().color(QPalette::Button).green(),
                ui->btnRegion3->palette().color(QPalette::Button).blue());

    QByteArray blob;

    try {
        switch(ui->cbModes->currentIndex()) {
        case 0: //Normal
            _keyboard.normal(left, center, right);
            blob.append(Keyboard::MODE_NORMAL);
            break;
        case 1: //Gaming
            _keyboard.gaming(left);
            blob.append(Keyboard::MODE_GAMING);
            break;
        case 2: //Breathing
            _keyboard.breathing(left, center, right);
            blob.append(Keyboard::MODE_BREATHING);
            break;
        case 3: //Wave
            _keyboard.wave(left, center, right);
            blob.append(Keyboard::MODE_WAVE);
            break;
        default: //Wut?
            QMessageBox msgBox;
            msgBox.setText("There is a typo somewhere in the mode selection");
            msgBox.exec();
            return;
        }
    } catch(std::runtime_error& e) {
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
        return;
    }

    blob.append(left.r());
    blob.append(left.g());
    blob.append(left.b());

    blob.append(center.r());
    blob.append(center.g());
    blob.append(center.b());

    blob.append(right.r());
    blob.append(right.g());
    blob.append(right.b());

    QString filename(QDir::homePath());
    filename.append("/.msikeyboard/current.preset");

    QFile file(filename);
    file.open(QIODevice::Truncate | QIODevice::WriteOnly);
    file.write(blob);
    file.flush();
    file.close();
}

void MainWindow::on_btnReset_clicked() {
    QPalette palette;
    palette.setColor(QPalette::Button, Qt::red);
    ui->btnRegion1->setPalette(palette);
    palette.setColor(QPalette::Button, Qt::green);
    ui->btnRegion2->setPalette(palette);
    palette.setColor(QPalette::Button, Qt::blue);
    ui->btnRegion3->setPalette(palette);
}

void MainWindow::on_cbModes_currentIndexChanged(int index) {
    if(index == 1) {
        ui->btnRegion2->hide();
        ui->btnRegion3->hide();
    } else {
        ui->btnRegion2->show();
        ui->btnRegion3->show();
    }
}
