#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDebug>

/**
 * @brief Główny punkt wejścia do aplikacji.
 *
 * Funkcja inicjalizuje aplikację Qt, tworzy główne okno programu
 * oraz uruchamia pętlę zdarzeń.
 *
 * @param argc Liczba argumentów przekazanych do programu.
 * @param argv Tablica argumentów przekazanych do programu.
 * @return Kod zakończenia aplikacji.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
