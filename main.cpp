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

constexpr size_t BLOB_SIZE = 10;

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

            //DEBUG
            std::stringstream ss;
            for(int i = 0; i < blob.size(); ++i) {
                ss << std::to_string(data[i]) << " ";
            }

            std::cout << "DEBUG: Dump: " << ss.str() << std::endl;
            //DEBUG

            if(blob.size() != BLOB_SIZE) {
                std::cerr << "ERROR: Corrupted data - Invalid size" << std::endl;
                return -1;
            }

            Keyboard keyboard;
            try{
                switch(blob.at(0)) {
                case Keyboard::MODE_NORMAL:
                    keyboard.normal(Color(blob.at(1), blob.at(2), blob.at(3)),
                                    Color(blob.at(4), blob.at(5), blob.at(6)),
                                    Color(blob.at(7), blob.at(8), blob.at(9)));
                    break;
                case Keyboard::MODE_GAMING:
                    keyboard.gaming(Color(blob.at(1), blob.at(2), blob.at(3)));
                    break;
                case Keyboard::MODE_BREATHING:
                    keyboard.breathing(Color(blob.at(1), blob.at(2), blob.at(3)),
                                       Color(blob.at(4), blob.at(5), blob.at(6)),
                                       Color(blob.at(7), blob.at(8), blob.at(9)));
                    break;
                case Keyboard::MODE_WAVE:
                    keyboard.wave(Color(blob.at(1), blob.at(2), blob.at(3)),
                                  Color(blob.at(4), blob.at(5), blob.at(6)),
                                  Color(blob.at(7), blob.at(8), blob.at(9)));
                    break;
                default:
                    std::cerr << "ERROR: Corrupted data - Invalid mode" << std::endl;
                }
            } catch(std::runtime_error& e) {
                std::cerr << "ERROR: " << e.what() << std::endl;
                return -1;
            }

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
