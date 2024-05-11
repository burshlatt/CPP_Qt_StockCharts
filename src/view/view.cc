#include "view.hpp"
#include "./ui_view.h"

namespace s21 {
View::View(QWidget *parent) :
    QMainWindow(parent),
    ui_(std::make_unique<Ui::View>())
{
    ui_->setupUi(this);
    setFixedSize(800, 800);

    QScreen* screen{QGuiApplication::primaryScreen()};
    QRect screenGeometry{screen->geometry()};
    const int x_c{(screenGeometry.width() - 800) / 2};
    const int y_c{(screenGeometry.height() - 800) / 2};

    move(x_c, y_c);

    connect(ui_->btnAddGraph, &QPushButton::clicked, this, &View::BtnAddGraphClicked);

    BtnAddGraphClicked();
}

View::~View() {}

void View::BtnAddGraphClicked() {
    if (graphics_.size() < 5) {
        graphics_.push_back(std::make_unique<QCustomPlot>());
        QCustomPlot* plot{graphics_.back().get()};

        buttons_.push_back(std::make_unique<QPushButton>());
        QPushButton* btn_load_file{buttons_.back().get()};

        buttons_.push_back(std::make_unique<QPushButton>());
        QPushButton* btn_delete_graph{buttons_.back().get()};

        btn_load_file_to_plot_map_[btn_load_file] = plot;

        btn_load_file->setText("Load file");
        btn_delete_graph->setText("Delete graphic");

        layouts_.push_back(std::make_unique<QHBoxLayout>());
        QHBoxLayout* hor_layout{layouts_.back().get()};

        btn_del_graph_to_targets_map_[btn_delete_graph] = {plot, btn_load_file, hor_layout};

        hor_layout->addWidget(btn_load_file);
        hor_layout->addWidget(btn_delete_graph);

        ui_->vblGraphics->addWidget(plot);
        ui_->vblGraphics->addLayout(hor_layout);

        this->update();

        connect(btn_load_file, &QPushButton::clicked, this, &View::BtnLoadFileClicked);
        connect(btn_delete_graph, &QPushButton::clicked, this, &View::BtnDelGraphClicked);
    }

    if (graphics_.size() == 5) {
        ui_->btnAddGraph->setEnabled(false);
    }
}

void View::BtnDelGraphClicked() {
    if (graphics_.size() == 5) {
        ui_->btnAddGraph->setEnabled(true);
    }

    if (graphics_.size() > 1) {
        QPushButton* clicked_button{static_cast<QPushButton*>(sender())};
        auto del_targets{btn_del_graph_to_targets_map_[clicked_button]};

        buttons_.erase(std::find_if(buttons_.begin(), buttons_.end(), [&clicked_button](const Button& ptr) {
            return ptr.get() == clicked_button;
        }));

        buttons_.erase(std::find_if(buttons_.begin(), buttons_.end(), [&del_targets](const Button& ptr) {
            return ptr.get() == del_targets.button;
        }));

        graphics_.erase(std::find_if(graphics_.begin(), graphics_.end(), [&del_targets](const Graphic& ptr) {
            return ptr.get() == del_targets.plot;
        }));

        layouts_.erase(std::find_if(layouts_.begin(), layouts_.end(), [&del_targets](const HLayout& ptr) {
            return ptr.get() == del_targets.layout;
        }));

        this->update();
    }
}

void View::BtnLoadFileClicked() {
    QString path{QFileDialog::getOpenFileName(nullptr, "Open file", QString(), "CSV Files (*.csv)")};

    if (!path.isEmpty()) {
        auto coords{interpolator_.LoadFile(path.toStdString())};

        QVector<double> x(coords.first.begin(), coords.first.end());
        QVector<double> y(coords.second.begin(), coords.second.end());

        QPushButton* clicked_button{static_cast<QPushButton*>(sender())};
        QCustomPlot* plot{btn_load_file_to_plot_map_[clicked_button]};

        plot->clearGraphs();
        plot->clearItems();
        plot->addGraph();
        plot->graph(0)->setPen(QPen(Qt::red));
        plot->graph(0)->setData(x, y);

        QSharedPointer<QCPAxisTickerDateTime> date_ticker(new QCPAxisTickerDateTime);
        date_ticker->setDateTimeFormat("dd.MM.yyyy");
        plot->xAxis->setTicker(date_ticker);
        plot->xAxis->setRange(x.first(), x.last());
        plot->yAxis->setRange(0, *std::max_element(y.begin(), y.end()) + 10);

        plot->replot();
    }
}
} // namespace s21
