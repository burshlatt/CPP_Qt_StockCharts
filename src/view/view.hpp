#ifndef VIEW_HPP
#define VIEW_HPP

#include <memory>
#include <vector>
#include <unordered_map>

#include <QMainWindow>

#include "qcustomplot.h"
#include <interpolation.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

namespace s21 {
class View : public QMainWindow {
    Q_OBJECT

private:
    using Button  = std::unique_ptr<QPushButton>;
    using Graphic = std::unique_ptr<QCustomPlot>;
    using HLayout = std::unique_ptr<QHBoxLayout>;

private:
    struct DeletionTargets {
        QCustomPlot* plot;
        QPushButton* button;
        QHBoxLayout* layout;
    };

public:
    View(QWidget *parent = nullptr);
    ~View();

private:
    void BtnAddGraphClicked();
    void BtnDelGraphClicked();
    void BtnLoadFileClicked();

private:
    std::unique_ptr<Ui::View> ui_;

    std::vector<Button> buttons_;
    std::vector<HLayout> layouts_;
    std::vector<Graphic> graphics_;

    std::unordered_map<QPushButton*, QCustomPlot*> btn_load_file_to_plot_map_;
    std::unordered_map<QPushButton*, DeletionTargets> btn_del_graph_to_targets_map_;

    Interpolator interpolator_;
};
} // namespace s21

#endif // VIEW_HPP
