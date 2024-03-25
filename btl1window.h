#ifndef BTL1WINDOW_H
#define BTL1WINDOW_H

#include <QDialog>

namespace Ui {
class btl1window;
}

class btl1window : public QDialog
{
    Q_OBJECT

public:
    explicit btl1window(QWidget *parent = nullptr);
    ~btl1window();

private slots:

private:
    Ui::btl1window *ui;
};

#endif // BTL1WINDOW_H
