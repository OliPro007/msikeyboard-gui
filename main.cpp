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

QCoreApplication* createApplication(int& argc, char* argv[]) {
    if(argc == 1) //The only argument is the path to the executable (automatically added)
        return new QApplication(argc, argv);
    else if(argc > 1) //The user added an argument
        return new QCoreApplication(argc, argv);
    else //Wut?
        return nullptr;
}

int main(int argc, char* argv[]) {
    QScopedPointer<QCoreApplication> a(createApplication(argc, argv));

    if(a == nullptr) //Something weird happened
        return -1;

    if(qobject_cast<QApplication*>(a.data())) { //GUI version
        MainWindow w;
        w.show();
        return a->exec();
    } else { //"Batch" version
        QCommandLineOption presetOption(QStringList() << "p" << "preset", "Quickly load a registered preset", "preset");
        QCommandLineOption reloadOption(QStringList() << "r" << "reload", "Quickly reload the registered configuration");

        QCommandLineParser parser;
        parser.setApplicationDescription("\nSets the colors and the mode of the backlight of a MSI Steelpro keyboard."
                                         "\nLaunching without any options will launch the GUI."
                                         "\nThe options -p and -r can be used to quickly set the config in a terminal.");
        parser.addHelpOption();
        parser.addOption(presetOption);
        parser.addOption(reloadOption);
        parser.process(*a);

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
                    switch(data[0]) {
                    case Keyboard::MODE_NORMAL:
                        keyboard.normal(Color(data[1], data[2], data[3]),
                                        Color(data[4], data[5], data[6]),
                                        Color(data[7], data[8], data[9]));
                        break;
                    case Keyboard::MODE_GAMING:
                        keyboard.gaming(Color(data[1], data[2], data[3]));
                        break;
                    case Keyboard::MODE_BREATHING:
                        keyboard.breathing(Color(data[1], data[2], data[3]),
                                           Color(data[4], data[5], data[6]),
                                           Color(data[7], data[8], data[9]));
                        break;
                    case Keyboard::MODE_WAVE:
                        keyboard.wave(Color(data[1], data[2], data[3]),
                                      Color(data[4], data[5], data[6]),
                                      Color(data[7], data[8], data[9]));
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
        }
    }

}
