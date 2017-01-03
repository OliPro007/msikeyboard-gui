#include "MainWindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QFile>
#include <QDir>

#include <cstdint>

#include <iostream>
#include <string>
#include <sstream>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    QCommandLineOption presetOption(QStringList() << "p" << "preset", "Quickly load a registered preset", "preset");
    QCommandLineOption reloadOption(QStringList() << "r" << "reload", "Quickly reload the registered configuration");

    QCommandLineParser parser;
    parser.setApplicationDescription("\nSets the colors and the mode of the backlight of a MSI Steelpro keyboard."
                                     "\nLaunching without any options will launch the GUI."
                                     "\nThe options -p and -r can be used to quickly set the config in a terminal.");
    parser.addHelpOption();
    parser.addOption(presetOption);
    parser.addOption(reloadOption);
    parser.process(a);

    if(parser.isSet(reloadOption)) {
        QString filename(QDir::homePath());
        filename.append("/.msikeyboard/current.preset");

        if(QFile::exists(filename)) {
            QFile file(filename);
            file.open(QIODevice::ReadOnly);
            QByteArray blob = file.readAll();
            uint8_t* data = (uint8_t*) blob.data();
            file.close();

            std::stringstream ss;
            for(int i = 0; i < blob.size(); ++i) {
                ss << std::to_string(data[i]) << " ";
            }

            std::cout << "Dump: " << ss.str() << std::endl;
            return 0;
        } else {
            std::cerr << "ERROR: The file " << filename.toStdString() << " does not exists" << std::endl;
            return -1;
        }
    } else if(parser.isSet(presetOption)) {
        QString filename(QDir::homePath());
        filename.append("/.msikeyboard/");
        filename.append(parser.value(presetOption));
        filename.append(".preset");

        if(QFile::exists(filename)) {
            std::cout << "TODO: Load preset " << parser.value(presetOption).toStdString() << std::endl;
            return 0;
        } else {
            std::cerr << "ERROR: The preset " << filename.toStdString() << " does not exist" << std::endl;
            return -1;
        }
    } else {
        MainWindow w;
        w.show();
        return a.exec();
    }
}
