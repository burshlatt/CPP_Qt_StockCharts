#include <QApplication>

#include "view.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    s21::View window;
    window.show();
    return app.exec();
}
